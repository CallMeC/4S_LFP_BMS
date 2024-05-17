#ifndef __BATTSTATS__H
#define __BATTSTATS__H

#include "pico/stdlib.h"
#include <stdio.h>


#define MAX_IN_LENGTH 8


class c_battStats
{
    public:
        c_battStats();

        void randomVal();
        void displayVal();

        uint16_t randomVoltage();
        uint16_t randomCurrent();
        uint16_t randomSoC();
        uint16_t randomSoH();
        uint16_t randomTemp();
        void commandParser(uint8_t command[MAX_IN_LENGTH]);       

        uint16_t SoCCell1;
        uint16_t SoCCell2;
        uint16_t SoCCell3;
        uint16_t SoCCell4;
        uint8_t SoCCellMax;
        uint8_t SoCCellMin;

        uint16_t SoHCell1;
        uint16_t SoHCell2;
        uint16_t SoHCell3;
        uint16_t SoHCell4;
        uint8_t SoHCellMax;
        uint8_t SoHCellMin;

        uint16_t VCell1;
        uint16_t VCell2;
        uint16_t VCell3;
        uint16_t VCell4;
        uint8_t VCellMax;
        uint8_t VCellMin;

        uint16_t TCell1;
        uint16_t TCell2;
        uint16_t TCell3;
        uint16_t TCell4;
        uint8_t TCellMax;
        uint8_t TCellMin;

        uint16_t calibrationShuntValue;
        uint16_t IShunt;
        uint16_t VPack;
        uint16_t SoCPack;
        uint16_t SoHPack;
        uint32_t SerialNumber;

        uint8_t alarmN1;
        uint8_t alarmN2;
        uint8_t alarmN3;

        char BigStr[100];

        uint8_t incomingByte[MAX_IN_LENGTH];
        uint8_t incomingByteCounter;
    
    private:
        uint16_t min(uint16_t a, uint16_t b);
};

#endif