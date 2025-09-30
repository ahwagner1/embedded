cmake_minimum_required(VERSION 3.16)

# Set toolchain before project()
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain-arm.cmake)

project(my_firmware C ASM)

# MCU-specific settings
set(MCU_FAMILY STM32F4xx)
set(MCU_MODEL STM32F407xx)

# Add executable
add_executable(firmware
    src/main.c
    startup/startup_stm32f407xx.s
)

# Set properties for embedded target
set_target_properties(firmware PROPERTIES
    SUFFIX ".elf"
    LINK_FLAGS "-T${CMAKE_SOURCE_DIR}/linker/STM32F407VGTx_FLASH.ld"
)

# Add custom targets for flashing
add_custom_target(flash
    COMMAND openocd -f interface/stlink.cfg -f target/stm32f4x.cfg 
            -c "program firmware.elf verify reset exit"
    DEPENDS firmware
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
