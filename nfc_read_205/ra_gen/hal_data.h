/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_gpt.h"
#include "r_timer_api.h"
#include "r_sci_uart.h"
#include "r_uart_api.h"
#include "r_sci_spi.h"
#include "r_spi_api.h"
FSP_HEADER
/** Timer on GPT Instance. */
extern const timer_instance_t ptx_tmr1;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t ptx_tmr1_ctrl;
extern const timer_cfg_t ptx_tmr1_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t ptx_tmr0;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t ptx_tmr0_ctrl;
extern const timer_cfg_t ptx_tmr0_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
/** UART on SCI Instance. */
extern const uart_instance_t ptx_uart_dbg;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_uart_instance_ctrl_t ptx_uart_dbg_ctrl;
extern const uart_cfg_t ptx_uart_dbg_cfg;
extern const sci_uart_extended_cfg_t ptx_uart_dbg_cfg_extend;

#ifndef NULL
void NULL(uart_callback_args_t *p_args);
#endif
/** SPI on SCI Instance. */
extern const spi_instance_t ptx_pmod2_spi;

/** Access the SCI_SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_spi_instance_ctrl_t ptx_pmod2_spi_ctrl;
extern const spi_cfg_t ptx_pmod2_spi_cfg;

/** Called by the driver when a transfer has completed or an error has occurred (Must be implemented by the user). */
#ifndef NULL
void NULL(spi_callback_args_t *p_args);
#endif
void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER
#endif /* HAL_DATA_H_ */
