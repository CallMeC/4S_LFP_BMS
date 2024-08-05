#define __INSTANCE__
#include "appVariable.h"
#undef __INSTANCE__

int main()
{
    stdio_init_all();

    printf("START\n");
    IOManager.GPIO_init();
    uartHandler._uart_init();
    IOManager.peripheralsInit();
    IOManager.bootfinished();

    while (1)
    {
        uartHandler._uart_mainLoop();
        uartHandler._uart_get_from_USB();
        if (IO_OPERATION == 0)
            IO_OPERATION = UPDATE_VALUES;
        IOManager.mainLoop();

        if (uartHandler.UART_FLAG_USB == UART_MAIN_PROCESS) //Command to process
        {
            switch(BufferRxUSB[0])  //Depending on first character
            {
                case 0x21:  //Alarm ACK
                    printf("Alarm\n");
                    if (BufferRxUSB[1] == 0x31) //reset N1
                        BATTSTAT.alarmN1 = 0x00;
                    if (BufferRxUSB[1] == 0x32) //reset N2
                        BATTSTAT.alarmN2 = 0x00;
                    if (BufferRxUSB[1] == 0x33) //reset N3
                        BATTSTAT.alarmN3 = 0x00;
                    if (BufferRxUSB[1] == 0x52) //reset All
                    {
                        BATTSTAT.alarmN1 = 0x00;
                        BATTSTAT.alarmN2 = 0x00;
                        BATTSTAT.alarmN3 = 0x00;
                    }
                    uartHandler.UART_FLAG_USB = 0;
                    break;
                case 0x22:  //Shunt Update
                    printf("Buffer : %02X %02X %02X %02X %02X %02X %02X %02X\n",BufferRxUSB[0], BufferRxUSB[1], BufferRxUSB[2], BufferRxUSB[3], BufferRxUSB[4], BufferRxUSB[5], BufferRxUSB[6], BufferRxUSB[7]);
                    BATTSTAT.calibrationShuntValue = ((BufferRxUSB[1] << 8) | BufferRxUSB[2]);
                    FLASHMGR.updateParam();
                    IOManager.ImaxValue = (0.066 / BATTSTAT.calibrationShuntValue)*1000000;
                    printf("Shunt : %u\n",BATTSTAT.calibrationShuntValue);
                    uartHandler.UART_FLAG_USB = 0;
                    break;
                case 0x23:  //SN Update
                    //printf("Buffer : %02X %02X %02X %02X %02X %02X %02X %02X\n",BufferRxUSB[0], BufferRxUSB[1], BufferRxUSB[2], BufferRxUSB[3], BufferRxUSB[4], BufferRxUSB[5], BufferRxUSB[6], BufferRxUSB[7]);
                    BMS_SERIAL_NUMBER = (BufferRxUSB[1] << 24) | (BufferRxUSB[2] << 16) | (BufferRxUSB[3] << 8)  | BufferRxUSB[4];
                    printf("SN : %08X\n",BMS_SERIAL_NUMBER);
                    FLASHMGR.updateParam();
                    uartHandler.UART_FLAG_USB = 0;
                    break;   
                case 0x24:  //Verbose
                    printf("VERBOSE ENABLED\n");
                    VERBOSE_ENABLED = !VERBOSE_ENABLED;
                    //I2C_0.Scan(i2c0);
                    //FLASHMGR.readFlash();
                    //FLASHMGR.dumpEEPROM();
                    
                    uartHandler.UART_FLAG_USB = 0;
                    break;   
                default:
                    //printf("UNKNOWN COMMAND %02X\n", BufferRxUSB[0]);
                    uartHandler.UART_FLAG_USB = 0;
                    break;
            }
        }
    }
}

bool timer_callback(repeating_timer_t *rt)
{
    IOManager.callBack();
    IOManager.mainLoop();
    IO_OPERATION = UPDATE_CURRENT;
    BATTSTAT.randomVal();
    BATTSTAT.synthesisPack();
    //BATTSTAT.displayVal();
    if (VERBOSE_ENABLED) BATTSTAT.sendGUIVal();

    // Read flash procedure
    //FLASHMGR.writeFlash();

    return true;
}

void ext_callback_uart_rx()
{
    uartHandler.callback_uart_rx();
}