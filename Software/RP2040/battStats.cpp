#include "appVariable.h"
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
    cellNbrSoCMin = Cell0;
    cellNbrSoCMax = Cell0;

    Cell0.SoH = 0;
    Cell1.SoH = 0;
    Cell2.SoH = 0;
    Cell3.SoH = 0;
    SoHCellMax = 0;
    SoHCellMin = 0;
    cellNbrSoHMin = Cell0;
    cellNbrSoHMax = Cell0;

    Cell0.Voltage = 0;
    Cell1.Voltage = 0;
    Cell2.Voltage = 0;
    Cell3.Voltage = 0;
    VCellMax = 0;
    VCellMin = 0;
    cellNbrVMin = Cell0;
    cellNbrVMax = Cell0;

    Cell0.Temperature = 0;
    Cell1.Temperature = 0;
    Cell2.Temperature = 0;
    Cell3.Temperature = 0;
    TCellMax = 0;
    TCellMin = 0;
    cellNbrTMin = Cell0;
    cellNbrTMax = Cell0;

    calibrationShuntValue = 200;  //uOhms by default
    calibrationAoPGain = 50;  //by Default for A2 devices
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
    V_Delta_Cell_Max = 0;
    balancingState = 0;
    operatingArea_Pack = ZONE_GREEN;
    state = c_Cell::CellState::OFF;
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

    Cell0.Temperature = randomTemp();
    Cell1.Temperature = randomTemp();
    Cell2.Temperature = randomTemp();
    Cell3.Temperature = randomTemp();

    Cell0.SoC = randomSoC();
    Cell1.SoC = randomSoC();
    Cell2.SoC = randomSoC();
    Cell3.SoC = randomSoC();

    Cell0.SoH = randomSoH();
    Cell1.SoH = randomSoH();
    Cell2.SoH = randomSoH();
    Cell3.SoH = randomSoH();

    //IShunt = randomCurrent();
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

  V_Max_Pack = max(max(Cell0.Voltage, Cell1.Voltage), max(Cell2.Voltage, Cell3.Voltage));
  V_Min_Pack = min(min(Cell0.Voltage, Cell1.Voltage), min(Cell2.Voltage, Cell3.Voltage));
  cellNbrVMax = whoMax(Cell0.Voltage, Cell1.Voltage, Cell2.Voltage, Cell3.Voltage);
  cellNbrVMin = whoMin(Cell0.Voltage, Cell1.Voltage, Cell2.Voltage, Cell3.Voltage);

  T_Max_Pack = max(max(Cell0.Temperature, Cell1.Temperature), max(Cell2.Temperature, Cell3.Temperature));
  T_Min_Pack = min(min(Cell0.Temperature, Cell1.Temperature), min(Cell2.Temperature, Cell3.Temperature));
  cellNbrTMax = whoMax(Cell0.Temperature, Cell1.Temperature, Cell2.Temperature, Cell3.Temperature);
  cellNbrTMin = whoMin(Cell0.Temperature, Cell1.Temperature, Cell2.Temperature, Cell3.Temperature);

  SoCCellMax = max(max(Cell0.SoC, Cell1.SoC), max(Cell2.SoC, Cell3.SoC));
  SoCCellMin = min(min(Cell0.SoC, Cell1.SoC), min(Cell2.SoC, Cell3.SoC));
  cellNbrSoCMax = whoMax(Cell0.SoC, Cell1.SoC, Cell2.SoC, Cell3.SoC);
  cellNbrSoCMin = whoMin(Cell0.SoC, Cell1.SoC, Cell2.SoC, Cell3.SoC);

  SoHCellMax = max(max(Cell0.SoH, Cell1.SoH), max(Cell2.SoH, Cell3.SoH));
  SoHCellMin = min(min(Cell0.SoH, Cell1.SoH), min(Cell2.SoH, Cell3.SoH));
  cellNbrSoHMax = whoMax(Cell0.SoH, Cell1.SoH, Cell2.SoH, Cell3.SoH);
  cellNbrSoHMin = whoMin(Cell0.SoH, Cell1.SoH, Cell2.SoH, Cell3.SoH);
  V_Delta_Cell_Max = V_Max_Pack - V_Min_Pack; //Shall be bellow DELTA_U_MAX for a balanced pack
  
  V_Pack = Cell0.Voltage + Cell1.Voltage + Cell2.Voltage + Cell3.Voltage;

  SoC_Pack = min(min(Cell0.SoC, Cell1.SoC), min(Cell2.SoC, Cell3.SoC));
  SoH_Pack = min(min(Cell0.SoH, Cell1.SoH), min(Cell2.SoH, Cell3.SoH));

  IMD_Pack = min(min(Cell0.IMD, Cell1.IMD), min(Cell2.IMD, Cell3.IMD));
  IMR_Pack = min(min(Cell0.IMR, Cell1.IMR), min(Cell2.IMR, Cell3.IMR));
  IMC_Pack = min(min(Cell0.IMC, Cell1.IMC), min(Cell2.IMC, Cell3.IMC));

  // All Cells must have the same state, else the pack is off
  if ((Cell0.getState() == Cell1.getState()) && (Cell1.getState() == Cell2.getState()) && (Cell2.getState() == Cell3.getState()))
    state = Cell0.getState();
  else
    state = c_Cell::CellState::OFF;

  // Updating data according to state
  if (V_Delta_Cell_Max > DELTA_U_MAX) //A cell is too charged compared to others, if we are in charge or standby we should balance it
  {
    if (DEG_C_TO_TEMP(cellNbrVMax.Temperature) < YELLOW_TEMP_HIGH) //On fait attention à ne pas trop chauffer
    {
      cellNbrVMax.setBypassState(true);
      if (cellNbrVMax == Cell0) { IOManager.setBalancingC1(true); balancingState = 0x01;}
      if (cellNbrVMax == Cell1) { IOManager.setBalancingC2(true); balancingState = 0x02;}
      if (cellNbrVMax == Cell2) { IOManager.setBalancingC3(true); balancingState = 0x04;}
      if (cellNbrVMax == Cell3) { IOManager.setBalancingC4(true); balancingState = 0x08;}
    }
  }
  else
  {
    Cell0.setBypassState(false); Cell1.setBypassState(false); Cell2.setBypassState(false); Cell3.setBypassState(false);
    IOManager.setBalancingC1(false); IOManager.setBalancingC2(false); IOManager.setBalancingC3(false); IOManager.setBalancingC4(false);
    balancingState = 0x00;
  }
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

  printf("%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;%u;W\n",
          Cell0.Voltage, Cell1.Voltage, Cell2.Voltage, Cell3.Voltage, cellNumberToU8(cellNbrVMax), cellNumberToU8(cellNbrVMin), Cell0.Temperature, Cell1.Temperature, Cell2.Temperature, Cell3.Temperature,
          cellNumberToU8(cellNbrTMax), cellNumberToU8(cellNbrTMin), Cell0.SoC, Cell1.SoC, Cell2.SoC, Cell3.SoC, cellNumberToU8(cellNbrSoCMax), cellNumberToU8(cellNbrSoCMin),
          Cell0.SoH, Cell1.SoH, Cell2.SoH, Cell3.SoH, cellNumberToU8(cellNbrSoHMax), cellNumberToU8(cellNbrSoHMin), V_Pack, IShunt, SoC_Pack,
          SoH_Pack, alarmN1, alarmN2, alarmN3, calibrationShuntValue, IMD_Pack, IMR_Pack, IMC_Pack, stateValue, balancingState, BMS_SERIAL_NUMBER);
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

c_Cell c_battStats::whoMin(uint16_t Cell0Val, uint16_t Cell1Val, uint16_t Cell2Val, uint16_t Cell3Val)
{
  uint16_t minVal = min(min(Cell0Val, Cell1Val), min(Cell2Val, Cell3Val));
  if (minVal == Cell0Val) return Cell0;
  if (minVal == Cell1Val) return Cell1;
  if (minVal == Cell2Val) return Cell2;
  if (minVal == Cell3Val) return Cell3;
  else return Cell0;
}

c_Cell c_battStats::whoMax(uint16_t Cell0Val, uint16_t Cell1Val, uint16_t Cell2Val, uint16_t Cell3Val)
{
  uint16_t maxVal = max(max(Cell0Val, Cell1Val), max(Cell2Val, Cell3Val));
  if (maxVal == Cell0Val) return Cell0;
  if (maxVal == Cell1Val) return Cell1;
  if (maxVal == Cell2Val) return Cell2;
  if (maxVal == Cell3Val) return Cell3;  
  else return Cell0;
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

uint8_t c_battStats::cellNumberToU8(c_Cell cell)
{
  if (cell == Cell0) return 0;
  if (cell == Cell1) return 1;
  if (cell == Cell2) return 2;
  if (cell == Cell3) return 3;
  else return 0;
}