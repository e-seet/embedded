# cmake_minimum_required(VERSION 3.13)
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(PICO_BOARD pico_w CACHE STRING "Board type")

# # Include the Pico SDK
# # Ensure PICO_SDK_PATH is set correctly
# if (NOT DEFINED PICO_SDK_PATH)
#     set(PICO_SDK_PATH "/Users/eddieseet/pico/pico-sdk")
# endif()
# include(${PICO_SDK_PATH}/pico_sdk_init.cmake)

# Pull in Raspberry Pi Pico SDK (must be before project)
# include(pico_sdk_import.cmake)

# # FreeRTOS Kernel Path
# set(FREERTOS_KERNEL_PATH "/Users/eddieseet/FreeRTOS-Kernel")
# set(CurrDir "/Users/eddieseet/MDesktop/Embedded/Lab2/MySampleCode/blink_target")

# # Add FreeRTOS Kernel Source
# add_library(freertos
#     ${FREERTOS_KERNEL_PATH}/portable/GCC/ARM_CM0/port.c
#     ${FREERTOS_KERNEL_PATH}/portable/MemMang/heap_4.c
#     ${FREERTOS_KERNEL_PATH}/tasks.c
#     ${FREERTOS_KERNEL_PATH}/list.c
#     ${FREERTOS_KERNEL_PATH}/queue.c
#     ${FREERTOS_KERNEL_PATH}/timers.c
#     ${FREERTOS_KERNEL_PATH}/event_groups.c
# )

# target_include_directories(freertos PUBLIC
#     ${FREERTOS_KERNEL_PATH}/include
#     ${FREERTOS_KERNEL_PATH}/portable/GCC/ARM_CM0
#     "/Users/eddieseet/MDesktop/Embedded/Lab2/MySampleCode/blink_target/include/"  # Add the path directly for freertos
# )

# # Project name is 'blink'
# project(blink C CXX ASM)

# # Initialize the Pico SDK
# pico_sdk_init()

# add_executable(blink
#     blink.c
# )

# add_library(freertos_config INTERFACE)
# target_include_directories(freertos_config SYSTEM
#   INTERFACE "/Users/eddieseet/MDesktop/Embedded/Lab2/MySampleCode/blink_target/include/"
# )

# message(STATUS "Using FreeRTOSConfig.h from: /Users/eddieseet/MDesktop/Embedded/Lab2/MySampleCode/blink_target/include/")
# # Link the standard Pico libraries and FreeRTOS
# target_link_libraries(blink 
# 	freertos
# 	freertos_config  # Link the config target here

# 	pico_stdlib
#     hardware_pwm
#     hardware_adc 
#     freertos
#     pico_cyw43_arch_lwip_sys_freertos
#     pico_lwip_iperf
#     pico_cyw43_arch_lwip_threadsafe_background
	
# )

# # Enable USB serial output (newly added)
# pico_enable_stdio_usb(blink 1)   # Enable USB serial output
# pico_enable_stdio_uart(blink 0)  # Disable UART serial output (optional)

# # Create map/bin/hex/uf2 files
# pico_add_extra_outputs(blink)

# # checked resources
# #https://forums.raspberrypi.com/viewtopic.php?t=334622


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

# FreeRTOS Kernel Path
set(FREERTOS_KERNEL_PATH "/Users/eddieseet/FreeRTOS-Kernel")
set(CurrDir "/Users/eddieseet/MDesktop/Embedded/Lab2/MySampleCode/blink_target")

# Add FreeRTOS Kernel Source
add_library(freertos
    ${FREERTOS_KERNEL_PATH}/portable/GCC/ARM_CM0/port.c
    # ${FREERTOS_KERNEL_PATH}/portable/MemMang/heap_4.c
    ${FREERTOS_KERNEL_PATH}/tasks.c
    ${FREERTOS_KERNEL_PATH}/list.c
    ${FREERTOS_KERNEL_PATH}/queue.c
    ${FREERTOS_KERNEL_PATH}/timers.c
    ${FREERTOS_KERNEL_PATH}/event_groups.c
)

# # Add lwipopts.h for lwIP options
# add_definitions(-DLWIP_TIMEVAL_PRIVATE=0)

# Include FreeRTOS and lwIP configuration files
target_include_directories(freertos PUBLIC
    ${FREERTOS_KERNEL_PATH}/include
    ${FREERTOS_KERNEL_PATH}/portable/GCC/ARM_CM0
    "/Users/eddieseet/MDesktop/Embedded/Lab2/MySampleCode/blink_target/include/"  # Include directory with lwipopts.h and FreeRTOSConfig.h
)



# Project name is 'blink'
project(blink C CXX ASM)


# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# Initialize the Pico SDK
pico_sdk_init()

add_executable(blink
    blink.c
)

add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM
  INTERFACE "/Users/eddieseet/MDesktop/Embedded/Lab2/MySampleCode/blink_target/include/"
)

add_definitions(-DLWIP_TIMEVAL_PRIVATE=0)

# Link the standard Pico libraries and FreeRTOS
target_link_libraries(blink 
    freertos
    freertos_config  # Link the config target here

    pico_stdlib
    hardware_pwm
    hardware_adc 
    freertos
    pico_cyw43_arch_lwip_sys_freertos
    pico_lwip_iperf
    # pico_cyw43_arch_lwip_threadsafe_background
	FreeRTOS-Kernel-Heap4 # FreeRTOS kernel and dynamic heap
)


# Enable USB serial output
pico_enable_stdio_usb(blink 1)
pico_enable_stdio_uart(blink 0)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(blink)









