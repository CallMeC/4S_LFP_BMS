#ifndef __IO_MANAGER__
#define __IO_MANAGER__

#include "pico/stdlib.h"
#include <stdio.h>

#define SOMETHINGTODODEFAULT    0
#define SOMETHINGTODO           1
#define SOMETHINGTODOTOO        2
#define UPDATE_VALUES           3

class c_IO_Manager
{
    public:
        uint8_t LED_MICRO_STATE;

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
};

#endif