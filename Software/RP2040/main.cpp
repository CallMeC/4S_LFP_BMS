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
        IO_OPERATION = UPDATE_VALUES;
        IOManager.mainLoop(); 

        if (uartHandler.UART_FLAG_USB == UART_MAIN_PROCESS) //Command to process
        {
            switch(BufferRxUSB[0])  //Depending on first character
            {
                case 0x21:  //Alarm ACK
                    //printf("Alarm\n");
                    uartHandler.UART_FLAG_USB = 0;
                    break;
                case 0x22:  //Shunt Update
                    printf("Buffer : %02X %02X %02X %02X %02X %02X %02X %02X\n",BufferRxUSB[0], BufferRxUSB[1], BufferRxUSB[2], BufferRxUSB[3], BufferRxUSB[4], BufferRxUSB[5], BufferRxUSB[6], BufferRxUSB[7]);
                    BATTSTAT.calibrationShuntValue = ((BufferRxUSB[1] << 8) | BufferRxUSB[2]);
                    FLASHMGR.updateParam();
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
                case 0x24:  //I2C Scan
                    printf("I2C Scan\n");
                    I2C_0.Scan(i2c0);
                    //FLASHMGR.readFlash();
                    FLASHMGR.dumpEEPROM();
                    
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
    BATTSTAT.randomVal();
    BATTSTAT.synthesisPack();
    //BATTSTAT.displayVal();
    BATTSTAT.sendGUIVal();

    // Read flash procedure
    //FLASHMGR.writeFlash();

    return true;
}

void ext_callback_uart_rx()
{
    uartHandler.callback_uart_rx();
}