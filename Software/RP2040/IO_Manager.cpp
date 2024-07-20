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