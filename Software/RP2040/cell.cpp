#include "cell.h"

#include "pico/stdlib.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include "appVariable.h"

c_Cell::c_Cell()
{
    SoC                         = 0;            //Dummy Value, to be grabbed from the EEPROM
    SoH                         = 0;            //Dummy Value, to be grabbed from the EEPROM
    Voltage                     = 0;            //Dummy Value, to be grabbed from the EEPROM
    Temperature                 = 0;            //Dummy Value, to be grabbed from the EEPROM
    CyclesCpt                   = 45;           //Dummy Value, to be grabbed from the EEPROM
    operatingArea               = ZONE_GREEN;   //Dummy Value, to be grabbed from the EEPROM
    IMR                         = 0;
    IMC                         = 0;
    IMD                         = 0;

    overTemperatureBlackOn      = false;
    underTemperatureBlackOn     = false;
    overTemperatureRedOn        = false;
    underTemperatureRedOn       = false;
    overTemperatureYellowOn     = false;
    underTemperatureYellowOn    = false;

    overVoltageBlackOn          = false;
    underVoltageBlackOn         = false;
    overVoltageRedOn            = false;
    underVoltageRedOn           = false;
    overVoltageYellowOn         = false;
    underVoltageYellowOn        = false;

    alarmsMask = (overTemperatureBlackOn      << 0)  |
                 (underTemperatureBlackOn     << 1)  |
                 (overTemperatureRedOn        << 2)  |
                 (underTemperatureRedOn       << 3)  |
                 (overTemperatureYellowOn     << 4)  |
                 (underTemperatureYellowOn    << 5)  |
                 (overVoltageBlackOn          << 6)  |
                 (underVoltageBlackOn         << 7)  |
                 (overVoltageRedOn            << 8)  |
                 (underVoltageRedOn           << 9)  |
                 (overVoltageYellowOn         << 10) |
                 (underVoltageYellowOn        << 11);
}

bool c_Cell::setBypassState(bool bypassState)
{
    return true;
}

void c_Cell::checkCell()
{
    IMDRCcheck();
    //Voltage Check
    if (Voltage > RED_VOLTAGE_HIGH)
    {
        operatingArea = ZONE_BLACK;
        overVoltageBlackOn = true;
        overVoltageRedOn = true;
        overVoltageYellowOn = true;
    }
    else if (Voltage > YELLOW_VOLTAGE_HIGH)
    {
        operatingArea = ZONE_RED;
        overVoltageBlackOn = false;
        overVoltageRedOn = true;
        overVoltageYellowOn = true;
    }
    else if (Voltage > GREEN_VOLTAGE_HIGH)
    {
        operatingArea = ZONE_YELLOW;
        overVoltageBlackOn = false;
        overVoltageRedOn = false;
        overVoltageYellowOn = true;
    }
    else if (Voltage > GREEN_VOLTAGE_LOW)
    {
        operatingArea = ZONE_GREEN;
        overVoltageBlackOn = false;
        overVoltageRedOn = false;
        overVoltageYellowOn = false;
    }
    else if (Voltage > YELLOW_VOLTAGE_LOW)
    {
        operatingArea = ZONE_YELLOW;
        overVoltageBlackOn = false;
        overVoltageRedOn = false;
        overVoltageYellowOn = true;
    }
    else if (Voltage > RED_VOLTAGE_LOW)
    {
        operatingArea = ZONE_RED;
        overVoltageBlackOn = false;
        overVoltageRedOn = true;
        overVoltageYellowOn = true;
    }
    else if (Voltage < RED_VOLTAGE_LOW)
    {
        operatingArea = ZONE_BLACK;
        overVoltageBlackOn = true;
        overVoltageRedOn = true;
        overVoltageYellowOn = true;
    }
    else
    {
        operatingArea = ZONE_BLACK;
        overVoltageBlackOn = true;
        overVoltageRedOn = true;
        overVoltageYellowOn = true;
    }

    //Temperature Check
    if (Temperature > DEG_C_TO_TEMP(RED_TEMP_HIGH))
    {
        operatingArea = ZONE_BLACK;
        overTemperatureBlackOn = true;
        overTemperatureRedOn = true;
        overTemperatureYellowOn = true;
    }
    else if (Temperature > DEG_C_TO_TEMP(YELLOW_TEMP_HIGH))
    {
        operatingArea = ZONE_RED;
        overTemperatureBlackOn = false;
        overTemperatureRedOn = true;
        overTemperatureYellowOn = true;
    }
    else if (Temperature > DEG_C_TO_TEMP(GREEN_TEMP_HIGH))
    {
        operatingArea = ZONE_YELLOW;
        overTemperatureBlackOn = false;
        overTemperatureRedOn = false;
        overTemperatureYellowOn = true;
    }
    else if (Temperature > DEG_C_TO_TEMP(GREEN_TEMP_LOW))
    {
        operatingArea = ZONE_GREEN;
        overTemperatureBlackOn = false;
        overTemperatureRedOn = false;
        overTemperatureYellowOn = false;
    }
    else if (Temperature > DEG_C_TO_TEMP(YELLOW_TEMP_LOW))
    {
        operatingArea = ZONE_YELLOW;
        overTemperatureBlackOn = false;
        overTemperatureRedOn = false;
        overTemperatureYellowOn = true;
    }
    else if (Temperature > DEG_C_TO_TEMP(RED_TEMP_LOW))
    {
        operatingArea = ZONE_RED;
        overTemperatureBlackOn = false;
        overTemperatureRedOn = true;
        overTemperatureYellowOn = true;
    }
    else if (Temperature < DEG_C_TO_TEMP(RED_TEMP_LOW))
    {
        operatingArea = ZONE_BLACK;
        overTemperatureBlackOn = true;
        overTemperatureRedOn = true;
        overTemperatureYellowOn = true;
    }
    else
    {
        operatingArea = ZONE_BLACK;
        overTemperatureBlackOn = true;
        overTemperatureRedOn = true;
        overTemperatureYellowOn = true;
    }

    alarmsMask = (overTemperatureBlackOn      << 0)  |
                 (underTemperatureBlackOn     << 1)  |
                 (overTemperatureRedOn        << 2)  |
                 (underTemperatureRedOn       << 3)  |
                 (overTemperatureYellowOn     << 4)  |
                 (underTemperatureYellowOn    << 5)  |
                 (overVoltageBlackOn          << 6)  |
                 (underVoltageBlackOn         << 7)  |
                 (overVoltageRedOn            << 8)  |
                 (underVoltageRedOn           << 9)  |
                 (overVoltageYellowOn         << 10) |
                 (underVoltageYellowOn        << 11);
}

void c_Cell::displayCell()
{
    printf("\n----CELL--DISPLAY----\n");
    printf("SoC : %u %\n", SoC);
    printf("SoH : %u %\n", SoH);
    printf("Voltage : %u mV\n", Voltage);
    printf("Temperature : %u °C\n", DEG_C_TO_TEMP(Temperature));
    printf("CyclesCpt : %u\n", CyclesCpt);
    printf("operatingArea : %u\n", operatingArea);
    printf("Alarms : %08X\n", alarmsMask);
    printf("IMD : %u\n", IMD);
    printf("IMR : %u\n", IMR);
    printf("IMC : %u\n", IMC);
}

void c_Cell::IMDRCcheck()
{
    std::vector<float> f_Map_IMD_Soc_TMax =
    {
        // [x] : Soc
        // [y] : T Cell Max
        0,          0.0,   10.0,   40.0,   50.0,   60.0,   100.0,
        //--------------------------------------------------
        -20.0, /**/ 11.0,   11.0,   11.0,   21.0,   21.0,    21.0, 
        -10.0, /**/ 11.0,   21.0,   21.0,   40.0,   53.0,    53.0, 
          0.0, /**/ 21.0,   21.0,   40.0,   53.0,  105.0,   105.0,  
         10.0, /**/ 40.0,   53.0,   53.0,  105.0,  210.0,   210.0,  
         25.0, /**/ 40.0,  105.0,  210.0,  210.0,  210.0,   210.0,  
         40.0, /**/ 40.0,  105.0,  210.0,  210.0,  210.0,   210.0,  
         50.0, /**/ 21.0,   53.0,  105.0,  105.0,  105.0,   105.0,  
         55.0, /**/ 21.0,   21.0,   21.0,   21.0,   21.0,    21.0,  
    };

    std::vector<float> f_Map_IMC_Soc_TMax =
    {
        // [x] : Soc
        // [y] : T.Cell Max
        //
        0,       0.0,    5.0,   50.0,  90.0,   100.0,
        //-------------------------------------------
        0,  /**/ 1.0,    1.0,    1.0,    1.0,    1.0,
        10, /**/ 21.0,   21.0,   21.0,   21.0,   21.0,
        15, /**/ 53.0,   53.0,   53.0,   53.0,   53.0,
        45, /**/ 53.0,   53.0,   53.0,   53.0,   53.0,
        50, /**/ 21.0,   21.0,   21.0,   21.0,   21.0,
        54, /**/ 11.0,   11.0,   11.0,   11.0,   11.0,
        55, /**/  0.0,    0.0,    0.0,    0.0,    0.0,
    };

    std::vector<float> f_Map_IMR_Soc_TMax =
    {
        // [x] : Soc
        // [y] : T.Cell Moy
        //
        0,         0.0,   10.0,   50.0,  90.0,    95.0,   100.0,
        //------------------------------------------------------
        0,  /**/   0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
        10, /**/ 105.0,  105.0,  105.0,   53.0,   21.0,   21.0,
        25, /**/ 105.0,  105.0,  105.0,  105.0,  105.0,   53.0,
        45, /**/ 105.0,  105.0,  105.0,  105.0,   53.0,   53.0,
        50, /**/  53.0,   53.0,   53.0,   53.0,   21.0,   21.0,
        55, /**/   0.0,    0.0,    0.0,    0.0,    0.0,    0.0,
    };

    float x = SoC/10;
    float y = DEG_C_TO_TEMP(Temperature);
    int numRows = 9; // Number of rows including the first T Cell Max row
    int numCols = 7; // Number of columns including the first SoC column
    IMD = (uint16_t) SysCore.getInterpolatedValue(f_Map_IMD_Soc_TMax, numRows, numCols, x, y);

    numRows = 7; // Number of rows including the first T Cell Max row
    numCols = 5; // Number of columns including the first SoC column
    IMC = (uint16_t) SysCore.getInterpolatedValue(f_Map_IMC_Soc_TMax, numRows, numCols, x, y);

    numRows = 6; // Number of rows including the first T Cell Max row
    numCols = 6; // Number of columns including the first SoC column
    IMR = (uint16_t) SysCore.getInterpolatedValue(f_Map_IMR_Soc_TMax, numRows, numCols, x, y);
}