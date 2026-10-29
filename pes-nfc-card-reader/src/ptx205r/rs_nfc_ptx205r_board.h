/**
 * rs_nfc_ptx205r_board.h
 *
 * Integrator-supplied board symbols and discovery tuning used by
 * rs_nfc_ptx205r.c.
 *
 * The PTX205R SDK (<repo>/ptx_sdk) is not an FSP module, so there is no
 * FSP-generated g_nfc_reader_ptx0_cfg. Its RA HAL port
 * (ptx_sdk/SRC/COMPS/HAL/DRV/MCU/RA4M2, adapted to the FPB-RA2E3) binds to
 * these FSP instances by name instead, so the FSP configuration must provide
 * them:
 *
 *   ptx_pmod2_spi   r_sci_spi    SCI0 simple SPI (P100 MISO, P101 MOSI, P102 SCK;
 *                                P103 is the SS line, driven as a GPIO)
 *   ptx_pmod2_irq   r_icu        IRQ7 on P015 (PTX205R IRQ), rising edge
 *   ptx_tmr0/1      r_gpt        two timers used for SDK timeouts / sleeps
 *   g_ioport        r_ioport     P915 is the PTX205R SEN (reset) line
 *
 * Only ptx_pmod2_irq is referenced by rs_nfc_ptx205r.c itself: it installs
 * its own ICU callback while waiting for a card, so the read loop sleeps
 * until the PTX205R raises its IRQ line.
 */

#ifndef RS_NFC_PTX205R_BOARD_H
#define RS_NFC_PTX205R_BOARD_H

#include "hal_data.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const external_irq_instance_t ptx_pmod2_irq;

/* Pause between polling-loop cycles while no card is in the field. */
#ifndef RS_NFC_PTX205R_IDLE_TIME_MS
#define RS_NFC_PTX205R_IDLE_TIME_MS        (100U)
#endif

/* Maximum number of cards resolved per technology during discovery. */
#ifndef RS_NFC_PTX205R_DEVICE_LIMIT
#define RS_NFC_PTX205R_DEVICE_LIMIT        (5U)
#endif

/* Safety re-check period of rs_nfc_ptx_wait_for_card(). The wait is woken
 * by the PTX205R IRQ; this bound only matters if an IRQ edge is ever lost
 * (e.g. P015 not muxed to IRQ7). 0 = block on the IRQ only. */
#ifndef RS_NFC_PTX205R_WAIT_RECHECK_MS
#define RS_NFC_PTX205R_WAIT_RECHECK_MS     (100U)
#endif

#ifdef __cplusplus
}
#endif

#endif /* RS_NFC_PTX205R_BOARD_H */
