#ifndef __FLASH_MANAGER__
#define __FLASH_MANAGER__

#include "pico/stdlib.h"
#include <stdio.h>

// We are going to erase and reprogram a region 256k from the start of flash.
// Once done, we can access this at XIP_BASE + 256k.
// XIP is the begining on the flash.
#define FLASH_TARGET_OFFSET (256 * 1024)

#define PARAM_FLASH_0   FMGR_NAME.pageOne[0]
#define PARAM_FLASH_1   FMGR_NAME.pageOne[1]
#define PARAM_FLASH_2   FMGR_NAME.pageOne[2]
#define PARAM_FLASH_3   FMGR_NAME.pageOne[3]
#define PARAM_FLASH_4   FMGR_NAME.pageOne[4]
#define PARAM_FLASH_5   FMGR_NAME.pageOne[5]
#define PARAM_FLASH_6   FMGR_NAME.pageOne[6]
#define PARAM_FLASH_7   FMGR_NAME.pageOne[7]

class c_FlashManager
{
    public:
        c_FlashManager();

        bool writeFlash();
        bool readFlash();
        const uint8_t *flash_target_contents;

        uint8_t pageOne[256];   //One page size is 256 bytes
};

#endif