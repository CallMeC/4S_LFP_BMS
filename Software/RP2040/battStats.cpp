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
    V_Pack = 0;
    SoC_Pack = 0;
    SoH_Pack = 0;
    IMD_Pack = 0;
    IMR_Pack = 0;
    IMC_Pack = 0;
    T_Max_Pack = 0;
    T_Min_Pack = 0;
    V_Max_Pack = 0;
    V_Min_Pack = 0;
    operatingArea_Pack = ZONE_GREEN;
    state = c_Cell::CellState::OFF;
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
    VCellMax = 1;//
    VCellMin = 4;//
    V_Max_Pack = max(max(Cell0.Voltage, Cell1.Voltage), max(Cell2.Voltage, Cell3.Voltage));
    V_Min_Pack = min(min(Cell0.Voltage, Cell1.Voltage), min(Cell2.Voltage, Cell3.Voltage));

    Cell0.Temperature = randomTemp();
    Cell1.Temperature = randomTemp();
    Cell2.Temperature = randomTemp();
    Cell3.Temperature = randomTemp();
    TCellMax = 1;//
    TCellMin = 4;//
    T_Max_Pack = max(max(Cell0.Temperature, Cell1.Temperature), max(Cell2.Temperature, Cell3.Temperature));
    T_Min_Pack = min(min(Cell0.Temperature, Cell1.Temperature), min(Cell2.Temperature, Cell3.Temperature));

    Cell0.SoC = randomSoC();
    Cell1.SoC = randomSoC();
    Cell2.SoC = randomSoC();
    Cell3.SoC = randomSoC();
    SoCCellMax = 1;//max(max(Cell0.SoC, Cell1.SoC), max(Cell2.SoC, Cell3.SoC));
    SoCCellMin = 4;//min(min(Cell0.SoC, Cell1.SoC), min(Cell2.SoC, Cell3.SoC));

    Cell0.SoH = randomSoH();
    Cell1.SoH = randomSoH();
    Cell2.SoH = randomSoH();
    Cell3.SoH = randomSoH();
    SoHCellMax = 1;//max(max(Cell0.SoH, Cell1.SoH), max(Cell2.SoH, Cell3.SoH));
    SoHCellMin = 4;//min(min(Cell0.SoH, Cell1.SoH), min(Cell2.SoH, Cell3.SoH));

    IShunt = randomCurrent();
}

void c_battStats::synthesisPack()
{
  Cell0.setState(c_Cell::CellState::DISCHARGE);
  Cell1.setState(c_Cell::CellState::DISCHARGE);
  Cell2.setState(c_Cell::CellState::DISCHARGE);
  Cell3.setState(c_Cell::CellState::DISCHARGE);

  Cell0.checkCell();
  Cell1.checkCell();
  Cell2.checkCell();
  Cell3.checkCell();

  V_Pack = Cell0.Voltage + Cell1.Voltage + Cell2.Voltage + Cell3.Voltage;

  SoC_Pack = min(min(Cell0.SoC, Cell1.SoC), min(Cell2.SoC, Cell3.SoC));
  SoH_Pack = min(min(Cell0.SoH, Cell1.SoH), min(Cell2.SoH, Cell3.SoH));

  IMD_Pack = min(min(Cell0.IMD, Cell1.IMD), min(Cell2.IMD, Cell3.IMD));
  IMR_Pack = min(min(Cell0.IMR, Cell1.IMR), min(Cell2.IMR, Cell3.IMR));
  IMC_Pack = min(min(Cell0.IMC, Cell1.IMC), min(Cell2.IMC, Cell3.IMC));

  if ((Cell0.getState() == Cell1.getState()) && (Cell1.getState() == Cell2.getState()) && (Cell2.getState() == Cell3.getState()))
    state = Cell0.getState();
  else
    state = c_Cell::CellState::OFF;
}

void c_battStats::displayVal()
{    
    printf("\n----Pack--DISPLAY----\n");
    printf("SoC : %u %%\n", SoC_Pack/10);
    printf("SoH : %u %%\n", SoH_Pack/10);
    printf("Voltage : %.3f V\n", (float)V_Pack/1000);
    printf("Current : %.2f A\n", (float)IShunt/100);
    printf("Cells Temperatures : Max %.1f °C | Min %.1f °C\n", (float)DEG_C_TO_TEMP(T_Max_Pack), (float)DEG_C_TO_TEMP(T_Min_Pack));
    printf("Cells Voltages : Max %.3f V | Min %.3f V\n", (float)V_Max_Pack/1000, (float)V_Min_Pack/1000);
    printf("operatingArea : %u\n", operatingArea_Pack);
    printf("Pack State : %s\n", getStateString());

    /*printf("Alarms C0 : %08X\n", Cell0.alarmsMask);   //Slows down printing if enabled
    printf("Alarms C1 : %08X\n", Cell1.alarmsMask);
    printf("Alarms C2 : %08X\n", Cell2.alarmsMask);
    printf("Alarms C3 : %08X\n", Cell3.alarmsMask);*/

    printf("Pack Alarms %02X %02X %02X\n", alarmN1, alarmN2, alarmN3);

    printf("IMD : %u ", IMD_Pack);
    printf("IMR : %u ", IMR_Pack);
    printf("IMC : %u\n", IMC_Pack);
}

void c_battStats::sendGUIVal()
{    
  uint8_t stateValue = 0;
  if (state == c_Cell::CellState::CHARGE) stateValue = 1;
  if (state == c_Cell::CellState::DISCHARGE) stateValue = 2;
  if (state == c_Cell::CellState::STORAGE) stateValue = 3;

  printf("%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;W\n",
          Cell0.Voltage, Cell1.Voltage, Cell2.Voltage, Cell3.Voltage, VCellMax, VCellMin, Cell0.Temperature, Cell1.Temperature, Cell2.Temperature, Cell3.Temperature,
          TCellMax, TCellMin, Cell0.SoC, Cell1.SoC, Cell2.SoC, Cell3.SoC, SoCCellMax, SoCCellMin,
          Cell0.SoH, Cell1.SoH, Cell2.SoH, Cell3.SoH, SoHCellMax, SoHCellMin, V_Pack, IShunt, SoC_Pack,
          SoH_Pack, alarmN1, alarmN2, alarmN3, calibrationShuntValue, IMD_Pack, IMR_Pack, IMC_Pack, stateValue);
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
  return (1000 - (rand() % 450));
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

uint16_t c_battStats::max(uint16_t a, uint16_t b)
{
    if (a > b) return a;
    return b;
}

// Get the state of the cell as a string
const char* c_battStats::getStateString() const
{
    switch (state)
    {
        case c_Cell::CellState::OFF:
            return "OFF";
        case c_Cell::CellState::CHARGE:
            return "CHARGE";
        case c_Cell::CellState::DISCHARGE:
            return "DISCHARGE";
        case c_Cell::CellState::STORAGE:
            return "STORAGE";
        default:
            return "UNKNOWN";
    }
}