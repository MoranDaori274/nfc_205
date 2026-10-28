/*
 * console_io.h
 *
 * Minimal console I/O over the FSP SCI UART (g_uart_jlob_vcom).
 * Replaces the picolibc stdio retarget (rs-console-io): no libc printf /
 * getchar, no stdout/stdin FILE, no byte queue module, no malloc.
 *
 * Output - console_io_printf() supports: %c %s %d %i %u %x %X %%
 *   flags  : '0' (zero pad)
 *   width  : decimal field width (numbers only), e.g. %02X, %5u
 *   length : 'l' / 'h' accepted and ignored (32-bit target: long == int)
 * Not supported: floating point, 64-bit, precision, '-' / '+' flags.
 * Output is buffered per call and sent with one UART write when the call
 * returns (or when the internal buffer fills). The call blocks until the
 * bytes have left the UART. If the UART cannot be opened, a write fails, or
 * TX-complete does not arrive in time (the transfer is then aborted), the
 * affected output is dropped and the call returns -1.
 *
 * Input - received bytes are stored by the UART RX interrupt in a ring
 * buffer (CONSOLE_IO_RX_BUF_SIZE); console_io_getc() takes them out. While
 * the buffer is empty the calling task blocks on a FreeRTOS binary semaphore
 * that the RX interrupt gives, so it uses no CPU while waiting. If input
 * is lost (buffer full or UART receive error), console_io_getc() returns
 * CONSOLE_IO_RX_LOST at the point in the stream where the gap occurred.
 *
 * Not re-entrant: use from one task at a time and never from an ISR.
 */

#ifndef CONSOLE_IO_H_
#define CONSOLE_IO_H_

#include "bsp_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/** console_io_getc() result: received input was lost before this point. */
#define CONSOLE_IO_RX_LOST    (-2)

/**
 * Open the UART and install the console callback (done lazily by every other
 * console_io function too). Returns FSP_SUCCESS, or the FSP error of the
 * failed open()/callbackSet(). The first failure is latched: the UART is
 * closed again and later calls return the same error without retrying, so
 * all console_io output is dropped and console_io_getc() returns -1.
 */
fsp_err_t console_io_open(void);

/**
 * Formatted print to the UART console (lazy-opens the UART on first use).
 * Returns the number of characters sent, or -1 if any output was lost.
 */
int console_io_printf(const char * p_format, ...) __attribute__((format(printf, 1, 2)));

/**
 * Send one character immediately (e.g. local echo).
 * Returns the character (0..255), or -1 if it was not sent.
 */
int console_io_putc(char c);

/**
 * Block until one character is received on the UART and return it (0..255).
 * The calling task sleeps while no input is available (it polls only if
 * called before the FreeRTOS scheduler has started).
 * Opens the UART on first use, so reception starts with the first call to
 * any console_io function.
 * Returns CONSOLE_IO_RX_LOST once after input was lost (all characters
 * received before the loss are returned first), or -1 if the UART cannot be
 * opened.
 */
int console_io_getc(void);

#ifdef __cplusplus
}
#endif

#endif /* CONSOLE_IO_H_ */
