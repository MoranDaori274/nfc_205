/**
 * rs_nfc_ptx205r.c
 *
 * Implementation for the Renesas PTX205R NFC reader, calling the PTX2xx SDK
 * (ptx_RUL_* / ptx_RUL.h) directly. The SDK lives in <repo>/ptx_sdk; there
 * is no FSP module for it. Its RA HAL port talks to the FSP peripheral
 * instances listed in rs_nfc_ptx205r_board.h (SPI, IRQ, two GPT timers).
 *
 * Each function is a direct, non-static entry point declared in
 * rs_nfc_ptx205r.h and called by name from the rest of the RS NFC Card
 * Reader module — no function-pointer vtable indirection.
 *
 * There is no local state-machine (open/idle/discovered/activated) here:
 * we rely on the PTX SDK's own error handling/return codes.
 *
 * Messages from the PTX205R are processed by ptx_RUL_GetStatusInfo(), one
 * message per call, and the PTX205R holds its IRQ line high while a
 * message is pending. rs_nfc_ptx_wait_for_card() therefore drains messages
 * while the line is high and otherwise sleeps until the IRQ7 edge.
 */

#include "rs_nfc_ptx205r.h"
#include "rs_nfc_ptx205r_board.h"
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "hal_data.h"
/* PTX2xx SDK. NOTE: we intentionally use the lean per-tag-type NDEF
 * components (ptx_NDEF_T3T/T4T/T5T) instead of the generic ptx_NDEF
 * dispatcher. ptx_NDEF_Open() links all four tag-type components, which
 * does not fit this MCU's flash budget next to the PTX205R firmware image.
 * T2T NDEF read remains hand-rolled (rs_ndef_read.c). */
#include "ptx_RUL.h"
#include "ptx_HAL.h"
#include "ptx_NDEF_T4T.h"
#if !defined(PTX_DISABLE_NDEF_T3T)
#include "ptx_NDEF_T3T.h"
#endif
#if !defined(PTX_DISABLE_NDEF_T5T)
#include "ptx_NDEF_T5T.h"
#endif

/***********************************************************************************************************************
 * Module-local constants
 **********************************************************************************************************************/
#define PTX205R_TIMEOUT_RAW           (200U) /* Application-timeout for raw-protocol exchanges */
#define PTX205R_NUMBER_OF_TIMERS      (2U)   /* ptx_tmr0 + ptx_tmr1 */
#define PTX205R_INTERFACE_PORT_ID     (0U)

/* RUL context (PTX SDK main component). Large — must not live on a stack. */
static ptx_RUL_t g_rul;

/* True once ptx_RUL_Init() + ptx_RUL_InitHardware() succeeded. The PTX205R
 * firmware download takes a while, so the SDK stays initialized between
 * Read() calls; rs_nfc_ptx_close() only switches the RF field off. */
static bool g_hw_ready = false;

/* Tracks whether rs_nfc_ptx_open() succeeded for the current session */
static bool g_ptx_opened = false;

/* Card registry owned by g_rul (valid while g_hw_ready). */
static ptx_RUL_CardRegistry_t * g_registry = NULL;

/* RF technologies requested by the caller (rs_nfc_reader_cfg_t.tech_mask). */
static rs_nfc_tech_mask_t g_tech_mask = RS_NFC_TECH_ALL;

/* Discovery configuration of the current session (re-used to restart
 * discovery when the PTX205R leaves it on its own, see system check). */
static ptx_RUL_DiscoverConfig_t g_disc_config;

/***********************************************************************************************************************
 * SDK NDEF components — static allocation (shared TX/RX buffers).
 * Lean alternative to the generic ptx_NDEF dispatcher (see note above).
 **********************************************************************************************************************/
static ptx_NDEF_T4T_t   g_t4t_ndef_comp;
static uint8_t          g_ndef_tx_buf[RS_NFC_PTX_TX_BUF_SIZE];
static uint8_t          g_ndef_rx_buf[RS_NFC_PTX_RX_BUF_SIZE];

#if !defined(PTX_DISABLE_NDEF_T3T)
static ptx_NDEF_T3T_t   g_t3t_ndef_comp;
#endif

#if !defined(PTX_DISABLE_NDEF_T5T)
/* T5T needs a small dedicated work buffer (block-sized scratch area). */
#define T5T_WORK_BUF_SIZE  32U
static ptx_NDEF_T5T_t   g_t5t_ndef_comp;
static uint8_t          g_t5t_work_buf[T5T_WORK_BUF_SIZE];
#endif

/***********************************************************************************************************************
 * Interrupt-driven wait support
 **********************************************************************************************************************/

static volatile TaskHandle_t g_waiting_task = NULL;

/* Set by rs_nfc_ptx_wake_waiting_task() (i.e. rs_nfc_reader_Stop()) to make
 * rs_nfc_ptx_wait_for_card() return instead of re-arming its wait. Sticky
 * for the rest of the session; cleared by rs_nfc_ptx_open(). */
static volatile bool g_wait_abort = false;

/* Replaces the SDK's (intentionally empty) IRQ7 callback while a task waits
 * for a card. Outside a wait it does nothing, exactly like the SDK's. */
static void ptx205r_irq_wake_cb (external_irq_callback_args_t * p_args)
{
    RS_COMMON_UNUSED(p_args);
    if (NULL != g_waiting_task)
    {
        BaseType_t higher_prio_task_woken = pdFALSE;
        vTaskNotifyGiveFromISR(g_waiting_task, &higher_prio_task_woken);
        portYIELD_FROM_ISR(higher_prio_task_woken);
    }
}

/***********************************************************************************************************************
 * Internal helpers
 **********************************************************************************************************************/

static rs_nfc_card_type_t map_card_type (const ptx_RUL_CardParams_t * card,
                                         ptx_RUL_CardProtocolType_t   prot)
{
    if (NULL == card)
    {
        return RS_NFC_CARD_TYPE_UNKNOWN;
    }

    switch (card->TechType)
    {
        case ptx_RUL_CardRfTechType_A:
            switch (prot)
            {
                case ptx_RUL_CardProtocolType_T2T: return RS_NFC_CARD_TYPE_NFC_TAG_TYPE_2;
                case ptx_RUL_CardProtocolType_T4T: return RS_NFC_CARD_TYPE_NFC_TAG_TYPE_4A;
                default:                           return RS_NFC_CARD_TYPE_ISO14443A;
            }
        case ptx_RUL_CardRfTechType_B:
            return (ptx_RUL_CardProtocolType_T4T == prot) ? RS_NFC_CARD_TYPE_NFC_TAG_TYPE_4B
                                                          : RS_NFC_CARD_TYPE_ISO14443B;
        case ptx_RUL_CardRfTechType_F:
            return (ptx_RUL_CardProtocolType_T3T == prot) ? RS_NFC_CARD_TYPE_NFC_TAG_TYPE_3
                                                          : RS_NFC_CARD_TYPE_FELICA;
        case ptx_RUL_CardRfTechType_V:
            return (ptx_RUL_CardProtocolType_T5T == prot) ? RS_NFC_CARD_TYPE_NFC_TAG_TYPE_5
                                                          : RS_NFC_CARD_TYPE_ISO15693;
        default:
            return RS_NFC_CARD_TYPE_UNKNOWN;
    }
}

/* Pick the protocol to activate from the protocols a discovered card
 * supports (same precedence as the PTX SDK NDEF example). */
static ptx_RUL_CardProtocolType_t choose_protocol (const ptx_RUL_CardParams_t * card)
{
    ptx_RUL_CardProtocolType_t prot = ptx_RUL_CardProtocolType_Undefined;

    if (NULL == card)
    {
        return prot;
    }

    if (0U != (ptx_RUL_SupportedProtocols_T2T & card->Protocols))
    {
        prot = ptx_RUL_CardProtocolType_T2T;
    }

    if (0U != (ptx_RUL_SupportedProtocols_T3T & card->Protocols))
    {
        prot = ptx_RUL_CardProtocolType_T3T;
    }

    if (0U != (ptx_RUL_SupportedProtocols_T4T & card->Protocols))
    {
        prot = ptx_RUL_CardProtocolType_T4T;
    }

    if (0U != (ptx_RUL_SupportedProtocols_T5T & card->Protocols))
    {
        prot = ptx_RUL_CardProtocolType_T5T;
    }

    if (0U != (ptx_RUL_SupportedProtocols_NfcDep & card->Protocols))
    {
        prot = ptx_RUL_CardProtocolType_NfcDep;
    }

    return prot;
}

static void extract_uid (const ptx_RUL_CardParams_t * card, uint8_t * uid, uint8_t * uid_len)
{
    uint8_t len = 0;
    *uid_len = 0;

    if (NULL == card)
    {
        return;
    }

    switch (card->TechType)
    {
        case ptx_RUL_CardRfTechType_A:
        {
            len = card->TechParam.TypeA.NfcId1Len;

            if (len > RS_NFC_UID_MAX_BYTES)
            {
                len = RS_NFC_UID_MAX_BYTES;
            }

            memcpy(uid, card->TechParam.TypeA.NfcId1, len);
            *uid_len = len;
            break;
        }
        case ptx_RUL_CardRfTechType_B:
            /* NFCID0 */
            memcpy(uid, &card->TechParam.TypeB.SensBRes[PTX_RUL_SENSB_RES_NFCID0_OFFSET], PTX_RUL_NFCID0_LEN);
            *uid_len = PTX_RUL_NFCID0_LEN;
            break;
        case ptx_RUL_CardRfTechType_F:
            /* NFCID2 */
            memcpy(uid, &card->TechParam.TypeF.SensFRes[PTX_RUL_SENSF_RES_NFCID2_OFFSET], PTX_RUL_NFCID2_LEN);
            *uid_len = PTX_RUL_NFCID2_LEN;
            break;
        case ptx_RUL_CardRfTechType_V:
            /* INVENTORY_RES carries the UID LSB first; report it MSB first. */
            for (uint8_t i = 0; i < PTX_RUL_INVENTORYRES_UID_LEN; i++)
            {
                uid[i] = card->TechParam.TypeV.InventoryRes[PTX_RUL_INVENTORYRES_UID_OFFSET + PTX_RUL_INVENTORYRES_UID_LEN - 1U - i];
            }
            *uid_len = PTX_RUL_INVENTORYRES_UID_LEN;
            break;
        default:
            break;
    }
}

static const ptx_RUL_CardParams_t * active_card (void)
{
    if (NULL == g_registry)
    {
        return NULL;
    }

    return g_registry->ActiveCard;
}

/* Reads (and thereby processes) one pending PTX205R message, then reports
 * the discovery state. */
static rs_status_t ptx205r_discover_status (rs_nfc_ptx_disc_status_t * out_status)
{
    if (NULL == out_status)
    {
        return RS_ERR_INVALID_CFG;
    }

    ptx_RUL_Status_Params_t info;
    (void)memset(&info, 0, sizeof(info));
    info.Type = ptx_RUL_StatusType_Discover;

    if (PTX_STATUS_SUCCESS != ptx_RUL_GetStatusInfo(&g_rul, &info))
    {
        return RS_ERR_INTERNAL;
    }

    switch (info.Info.Discover)
    {
        case ptx_RUL_StatusInfoDiscover_Active:               *out_status = RS_NFC_DISC_CARD_ACTIVE; break;
        case ptx_RUL_StatusInfoDiscover_CardDetectionOngoing: *out_status = RS_NFC_DISC_RUNNING;     break;
        case ptx_RUL_StatusInfoDiscover_CardsAvailable:       /* fall through: card(s) must be selected */
        case ptx_RUL_StatusInfoDiscover_MultiProtocol:        *out_status = RS_NFC_DISC_DONE;        break;
        default:                                              *out_status = RS_NFC_DISC_NO_CARD;     break;
    }

    return RS_OK;
}

static rs_status_t ptx205r_system_check (void)
{
    ptx_RUL_Status_Params_t info;
    (void)memset(&info, 0, sizeof(info));
    info.Type = ptx_RUL_StatusType_System;

    if (PTX_STATUS_SUCCESS != ptx_RUL_GetStatusInfo(&g_rul, &info))
    {
        return RS_ERR_INTERNAL;
    }

    if (ptx_RUL_StatusInfoSystem_ErrorStandby == info.Info.System)
    {
        /* Not an error: the PTX205R left discovery on its own (automatic
         * standby). Restart it with the session's configuration. */
        return (PTX_STATUS_SUCCESS == ptx_RUL_InitiateDiscovery(&g_rul, &g_disc_config)) ? RS_OK : RS_ERR_INTERNAL;
    }

    return (ptx_RUL_StatusInfoSystem_OK == info.Info.System) ? RS_OK : RS_ERR_INTERNAL;
}

static void ptx205r_deactivate_to (ptx_RUL_DeactivateState_t state)
{
    ptx_RUL_DeactivateParams_t params;
    (void)memset(&params, 0, sizeof(params));
    params.Type  = ptx_RUL_DeactivateType_Generic;
    params.State = state;
    (void)ptx_RUL_Deactivate(&g_rul, &params);
}

/***********************************************************************************************************************
 * PTX SDK function implementations
 **********************************************************************************************************************/

rs_status_t rs_nfc_ptx_open (void)
{
    /* New session: forget any abort request left over from a previous Stop(). */
    g_wait_abort = false;

    if (g_hw_ready)
    {
        /* SDK still initialized from a previous session: reuse it unless the
         * PTX205R reports a problem, in which case it is brought up again. */
        if (RS_OK == ptx205r_system_check())
        {
            g_ptx_opened = true;
            return RS_OK;
        }

        g_hw_ready = false;
    }

    ptx_RUL_InitParams_t init_params;
    (void)memset(&init_params, 0, sizeof(init_params));
    init_params.InterfaceInitParams.InterfaceType     = HAL_Interface_Type_SPI;
    init_params.InterfaceInitParams.InterfacePortType = HAL_DRV_Interface_Port_Type_Default;
    init_params.InterfaceInitParams.InterfacePortID   = PTX205R_INTERFACE_PORT_ID;
    init_params.InterfaceInitParams.NumberOfTimers    = PTX205R_NUMBER_OF_TIMERS;

    /* ptx_RUL_Init() clears g_rul and opens HAL (SPI/IRQ/GPIO/timers), HIP
     * and NSC. ptx_RUL_InitHardware() resets the PTX205R, downloads and
     * starts its firmware and loads the default RF configuration. */
    ptx_Status_t st = ptx_RUL_Init(&g_rul, &init_params);

    if (PTX_STATUS_SUCCESS == st)
    {
        st = ptx_RUL_InitHardware(&g_rul);

        if (PTX_STATUS_SUCCESS != st)
        {
            /* Retry once: the first SPI transfers after power-up may fail. */
            st = ptx_RUL_InitHardware(&g_rul);
        }
    }

    if (PTX_STATUS_SUCCESS == st)
    {
        st = ptx_RUL_GetCardRegistry(&g_rul, &g_registry);
    }

    if (PTX_STATUS_SUCCESS != st)
    {
        g_registry = NULL;
        return RS_ERR_INTERNAL;
    }

    g_hw_ready   = true;
    g_ptx_opened = true;
    return RS_OK;
}

rs_status_t rs_nfc_ptx_close (void)
{
    /* RF field off. The SDK (and the PTX205R firmware) stays initialized
     * for the next Read() — see g_hw_ready. */
    if (g_hw_ready)
    {
        ptx205r_deactivate_to(ptx_RUL_DeactivateState_Idle);
    }

    g_ptx_opened = false;
    return RS_OK;
}

bool rs_nfc_ptx_is_open (void)
{
    return g_ptx_opened;
}

rs_status_t rs_nfc_ptx_configure_discovery (rs_nfc_tech_mask_t tech_mask)
{
    g_tech_mask = tech_mask;
    return RS_OK;
}

rs_status_t rs_nfc_ptx_start_discovery (void)
{
    /* NFC Forum tag types span all four RF technologies, so that bit selects all of them. */
    rs_nfc_tech_mask_t mask = g_tech_mask;

    if (0U != (mask & RS_NFC_TECH_NFC_FORUM))
    {
        mask |= RS_NFC_TECH_ISO14443A | RS_NFC_TECH_ISO14443B | RS_NFC_TECH_FELICA | RS_NFC_TECH_ISO15693;
    }

    ptx_RUL_DiscoverConfig_t * cfg = &g_disc_config;
    (void)memset(cfg, 0, sizeof(*cfg));

    cfg->PollMode      = ptx_RUL_PollMode_NfcForum;
    cfg->EnablePolling = 1U;
    cfg->IdleTime      = RS_NFC_PTX205R_IDLE_TIME_MS;

    if (0U != (mask & RS_NFC_TECH_ISO14443A))
    {
        cfg->PollTypeA              = 1U;
        cfg->PollTypeAPollFrequency = ptx_RUL_PollFreq_EveryCycle;
        cfg->PollTypeADeviceLimit   = RS_NFC_PTX205R_DEVICE_LIMIT;
    }

    if (0U != (mask & RS_NFC_TECH_ISO14443B))
    {
        cfg->PollTypeB              = 1U;
        cfg->PollTypeBPollFrequency = ptx_RUL_PollFreq_EveryCycle;
        cfg->PollTypeBDeviceLimit   = RS_NFC_PTX205R_DEVICE_LIMIT;
    }

    if (0U != (mask & RS_NFC_TECH_FELICA))
    {
        cfg->PollTypeF212           = 1U;
        cfg->PollTypeFPollFrequency = ptx_RUL_PollFreq_EveryCycle;
        cfg->PollTypeFDeviceLimit   = RS_NFC_PTX205R_DEVICE_LIMIT;
    }

    if (0U != (mask & RS_NFC_TECH_ISO15693))
    {
        cfg->PollTypeV              = 1U;
        cfg->PollTypeVPollFrequency = ptx_RUL_PollFreq_EveryCycle;
        cfg->PollTypeVDeviceLimit   = RS_NFC_PTX205R_DEVICE_LIMIT;
    }

    if (!(cfg->PollTypeA || cfg->PollTypeB || cfg->PollTypeF212 || cfg->PollTypeV))
    {
        return RS_ERR_INVALID_CFG;
    }

    ptx_Status_t st = ptx_RUL_InitiateDiscovery(&g_rul, cfg);
    return (PTX_STATUS_SUCCESS == st) ? RS_OK : RS_ERR_INTERNAL;
}

rs_status_t rs_nfc_ptx_stop_discovery (void)
{
    ptx205r_deactivate_to(ptx_RUL_DeactivateState_Idle);
    return RS_OK;
}

rs_status_t rs_nfc_ptx_wait_for_card (uint32_t timeout_ms, rs_nfc_ptx_disc_status_t * out_status)
{
    if (NULL == out_status) { return RS_ERR_INVALID_CFG; }
    *out_status = RS_NFC_DISC_NO_CARD;

    rs_status_t st = ptx205r_system_check();
    if (RS_OK != st) { return st; }

    const bool forever     = (UINT32_MAX == timeout_ms);
    const TickType_t total = forever ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    const TickType_t start = xTaskGetTickCount();

    /* Publish the waiter and clear stale notifications BEFORE looking at
     * the IRQ line: an IRQ edge (or Stop()) after that point is never lost. */
    g_waiting_task = xTaskGetCurrentTaskHandle();
    (void)ptx_pmod2_irq.p_api->callbackSet(ptx_pmod2_irq.p_ctrl, ptx205r_irq_wake_cb, NULL, NULL);
    (void)ulTaskNotifyTake(pdTRUE, 0);

    for (;;)
    {
        /* Stop() requested: return RS_OK with *out_status == NO_CARD. */
        if (g_wait_abort) { break; }

        /* Processes one pending PTX205R message (if any). */
        st = ptx205r_discover_status(out_status);
        if (RS_OK != st) { break; }

        if (RS_NFC_DISC_NO_CARD != *out_status)
        {
            break;
        }

        /* More messages queued: keep draining without sleeping. */
        uint8_t pending = 0U;
        if ((PTX_STATUS_SUCCESS == ptx_HAL_Interface_RxPending(&g_rul.Hal, &pending)) && (0U != pending))
        {
            continue;
        }

        TickType_t wait = portMAX_DELAY;

        if (!forever)
        {
            TickType_t elapsed = xTaskGetTickCount() - start;
            if (elapsed >= total) { break; }
            wait = total - elapsed;
        }

#if (RS_NFC_PTX205R_WAIT_RECHECK_MS > 0U)
        if (wait > pdMS_TO_TICKS(RS_NFC_PTX205R_WAIT_RECHECK_MS))
        {
            wait = pdMS_TO_TICKS(RS_NFC_PTX205R_WAIT_RECHECK_MS);
        }
#endif

        /* Sleep (0% CPU) until the PTX205R raises IRQ7 or Stop() wakes us. */
        (void)ulTaskNotifyTake(pdTRUE, wait);
    }

    g_waiting_task = NULL;
    return st;
}

rs_status_t rs_nfc_ptx_activate_card (rs_nfc_ptx_card_info_t * card_info)
{
    if (NULL == card_info) { return RS_ERR_INVALID_CFG; }
    (void)memset(card_info, 0, sizeof(*card_info));

    if (NULL == g_registry) { return RS_ERR_INTERNAL; }

    card_info->cards_in_field = (uint8_t) ((g_registry->CardsCnt > UINT8_MAX) ? UINT8_MAX : g_registry->CardsCnt);

    if (NULL == active_card())
    {
        /* Discovery resolved one or more cards (or a multi-protocol card)
         * without activating one: select the first card in the registry. */
        if (0U == g_registry->CardsCnt) { return RS_ERR_NOT_FOUND; }

        ptx_RUL_CardProtocol_t prot;
        (void)memset(&prot, 0, sizeof(prot));
        prot.Type = choose_protocol(&g_registry->Cards[0]);

        if (PTX_STATUS_SUCCESS != ptx_RUL_SelectCard(&g_rul, 0U, prot)) { return RS_ERR_INTERNAL; }
        if (NULL == active_card()) { return RS_ERR_INTERNAL; }
    }

    if (0U == card_info->cards_in_field)
    {
        card_info->cards_in_field = 1U;
    }

    card_info->card_type = map_card_type(active_card(), g_registry->ActiveProtocol);
    card_info->protocol  = g_registry->ActiveProtocol;
    extract_uid(active_card(), card_info->uid, &card_info->uid_len);
    return RS_OK;
}

rs_status_t rs_nfc_ptx_get_card_type (rs_nfc_card_type_t * out_type)
{
    if (NULL == out_type) { return RS_ERR_INVALID_CFG; }

    if (NULL == active_card())
    {
        *out_type = RS_NFC_CARD_TYPE_UNKNOWN;
        return RS_ERR_NOT_FOUND;
    }

    *out_type = map_card_type(active_card(), g_registry->ActiveProtocol);
    return RS_OK;
}

rs_status_t rs_nfc_ptx_get_uid (uint8_t * uid, uint8_t * uid_len)
{
    if ((NULL == uid) || (NULL == uid_len)) { return RS_ERR_INVALID_CFG; }

    if (NULL == active_card())
    {
        *uid_len = 0;
        return RS_ERR_NOT_FOUND;
    }

    extract_uid(active_card(), uid, uid_len);
    return RS_OK;
}

void rs_nfc_ptx_sleep (uint32_t ms)
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        vTaskDelay(pdMS_TO_TICKS(ms));
    }
    else
    {
        R_BSP_SoftwareDelay(ms, BSP_DELAY_UNITS_MILLISECONDS);
    }
}

rs_status_t rs_nfc_ptx_data_exchange (const uint8_t * tx, uint32_t tx_len,
                                      uint8_t * rx, uint32_t * rx_len)
{
    if ((NULL == tx) || (NULL == rx) || (NULL == rx_len)) { return RS_ERR_INVALID_CFG; }

    ptx_Status_t st = ptx_RUL_Exchange(&g_rul, (uint8_t *)(uintptr_t)tx, tx_len,
                                       rx, rx_len, PTX205R_TIMEOUT_RAW);

    return (PTX_STATUS_SUCCESS == st) ? RS_OK : RS_ERR_INTERNAL;
}

rs_status_t rs_nfc_ptx_deactivate (void)
{
    if (!g_hw_ready) { return RS_ERR_INTERNAL; }

    /* Short RF reset and back to the polling loop. */
    ptx_RUL_DeactivateParams_t params;
    (void)memset(&params, 0, sizeof(params));
    params.Type  = ptx_RUL_DeactivateType_Generic;
    params.State = ptx_RUL_DeactivateState_DiscoveryRfReset;

    ptx_Status_t st = ptx_RUL_Deactivate(&g_rul, &params);
    return (PTX_STATUS_SUCCESS == st) ? RS_OK : RS_ERR_INTERNAL;
}

rs_status_t rs_nfc_ptx_get_system_state (uint8_t * out_state)
{
    if (NULL == out_state) { return RS_ERR_INVALID_CFG; }

    ptx_RUL_Status_Params_t info;
    (void)memset(&info, 0, sizeof(info));
    info.Type = ptx_RUL_StatusType_System;

    ptx_Status_t st = ptx_RUL_GetStatusInfo(&g_rul, &info);

    /* Automatic-standby exit is informational, not a system error. */
    *out_state = (ptx_RUL_StatusInfoSystem_ErrorStandby == info.Info.System)
                 ? (uint8_t) RS_NFC_PTX_SYS_STATUS_OK : (uint8_t) info.Info.System;
    return (PTX_STATUS_SUCCESS == st) ? RS_OK : RS_ERR_INTERNAL;
}

rs_status_t rs_nfc_ptx_get_last_rf_error (uint8_t * out_err)
{
    if (NULL == out_err) { return RS_ERR_INVALID_CFG; }

    ptx_RUL_Status_Params_t info;
    (void)memset(&info, 0, sizeof(info));
    info.Type = ptx_RUL_StatusType_LastRfError;

    ptx_Status_t st = ptx_RUL_GetStatusInfo(&g_rul, &info);
    *out_err = (uint8_t) info.Info.RfError;
    return (PTX_STATUS_SUCCESS == st) ? RS_OK : RS_ERR_INTERNAL;
}

void rs_nfc_ptx_wake_waiting_task (void)
{
    /* Flag first, then notify: the waiter checks the flag after publishing
     * g_waiting_task, so no ordering of the two can lose the request. */
    g_wait_abort = true;

    TaskHandle_t task = g_waiting_task;
    if (NULL != task)
    {
        (void)xTaskNotifyGive(task);
    }
}

/***********************************************************************************************************************
 * SDK T4T NDEF component lifecycle
 *
 * Call rs_nfc_ptx_ndef_open() once per card activation (protocol ==
 * ptx_RUL_CardProtocolType_T4T), and rs_nfc_ptx_ndef_close() after NDEF
 * operations are finished.
 **********************************************************************************************************************/

rs_status_t rs_nfc_ptx_ndef_open (void)
{
    ptx_NDEF_T4T_InitParams_t params;
    (void)memset(&params, 0, sizeof(params));

    params.rul          = &g_rul;
    params.RxBuffer     = g_ndef_rx_buf;
    params.RxBufferSize = sizeof(g_ndef_rx_buf);
    params.TxBuffer     = g_ndef_tx_buf;
    params.TxBufferSize = sizeof(g_ndef_tx_buf);

    ptx_Status_t st = ptx_NDEF_T4T_Open(&g_t4t_ndef_comp, &params);
    return (PTX_STATUS_SUCCESS == st) ? RS_OK : RS_ERR_INTERNAL;
}

void rs_nfc_ptx_ndef_close (void)
{
    (void)ptx_NDEF_T4T_Close(&g_t4t_ndef_comp);
}

ptx_NDEF_T4T_t * rs_nfc_ptx_get_ndef_comp (void)
{
    return &g_t4t_ndef_comp;
}

#if !defined(PTX_DISABLE_NDEF_T5T)
/***********************************************************************************************************************
 * SDK T5T NDEF component lifecycle
 *
 * Call rs_nfc_ptx_ndef_t5t_open() once per card activation (protocol ==
 * ptx_RUL_CardProtocolType_T5T), and rs_nfc_ptx_ndef_t5t_close() after NDEF
 * operations are finished.
 **********************************************************************************************************************/

rs_status_t rs_nfc_ptx_ndef_t5t_open (void)
{
    ptx_NDEF_T5T_InitParams_t params;
    (void)memset(&params, 0, sizeof(params));

    params.rul            = &g_rul;
    params.RxBuffer       = g_ndef_rx_buf;
    params.RxBufferSize   = sizeof(g_ndef_rx_buf);
    params.TxBuffer       = g_ndef_tx_buf;
    params.TxBufferSize   = sizeof(g_ndef_tx_buf);
    params.WorkBuffer     = g_t5t_work_buf;
    params.WorkBufferSize = sizeof(g_t5t_work_buf);
    /* Addressed mode: the SDK takes the UID from the active card. */
    params.isSelected     = 0U;
    params.isAddressed    = 1U;

    ptx_Status_t st = ptx_NDEF_T5T_Open(&g_t5t_ndef_comp, &params);
    return (PTX_STATUS_SUCCESS == st) ? RS_OK : RS_ERR_INTERNAL;
}

void rs_nfc_ptx_ndef_t5t_close (void)
{
    (void)ptx_NDEF_T5T_Close(&g_t5t_ndef_comp);
}

ptx_NDEF_T5T_t * rs_nfc_ptx_get_ndef_t5t_comp (void)
{
    return &g_t5t_ndef_comp;
}
#endif /* PTX_DISABLE_NDEF_T5T */

#if !defined(PTX_DISABLE_NDEF_T3T)
/***********************************************************************************************************************
 * SDK T3T NDEF component lifecycle
 *
 * Call rs_nfc_ptx_ndef_t3t_open() once per card activation (protocol ==
 * ptx_RUL_CardProtocolType_T3T), and rs_nfc_ptx_ndef_t3t_close() after NDEF
 * operations are finished. The SDK reads NFCID2 and MRTI timing parameters
 * from the active card's SENSF_RES — the card must already be activated.
 **********************************************************************************************************************/

rs_status_t rs_nfc_ptx_ndef_t3t_open (void)
{
    /* Guard: need an activated FeliCa / T3T card in the registry */
    if (NULL == active_card())
    {
        return RS_ERR_NOT_FOUND;
    }

    ptx_NDEF_T3T_InitParams_t params;
    (void)memset(&params, 0, sizeof(params));

    params.rul          = &g_rul;
    params.RxBuffer     = g_ndef_rx_buf;
    params.RxBufferSize = sizeof(g_ndef_rx_buf);
    params.TxBuffer     = g_ndef_tx_buf;
    params.TxBufferSize = sizeof(g_ndef_tx_buf);

    ptx_Status_t st = ptx_NDEF_T3T_Open(&g_t3t_ndef_comp, &params);

    return (PTX_STATUS_SUCCESS == st) ? RS_OK : RS_ERR_INTERNAL;
}

void rs_nfc_ptx_ndef_t3t_close (void)
{
    (void)ptx_NDEF_T3T_Close(&g_t3t_ndef_comp);
}

ptx_NDEF_T3T_t * rs_nfc_ptx_get_ndef_t3t_comp (void)
{
    return &g_t3t_ndef_comp;
}
#endif /* PTX_DISABLE_NDEF_T3T */
