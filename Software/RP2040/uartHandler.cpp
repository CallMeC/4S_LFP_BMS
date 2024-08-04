#include "uartHandler.h"
#include "appVariable.h"
#include "pico/stdlib.h"

#include "hardware/gpio.h"
#include "IO_Define.h"
#include <string.h>

c_uartHandler::c_uartHandler()
{
    UART_FLAG = 0;
    ch = 0;
    UART_IRQ = UART_PORT == uart0 ? UART0_IRQ : UART1_IRQ;  //Interuptions
}

void c_uartHandler::callback_uart_rx()
{
    printf("CALLBACK\n");
    while ((uart_is_readable(UART_PORT)) && (UART_FLAG == 0))
    {
        ch = uart_getc(UART_PORT);
        if (uart_rcv_cpt >= UART_MAX_LENGTH)            //On va dépasser la longueur MAX ?
        {   
            UART_FLAG = 1;                              //Paquet pret a être récupéré
            double_pointer[UART_MAX_LENGTH-1] = '\0';   //Fin du String
            uart_rcv_cpt = 0;
            break;
        }

        if (ch != 0x0D)                                 //Si on a pas de fin de ligne
        {
            double_pointer[uart_rcv_cpt] = ch;          //On allimente le buffer
            uart_rcv_cpt++;                             //On se prépare pour le prochain caractère
        }
        else                                            //On a une fin de ligne
        {
            UART_FLAG = 1;                              //Paquet pret a être récupéré
            double_pointer[uart_rcv_cpt] = '\0';
            uart_rcv_cpt = 0;
            break;
        }
    }
}

void c_uartHandler::_uart_init()
{
    uart_init(UART_PORT, BAUD_RATE);  
    //gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    //gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    uart_set_hw_flow(UART_PORT, false, false);                  //Interuptions
    uart_set_format(UART_PORT, DATA_BITS, STOP_BITS, PARITY);
    uart_set_fifo_enabled(UART_PORT, false);
    
    double_pointer = &uart_rcv_buf_first[0];
    double_pointer_USB = &uart_rcv_buf_first_USB[0];

    irq_set_exclusive_handler(UART_IRQ, ext_callback_uart_rx);
    irq_set_enabled(UART_IRQ, true);
    uart_set_irq_enables(UART_PORT, true, false);
}

void c_uartHandler::_uart_write(uint8_t* buf, uint8_t len)
{
    irq_set_enabled(UART_IRQ, false);
    for (uint8_t i = 0; i < len; i++)
        uart_putc(UART_PORT, buf[i]);
    irq_set_enabled(UART_IRQ, true);
}

void c_uartHandler::_uart_mainLoop()
{
    if (UART_FLAG_USB == UART_FLAG_RX)
    {
        //Le contenu est pret dans DOUBLE_POINTEUR
        memcpy(BufferRxUSB, double_pointer_USB, 8);
        //sprintf((char *)BufferRxUSB, "%s\n", double_pointer_USB);
        //printf("%s", BufferRxUSB);
        double_pointer_USB = &uart_rcv_buf_first_USB[0];
        UART_FLAG_USB = UART_MAIN_PROCESS;
    }
    else if (UART_FLAG == UART_FLAG_RX)
    {
        irq_set_enabled(UART_IRQ, false);
        //Le contenu est pret dans DOUBLE_POINTEUR
        sprintf((char *)BufferRx, "%s\n", double_pointer);
        //printf("%s", double_pointer);
        double_pointer = &uart_rcv_buf_first[0];
        UART_FLAG = 0;
        irq_set_enabled(UART_IRQ, true);
    }
    else if (UART_FLAG == UART_FLAG_TX) //Un paquet à envoyer ?
    {
        _uart_write(&BufferTx[0], strlen((const char*)BufferTx)-1);
        //printf("Size Tx : %u\n", strlen((const char*)BufferTx));
        sprintf((char *)BufferTx, "%s", "");    //On reset le buffer d'envoi une fois la réponse émise
    }
}

void c_uartHandler::_uart_get_from_USB()
{   
    if (UART_FLAG_USB == 0)                                 //Reception allowed
    {
        ch_USB = getchar_timeout_us(10);
        if (ch_USB >= 0xF0) //Timedout
                return;
        if ((ch_USB >= 'a' && ch_USB <= 'z') || 
            (ch_USB >= 'A' && ch_USB <= 'Z') || 
            (ch_USB >= '0' && ch_USB <= '9') || 
            (ch_USB >= 0x21 && ch_USB <= 0x24) || 
            ch_USB == '\n' || 
            ch_USB == '\r' || 
            okaybecauseCommand == 1)
        {
            if (ch_USB >= 0x21 && ch_USB <= 0x23)   //Command in comming
                okaybecauseCommand = 1;
            if (uart_rcv_cpt_USB >= UART_MAX_LENGTH)            //On va dépasser la longueur MAX ?
            {   
                UART_FLAG_USB = 1;                              //Paquet pret a être récupéré
                double_pointer_USB[UART_MAX_LENGTH-1] = '\0';   //Fin du String
                uart_rcv_cpt_USB = 0;
                okaybecauseCommand = 0;
                return;
            }

            if (ch_USB != 0x0D)                                 //Si on a pas de fin de ligne
            {
                double_pointer_USB[uart_rcv_cpt_USB] = ch_USB;  //On allimente le buffer
                uart_rcv_cpt_USB++;                             //On se prépare pour le prochain caractère
            }
            else                                                //On a une fin de ligne
            {
                UART_FLAG_USB = 1;                              //Paquet pret a être récupéré
                double_pointer_USB[uart_rcv_cpt_USB] = '\0';
                uart_rcv_cpt_USB = 0;
                okaybecauseCommand = 0;
                return;
            }
        }
        else
            printf("NO : %02X\n", ch_USB);
    }
}