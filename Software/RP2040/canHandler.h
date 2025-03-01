#ifndef __CAN_HANDLER__
#define __CAN_HANDLER__

#include "pico/binary_info.h"
#include <stdio.h>
#include <string.h>
#include "hardware/irq.h"
extern "C" {
#include "can2040.h"
}
#include "IO_Define.h"

class c_canHandler
{
    public:
        c_canHandler();
        void _can_init();
        void _can_send();
        void _can_heartbeat();
        uint8_t aliveFlag;
        uint8_t canTOCounter;
    
    private:
        uint8_t heartbeat;
        
};

#endif