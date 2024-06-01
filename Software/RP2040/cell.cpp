#include "cell.h"

#include "pico/stdlib.h"
#include <stdio.h>

c_Cell::c_Cell()
{
    SoC                         = 0;            //Dummy Value, to be grabbed from the EEPROM
    SoH                         = 0;            //Dummy Value, to be grabbed from the EEPROM
    Voltage                     = 0;            //Dummy Value, to be grabbed from the EEPROM
    Temperature                 = 0;            //Dummy Value, to be grabbed from the EEPROM
    CyclesCpt                   = 45;           //Dummy Value, to be grabbed from the EEPROM
    operatingArea               = ZONE_GREEN;   //Dummy Value, to be grabbed from the EEPROM

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

    alarms[0] = overTemperatureBlackOn; alarms[1] = underTemperatureBlackOn; alarms[2] = overTemperatureRedOn;
    alarms[3] = underTemperatureRedOn; alarms[4] = overTemperatureYellowOn; alarms[5] = underTemperatureYellowOn;
    alarms[6] = overVoltageBlackOn; alarms[7] = underVoltageBlackOn; alarms[8] = overVoltageRedOn;
    alarms[9] = underVoltageRedOn; alarms[10] = overVoltageYellowOn; alarms[11] = underVoltageYellowOn;
}

bool c_Cell::setBypassState(bool bypassState)
{
    return true;
}

void c_Cell::checkCell()
{
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
    printf("Alarms : [%u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u]\n", alarms[0], alarms[1],alarms[2], alarms[3], alarms[4],
                                                                         alarms[5], alarms[6], alarms[7], alarms[8], alarms[9],
                                                                         alarms[10], alarms[11]);
}