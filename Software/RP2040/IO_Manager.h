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

#define READ_NTC_1              1
#define READ_NTC_2              2
#define READ_NTC_3              3
#define READ_NTC_4              4
#define READ_CELL_1             5
#define READ_CELL_2             6
#define READ_CELL_3             7
#define READ_CELL_4             8

#define V_IN                    3.3         // Power Supply Voltage Value
#define R1                      100000      // Résistance fixe en ohms
#define R0                      10000       // Résistance NTC à 25°C en ohms
#define BETA                    3950        // Coefficient Beta en Kelvin
#define T0                      298.15      // Température de référence en Kelvin (25°C)

class c_IO_Manager
{
    public:
        uint8_t LED_MICRO_STATE;
        uint8_t ADC_READ_STATE;

        double rawTemp1, rawTemp2, rawTemp3, rawTemp4;
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
        void setBalancingC1(bool state);
        void setBalancingC2(bool state);
        void setBalancingC3(bool state);
        void setBalancingC4(bool state);
        double calculateTemperature(double v_out);
};

class CurrentFilter
{
private:
    static const int BUFFER_SIZE = 5;
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
#endif