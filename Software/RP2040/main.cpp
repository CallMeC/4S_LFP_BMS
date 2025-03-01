#define __INSTANCE__
#include "appVariable.h"
#undef __INSTANCE__
#include <string.h>

int main()
{
    stdio_init_all();

    printf("START\n");
    IOManager.GPIO_init();
    uartHandler._uart_init();
    canHandler._can_init();
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
                    if (!VERBOSE_ENABLED) printf("VERBOSE ENABLED\n");
                    else printf("VERBOSE DISABLED\n");
                    VERBOSE_ENABLED = !VERBOSE_ENABLED;
                    uartHandler.UART_FLAG_USB = 0;
                    break;   
                
                case 0x25:  //ADC Cells
                    printf("OCV Calc\n");
                    //I2C_0.Scan(i2c0);
                    /*printf("Cell 1 : %u V\n", BATTSTAT.Cell0.Voltage);
                    printf("Cell 2 : %u V\n", BATTSTAT.Cell1.Voltage);
                    printf("Cell 3 : %u V\n", BATTSTAT.Cell2.Voltage);
                    printf("Cell 4 : %u V\n", BATTSTAT.Cell3.Voltage);*/
                    BATTSTAT.Cell0.SoC = BATTSTAT.Cell0.estimateSoCFromVoltage(BATTSTAT.Cell0.Voltage/1000.0)*10;
                    BATTSTAT.Cell1.SoC = BATTSTAT.Cell1.estimateSoCFromVoltage(BATTSTAT.Cell1.Voltage/1000.0)*10;
                    BATTSTAT.Cell2.SoC = BATTSTAT.Cell2.estimateSoCFromVoltage(BATTSTAT.Cell2.Voltage/1000.0)*10;
                    BATTSTAT.Cell3.SoC = BATTSTAT.Cell3.estimateSoCFromVoltage(BATTSTAT.Cell3.Voltage/1000.0)*10;
                    //FLASHMGR.dumpEEPROM();
                    FLASHMGR.updateParam();
                    uartHandler.UART_FLAG_USB = 0;
                    break;
                
                case 0x26:  //Cells SN
                    printf("\n---\n");
                    BATTSTAT.cellNbrVMax.setBypassState(false);
                    BYPASS_ENABLED = !BYPASS_ENABLED;
                    if (BYPASS_ENABLED) printf("BYPASS ENABLED\n");
                    else printf("BYPASS DISABLED\n");
                    /*FLASHMGR.dumpEEPROM();
                    memcpy(BATTSTAT.Cell0.serialNumber, "02YCB65117400J89C0000001", 24);
                    memcpy(BATTSTAT.Cell1.serialNumber, "02YCB65117400J89C0000002", 24);
                    memcpy(BATTSTAT.Cell2.serialNumber, "02YCB65117400J89C0000003", 24);
                    memcpy(BATTSTAT.Cell3.serialNumber, "02YCB65117400J89C0000004", 24);
                    FLASHMGR.updateParam();*/
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

bool timer_1000ms_callback(repeating_timer_t *rt)
{
    IOManager.callBack();
    IOManager.mainLoop();
    IO_OPERATION = UPDATE_CURRENT;
    //BATTSTAT.randomVal();
    BATTSTAT.synthesisPack();
    //BATTSTAT.displayVal();
    canHandler._can_heartbeat();
    if (VERBOSE_ENABLED) BATTSTAT.sendGUIVal();

    // Read flash procedure
    //FLASHMGR.writeFlash();
    canHandler._can_send();
    return true;
}

bool timer_400ms_callback(repeating_timer_t *rt)
{
    IOManager.cells_update();
    IOManager.mainLoop();
    IO_OPERATION = READ_CELLS;
    //printf("BATTSTAT.balancingCounter %u\n", BATTSTAT.balancingCounter);
    if (BATTSTAT.balancingCounter++ > 200)
        BATTSTAT.balancingEnabled = 0; //No Bypass for a while until good measurements
    if (BATTSTAT.balancingCounter > 250)
    {
        BATTSTAT.balancingCounter = 0;
        BATTSTAT.balancingEnabled = 1;
    }

    canHandler.canTOCounter++;
    if ((canHandler.canTOCounter > 4) && (IOManager.loopCounterAlive++ == 3))   //Time to die
        IOManager.autoWakeUpEnd();
    return true;
}


void ext_callback_uart_rx()
{
    uartHandler.callback_uart_rx();
}