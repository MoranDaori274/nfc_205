/* generated thread header file - do not edit */
#ifndef NFC_THREAD_H_
#define NFC_THREAD_H_
#include "bsp_api.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void nfc_thread_entry(void * pvParameters);
                #else
extern void nfc_thread_entry(void *pvParameters);
#endif
FSP_HEADER
FSP_FOOTER
#endif /* NFC_THREAD_H_ */
