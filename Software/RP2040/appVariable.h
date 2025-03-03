/*#include "IO_Types.h"
#include "Paquet.h"
#include "I2C.h"
#include "MCP3424.h"
#include "MCP4551.h"*/
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include <stdio.h>
#include <stdint.h>
#include "IO_Manager.h"
#include "uartHandler.h"
#include "canHandler.h"
#include "I2C.h"
#include "MCP3424.h"
#include "battStats.h"
#include "flashManager.h"
#include "faultsManager.h"
#include "systemCore.h"
#include "RP2040.h"

#define SYS_VERS_MAJEUR         0
#define SYS_VERS_MINEUR         1   // 0 à 99

#ifdef __INSTANCE__
 //Définition des instances des variables
 #define EXT_APP_DATA //__no_init
 #define EXT_APP_DIAG 
#else
 //Déclaration des variables
 #define EXT_APP_DATA /*__no_init*/ extern
 #define EXT_APP_DIAG extern 
#endif

EXT_APP_DATA uint8_t VERBOSE_ENABLED;
EXT_APP_DATA uint8_t BYPASS_ENABLED;
EXT_APP_DATA uint32_t BMS_SERIAL_NUMBER;    //Type (04) - Revision (02) - Batch (01) - SN (01)


//IO
EXT_APP_DATA c_IO_Manager IOManager;
EXT_APP_DATA uint8_t IO_OPERATION;
EXT_APP_DATA uint16_t IO_OPERATION_PARAM;

EXT_APP_DATA bool timer_1000ms_callback(repeating_timer_t *rt);
EXT_APP_DATA bool timer_400ms_callback(repeating_timer_t *rt);

//UART
EXT_APP_DATA c_uartHandler uartHandler;
EXT_APP_DATA uint8_t BufferRx[200];
EXT_APP_DATA uint8_t BufferRxUSB[200];
EXT_APP_DATA uint8_t BufferTx[200];
EXT_APP_DATA void ext_callback_uart_rx();

//CAN
EXT_APP_DATA struct can2040 canBus;
EXT_APP_DATA void PIOx_IRQHandler(void);
EXT_APP_DATA void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg);
EXT_APP_DATA can2040_msg frameToSend;
EXT_APP_DATA c_canHandler canHandler;

//I2C
EXT_APP_DATA c_I2C I2C_0;
EXT_APP_DATA c_MCP3424 ADC_0;
EXT_APP_DATA c_MCP3424 ADC_1;
EXT_APP_DATA uint8_t ADC_IN_USE;

//Batt Stats
EXT_APP_DATA c_battStats BATTSTAT;

//Flash Manager
#define FMGR_NAME FLASHMGR
EXT_APP_DATA c_FlashManager FMGR_NAME;

//Faults Manager
EXT_APP_DATA c_faultsManager FAULTS_MGR;

//System Core
EXT_APP_DATA c_SystemCore SysCore;

//Filters
EXT_APP_DATA TemperatureFilter tempFilterNTC1;
EXT_APP_DATA TemperatureFilter tempFilterNTC2;
EXT_APP_DATA TemperatureFilter tempFilterNTC3;
EXT_APP_DATA TemperatureFilter tempFilterNTC4;

EXT_APP_DATA CurrentFilter currentFilter;

EXT_APP_DATA VoltageFilter voltFilterC1;
EXT_APP_DATA VoltageFilter voltFilterC2;
EXT_APP_DATA VoltageFilter voltFilterC3;
EXT_APP_DATA VoltageFilter voltFilterC4;