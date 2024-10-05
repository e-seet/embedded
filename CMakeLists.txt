# cmake_minimum_required(VERSION 3.13)
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(PICO_BOARD pico_w CACHE STRING "Board type")

# # Include the Pico SDK
# include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

# # Pull in Raspberry Pi Pico SDK (must be before project)
# include(pico_sdk_import.cmake)

# # FreeRTOS Kernel Path
# set(FREERTOS_KERNEL_PATH "/Users/eddieseet/FreeRTOS-Kernel")
# # include($ENV{FREERTOS_KERNEL_PATH}/include/FreeRTOS.h)
# # include("/Users/eddieseet/FreeRTOS-Kernel/include/FreeRTOS.h")
# include("/Users/eddieseet/FreeRTOS-Kernel/include/task.h")
# include("/Users/eddieseet/FreeRTOS-Kernel/include/queue.h")
# include(FreeRTOS_Kernel_import.cmake)

# # Add FreeRTOS Kernel Source
# add_library(freertos
# 	${FREERTOS_KERNEL_PATH}/portable/GCC/ARM_CM0/port.c
#     ${FREERTOS_KERNEL_PATH}/portable/MemMang/heap_4.c
#     ${FREERTOS_KERNEL_PATH}/tasks.c
#     ${FREERTOS_KERNEL_PATH}/list.c
#     ${FREERTOS_KERNEL_PATH}/queue.c
#     ${FREERTOS_KERNEL_PATH}/timers.c
#     ${FREERTOS_KERNEL_PATH}/event_groups.c
# )
# # Add FreeRTOS include directories
# target_include_directories(freertos PUBLIC

# 	${FREERTOS_KERNEL_PATH}/include
#     ${FREERTOS_KERNEL_PATH}/portable/GCC/ARM_CM0
# )

# # Project name is 'blink'
# project(blink C CXX ASM)

# # Initialize the Pico SDK
# pico_sdk_init()

# add_executable(blink
#     blink.c
#     )

# # Link the standard Pico libraries
# target_link_libraries(
# 	blink 
# 	pico_stdlib
# 	hardware_pwm
# 	hardware_adc 
# 	# freertos
# 	pico_cyw43_arch_lwip_sys_freertos
# 	pico_lwip_iperf
# 	pico_cyw43_arch_lwip_threadsafe_background
# 	FreeRTOS-Kernel-Heap4 # FreeRTOS kernel and dynamic heap

# 	LWIP_PORT                                   # LWIP config files
# 	FREERTOS_PORT                               # FreeRTOS config files
        
# 	)

# # # if (PICO_CYW43_SUPPORTED)
# # #     target_link_libraries(blink pico_cyw43_arch_none)
# # # endif()

# # Enable USB serial output (newly added)
# pico_enable_stdio_usb(blink 1)   # Enable USB serial output
# pico_enable_stdio_uart(blink 0)  # Disable UART serial output (optional)

# # Create map/bin/hex/uf2 files
# pico_add_extra_outputs(blink)




cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Include the Pico SDK
include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# FreeRTOS Kernel Path
set(FREERTOS_KERNEL_PATH "/Users/eddieseet/FreeRTOS-Kernel")

# Add FreeRTOS Kernel Source
add_library(freertos
    ${FREERTOS_KERNEL_PATH}/portable/GCC/ARM_CM0/port.c
    ${FREERTOS_KERNEL_PATH}/portable/MemMang/heap_4.c
    ${FREERTOS_KERNEL_PATH}/tasks.c
    ${FREERTOS_KERNEL_PATH}/list.c
    ${FREERTOS_KERNEL_PATH}/queue.c
    ${FREERTOS_KERNEL_PATH}/timers.c
    ${FREERTOS_KERNEL_PATH}/event_groups.c
)


# Add FreeRTOS include directories
target_include_directories(freertos PUBLIC
    ${FREERTOS_KERNEL_PATH}/include
    ${FREERTOS_KERNEL_PATH}/portable/GCC/ARM_CM0
	${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_include_directories(freertos PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Project name is 'blink'
project(blink C CXX ASM)

# Initialize the Pico SDK
pico_sdk_init()

add_executable(blink
    blink.c
)

# Link the standard Pico libraries and FreeRTOS
target_link_libraries(blink 
    pico_stdlib
    hardware_pwm
    hardware_adc 
    freertos
    pico_cyw43_arch_lwip_sys_freertos
    pico_lwip_iperf
    pico_cyw43_arch_lwip_threadsafe_background
)

# Enable USB serial output (newly added)
pico_enable_stdio_usb(blink 1)   # Enable USB serial output
pico_enable_stdio_uart(blink 0)  # Disable UART serial output (optional)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(blink)