/*
 * console_io.c
 *
 * Minimal console I/O (printf-style output + character input) over the
 * FSP SCI UART. See console_io.h.
 */
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "hal_data.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "console_io.h"

/* Bytes buffered before a UART write is issued (one write per call if shorter). */
#define CONSOLE_IO_BUF_SIZE        (64u)

/* Busy-wait guard so a missing TX-complete IRQ can never hang the caller. */
#define CONSOLE_IO_TX_WAIT_LOOPS   (0x00100000u)

/* RX ring buffer size in bytes; must be a power of two (<= 65536). One slot is
 * kept free, so it holds up to (size - 1) unread characters. Sized so that a
 * full-length CLI line (255 characters + CR/LF) typed or pasted while the
 * application is busy fits, with room to spare. */
#define CONSOLE_IO_RX_BUF_SIZE     (512u)
#define CONSOLE_IO_RX_MASK         (CONSOLE_IO_RX_BUF_SIZE - 1u)

#if (0u != (CONSOLE_IO_RX_BUF_SIZE & CONSOLE_IO_RX_MASK))
 #error "CONSOLE_IO_RX_BUF_SIZE must be a power of two"
#endif

/* UART receive errors: the received character is missing or corrupted. */
#define CONSOLE_IO_RX_ERR_EVENTS   (UART_EVENT_ERR_PARITY | UART_EVENT_ERR_FRAMING | UART_EVENT_ERR_OVERFLOW)

static uint8_t       s_buf[CONSOLE_IO_BUF_SIZE];
static uint32_t      s_len      = 0u;
static bool          s_open     = false;
static fsp_err_t     s_open_err = FSP_SUCCESS;  /* first UART init error, latched */
static volatile bool s_tx_busy  = false;  /* true while the UART may still read s_buf */
static bool          s_tx_lost  = false;  /* output dropped since the last public call */
static uint32_t      s_tx_count = 0u;     /* characters accepted since the last public call */

static volatile uint8_t  s_rx_buf[CONSOLE_IO_RX_BUF_SIZE];
static volatile uint16_t s_rx_head = 0u;  /* written only by the RX interrupt */
static volatile uint16_t s_rx_tail = 0u;  /* written only by console_io_getc() */

/* Set by the RX interrupt when input is lost (ring full or UART receive
 * error); cleared only by console_io_getc() once it has reported the loss.
 * While it is set the interrupt stores nothing, so the gap is always at the
 * end of the buffered data and is reported in stream order. */
static volatile bool     s_rx_lost = false;

/* Given by the RX interrupt whenever it stores a character or records an input
 * loss; console_io_getc() blocks on it while the ring is empty, so the waiting
 * task does not spin and other tasks can run. A dedicated binary semaphore is
 * used instead of a task notification because notification index 0 of the
 * calling task is already used by the PTX105R IRQ wait in the NFC module
 * (configTASK_NOTIFICATION_ARRAY_ENTRIES is 1), and a stray console wake-up
 * there would end that wait early. Created in console_open() before the UART
 * is opened, so the interrupt never sees it uninitialised. */
static StaticSemaphore_t s_rx_sem_buf;
static SemaphoreHandle_t s_rx_sem = NULL;

/* Wake a task blocked in console_io_getc(). Called from the UART interrupt. */
static void console_rx_signal_from_isr (void)
{
    if (NULL != s_rx_sem)
    {
        BaseType_t higher_prio_task_woken = pdFALSE;

        (void) xSemaphoreGiveFromISR(s_rx_sem, &higher_prio_task_woken);
        portYIELD_FROM_ISR(higher_prio_task_woken);
    }
}

static void console_uart_callback (uart_callback_args_t * p_args)
{
    if (UART_EVENT_TX_COMPLETE == p_args->event)
    {
        s_tx_busy = false;
    }
    else if (UART_EVENT_RX_CHAR == p_args->event)
    {
        uint16_t next = (uint16_t) ((s_rx_head + 1u) & CONSOLE_IO_RX_MASK);

        if (s_rx_lost)
        {
            /* A loss is still waiting to be reported: drop. */
        }
        else if (next == s_rx_tail)
        {
            s_rx_lost = true;   /* ring full: this character is lost */
            console_rx_signal_from_isr();
        }
        else
        {
            s_rx_buf[s_rx_head] = (uint8_t) p_args->data;
            s_rx_head           = next;
            console_rx_signal_from_isr();
        }
    }
    else if (0u != ((uint32_t) p_args->event & (uint32_t) CONSOLE_IO_RX_ERR_EVENTS))
    {
        if (!s_rx_lost)
        {
            s_rx_lost = true;
            console_rx_signal_from_isr();
        }
    }
    else
    {
        /* Other events are ignored. */
    }
}

static bool console_open (void)
{
    if (s_open)
    {
        return true;
    }

    /* Initialisation already failed: do not retry on every call. */
    if (FSP_SUCCESS != s_open_err)
    {
        return false;
    }

    /* Static allocation: never fails. Created before the UART is opened, so
     * the RX interrupt can always signal it. */
    if (NULL == s_rx_sem)
    {
        s_rx_sem = xSemaphoreCreateBinaryStatic(&s_rx_sem_buf);
    }

    fsp_err_t err = g_uart_jlob_vcom.p_api->open(g_uart_jlob_vcom.p_ctrl, g_uart_jlob_vcom.p_cfg);

    if (FSP_SUCCESS == err)
    {
        err = g_uart_jlob_vcom.p_api->callbackSet(g_uart_jlob_vcom.p_ctrl, console_uart_callback, NULL, NULL);

        if (FSP_SUCCESS != err)
        {
            /* Do not leave the UART open without our callback. */
            (void) g_uart_jlob_vcom.p_api->close(g_uart_jlob_vcom.p_ctrl);
        }
    }

    if (FSP_SUCCESS != err)
    {
        s_open_err = err;

        return false;
    }

    s_open = true;

    return true;
}

fsp_err_t console_io_open (void)
{
    return console_open() ? FSP_SUCCESS : s_open_err;
}

/* Wait up to 'loops' for the TX-complete IRQ; if it does not arrive, abort the
 * transfer. Returns true when s_buf is no longer used by the UART. If the abort
 * fails, s_tx_busy stays set so s_buf remains protected. */
static bool console_tx_release (uint32_t loops)
{
    for (uint32_t n = 0u; s_tx_busy && (n < loops); n++)
    {
    }

    if (!s_tx_busy)
    {
        return true;
    }

    /* Timed out: the transfer did not complete, so its output is incomplete. */
    s_tx_lost = true;

    if (FSP_SUCCESS == g_uart_jlob_vcom.p_api->communicationAbort(g_uart_jlob_vcom.p_ctrl, UART_DIR_TX))
    {
        s_tx_busy = false;
    }

    return !s_tx_busy;
}

static void console_flush (void)
{
    if (0u == s_len)
    {
        return;
    }

    if (console_open())
    {
        s_tx_busy = true;

        if (FSP_SUCCESS == g_uart_jlob_vcom.p_api->write(g_uart_jlob_vcom.p_ctrl, s_buf, s_len))
        {
            /* s_buf must stay untouched until the UART has finished with it. */
            (void) console_tx_release(CONSOLE_IO_TX_WAIT_LOOPS);
        }
        else
        {
            /* No transfer was started, so s_buf is free again. */
            s_tx_busy = false;
            s_tx_lost = true;
        }
    }
    else
    {
        s_tx_lost = true;
    }

    s_len = 0u;
}

static void console_putc (char c)
{
    /* A previous transfer that could not be aborted may still own s_buf.
     * Retry the abort (no further waiting) and drop the character if the
     * buffer is still not free. */
    if (s_tx_busy && !console_tx_release(0u))
    {
        return;
    }

    s_buf[s_len] = (uint8_t) c;
    s_len++;
    s_tx_count++;

    if (s_len >= CONSOLE_IO_BUF_SIZE)
    {
        console_flush();
    }
}

static void console_puts (const char * p_str)
{
    if (NULL == p_str)
    {
        p_str = "(null)";
    }

    while ('\0' != *p_str)
    {
        console_putc(*p_str);
        p_str++;
    }
}

static void console_put_num (uint32_t value, uint32_t base, bool upper, uint32_t width, char pad, bool negative)
{
    static const char digits_lc[] = "0123456789abcdef";
    static const char digits_uc[] = "0123456789ABCDEF";
    const char *      p_digits    = upper ? digits_uc : digits_lc;
    char              tmp[10];    /* 0xFFFFFFFF = 4294967295: 10 digits max */
    uint32_t          n = 0u;

    do
    {
        tmp[n] = p_digits[value % base];
        n++;
        value /= base;
    } while (0u != value);

    if (negative)
    {
        width = (width > 0u) ? (width - 1u) : 0u;

        if ('0' == pad)
        {
            console_putc('-');
        }
    }

    while (width > n)
    {
        console_putc(pad);
        width--;
    }

    if (negative && (' ' == pad))
    {
        console_putc('-');
    }

    while (n > 0u)
    {
        n--;
        console_putc(tmp[n]);
    }
}

int console_io_printf (const char * p_format, ...)
{
    va_list args;

    if (NULL == p_format)
    {
        return -1;
    }

    s_tx_lost  = false;
    s_tx_count = 0u;

    va_start(args, p_format);

    for (const char * p = p_format; '\0' != *p; p++)
    {
        if ('%' != *p)
        {
            console_putc(*p);
            continue;
        }

        p++;

        char     pad   = ' ';
        uint32_t width = 0u;

        if ('0' == *p)
        {
            pad = '0';
            p++;
        }

        while ((*p >= '0') && (*p <= '9'))
        {
            width = (width * 10u) + (uint32_t) (*p - '0');
            p++;
        }

        while (('l' == *p) || ('h' == *p))
        {
            p++;
        }

        switch (*p)
        {
            case 'c':
            {
                console_putc((char) va_arg(args, int));
                break;
            }

            case 's':
            {
                console_puts(va_arg(args, const char *));
                break;
            }

            case 'd':
            case 'i':
            {
                int32_t v = (int32_t) va_arg(args, int);
                console_put_num((v < 0) ? (0u - (uint32_t) v) : (uint32_t) v, 10u, false, width, pad, v < 0);
                break;
            }

            case 'u':
            {
                console_put_num((uint32_t) va_arg(args, unsigned int), 10u, false, width, pad, false);
                break;
            }

            case 'x':
            case 'X':
            {
                console_put_num((uint32_t) va_arg(args, unsigned int), 16u, ('X' == *p), width, pad, false);
                break;
            }

            case '%':
            {
                console_putc('%');
                break;
            }

            case '\0':
            {
                /* Trailing '%': stop (step back so the loop terminates). */
                p--;
                break;
            }

            default:
            {
                /* Unknown conversion: print it verbatim. */
                console_putc('%');
                console_putc(*p);
                break;
            }
        }
    }

    va_end(args);

    console_flush();

    return s_tx_lost ? -1 : (int) s_tx_count;
}

int console_io_putc (char c)
{
    s_tx_lost = false;

    console_putc(c);
    console_flush();

    return s_tx_lost ? -1 : (int) (uint8_t) c;
}

int console_io_getc (void)
{
    if (!console_open())
    {
        return -1;
    }

    for (;;)
    {
        /* Read the flag before checking for data. Once it is set the RX
         * interrupt stores nothing more, so if the buffer is then empty every
         * character received before the loss has already been returned. */
        bool lost = s_rx_lost;

        if (s_rx_head != s_rx_tail)
        {
            uint8_t c = s_rx_buf[s_rx_tail];
            s_rx_tail = (uint16_t) ((s_rx_tail + 1u) & CONSOLE_IO_RX_MASK);

            return (int) c;
        }

        if (lost)
        {
            s_rx_lost = false;

            return CONSOLE_IO_RX_LOST;
        }

        /* Nothing to return yet: block until the RX interrupt signals a new
         * character or an input loss, then check again. If it fired after the
         * checks above, the semaphore is already given and this returns at
         * once, so no wake-up is missed. Before the scheduler starts there is
         * no task to block, so keep polling. */
        if (taskSCHEDULER_RUNNING == xTaskGetSchedulerState())
        {
            (void) xSemaphoreTake(s_rx_sem, portMAX_DELAY);
        }
    }
}
