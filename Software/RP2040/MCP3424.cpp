#include "pico/stdlib.h"
#include <stdio.h>
#include "appVariable.h"
#include "MCP3424.h"

c_MCP3424::c_MCP3424() {}

void c_MCP3424::Init(uint8_t x,  c_I2C whichI2C)
{
    ADC_ADDRESS = x;
    I2C_ADC = whichI2C;
}

int c_MCP3424::readADC(uint8_t ADC_CHANNEL)
{
    uint8_t rcv_buf[3] = {0};
        
    I2C_ADC.sys_i2c_rbuf_reg(I2C_PORT, this->ADC_ADDRESS, ADC_CHANNEL, rcv_buf, 3);
    //printf("BUFF %02X %02X %02X\n", rcv_buf[0], rcv_buf[1], rcv_buf[2]);
    int value = (rcv_buf[0]&0x03)<<16 | rcv_buf[1]<<8 | rcv_buf[2];

    //Gestion de nombres négatifs
    if ((value & (1 << (BITS - 1))) != 0)  
        value = value - (1 << BITS);
    //printf("Value : %u %f V\n", value, value*4.096/262143.0);
    //generalCallReset();

    return value;
}

void c_MCP3424::generalCallReset()
{
    uint8_t resetCommand = 0x06; // Commande de réinitialisation pour le MCP3424
    I2C_ADC.sys_i2c_wbuf_reg(I2C_PORT, 0x00, resetCommand, nullptr, 0); // Adresse générale 0x00
}