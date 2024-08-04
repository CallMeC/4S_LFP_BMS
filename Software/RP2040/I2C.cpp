#include "I2C.h"
#include <string.h>

c_I2C::c_I2C() {}

//Ecrit dans un registre à partir du buffer
void c_I2C::sys_i2c_rbuf_reg(i2c_inst_t* i2c, uint8_t addr, uint8_t reg, uint8_t* buf, size_t len)
{
    i2c_write_blocking(i2c, addr, &reg, 1, true);
    i2c_read_blocking(i2c, addr, buf, len, false);
}

void c_I2C::sys_i2c_wbuf_reg(i2c_inst_t* i2c, uint8_t addr, uint8_t reg, const uint8_t* buf, size_t len)
{
    uint8_t data[len + 1];
    data[0] = reg;
    memcpy(&data[1], buf, len);
    i2c_write_blocking(i2c, addr, data, len + 1, false);
}

void c_I2C::Init() //Init of the I2C peripheral
{
    i2c_init(I2C_PORT, I2C_FREQUENCY);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

void c_I2C::Scan(i2c_inst_t* i2c)
{
    for (uint8_t i = 1; i < 255; i++)
    {
        int32_t ret = i2c_write_timeout_us(i2c, i, 0x00, 1, true, I2C_RTC_TIMEOUT);
        if (ret > 0)
            printf("%02X\n", i);
    }
}