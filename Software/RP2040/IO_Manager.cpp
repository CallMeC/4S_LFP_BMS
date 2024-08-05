#include "appVariable.h"
#include "hardware/adc.h"
#include "IO_Manager.h"
#include "IO_Define.h"
#include "cmath"

void c_IO_Manager::IO_Toggle()
{   
    if (LED_MICRO_STATE == 0) LED_MICRO_STATE = 1; else LED_MICRO_STATE = 0;
    gpio_put(OUTPUT_LED, LED_MICRO_STATE);
}

void c_IO_Manager::GPIO_init()
{
    VERBOSE_ENABLED = 0;
    ADC_READ_STATE = 0;
    

    gpio_init(RESET);
    gpio_init(OUTPUT_LED);
    gpio_init(BALANCING_C1);
    gpio_init(BALANCING_C2);
    gpio_init(BALANCING_C3);
    gpio_init(BALANCING_C4);
    gpio_init(TIMER_DONE);
    gpio_init(STAYING_ALIVE);
    gpio_init(RELAY_DRIVE);

    gpio_set_dir(PIN_25, GPIO_OUT);

    gpio_set_dir(RESET, GPIO_OUT);
    gpio_set_dir(OUTPUT_LED, GPIO_OUT);
    gpio_set_dir(BALANCING_C1, GPIO_OUT);
    gpio_set_dir(BALANCING_C2, GPIO_OUT);
    gpio_set_dir(BALANCING_C3, GPIO_OUT);
    gpio_set_dir(BALANCING_C4, GPIO_OUT);
    gpio_set_dir(TIMER_DONE, GPIO_OUT);
    gpio_set_dir(STAYING_ALIVE, GPIO_OUT);
    gpio_set_dir(RELAY_DRIVE, GPIO_OUT);


    gpio_put(PIN_25, 1);            //Used for dev.

    gpio_put(RESET, 0);             //We want to stay alive
    gpio_put(STAYING_ALIVE, 1);     //We keep the DCDC enabled
    gpio_put(TIMER_DONE, 0);        //We want to stay alive

    gpio_put(OUTPUT_LED, 1);        //Assert good startup
    gpio_put(BALANCING_C1, 0);      //No active balancing
    gpio_put(BALANCING_C2, 0);      //No active balancing
    gpio_put(BALANCING_C3, 0);      //No active balancing
    gpio_put(BALANCING_C4, 0);      //No active balancing
    gpio_put(RELAY_DRIVE, 0);       //Relay is not activated yet
    

    adc_init();
    adc_gpio_init(CURRENT_SHUNT);
    adc_select_input(0);
}

void c_IO_Manager::bootfinished()
{
    ImaxValue = (0.066 / BATTSTAT.calibrationShuntValue)*1000000;
    static repeating_timer_t timer;
    add_repeating_timer_ms(500, &timer_callback, NULL, &timer);
    LED_MICRO_STATE = 0;
}

void c_IO_Manager::peripheralsInit()
{
    /*INIT EXTERNAL ADC HERE*/
    I2C_0.Init();

    ADC_0.Init(0x6B, I2C_0);
    FMGR_NAME.Init(0x50, I2C_0);
}

void c_IO_Manager::mainLoop()
{
    switch(IO_OPERATION)
    {
        case SOMETHINGTODO:
            printf("IO TOGGLED\n");
            break;

        case UPDATE_VALUES:
            BATTSTAT.Cell0.Temperature = TEMP_TO_DEG_C(rawTemp4);
            BATTSTAT.Cell1.Temperature = TEMP_TO_DEG_C(rawTemp2);
            BATTSTAT.Cell2.Temperature = TEMP_TO_DEG_C(rawTemp3);
            BATTSTAT.Cell3.Temperature = TEMP_TO_DEG_C(rawTemp1);
            break;
        
        case UPDATE_CURRENT:
            //printf("ADC PTS : %u\n", adc_read());
            //330A - 4096 pts
            BATTSTAT.IShunt = ImaxValue/4096*adc_read();   //Conversion 12-bits -> Amps
            break;
        
        case READ_ADC:
            //printf("READ ADC\n");
            switch(ADC_READ_STATE)
            {
            case READ_NTC_1:
                rawTemp1 = calculateTemperature(ADC_0.readADC(CH1_HEX)*4.096/262143.0);
                //printf("T1 : %.2f\n", rawTemp1);
                rawTemp1 = tempFilterNTC1.addTemperature(rawTemp1);
                break;
            
            case READ_NTC_2:
                rawTemp2 = calculateTemperature(ADC_0.readADC(CH2_HEX)*4.096/262143.0);
                //printf("T2 : %.2f\n", rawTemp2);
                rawTemp2 = tempFilterNTC2.addTemperature(rawTemp2);
                break;

            case READ_NTC_3:
                rawTemp3 = calculateTemperature(ADC_0.readADC(CH3_HEX)*4.096/262143.0);
                //printf("T3 : %.2f\n", rawTemp3);
                rawTemp3 = tempFilterNTC3.addTemperature(rawTemp3);
                break;
            
            case READ_NTC_4:
                rawTemp4 = calculateTemperature(ADC_0.readADC(CH4_HEX)*4.096/262143.0);
                //printf("T4 : %.2f\n", rawTemp4);
                rawTemp4 = tempFilterNTC4.addTemperature(rawTemp4);
                break;
            
            default:
                printf("ADC STATE : %u\n", ADC_READ_STATE);
                break;
            }

        default:
            break;
    }
    IO_OPERATION = 0;   
}

void c_IO_Manager::callBack()
{
    if (IOManager.TimeoutCount++ >= 4294967296) IOManager.TimeoutCount = 0;
    IOManager.IO_Toggle();
    IO_OPERATION = READ_ADC;
    ADC_READ_STATE++; if (ADC_READ_STATE > 4) ADC_READ_STATE = 1;
    //printf("ADC_STATE : %u\n", ADC_READ_STATE);
}

void c_IO_Manager::setBalancingC1(bool state)
{
    gpio_put(BALANCING_C1, state);
}

void c_IO_Manager::setBalancingC2(bool state)
{
    gpio_put(BALANCING_C2, state);
}

void c_IO_Manager::setBalancingC3(bool state)
{
    gpio_put(BALANCING_C3, state);
}

void c_IO_Manager::setBalancingC4(bool state)
{
    gpio_put(BALANCING_C4, state);
}

double c_IO_Manager::calculateTemperature(double v_out)
{
    // Calculer la résistance de la NTC
    //printf("Vout : %.2f\n", v_out);
    double R_NTC = R1 * v_out / (V_IN - v_out);
    //printf("R NTC : %.2f\n", R_NTC);
    // Calculer la température en Kelvin
    double invT = (1.0 / T0) + (1.0 / BETA) * log(R_NTC / R0);
    double T_Kelvin = 1.0 / invT;

    // Convertir en Celsius
    double T_Celsius = T_Kelvin - 273.15;

    if ((T_Celsius < -25.0) || (T_Celsius > 125.0))    //Temperature error, maybe measurement
        T_Celsius = -250.0; //Not ambiguous
    return T_Celsius;
}

TemperatureFilter::TemperatureFilter() : index(0), count(0)
{
    // Initialiser le buffer avec des valeurs nulles
    for (int i = 0; i < BUFFER_SIZE; ++i)
        buffer[i] = 0.0;
}

double TemperatureFilter::addTemperature(double newTemp)
{
    // Ajouter la nouvelle température dans le buffer
    buffer[index] = newTemp;
    index = (index + 1) % BUFFER_SIZE;
    if (count < BUFFER_SIZE)
        count++;

    // Calculer la moyenne des valeurs dans le buffer
    double sum = 0.0;
    for (int i = 0; i < count; ++i)
        sum += buffer[i];

    return sum / count;
}

CurrentFilter::CurrentFilter() : index(0), count(0)
{
    // Initialiser le buffer avec des valeurs nulles
    for (int i = 0; i < BUFFER_SIZE; ++i)
        buffer[i] = 0.0;
}

double CurrentFilter::addCurrent(double newCurrent)
{
    // Ajouter le nouveau courant dans le buffer
    buffer[index] = newCurrent;
    index = (index + 1) % BUFFER_SIZE;
    if (count < BUFFER_SIZE)
        count++;

    // Calculer la moyenne des valeurs dans le buffer
    double sum = 0.0;
    for (int i = 0; i < count; ++i)
        sum += buffer[i];

    return sum / count;
}