# # ====================================================================================
# # == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
# if(WIN32)
#     set(USERHOME $ENV{USERPROFILE})
# else()
#     set(USERHOME $ENV{HOME})
# endif()
# set(sdkVersion 2.0.0)
# set(toolchainVersion 13_2_Rel1)
# set(picotoolVersion 2.0.0)
# include(${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
# # ====================================================================================
# # Generated Cmake Pico project file

# cmake_minimum_required(VERSION 3.13)

# set(CMAKE_C_STANDARD 11)
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# # Initialise pico_sdk from installed location
# # (note this can come from environment, CMake cache etc)

# # == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
# if(WIN32)
#     set(USERHOME $ENV{USERPROFILE})
# else()
#     set(USERHOME $ENV{HOME})
# endif()
# set(sdkVersion 1.5.1)
# set(toolchainVersion 13_2_Rel1)
# set(picotoolVersion 2.0.0)
# include(${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
# # ====================================================================================
# set(PICO_BOARD pico_w CACHE STRING "Board type")

# # Pull in Raspberry Pi Pico SDK (must be before project)
# include(pico_sdk_import.cmake)

# # Project name is 'blink'
# project(blink C CXX ASM)

# # Initialise the Raspberry Pi Pico SDK
# pico_sdk_init()

# # Add executable. Default name is the project name, version 0.1

# add_executable(blink
#     blink.c
#     )

# # pull in common dependencies
# target_link_libraries(blink pico_stdlib)

# if (PICO_CYW43_SUPPORTED)
#     target_link_libraries(blink pico_cyw43_arch_none)
# endif()

# # Enable USB serial output (newly added)
# pico_enable_stdio_usb(blink 1)   # Enable USB serial output
# pico_enable_stdio_uart(blink 0)  # Disable UART serial output (optional)

# # newly added
# # pico_enable_stdio_usb(hello_gpio_irq 1)

# # create map/bin/hex file etc.
# pico_add_extra_outputs(blink)

# # Note: the argument inside should correspond to the "XYXYXY" in the line
# # project(XYXYXY c cxx ASM)

# # add url via pico_set_program_url

# # set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.0.sdk)






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
include($ENV{FREERTOS_KERNEL_PATH}/include/FreeRTOS.h)

# include("/Users/eddieseet/FreeRTOS-Kernel/include/FreeRTOS.h")

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
)



# Project name is 'blink'
project(blink C CXX ASM)

# Initialize the Pico SDK
pico_sdk_init()

add_executable(blink
    blink.c
    )

# Link the standard Pico libraries
target_link_libraries(
	blink 
	pico_stdlib
	hardware_pwm
	hardware_adc 
	freertos
)

if (PICO_CYW43_SUPPORTED)
    target_link_libraries(blink pico_cyw43_arch_none)
endif()

# Enable USB serial output (newly added)
pico_enable_stdio_usb(blink 1)   # Enable USB serial output
pico_enable_stdio_uart(blink 0)  # Disable UART serial output (optional)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(blink)
