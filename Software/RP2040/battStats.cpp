#include "battStats.h"
#include <time.h>
#include <cstdlib>

c_battStats::c_battStats()
{
    SoCCell1 = 0;
    SoCCell2 = 0;
    SoCCell3 = 0;
    SoCCell4 = 0;
    SoCCellMax = 0;
    SoCCellMin = 0;

    SoHCell1 = 0;
    SoHCell2 = 0;
    SoHCell3 = 0;
    SoHCell4 = 0;
    SoHCellMax = 0;
    SoHCellMin = 0;

    VCell1 = 0;
    VCell2 = 0;
    VCell3 = 0;
    VCell4 = 0;
    VCellMax = 0;
    VCellMin = 0;

    TCell1 = 0;
    TCell2 = 0;
    TCell3 = 0;
    TCell4 = 0;
    TCellMax = 0;
    TCellMin = 0;

    calibrationShuntValue = 300;  //uOhms
    IShunt = 0;
    VPack = 0;
    SoCPack = 0;
    SoHPack = 0;
    SerialNumber = 0x12345678;
    char BigStr[100];

    alarmN1 = 0xAA;
    alarmN2 = 0xAA;
    alarmN3 = 0xAA;

    incomingByteCounter = 0;
}

void c_battStats::randomVal()
{
    srand (time(NULL));

    VCell1 = randomVoltage();
    VCell2 = randomVoltage();
    VCell3 = randomVoltage();
    VCell4 = randomVoltage();
    VCellMax = 2;
    VCellMin = 4;
    TCell1 = randomTemp();
    TCell2 = randomTemp();
    TCell3 = randomTemp();
    TCell4 = randomTemp();
    TCellMax = 2;
    TCellMin = 3;
    SoCCell1 = randomSoC();
    SoCCell2 = randomSoC();
    SoCCell3 = randomSoC();
    SoCCell4 = randomSoC();
    SoCCellMax = 2;
    SoCCellMin = 4;
    SoHCell1 = randomSoH();
    SoHCell2 = randomSoH();
    SoHCell3 = randomSoH();
    SoHCell4 = randomSoH();
    SoHCellMax = 1;
    SoHCellMin = 2;
    IShunt = randomCurrent();
    VPack = VCell1 + VCell2 + VCell3 + VCell4;
    SoCPack = min(min(SoCCell1, SoCCell2), min(SoCCell3, SoCCell4));
    SoHPack = min(min(SoHCell1, SoHCell2), min(SoHCell3, SoHCell4));
}

void c_battStats::displayVal()
{
    printf("%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;W\n",
                  VCell1, VCell2, VCell3, VCell4, VCellMax, VCellMin, TCell1, TCell2, TCell3, TCell4,
                  TCellMax, TCellMin, SoCCell1, SoCCell2, SoCCell3, SoCCell4, SoCCellMax, SoCCellMin,
                  SoHCell1, SoHCell2, SoHCell3, SoHCell4, SoHCellMax, SoHCellMin, VPack, IShunt, SoCPack,
                  SoHPack, alarmN1, alarmN2, alarmN3, calibrationShuntValue);
    //printf("\n",BigStr);

}

uint16_t c_battStats::randomVoltage()  //Range 2500-3650
{
  return (3200 + (rand() % 200 - 100));
}

uint16_t c_battStats::randomTemp()     //Range 0 - 3000 Offset 1000
{
  return (1210 + (rand() % 20 - 10));
}

uint16_t c_battStats::randomCurrent()  //Range 0 - 30000
{
  return (15000 + (rand() % 2000 - 1000));
}

uint16_t c_battStats::randomSoC()  //Range 0 - 1000
{
  return (1000 - (rand() % 45));
}

uint16_t c_battStats::randomSoH()  //Range 0 - 1000
{
  return (1000 - (rand() % 45));
}

uint16_t c_battStats::min(uint16_t a, uint16_t b)
{
    if (a <= b) return a;
    return b;
}