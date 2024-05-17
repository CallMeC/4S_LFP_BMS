#include "appVariable.h"
#include "hardware/adc.h"
#include "IO_Manager.h"
#include "IO_Define.h"

void c_IO_Manager::IO_Toggle()
{   
    if (LED_MICRO_STATE == 0) LED_MICRO_STATE = 1; else LED_MICRO_STATE = 0;
    gpio_put(PIN_25, LED_MICRO_STATE);
}

void c_IO_Manager::GPIO_init()
{
    VERBOSE_ENABLED = 0;
    
    gpio_init(PIN_25);

    gpio_set_dir(PIN_25, GPIO_OUT);

    gpio_put(PIN_25, 1);

    adc_init();
    adc_gpio_init(CURRENT_SHUNT);
    adc_select_input(0);
}

void c_IO_Manager::bootfinished()
{
    static repeating_timer_t timer;
    add_repeating_timer_ms(500, &timer_callback, NULL, &timer);
    LED_MICRO_STATE = 0;
}

void c_IO_Manager::peripheralsInit()
{
    /*INIT EXTERNAL ADC HERE*/
    I2C_0.Init();

    ADC_0.Init(0x6D, I2C_0);
}

void c_IO_Manager::mainLoop()
{
    switch(IO_OPERATION)
    {
        case SOMETHINGTODO:
            printf("IO TOGGLED\n");
            break;
        
        default:
            break;
    }
    IO_OPERATION = 0;   
}

void c_IO_Manager::callBack()
{
    if (IOManager.TimeoutCount++ >= 4294967296) IOManager.TimeoutCount = 0;
    IOManager.IO_Toggle();
}