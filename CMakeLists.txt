cmake_minimum_required(VERSION 3.12)

# initialize pico_sdk from GIT
# (note this can come from environment, CMake cache etc)
# set(PICO_SDK_FETCH_FROM_GIT on)

# pico_sdk_import.cmake is a single file copied from this SDK
# note: this must happen before project()
set(PICO_SDK_PATH "../../pico-sdk")
include(pico_sdk_import.cmake)

project(pico_inference_app)

# initialize the Pico SDK
pico_sdk_init()

# Define ARM_CPU, CMSIS ROOT and DSP to use CMSIS-DSP
set(ARM_CPU "cortex-m0plus")

set(CONFIGTABLE ON)
set(RFFT_Q15_256 ON)
set(ALLFAST ON)

# rest of your project
add_executable(pico_inference_app
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ml_model.cpp
)

target_link_libraries(pico_inference_app
    pico_stdlib
    hardware_pwm
    pico-tflmicro
)

# enable usb output, disable uart output
pico_enable_stdio_usb(pico_inference_app 1)
pico_enable_stdio_uart(pico_inference_app 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(pico_inference_app)

add_subdirectory("lib/pico-tflmicro" EXCLUDE_FROM_ALL)
