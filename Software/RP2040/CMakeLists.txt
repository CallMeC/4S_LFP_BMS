cmake_minimum_required(VERSION 3.12)

include(/home/cve/Documents/pico/pico-sdk/external/pico_sdk_import.cmake)

project(BMS_Firmware C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

set(TARGET_NAME BMS_Firmware)

add_executable(${TARGET_NAME} main.cpp IO_Manager.cpp uartHandler.cpp I2C.cpp
                              MCP3424.cpp battStats.cpp flashManager.cpp faultsManager.cpp
                              cell.cpp systemCore.cpp can2040.c can2040.h canHandler.cpp)

target_link_libraries(${TARGET_NAME}
        pico_stdlib
        hardware_spi
        hardware_i2c
        hardware_adc
        hardware_dma
        hardware_gpio
        hardware_flash
        hardware_sync
        hardware_irq
        cmsis_core
        )

pico_enable_stdio_usb(${TARGET_NAME} 1)
pico_enable_stdio_uart(${TARGET_NAME} 0)

pico_add_extra_outputs(${TARGET_NAME})
