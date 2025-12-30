# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain-arm.cmake)

project(my_firmware LANGUAGES C ASM)

set(MCU_FAMILY STM32F401)
set(MCU_MODEL STM32F401RE)

add_executable(firmware
    src/main.c
    startup/startup_stm32f407xx.s
)

set_target_properties(firmware PROPERTIES
    SUFFIX ".elf"
    LINK_FLAGS "-T${CMAKE_SOURCE_DIR}/linker/STM32F401RETX_FLASH.ld"
)

add_custom_target(flash
    COMMAND openocd -f interface/stlink.cfg -f target/stm32f4x.cfg 
            -c "program firmware.elf verify reset exit"
    DEPENDS firmware
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
