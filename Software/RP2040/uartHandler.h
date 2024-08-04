#ifndef __UART_HANDLER__
#define __UART_HANDLER__

#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/uart.h"


#define UART_PORT           uart1
#define UART_COMMAND        uart0
#define BAUD_RATE           115200             //UART à 115200 bauds
#define DATA_BITS           8
#define STOP_BITS           1
#define PARITY              UART_PARITY_NONE
#define UART_MAX_LENGTH     512
#define UART_FLAG_RX        1
#define UART_FLAG_TX        2
#define UART_MAIN_PROCESS   3

class c_uartHandler
{
    public:
        uint8_t uart_rcv_buf_first[UART_MAX_LENGTH] = {0};
        uint8_t uart_rcv_buf_first_USB[UART_MAX_LENGTH] = {0};
        uint16_t uart_rcv_cpt;
        uint16_t uart_rcv_cpt_USB;
        uint8_t* double_pointer = &uart_rcv_buf_first[0];
        uint8_t* double_pointer_USB = &uart_rcv_buf_first_USB[0];

        volatile uint8_t UART_FLAG;
        volatile uint8_t UART_FLAG_USB;
        uint8_t ch;
        uint8_t ch_USB;
        uint8_t okaybecauseCommand;
        int UART_IRQ;
        

        c_uartHandler();
        void _uart_init();
        void callback_uart_rx();
        void _uart_write(uint8_t* buf, uint8_t len);
        void _uart_mainLoop();
        void _uart_get_from_USB();
};

#endif