#include "battStats.h"
#include <time.h>
#include <cstdlib>

c_battStats::c_battStats()
{
    Cell0.SoC = 0;
    Cell1.SoC = 0;
    Cell2.SoC = 0;
    Cell3.SoC = 0;
    SoCCellMax = 0;
    SoCCellMin = 0;

    Cell0.SoH = 0;
    Cell1.SoH = 0;
    Cell2.SoH = 0;
    Cell3.SoH = 0;
    SoHCellMax = 0;
    SoHCellMin = 0;

    Cell0.Voltage = 0;
    Cell1.Voltage = 0;
    Cell2.Voltage = 0;
    Cell3.Voltage = 0;
    VCellMax = 0;
    VCellMin = 0;

    Cell0.Temperature = 0;
    Cell1.Temperature = 0;
    Cell2.Temperature = 0;
    Cell3.Temperature = 0;
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

    Cell0.Voltage = randomVoltage();
    Cell1.Voltage = randomVoltage();
    Cell2.Voltage = randomVoltage();
    Cell3.Voltage = randomVoltage();
    VCellMax = 2;
    VCellMin = 4;
    Cell0.Temperature = randomTemp();
    Cell1.Temperature = randomTemp();
    Cell2.Temperature = randomTemp();
    Cell3.Temperature = randomTemp();
    TCellMax = 2;
    TCellMin = 3;
    Cell0.SoC = randomSoC();
    Cell1.SoC = randomSoC();
    Cell2.SoC = randomSoC();
    Cell3.SoC = randomSoC();
    SoCCellMax = 2;
    SoCCellMin = 4;
    Cell0.SoH = randomSoH();
    Cell1.SoH = randomSoH();
    Cell2.SoH = randomSoH();
    Cell3.SoH = randomSoH();
    SoHCellMax = 1;
    SoHCellMin = 2;
    IShunt = randomCurrent();
    VPack = Cell0.Voltage + Cell1.Voltage + Cell2.Voltage + Cell3.Voltage;
    SoCPack = min(min(Cell0.SoC, Cell1.SoC), min(Cell2.SoC, Cell3.SoC));
    SoHPack = min(min(Cell0.SoH, Cell1.SoH), min(Cell2.SoH, Cell3.SoH));

}

void c_battStats::displayVal()
{
    Cell0.displayCell();
    
    /*printf("%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;W\n",
                  Cell0.Voltage, Cell1.Voltage, Cell2.Voltage, Cell3.Voltage, VCellMax, VCellMin, Cell0.Temperature, Cell1.Temperature, Cell2.Temperature, Cell3.Temperature,
                  TCellMax, TCellMin, Cell0.SoC, Cell1.SoC, Cell2.SoC, Cell3.SoC, SoCCellMax, SoCCellMin,
                  Cell0.SoH, Cell1.SoH, Cell2.SoH, Cell3.SoH, SoHCellMax, SoHCellMin, VPack, IShunt, SoCPack,
                  SoHPack, alarmN1, alarmN2, alarmN3, calibrationShuntValue);
    //printf("\n",BigStr);*/

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