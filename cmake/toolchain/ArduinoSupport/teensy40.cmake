

# MCU unit and loader script used by the Teensy 4.0
set(TEENSY_MCU "IMXRT1062")
string(TOLOWER "${TEENSY_MCU}" TEENSY_MCU_LOWER)
set(MCU_LD "imxrt1062.ld")

set(TEENSY_FREQ 600000000)
set(TEENSY_ARDUINO_NUM 10808)
set(TEENSY_TEENSYDUINO_NUM 145)
set(TEENSY_CPU_ARCH  cortex-m7)


set(TEENSY_CPP "-mfloat-abi=hard -mfpu=fpv4-sp-d16 -fsingle-precision-constant")
set(TEENSY_LD "-Wl,--gc-sections")
