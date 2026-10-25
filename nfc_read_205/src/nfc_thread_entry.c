/*
* Copyright (c) 2020 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "nfc_thread.h"

/* Entry point of the PTX205R NFC Forum NDEF example application. */
extern int ptx_App_Entry(void);

/* NFC Thread entry function */
void nfc_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED(pvParameters);

    (void) ptx_App_Entry();

    /* ptx_App_Entry() normally runs its own loop. If it ever returns, block here. */
    while (1)
    {
        vTaskDelay(portMAX_DELAY);
    }
}
