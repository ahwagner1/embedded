# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/toolchain-arm.cmake)

project(my_firmware LANGUAGES C ASM)

set(MCU_FAMILY STM32F401)
set(MCU_MODEL STM32F401RE)

add_executable(firmware_tx
    src/tx/main.c
    startup/startup_stm32f401retx.s
    system/syscalls.c
    system/sysmem.c
)

target_include_directories(firmware_tx PRIVATE
    src/include
)

set_target_properties(firmware_tx PROPERTIES
    SUFFIX ".elf"
    LINK_FLAGS "-T${CMAKE_SOURCE_DIR}/linker/STM32F401RETX_FLASH.ld"
)

add_custom_command(TARGET firmware_tx POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:firmware_tx> firmware_tx.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:firmware_tx> firmware_tx.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:firmware_tx>
    COMMENT "Generating TX HEX and BIN files"
)

# rx firmware
add_executable(firmware_rx
    src/rx/main.c
    startup/startup_stm32f401retx.s
    system/syscalls.c
    system/sysmem.c
)

target_include_directories(firmware_rx PRIVATE
    src/include
)

set_target_properties(firmware_rx PROPERTIES
    SUFFIX ".elf"
    LINK_FLAGS "-T${CMAKE_SOURCE_DIR}/linker/STM32F401RETX_FLASH.ld"
)

add_custom_command(TARGET firmware_rx POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:firmware_rx> firmware_rx.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:firmware_rx> firmware_rx.bin
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:firmware_rx>
    COMMENT "Generating RX HEX and BIN files"
)

# gotta flash based on serial numbers
set(STLINK_SERIAL_TX "066AFF544884654867033519")
set(STLINK_SERIAL_RX "066AFF3332504E3043214636")

# flash the targets
add_custom_target(flash_tx
    COMMAND openocd -f interface/stlink.cfg -f target/stm32f4x.cfg 
            -c "hla_serial ${STLINK_SERIAL_TX}"
            -c "program firmware_tx.elf verify reset exit"
    DEPENDS firmware_tx
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Flashing TX firmware (Serial: ${STLINK_SERIAL_TX})"
)

add_custom_target(flash_rx
    COMMAND openocd -f interface/stlink.cfg -f target/stm32f4x.cfg 
            -c "hla_serial ${STLINK_SERIAL_RX}"
            -c "program firmware_rx.elf verify reset exit"
    DEPENDS firmware_rx
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Flashing RX firmware (Serial: ${STLINK_SERIAL_RX})"
)
