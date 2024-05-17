#define __INSTANCE__
#include "appVariable.h"
#undef __INSTANCE__

int main()
{
    stdio_init_all();

    printf("START\n");
    IOManager.GPIO_init();
    IOManager.peripheralsInit();
    IOManager.bootfinished();
    
    /* // Write procedure to flash
    PARAM_FLASH_0 = 45;
    PARAM_FLASH_1 = 78;
    FLASHMGR.writeFlash();*/

    while (1)
    {
        uartHandler._uart_mainLoop();
        IOManager.mainLoop();
        //printf("....H\n");    
    }
}

bool timer_callback(repeating_timer_t *rt)
{
    IOManager.callBack();
    BATTSTAT0.randomVal();
    BATTSTAT0.displayVal();
    
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