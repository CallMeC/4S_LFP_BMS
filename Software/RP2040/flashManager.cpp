#include "pico/stdlib.h"
#include <stdio.h>
#include <cstring>
#include "flashManager.h"
#include "appVariable.h"
#include "hardware/flash.h"
#include "hardware/irq.h"
#include "hardware/sync.h"

c_FlashManager::c_FlashManager()
{
    // XIP_BASE (0x10000000)
    // FLASH_TARGET_OFFSET : 256kb area
    const uint8_t *const_flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);
    flash_target_contents = const_flash_target_contents;    //Copy to access const element everywhere.
    
    readFlash();    //Synchronise parameters on startup
}

bool c_FlashManager::writeFlash()
{
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(FLASH_TARGET_OFFSET, FLASH_SECTOR_SIZE);
    flash_range_program(FLASH_TARGET_OFFSET, pageOne, FLASH_PAGE_SIZE);
    restore_interrupts (ints);
    
    return true;
}

bool c_FlashManager::readFlash()
{
    memcpy(&pageOne[0], flash_target_contents, 256);
    /*for (uint8_t i = 0; i < 255; i++)
    {
        printf(" %02X", pageOne[i]);
    }*/
    return true;
}