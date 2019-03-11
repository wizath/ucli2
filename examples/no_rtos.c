#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// example handler from nrf51 series
void uart_event_handle(app_uart_evt_t * p_event)
{
    unsigned char ch;
    switch (p_event->evt_type)
    {
        case APP_UART_DATA_READY:
            nrf_gpio_pin_toggle(21);
            UNUSED_VARIABLE(app_uart_get(&ch));
            ucli_process_chr(ch);
            break;
        case APP_UART_DATA:
        case APP_UART_TX_EMPTY:
        case APP_UART_COMMUNICATION_ERROR:
        case APP_UART_FIFO_ERROR:
            break;
    }
}

int __io_putchar(char c)
{
    app_uart_put(c);
    return c;
}

static void fh_hello(void * a_data) {
    printf("hello_world\r\n");
}

static void fh_hi(void * a_data) {
    printf("hi\r\n");
}

static ucli_cmd_t g_cmds[] = {
    { "hw", fh_hello, 0x00, "Prints hello world\r\n", "Test help command\r\n" },
    { "hi", fh_hi, 0x01 },

    // null
    { 0x00, 0x00, 0x00  }
};

int main(void)
{
    LEDS_CONFIGURE(LEDS_MASK);
    uart_init();    

    ucli_init((void*) __io_putchar, g_cmds);
    ucli_log(UCLI_LOG_INFO, "device boot\r\n");

    while (true)
    {
        nrf_gpio_pin_toggle(22);
        nrf_delay_ms(500);
    }
}