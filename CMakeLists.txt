cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(vcp C CXX ASM)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()


add_executable(vcp
	main.c
	vcp.c
	vcp.h
)

# enable usb output, disable uart output
pico_enable_stdio_usb(vcp 0)
pico_enable_stdio_uart(vcp 1)

target_link_libraries(vcp 
				pico_stdlib 
				hardware_i2c
				pico_cyw43_arch_none
				)

pico_add_extra_outputs(vcp)
