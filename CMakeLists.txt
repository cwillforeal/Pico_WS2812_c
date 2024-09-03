# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)
SET( CMAKE_BUILD_TYPE Debug )
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(WS2812_Example C CXX ASM)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)

set(PICO_CXX_ENABLE_RTTI 1)

# Initialise the Pico SDK
pico_sdk_init()

# Create the C example
add_executable(WS2812_Example_C
        example/example.c
        src/WS2812.c
)

target_include_directories(WS2812_Example_C
        PRIVATE
        src
)
pico_generate_pio_header(WS2812_Example_C ${CMAKE_CURRENT_LIST_DIR}/src/WS2812.pio)

pico_set_program_name(WS2812_Example_C "WS2812 Example")
pico_set_program_version(WS2812_Example_C "1.0")

pico_enable_stdio_uart(WS2812_Example_C 1)
pico_enable_stdio_usb(WS2812_Example_C 1)

# Add the standard library to the build
target_link_libraries(WS2812_Example_C
        pico_stdlib
        pico_malloc
        hardware_dma
)

# Add any user requested libraries
target_link_libraries(WS2812_Example_C
        hardware_pio
)

pico_add_extra_outputs(WS2812_Example_C)
