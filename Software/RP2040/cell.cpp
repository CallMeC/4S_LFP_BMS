#include "cell.h"

#include "pico/stdlib.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include "appVariable.h"
#include "time.h"

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
    BypassState                 = false;

    current_prev = 0;
    last_update_time = time(nullptr);

    state                       = OFF;

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

bool c_Cell::setBypassState(bool bypassStateCmd)
{
    BypassState = bypassStateCmd;
    return true;
}

void c_Cell::checkCell()
{
    IMDRCcheck();
    //Voltage Check
    overVoltageBlackOn = (Voltage > RED_VOLTAGE_HIGH || Voltage < RED_VOLTAGE_LOW)?true:false;
    overVoltageRedOn = (Voltage > YELLOW_VOLTAGE_HIGH || Voltage < YELLOW_VOLTAGE_LOW)?true:false;
    overVoltageYellowOn = (Voltage > GREEN_VOLTAGE_HIGH || Voltage < GREEN_VOLTAGE_LOW)?true:false;
    overTemperatureBlackOn = (Temperature > DEG_C_TO_TEMP(RED_TEMP_HIGH) || Temperature < DEG_C_TO_TEMP(RED_TEMP_LOW))?true:false;
    overTemperatureRedOn = (Temperature > DEG_C_TO_TEMP(YELLOW_TEMP_HIGH) || Temperature < DEG_C_TO_TEMP(YELLOW_TEMP_LOW))?true:false;
    overTemperatureYellowOn = (Temperature > DEG_C_TO_TEMP(GREEN_TEMP_HIGH) || Temperature < DEG_C_TO_TEMP(GREEN_TEMP_LOW))?true:false;
    operatingArea = (overVoltageBlackOn || overTemperatureBlackOn)?ZONE_BLACK:((overVoltageRedOn || overTemperatureRedOn)?ZONE_RED:((overVoltageYellowOn || overTemperatureYellowOn)?ZONE_YELLOW:ZONE_GREEN));

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
    printf("State : %s\n", getStateString());
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

// Set the state of the cell
void c_Cell::setState(CellState newState)
{
    state = newState;
}

// Get the state of the cell
c_Cell::CellState c_Cell::getState() const
{
    return state;
}

// Get the state of the cell as a string
const char* c_Cell::getStateString() const
{
    switch (state)
    {
        case OFF:
            return "OFF";
        case CHARGE:
            return "CHARGE";
        case DISCHARGE:
            return "DISCHARGE";
        case STORAGE:
            return "STORAGE";
        default:
            return "UNKNOWN";
    }
}

void c_Cell::updateSoC(double current)
{   
    time_t current_time = time(nullptr); // Temps actuel
    double delta_time = difftime(current_time, last_update_time) / 3600.0; // Convertir en heures

    // Calcul de la charge ajoutée/retirée
    double delta_q = current * delta_time;
    //printf("Delta Q : %.3f | Current : %.3f\n", delta_q, current);
    // Mise à jour du SoC
    SoC += (delta_q / CELL_CAPA_AH) * 100.0; 

    // Gestion des limites du SoC
    if (SoC > 100.0) SoC = 100.0;
    if (SoC < 0.0) SoC = 0.0;

    // Mise à jour du temps et du courant
    last_update_time = current_time;
    current_prev = current;
}

float c_Cell::estimateSoCFromVoltage(double pvoltage)
{
    std::vector<float> f_Map_LFP_Soc_Voltage = {
    2.65,   0.0,   // 0% SoC correspond à 2.65V
    2.80,   5.0,   // 5% SoC correspond à 2.80V
    2.95,  10.0,   // 10% SoC correspond à 2.95V
    3.05,  15.0,   // 15% SoC correspond à 3.05V
    3.15,  20.0,   // 20% SoC correspond à 3.15V
    3.20,  25.0,   // 25% SoC correspond à 3.20V
    3.25,  30.0,   // 30% SoC correspond à 3.25V
    3.28,  35.0,   // 35% SoC correspond à 3.28V
    3.30,  40.0,   // 40% SoC correspond à 3.30V
    3.32,  45.0,   // 45% SoC correspond à 3.32V
    3.34,  50.0,   // 50% SoC correspond à 3.34V
    3.36,  55.0,   // 55% SoC correspond à 3.36V
    3.38,  60.0,   // 60% SoC correspond à 3.38V
    3.40,  65.0,   // 65% SoC correspond à 3.40V
    3.42,  70.0,   // 70% SoC correspond à 3.42V
    3.44,  75.0,   // 75% SoC correspond à 3.44V
    3.46,  80.0,   // 80% SoC correspond à 3.46V
    3.48,  85.0,   // 85% SoC correspond à 3.48V
    3.50,  90.0,   // 90% SoC correspond à 3.50V
    3.53,  95.0,   // 95% SoC correspond à 3.53V
    3.60, 100.0    // 100% SoC correspond à 3.60V
};


    int size = f_Map_LFP_Soc_Voltage.size();
    
    // Si la tension est en dehors des bornes du tableau, retourner les valeurs aux limites
    if (pvoltage <= f_Map_LFP_Soc_Voltage[0]) {
        return f_Map_LFP_Soc_Voltage[1];
    }
    if (pvoltage >= f_Map_LFP_Soc_Voltage[size - 2]) {
        return f_Map_LFP_Soc_Voltage[size - 1];
    }

    // Parcours du tableau pour trouver l'intervalle où se situe la tension
    for (int i = 0; i < size - 2; i += 2) {
        float v1 = f_Map_LFP_Soc_Voltage[i];
        float soc1 = f_Map_LFP_Soc_Voltage[i + 1];
        float v2 = f_Map_LFP_Soc_Voltage[i + 2];
        float soc2 = f_Map_LFP_Soc_Voltage[i + 3];

        if (pvoltage >= v1 && pvoltage <= v2) {
            // Interpolation linéaire entre les deux points
            float soc = soc1 + (pvoltage - v1) * (soc2 - soc1) / (v2 - v1);
            return soc;
        }
    }

    return 0.0; // Valeur par défaut (ne devrait jamais être atteinte)
}