#define __INSTANCE__
#include "appVariable.h"
#undef __INSTANCE__

int main()
{
    stdio_init_all();

    BMS_SERIAL_NUMBER = 0x04020101; //Type (04) - Revision (02) - Batch (01) - SN (01)

    printf("START\n");
    IOManager.GPIO_init();
    uartHandler._uart_init();
    IOManager.peripheralsInit();
    IOManager.bootfinished();
    
    /* // Write procedure to flash
    PARAM_FLASH_0 = 45;
    PARAM_FLASH_1 = 78;
    FLASHMGR.writeFlash();*/

    while (1)
    {
        uartHandler._uart_mainLoop();
        uartHandler._uart_get_from_USB();
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
                    BATTSTAT.calibrationShuntValue = ((BufferRxUSB[1] << 8) | BufferRxUSB[2]);
                    //printf("Shunt : %u\n",BATTSTAT.calibrationShuntValue);
                    uartHandler.UART_FLAG_USB = 0;
                    break;
                case 0x23:  //SN Update
                    BMS_SERIAL_NUMBER = (BufferRxUSB[1] << 24) | (BufferRxUSB[2] << 16) | (BufferRxUSB[3] << 8)  | BufferRxUSB[4];
                    //printf("SN : %u\n",BMS_SERIAL_NUMBER);
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

    /* // Read flash procedure
    PARAM_FLASH_0 = 0;
    PARAM_FLASH_1 = 0;
    FLASHMGR.readFlash();
    printf("PARAM 0 : %u\n PARAM 1 : %u\n\n", PARAM_FLASH_0, PARAM_FLASH_1);*/

    return true;
}

void ext_callback_uart_rx()
{
    uartHandler.callback_uart_rx();
}