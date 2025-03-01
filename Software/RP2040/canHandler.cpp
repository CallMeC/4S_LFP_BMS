#include "canHandler.h"
#include "appVariable.h"

c_canHandler::c_canHandler()
{
    aliveFlag = 0;
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

void c_canHandler::_can_heartbeat()
{
    //Heartbeat Frame
    heartbeat = !heartbeat;
    frameToSend.id = 0x45;
    frameToSend.dlc = 8;
    frameToSend.data[0] = heartbeat;
    frameToSend.data[1] = (uint8_t)(BMS_SERIAL_NUMBER & 0x000000FF);
    frameToSend.data[2] = (uint8_t)((BMS_SERIAL_NUMBER & 0x0000FF00) >> 8);
    frameToSend.data[3] = (uint8_t)((BMS_SERIAL_NUMBER & 0x00FF0000) >> 8);
    frameToSend.data[4] = (uint8_t)((BMS_SERIAL_NUMBER & 0xFF000000) >> 8);
    frameToSend.data[5] = BATTSTAT.stateValue;
    frameToSend.data[6] = BATTSTAT.cellNumberToU8(BATTSTAT.cellNbrVMin) | (BATTSTAT.cellNumberToU8(BATTSTAT.cellNbrVMax) << 2)
                        | (BATTSTAT.cellNumberToU8(BATTSTAT.cellNbrTMin) << 4) | (BATTSTAT.cellNumberToU8(BATTSTAT.cellNbrTMax) << 6);
    frameToSend.data[7] = BATTSTAT.cellNumberToU8(BATTSTAT.cellNbrSoCMin) | (BATTSTAT.cellNumberToU8(BATTSTAT.cellNbrSoCMax) << 2)
                        | (BATTSTAT.cellNumberToU8(BATTSTAT.cellNbrSoHMin) << 4) | (BATTSTAT.cellNumberToU8(BATTSTAT.cellNbrSoHMax) << 6);
    can2040_transmit(&canBus, &frameToSend);
}

void c_canHandler::_can_send()
{
    if (heartbeat)
    {
        //VCell Frame
        frameToSend.id = 0x46;
        frameToSend.dlc = 8;
        frameToSend.data[0] = (uint8_t)(BATTSTAT.Cell0.Voltage & 0x00FF);
        frameToSend.data[1] = (uint8_t)((BATTSTAT.Cell0.Voltage & 0xFF00) >> 8);
        frameToSend.data[2] = (uint8_t)(BATTSTAT.Cell1.Voltage & 0x00FF);
        frameToSend.data[3] = (uint8_t)((BATTSTAT.Cell1.Voltage & 0xFF00) >> 8);
        frameToSend.data[4] = (uint8_t)(BATTSTAT.Cell2.Voltage & 0x00FF);
        frameToSend.data[5] = (uint8_t)((BATTSTAT.Cell2.Voltage & 0xFF00) >> 8);
        frameToSend.data[6] = (uint8_t)(BATTSTAT.Cell3.Voltage & 0x00FF);
        frameToSend.data[7] = (uint8_t)((BATTSTAT.Cell3.Voltage & 0xFF00) >> 8);
        can2040_transmit(&canBus, &frameToSend);

        //TCell Frame
        frameToSend.id = 0x47;
        frameToSend.dlc = 8;
        frameToSend.data[0] = (uint8_t)(BATTSTAT.Cell0.Temperature & 0x00FF);
        frameToSend.data[1] = (uint8_t)((BATTSTAT.Cell0.Temperature & 0xFF00) >> 8);
        frameToSend.data[2] = (uint8_t)(BATTSTAT.Cell1.Temperature & 0x00FF);
        frameToSend.data[3] = (uint8_t)((BATTSTAT.Cell1.Temperature & 0xFF00) >> 8);
        frameToSend.data[4] = (uint8_t)(BATTSTAT.Cell2.Temperature & 0x00FF);
        frameToSend.data[5] = (uint8_t)((BATTSTAT.Cell2.Temperature & 0xFF00) >> 8);
        frameToSend.data[6] = (uint8_t)(BATTSTAT.Cell3.Temperature & 0x00FF);
        frameToSend.data[7] = (uint8_t)((BATTSTAT.Cell3.Temperature & 0xFF00) >> 8);
        can2040_transmit(&canBus, &frameToSend);

        //SoCCell Frame
        frameToSend.id = 0x48;
        frameToSend.dlc = 8;
        frameToSend.data[0] = (uint8_t)(BATTSTAT.Cell0.SoC & 0x00FF);
        frameToSend.data[1] = (uint8_t)((BATTSTAT.Cell0.SoC & 0xFF00) >> 8);
        frameToSend.data[2] = (uint8_t)(BATTSTAT.Cell1.SoC & 0x00FF);
        frameToSend.data[3] = (uint8_t)((BATTSTAT.Cell1.SoC & 0xFF00) >> 8);
        frameToSend.data[4] = (uint8_t)(BATTSTAT.Cell2.SoC & 0x00FF);
        frameToSend.data[5] = (uint8_t)((BATTSTAT.Cell2.SoC & 0xFF00) >> 8);
        frameToSend.data[6] = (uint8_t)(BATTSTAT.Cell3.SoC & 0x00FF);
        frameToSend.data[7] = (uint8_t)((BATTSTAT.Cell3.SoC & 0xFF00) >> 8);
        can2040_transmit(&canBus, &frameToSend);

        //SoHCell Frame
        frameToSend.id = 0x49;
        frameToSend.dlc = 8;
        frameToSend.data[0] = (uint8_t)(BATTSTAT.Cell0.SoH & 0x00FF);
        frameToSend.data[1] = (uint8_t)((BATTSTAT.Cell0.SoH & 0xFF00) >> 8);
        frameToSend.data[2] = (uint8_t)(BATTSTAT.Cell1.SoH & 0x00FF);
        frameToSend.data[3] = (uint8_t)((BATTSTAT.Cell1.SoH & 0xFF00) >> 8);
        frameToSend.data[4] = (uint8_t)(BATTSTAT.Cell2.SoH & 0x00FF);
        frameToSend.data[5] = (uint8_t)((BATTSTAT.Cell2.SoH & 0xFF00) >> 8);
        frameToSend.data[6] = (uint8_t)(BATTSTAT.Cell3.SoH & 0x00FF);
        frameToSend.data[7] = (uint8_t)((BATTSTAT.Cell3.SoH & 0xFF00) >> 8);
        can2040_transmit(&canBus, &frameToSend);
    }
    else
    {
        //IMDRC Frame
        frameToSend.id = 0x50;
        frameToSend.dlc = 8;
        frameToSend.data[0] = (uint8_t)(BATTSTAT.IMD_Pack & 0x00FF);
        frameToSend.data[1] = (uint8_t)((BATTSTAT.IMD_Pack & 0xFF00) >> 8);
        frameToSend.data[2] = (uint8_t)(BATTSTAT.IMR_Pack & 0x00FF);
        frameToSend.data[3] = (uint8_t)((BATTSTAT.IMR_Pack & 0xFF00) >> 8);
        frameToSend.data[4] = (uint8_t)(BATTSTAT.IMC_Pack & 0x00FF);
        frameToSend.data[5] = (uint8_t)((BATTSTAT.IMC_Pack & 0xFF00) >> 8);
        frameToSend.data[6] = 0x00;
        frameToSend.data[7] = 0x00;
        can2040_transmit(&canBus, &frameToSend);

        //PackInfos Frame
        frameToSend.id = 0x51;
        frameToSend.dlc = 8;
        frameToSend.data[0] = (uint8_t)(BATTSTAT.IShunt & 0x00FF);
        frameToSend.data[1] = (uint8_t)((BATTSTAT.IShunt & 0xFF00) >> 8);
        frameToSend.data[2] = (uint8_t)(BATTSTAT.V_Pack & 0x00FF);
        frameToSend.data[3] = (uint8_t)((BATTSTAT.V_Pack & 0xFF00) >> 8);
        frameToSend.data[4] = (uint8_t)(BATTSTAT.SoC_Pack & 0x00FF);
        frameToSend.data[5] = (uint8_t)((BATTSTAT.SoC_Pack & 0xFF00) >> 8);
        frameToSend.data[6] = (uint8_t)(BATTSTAT.SoH_Pack & 0x00FF);
        frameToSend.data[7] = (uint8_t)((BATTSTAT.SoH_Pack & 0xFF00) >> 8);
        can2040_transmit(&canBus, &frameToSend);

        //Alarms Frame
        frameToSend.id = 0x52;
        frameToSend.dlc = 8;
        frameToSend.data[0] = BATTSTAT.alarmN1;
        frameToSend.data[1] = BATTSTAT.alarmN2;
        frameToSend.data[2] = BATTSTAT.alarmN3;
        frameToSend.data[3] = 0x00;
        frameToSend.data[4] = BATTSTAT.balancingState;
        frameToSend.data[5] = 0;
        frameToSend.data[6] = (uint8_t)(BATTSTAT.calibrationShuntValue & 0x00FF);
        frameToSend.data[7] = (uint8_t)((BATTSTAT.calibrationShuntValue & 0xFF00) >> 8);
        can2040_transmit(&canBus, &frameToSend);
    }
}

void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    // Processing here
    if (notify == CAN2040_NOTIFY_RX)
    {
        canHandler.canTOCounter = 0;
        //printf("CAN FRAME : \n");
        //printf("ID : %04X\n", msg->id);
        //printf("DLC : %u\n", msg->dlc);
        //printf("DATA : %02X %02X %02X %02X %02X %02X %02X %02X\n", msg->data[0], msg->data[1], msg->data[2], msg->data[3], msg->data[4], msg->data[5], msg->data[6], msg->data[7]);
        if (msg->id == 0x55)
        {
            memcpy(BufferRxUSB, msg->data, 8);
            uartHandler.UART_FLAG_USB = UART_MAIN_PROCESS;
            printf("CMD RCV\n");
        }
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