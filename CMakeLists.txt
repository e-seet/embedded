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
# # set(sdkVersion 1.5.1)
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

# # Define Wi-Fi credentials as preprocessor definitions
# target_compile_definitions(blink PRIVATE WIFI_SSID=\"SINGTEL-TD8A\" WIFI_PASSWORD=\"3fr3fca2uh\")

# # pull in common dependencies
# target_link_libraries(blink 
# 	pico_stdlib  
# 	pico_cyw43_arch_lwip_threadsafe_background
# )

# target_include_directories(blink PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lwipopts)

# # if (PICO_CYW43_SUPPORTED)
# #     target_link_libraries(blink pico_cyw43_arch_none)
# # endif()

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


# ====================================================================================
# Generated CMake Pico project file

cmake_minimum_required(VERSION 3.13)

# Explicitly set the Pico SDK path
# set(PICO_SDK_PATH "/Users/eddieseet/.pico-sdk/sdk/2.0.0")

# my own one
set(PICO_SDK_PATH "/Users/eddieseet/.pico-sdk/sdk/2.0.0")
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NEVER EDIT THE NEXT LINES for Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
include(${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# Project name is 'blink'
project(blink C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1
add_executable(blink
    blink.c
)

# Define Wi-Fi credentials as preprocessor definitions
target_compile_definitions(blink PRIVATE WIFI_SSID=\"SINGTEL-TD8A\" WIFI_PASSWORD=\"3fr3fca2uh\")

# Pull in common dependencies, including Wi-Fi (cyw43) and lwIP
target_link_libraries(blink 
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
)

# Include the lwipopts configuration directory
target_include_directories(blink PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}/lwipopts
${PICO_SDK_PATH}/lib/lwip/src/include  # Add this line to ensure lwIP headers are included
)

# Enable USB serial output
pico_enable_stdio_usb(blink 1)
pico_enable_stdio_uart(blink 0)  # Disable UART serial output (optional)

# Create map/bin/hex file etc.
pico_add_extra_outputs(blink)

# Uncomment and set this if building for macOS with a custom SDK
set(CMAKE_OSX_SYSROOT /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX15.0.sdk)