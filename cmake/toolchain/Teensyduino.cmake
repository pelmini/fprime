####
# Teensyduino.cmake:
#
# Toolchain file setup for building F prime for the Teensy hardware platform. This must, by definition, include the
# Arduino framework in order to ensure that basic functions of the Teensy hardware platform are available. This
# toolchain will build the core Teensy Arduino libraries, and include the headers as part of the target include headers.
####
# System setup for Teensyduino
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CROSSCOMPILING 1)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY" CACHE STRING "Try Static Lib Type" FORCE)
#set(CMAKE_CXX_COMPILER_)

# Check if ARDUINO_SDK_PATH is set, otherwise set it to /opt/arduino-1.8.9/
if (NOT DEFINED ARDUINO_SDK_PATH)
    set(ARDUINO_SDK_PATH "/opt/arduino-1.8.9")
endif()
if (NOT DEFINED TEENSY_VERSION)
    set(TEENSY_VERSION "teensy32")
endif()
string(REGEX MATCH "teensy[0-9]" TEENSY_CORE_DIR "${TEENSY_VERSION}")
include("${CMAKE_CURRENT_LIST_DIR}/ArduinoSupport/${TEENSY_VERSION}.cmake")
set(TEENSY_SRC_DIR "${ARDUINO_SDK_PATH}/hardware/teensy/avr/cores/${TEENSY_CORE_DIR}" CACHE PATH "Teensy SRC")

message(STATUS "Arduino SDK path:  ${ARDUINO_SDK_PATH}")
message(STATUS "Teensy Version:    ${TEENSY_VERSION}; ${TEENSY_CORE_DIR}")
message(STATUS "Teensy Source Dir: ${TEENSY_SRC_DIR}")

# Setup the cross compiling tools path
set(ARDUINO_TOOLS_PATH "${ARDUINO_SDK_PATH}/hardware/tools/${CMAKE_SYSTEM_PROCESSOR}/bin")
message(STATUS "Teensy Tools Dir:  ${ARDUINO_TOOLS_PATH}")

# Grab a suffix for tools
if(WIN32)
    set(TOOL_SUFFIX .exe)
else()
    set(TOOL_SUFFIX )
endif()
# Set the tools path
set(CMAKE_C_COMPILER   "${ARDUINO_TOOLS_PATH}/arm-none-eabi-gcc${TOOL_SUFFIX}"         CACHE PATH "gcc"     FORCE)
set(CMAKE_CXX_COMPILER "${ARDUINO_TOOLS_PATH}/arm-none-eabi-g++${TOOL_SUFFIX}"         CACHE PATH "g++"     FORCE)
set(CMAKE_AR           "${ARDUINO_TOOLS_PATH}/arm-none-eabi-gcc-ar${TOOL_SUFFIX}"      CACHE PATH "archive" FORCE)
set(CMAKE_LINKER       "${ARDUINO_TOOLS_PATH}/arm-none-eabi-ld${TOOL_SUFFIX}"          CACHE PATH "linker"  FORCE)
set(CMAKE_NM           "${ARDUINO_TOOLS_PATH}/arm-none-eabi-nm${TOOL_SUFFIX}"          CACHE PATH "nm"      FORCE)
set(CMAKE_OBJCOPY      "${ARDUINO_TOOLS_PATH}/arm-none-eabi-objcopy${TOOL_SUFFIX}"     CACHE PATH "objcopy" FORCE)
set(CMAKE_OBJDUMP      "${ARDUINO_TOOLS_PATH}/arm-none-eabi-objdump${TOOL_SUFFIX}"     CACHE PATH "objdump" FORCE)
set(CMAKE_STRIP        "${ARDUINO_TOOLS_PATH}/arm-none-eabi-strip${TOOL_SUFFIX}"       CACHE PATH "strip"   FORCE)
set(CMAKE_RANLIB       "${ARDUINO_TOOLS_PATH}/arm-none-eabi-gcc-ranlib${TOOL_SUFFIX}"  CACHE PATH "ranlib"  FORCE)


set(OPTIONS "-DF_CPU=${TEENSY_FREQ} -DUSB_SERIAL -DLAYOUT_US_ENGLISH -DUSING_MAKEFILE -D__${TEENSY_MCU}__")
set(OPTIONS "${OPTIONS} -DARDUINO=${TEENSY_ARDUINO_NUM} -DTEENSYDUINO=${TEENSY_TEENSYDUINO_NUM}")

set(TEENSY_BOTH_FLAGS "-Wall -g -Os -mcpu=${TEENSY_CPU_ARCH} -mthumb ${OPTIONS} ${TEENSY_CPP}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TEENSY_BOTH_FLAGS}" CACHE STRING "C_FLAGS")
set(CMAKE_CXX_FLAGS "-std=gnu++14 -felide-constructors -fno-exceptions -fno-rtti ${TEENSY_BOTH_FLAGS}" CACHE STRING "CXX_FLAGS")
set(CMAKE_EXE_LINKER_FLAGS "-mcpu=${TEENSY_CPU_ARCH} -mthumb -T${TEENSY_SRC_DIR}/${MCU_LD} ${TEENSY_LD}" CACHE STRING "LD_FLAGS")
set(CMAKE_CXX_COMPILE_OBJECT "${CMAKE_CXX_COMPILER} -c <SOURCE> -o <OBJECT> <FLAGS> <INCLUDES>" CACHE STRING "comp obj")
set(CMAKE_C_COMPILE_OBJECT   "${CMAKE_C_COMPILER} -c <SOURCE> -o <OBJECT> <FLAGS> <INCLUDES>" CACHE STRING "comp obj")

# Build the Arduino core library
file(GLOB ARDUINO_SRC "${TEENSY_SRC_DIR}/*.cpp" "${TEENSY_SRC_DIR}/*.c")
# Add the teensy core lib only once
if (NOT TARGET "teensycore")
    message(STATUS "SRC DIR: ${TEENSY_SRC_DIR}")
    add_library("teensycore" ${ARDUINO_SRC})
    target_include_directories("teensycore" BEFORE PUBLIC ${TEENSY_SRC_DIR})
endif()
####
#
####
function(add_arduino_dependency target)
    add_dependencies(${target} "teensycore")
    target_include_directories(${target} PUBLIC ${TEENSY_SRC_DIR})
    
endfunction()