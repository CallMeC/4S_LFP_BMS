#ifndef __IO_MANAGER__
#define __IO_MANAGER__

#include "pico/stdlib.h"
#include <stdio.h>

#define SOMETHINGTODODEFAULT    0
#define SOMETHINGTODO           1
#define SOMETHINGTODOTOO        2
#define UPDATE_VALUES           3
#define READ_ADC                4
#define UPDATE_CURRENT          5
#define READ_CELLS              6

#define READ_NTC_1              1
#define READ_NTC_2              2
#define READ_NTC_3              3
#define READ_NTC_4              4

#define READ_CELL_1             1
#define READ_CELL_2             2
#define READ_CELL_3             3
#define READ_CELL_4             4

#define V_IN                    3.3         // Power Supply Voltage Value
#define R1                      100000      // Résistance fixe en ohms
#define R0                      10000       // Résistance NTC à 25°C en ohms
#define BETA                    3950        // Coefficient Beta en Kelvin
#define T0                      298.15      // Température de référence en Kelvin (25°C)

#define GAIN_CELL_1             8.495
#define GAIN_CELL_2             6.01
#define GAIN_CELL_3             6.05
#define GAIN_CELL_4             2.04

class c_IO_Manager
{
    public:
        uint8_t LED_MICRO_STATE;
        uint8_t ADC_READ_STATE;
        uint8_t CELLS_READ_STATE;

        double rawTemp1, rawTemp2, rawTemp3, rawTemp4;
        double rawVCell1, rawVCell2, rawVCell3, rawVCell4;
        double ImaxValue;

        //Timeout
        uint32_t TimeoutCount;
        uint32_t TimeoutUserCount;

        void IO_Toggle();
        void GPIO_init();
        void bootfinished();
        void peripheralsInit();
        void mainLoop();
        void callBack();
        void cells_update();
        void setBalancingC1(bool state);
        void setBalancingC2(bool state);
        void setBalancingC3(bool state);
        void setBalancingC4(bool state);
        double calculateTemperature(double v_out);
        void autoWakeUpEnd();
    
    private:
        double gain_cell_1;
        double gain_cell_2;
        double gain_cell_3;
        double gain_cell_4;
        uint8_t loopCounterAlive;
        double updateResistance(double initialResistance, double temperature, double referenceTemperature = 25.0, double ppm = 100.0);
};

class CurrentFilter
{
private:
    static const int BUFFER_SIZE = 10;
    double buffer[BUFFER_SIZE];
    int index;
    int count;

public:
    CurrentFilter();
    double addCurrent(double newCurrent);
};

class TemperatureFilter
{
private:
    static const int BUFFER_SIZE = 10;
    double buffer[BUFFER_SIZE];
    int index;
    int count;

public:
    TemperatureFilter();
    double addTemperature(double newTemp);
};

class VoltageFilter
{
private:
    static const int BUFFER_SIZE = 5;
    double buffer[BUFFER_SIZE];
    int index;
    int count;

public:
    VoltageFilter();
    double addVoltage(double newVoltage);
};
#endif