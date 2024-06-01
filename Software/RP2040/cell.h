#ifndef __CELL__
#define __CELL__

#include "pico/stdlib.h"
#include <stdio.h>

//Operating Areas of the Cell
#define ZONE_BLACK          0
#define ZONE_RED            1
#define ZONE_YELLOW         2
#define ZONE_GREEN          3

#define RED_TEMP_LOW       -20      //°C
#define YELLOW_TEMP_LOW    -10      //°C
#define GREEN_TEMP_LOW      15      //°C
#define GREEN_TEMP_HIGH     45      //°C
#define YELLOW_TEMP_HIGH    55      //°C
#define RED_TEMP_HIGH       60      //°C

#define RED_VOLTAGE_LOW     2500    //mV
#define YELLOW_VOLTAGE_LOW  2600    //mV
#define GREEN_VOLTAGE_LOW   3000    //mV
#define GREEN_VOLTAGE_HIGH  3200    //mV
#define YELLOW_VOLTAGE_HIGH 3650    //mV
#define RED_VOLTAGE_HIGH    3700    //mV

#define NB_CYCLES_MAX       2500    //2500 Chg/Dchg cycles for 80% SoH
#define CELL_CAPA_AH        105     //A.h

#define DEG_C_TO_TEMP(DEG) (DEG-1000)/10

class c_Cell
{
    public :
        c_Cell();

        uint16_t SoC;           //%
        uint16_t SoH;           //%
        uint16_t Voltage;       //mV
        uint16_t Temperature;   //°C
        uint16_t CyclesCpt;     //N.A
        uint8_t  operatingArea;

        bool overTemperatureBlackOn;
        bool underTemperatureBlackOn;
        bool overTemperatureRedOn;
        bool underTemperatureRedOn;
        bool overTemperatureYellowOn;
        bool underTemperatureYellowOn;

        bool overVoltageBlackOn;
        bool underVoltageBlackOn;
        bool overVoltageRedOn;
        bool underVoltageRedOn;
        bool overVoltageYellowOn;
        bool underVoltageYellowOn;

        bool alarms[12];

        bool setBypassState(bool bypassState);
        void checkCell();
        void displayCell();
};

#endif