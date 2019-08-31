####
# teensy32.cmake:
#
# Teensy 3.2 support file. This file ensures that the Teensy 3.2 can be useds as a target for the CMake system's output.
####

# MCU unit and loader script used by the Teensy 3.2
set(TEENSY_MCU "MK20DX256")
string(TOLOWER "${TEENSY_MCU}" TEENSY_MCU_LOWER)
set(MCU_LD "${TEENSY_MCU_LOWER}.ld")

set(TEENSY_FREQ 72000000)
set(TEENSY_ARDUINO_NUM 10805)
set(TEENSY_TEENSYDUINO_NUM 144)
set(TEENSY_CPU_ARCH  cortex-m4)

set(TEENSY_CPP "-MMD")
set(TEENSY_LD  "-Os -Wl,--gc-sections,--defsym=__rtc_localtime=0 -mcpu=${TEENSY_CPU_ARCH}")