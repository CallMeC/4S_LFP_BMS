/*#include "IO_Types.h"
#include "Paquet.h"
#include "I2C.h"
#include "MCP3424.h"
#include "MCP4551.h"*/
#include "pico/stdlib.h"
#include <stdio.h>
#include "IO_Manager.h"
#include "uartHandler.h"
#include "I2C.h"
#include "MCP3424.h"
#include "battStats.h"
#include "flashManager.h"
#include "faultsManager.h"
#include "systemCore.h"

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
EXT_APP_DATA uint32_t BMS_SERIAL_NUMBER;    //Type (04) - Revision (02) - Batch (01) - SN (01)


//IO
EXT_APP_DATA c_IO_Manager IOManager;
EXT_APP_DATA uint8_t IO_OPERATION;
EXT_APP_DATA uint16_t IO_OPERATION_PARAM;

EXT_APP_DATA bool timer_callback(repeating_timer_t *rt);

//UART
EXT_APP_DATA c_uartHandler uartHandler;
EXT_APP_DATA uint8_t BufferRx[200];
EXT_APP_DATA uint8_t BufferRxUSB[200];
EXT_APP_DATA uint8_t BufferTx[200];
EXT_APP_DATA void ext_callback_uart_rx();

//I2C
EXT_APP_DATA c_I2C I2C_0;
EXT_APP_DATA c_MCP3424 ADC_0;

//Batt Stats
EXT_APP_DATA c_battStats BATTSTAT;

//Flash Manager
#define FMGR_NAME FLASHMGR
EXT_APP_DATA c_FlashManager FMGR_NAME;

//Faults Manager
EXT_APP_DATA c_faultsManager FAULTS_MGR;

//System Core
EXT_APP_DATA c_SystemCore SysCore;