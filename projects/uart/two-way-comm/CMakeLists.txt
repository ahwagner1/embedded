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

target_include_directories(firmware PRIVATE
    src/include
)

set_target_properties(firmware PROPERTIES
    SUFFIX ".elf"
    LINK_FLAGS "-T${CMAKE_SOURCE_DIR}/linker/STM32F401RETX_FLASH.ld"
)

add_custom_command(TARGET firmware POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:firmware> firmware.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:firmware> firmware.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:firmware>
    COMMENT "Generating HEX and BIN files"
)

# gotta flash based on serial numbers
set(STLINK_SERIAL_B1 "066AFF544884654867033519")
set(STLINK_SERIAL_B2 "066AFF3332504E3043214636")

add_custom_target(flash_b1
    COMMAND openocd -f interface/stlink.cfg -f target/stm32f4x.cfg 
            -c "hla_serial ${STLINK_SERIAL_B1}"
            -c "program firmware.elf verify reset exit"
    DEPENDS firmware
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Flashing B1 with firmware (Serial: ${STLINK_SERIAL_B1})"
)

add_custom_target(flash_b2
    COMMAND openocd -f interface/stlink.cfg -f target/stm32f4x.cfg 
            -c "hla_serial ${STLINK_SERIAL_B2}"
            -c "program firmware.elf verify reset exit"
    DEPENDS firmware
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Flashing B2 with firmware (Serial: ${STLINK_SERIAL_B2})"
)