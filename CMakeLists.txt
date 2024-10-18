
cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Include the Pico SDK
# Ensure PICO_SDK_PATH is set correctly
if (NOT DEFINED PICO_SDK_PATH)
    set(PICO_SDK_PATH "/Users/eddieseet/pico/pico-sdk")
endif()
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

# # Add lwipopts.h for lwIP options
# add_definitions(-DLWIP_TIMEVAL_PRIVATE=0)

# Project name is 'blink'
project(blink C CXX ASM)


# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# Initialize the Pico SDK
pico_sdk_init()

add_executable(blink
    blink.c
)

# Link the standard Pico libraries and FreeRTOS
target_link_libraries(blink 
    pico_stdlib
    # pico_cyw43_arch_lwip_threadsafe_background
	# FreeRTOS-Kernel-Heap4 # FreeRTOS kernel and dynamic heap
)

# Enable USB serial output
pico_enable_stdio_usb(blink 1)
pico_enable_stdio_uart(blink 0)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(blink)









