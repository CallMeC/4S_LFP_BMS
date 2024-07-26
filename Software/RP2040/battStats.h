#ifndef __BATTSTATS__H
#define __BATTSTATS__H

#include "pico/stdlib.h"
#include <stdio.h>
#include "cell.h"

#define MAX_IN_LENGTH   8
#define CELLS_NUMBER    4   //4S BMS
#define IMD_MAX         210 //A
#define IMC_MAX         53  //A
#define IMR_MAX         105 //A
#define DELTA_U_MAX     20  //mV


class c_battStats
{
    public:
        c_battStats();
        void randomVal();
        void displayVal();
        void sendGUIVal();
        void synthesisPack();

        uint16_t randomVoltage();
        uint16_t randomCurrent();
        uint16_t randomSoC();
        uint16_t randomSoH();
        uint16_t randomTemp();
        const char* getStateString() const;

        void commandParser(uint8_t command[MAX_IN_LENGTH]);       

        #if CELLS_NUMBER == 4
            c_Cell Cell0;
            c_Cell Cell1;
            c_Cell Cell2;
            c_Cell Cell3;
        #elif CELLS_NUMBER == 6
            c_Cell Cell0;
            c_Cell Cell1;
            c_Cell Cell2;
            c_Cell Cell3;
            c_Cell Cell4;
            c_Cell Cell5;
        #endif

        c_Cell cellNbrSoCMin;
        c_Cell cellNbrSoCMax;
        uint16_t SoCCellMax;
        uint16_t SoCCellMin;

        c_Cell cellNbrSoHMin;
        c_Cell cellNbrSoHMax;
        uint16_t SoHCellMax;
        uint16_t SoHCellMin;

        c_Cell cellNbrVMin;
        c_Cell cellNbrVMax;
        uint16_t VCellMax;
        uint16_t VCellMin;

        c_Cell cellNbrTMin;
        c_Cell cellNbrTMax;
        uint16_t TCellMax;
        uint16_t TCellMin;

        uint16_t calibrationShuntValue;
        uint16_t IShunt;
        uint16_t V_Pack;
        uint16_t SoC_Pack;
        uint16_t SoH_Pack;
        uint16_t IMD_Pack;
        uint16_t IMC_Pack;
        uint16_t IMR_Pack;
        uint16_t T_Max_Pack;
        uint16_t T_Min_Pack;
        uint16_t V_Max_Pack;
        uint16_t V_Min_Pack;
        uint16_t V_Delta_Cell_Max;
        uint8_t  operatingArea_Pack;
        uint8_t balancingState;
        c_Cell::CellState state;

        uint8_t alarmN1;
        uint8_t alarmN2;
        uint8_t alarmN3;

        char BigStr[100];

        uint8_t incomingByte[MAX_IN_LENGTH];
        uint8_t incomingByteCounter;
    
    private:
        uint16_t min(uint16_t a, uint16_t b);
        uint16_t max(uint16_t a, uint16_t b);
        c_Cell whoMin(uint16_t Cell0Val, uint16_t Cell1Val, uint16_t Cell2Val, uint16_t Cell3Val);
        c_Cell whoMax(uint16_t Cell0Val, uint16_t Cell1Val, uint16_t Cell2Val, uint16_t Cell3Val);
        uint8_t cellNumberToU8(c_Cell cell);
};

#endif