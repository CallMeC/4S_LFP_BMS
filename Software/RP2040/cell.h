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
        enum CellState
        {
            OFF,
            CHARGE,
            DISCHARGE,
            STORAGE
        };

        c_Cell();

        uint16_t SoC;           //% (*10)
        uint16_t SoH;           //% (*10)
        uint16_t Voltage;       //mV
        uint16_t Temperature;   //°C
        uint16_t CyclesCpt;     //N.A
        uint8_t  operatingArea;
        uint16_t IMD;           //A Maximum Dynamic Discharge Current
        uint16_t IMR;           //A Maximum Dynamic Regeneration Current
        uint16_t IMC;           //A Maximum Dynamic Charge Current
        

        CellState state;        // Cell state

        bool BypassState;

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
        uint32_t alarmsMask;

        bool setBypassState(bool bypassStateCmd);
        void checkCell();
        void displayCell();
        void IMDRCcheck();

          // Methods to manage cell state
        void setState(CellState newState);
        CellState getState() const;
        const char* getStateString() const;


        bool operator==(const c_Cell& other) const
        {
        return SoC == other.SoC &&
               SoH == other.SoH &&
               Voltage == other.Voltage &&
               Temperature == other.Temperature;
        }
};

#endif