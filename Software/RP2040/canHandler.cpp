#include "canHandler.h"
#include "appVariable.h"

c_canHandler::c_canHandler()
{

}

void c_canHandler::_can_init()
{
    uint32_t pio_num = 0;
    uint32_t sys_clock = 125000000, bitrate = 250000;
    uint32_t gpio_rx = CAN_RX, gpio_tx = CAN_TX;

    // Setup canbus
    can2040_setup(&canBus, pio_num);
    can2040_callback_config(&canBus, can2040_cb);

    // Enable irqs
    irq_set_exclusive_handler(PIO0_IRQ_0_IRQn, PIOx_IRQHandler);
    NVIC_SetPriority(PIO0_IRQ_0_IRQn, 1);
    NVIC_EnableIRQ(PIO0_IRQ_0_IRQn);

    // Start canbus
    can2040_start(&canBus, sys_clock, bitrate, gpio_rx, gpio_tx);
}

void c_canHandler::_can_send()
{
    frameToSend.id = 0x25;
    frameToSend.dlc = 7;
    frameToSend.data[0] = 0xAB;
    frameToSend.data[1] = 0xCD;
    frameToSend.data[2] = 0xEF;
    frameToSend.data[3] = 0x12;
    frameToSend.data[4] = 0x34;
    frameToSend.data[5] = 0x56;
    frameToSend.data[6] = 0x78;
    frameToSend.data[7] = 0x9A;
    
    can2040_transmit(&canBus, &frameToSend);
}

void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    // Processing here
    if (notify == CAN2040_NOTIFY_RX)
    {
        printf("CAN FRAME : \n");
        printf("ID : %04X\n", msg->id);
        printf("DLC : %u\n", msg->dlc);
        printf("DATA : %02X %02X %02X %02X %02X %02X %02X %02X\n", msg->data[0], msg->data[1], msg->data[2], msg->data[3], msg->data[4], msg->data[5], msg->data[6], msg->data[7]);
    }
    else if (notify == CAN2040_NOTIFY_TX)
    {
        //Sent frame
    }
    else
    {
        printf("FRAME ERROR\n");
    }
}

void PIOx_IRQHandler()
{
    can2040_pio_irq_handler(&canBus);
}