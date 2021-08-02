#Specify min version
cmake_minimum_required(VERSION 3.19)

#Include CMAKE IMPORT FILE
include(pico_sdk_import.cmake)
include(pico_extras_import.cmake)

project(cuplight_emma_project)

#init the sdk
pico_sdk_init()

#Create executable target from a source file
add_executable(cuplight cuplight.c)
pico_enable_stdio_usb(cuplight 1)


#Specify libraries to include
target_link_libraries(cuplight pico_stdlib hardware_sleep hardware_rtc hardware_pwm hardware_adc)

#Generate all file formats
pico_add_extra_outputs(cuplight)
