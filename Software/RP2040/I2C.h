#ifndef __I2C__H
#define __I2C__H

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "IO_Define.h"
#include <stdio.h>

//Param I2C Devices
#define I2C_PORT                i2c0
#define I2C_RTC_TIMEOUT         10000      //Timeout (µs)
#define I2C_FREQUENCY           10000      //I2C à 10kHz

class c_I2C
{
    public:
        c_I2C();
        int32_t sys_i2c_rbuf_reg(i2c_inst_t* i2c, uint8_t addr, uint8_t reg, uint8_t* pBuf, uint32_t len);
        int32_t sys_i2c_wbuf_reg(i2c_inst_t* i2c, uint8_t addr, uint8_t reg, uint8_t* pBuf, uint32_t len);
        void Init();
        void Scan(i2c_inst_t* i2c);
};



#endif