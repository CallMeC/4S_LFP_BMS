#include "pico/stdlib.h"
#include <stdio.h>
#include "appVariable.h"
#include "flashManager.h"
#include <string.h>

c_FlashManager::c_FlashManager()
{
    //ToBeCompleted
}

void c_FlashManager::Init(uint8_t x,  c_I2C whichI2C)
{
    EEPROM_ADDRESS = x;
    I2C_EEPROM = whichI2C;

    read_eeprom(0x0000, param_data, 8); //Retreiving Parameters
    BMS_SERIAL_NUMBER = (param_data[0] << 24) | (param_data[1] << 16) | (param_data[2] << 8)  | param_data[3];
    BATTSTAT.calibrationShuntValue = ((param_data[4] << 8) | param_data[5]);
    BATTSTAT.calibrationAoPGain = param_data[6];

    read_eeprom(0x0020, param_data, 24); //Retreiving Parameters
    for (uint8_t i = 0; i < 24; i++)
        BATTSTAT.Cell0.serialNumber[i] = param_data[i];
    read_eeprom(0x0040, param_data, 24); //Retreiving Parameters
    for (uint8_t i = 0; i < 24; i++)
        BATTSTAT.Cell1.serialNumber[i] = param_data[i];
    read_eeprom(0x0060, param_data, 24); //Retreiving Parameters
    for (uint8_t i = 0; i < 24; i++)
        BATTSTAT.Cell2.serialNumber[i] = param_data[i];
    read_eeprom(0x0080, param_data, 24); //Retreiving Parameters
    for (uint8_t i = 0; i < 24; i++)
        BATTSTAT.Cell3.serialNumber[i] = param_data[i];

    read_eeprom(0x00A0, param_data, 8); //Retreiving Parameters
    BATTSTAT.Cell0.SoC = ((param_data[0] << 8) | param_data[1]);
    BATTSTAT.Cell1.SoC = ((param_data[2] << 8) | param_data[3]);
    BATTSTAT.Cell2.SoC = ((param_data[4] << 8) | param_data[5]);
    BATTSTAT.Cell3.SoC = ((param_data[6] << 8) | param_data[7]);
    
}

void c_FlashManager::write_eeprom(uint16_t mem_addr, uint8_t *data, size_t len)
{
    uint8_t buf[len + 2];
    buf[0] = (mem_addr >> 8) & 0xFF;  // Adresse mémoire haute
    buf[1] = mem_addr & 0xFF;         // Adresse mémoire basse
    memcpy(&buf[2], data, len);       // Données à écrire

    i2c_write_blocking(I2C_PORT, EEPROM_ADDRESS, buf, len + 2, false);
    sleep_ms(5);  // Attendre la fin de l'écriture
}

void c_FlashManager::read_eeprom(uint16_t mem_addr, uint8_t *data, size_t len)
{
    uint8_t addr_buf[2];
    addr_buf[0] = (mem_addr >> 8) & 0xFF;  // Adresse mémoire haute
    addr_buf[1] = mem_addr & 0xFF;         // Adresse mémoire basse

    i2c_write_blocking(I2C_PORT, EEPROM_ADDRESS, addr_buf, 2, true);  // Écrire l'adresse mémoire
    i2c_read_blocking(I2C_PORT, EEPROM_ADDRESS, data, len, false);    // Lire les données
}

void c_FlashManager::updateParam()
{
    param_data[0] = (BMS_SERIAL_NUMBER >> 24) & 0xFF;
    param_data[1] = (BMS_SERIAL_NUMBER >> 16) & 0xFF;
    param_data[2] = (BMS_SERIAL_NUMBER >> 8) & 0xFF;
    param_data[3] = BMS_SERIAL_NUMBER & 0xFF;

    param_data[4] = (BATTSTAT.calibrationShuntValue >> 8) & 0xFF;
    param_data[5] = BATTSTAT.calibrationShuntValue & 0xFF;
    param_data[6] = BATTSTAT.calibrationAoPGain;

    write_eeprom(0x0000, param_data, 8);

    for (uint8_t i = 0; i < 24; i++)
        param_data[i] = BATTSTAT.Cell0.serialNumber[i];
    write_eeprom(0x0020, param_data, 24);

    for (uint8_t i = 0; i < 24; i++)
        param_data[i] = BATTSTAT.Cell1.serialNumber[i];
    write_eeprom(0x0040, param_data, 24);

    for (uint8_t i = 0; i < 24; i++)
        param_data[i] = BATTSTAT.Cell2.serialNumber[i];
    write_eeprom(0x0060, param_data, 24);

    for (uint8_t i = 0; i < 24; i++)
        param_data[i] = BATTSTAT.Cell3.serialNumber[i];
    write_eeprom(0x0080, param_data, 24);
    
    param_data[0] = (BATTSTAT.Cell0.SoC >> 8) & 0xFF;
    param_data[1] = BATTSTAT.Cell0.SoC & 0xFF;
    param_data[2] = (BATTSTAT.Cell0.SoC >> 8) & 0xFF;
    param_data[3] = BATTSTAT.Cell0.SoC & 0xFF;
    param_data[4] = (BATTSTAT.Cell0.SoC >> 8) & 0xFF;
    param_data[5] = BATTSTAT.Cell0.SoC & 0xFF;
    param_data[6] = (BATTSTAT.Cell0.SoC >> 8) & 0xFF;
    param_data[7] = BATTSTAT.Cell0.SoC & 0xFF;
    write_eeprom(0x00A0, param_data, 8);
}

void c_FlashManager::dumpEEPROM()
{
    read_eeprom(0x0000, param_data, 24); //Retreiving Parameters
    printf("EEPROM :\n");
    for (uint8_t i = 0; i < 24; i++)
        printf("%02X ", param_data[i]);
    read_eeprom(0x0020, param_data, 24); //Retreiving Parameters
    printf("\n");
    for (uint8_t i = 0; i < 24; i++)
        printf("%02X ", param_data[i]);
    read_eeprom(0x0040, param_data, 24); //Retreiving Parameters
    printf("\n");
    for (uint8_t i = 0; i < 24; i++)
        printf("%02X ", param_data[i]);
    read_eeprom(0x0060, param_data, 24); //Retreiving Parameters
    printf("\n");
    for (uint8_t i = 0; i < 24; i++)
        printf("%02X ", param_data[i]);
    printf("\n");
    for (uint8_t i = 0; i < 8; i++)
        printf("%02X ", param_data[i]);
}