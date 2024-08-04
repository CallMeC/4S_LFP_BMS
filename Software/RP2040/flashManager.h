#ifndef __FLASH_MANAGER__
#define __FLASH_MANAGER__

#include "pico/stdlib.h"
#include <stdio.h>

// EEPROM Parameters
#define EEPROM_ADDR 0x50    //Addr Flash ou D0

class c_FlashManager
{
    public:
        c_I2C I2C_EEPROM;
        uint8_t EEPROM_ADDRESS;
        
        c_FlashManager();
        void Init(uint8_t x,  c_I2C whichI2C);
        void write_eeprom(uint16_t mem_addr, uint8_t *data, size_t len);
        void read_eeprom(uint16_t mem_addr, uint8_t *data, size_t len);
        void updateParam();
        void dumpEEPROM();
        uint8_t param_data[8];
};

#endif