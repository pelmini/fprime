

# MCU unit and loader script used by the Teensy 3.2
set(TEENSY_MCU "IMXRT1052")
string(TOLOWER "${TEENSY_MCU}" TEENSY_MCU_LOWER)
set(MCU_LD "imxrt.ld")

set(TEENSY_FREQ 396000000)
set(TEENSY_ARDUINO_NUM 10808)
set(TEENSY_TEENSYDUINO_NUM 145)
set(TEENSY_CPU_ARCH  cortex-m7)


set(TEENSY_CPP "-ffunction-sections -fdata-sections -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fsingle-precision-constant")
set(TEENSY_LD "-Wl,--gc-sections,--print-gc-sections,--print-memory-usage -nostdlib -nostartfiles")






C_FILES := $(wildcard *.c)
CPP_FILES := $(wildcard *.cpp)

OBJS := $(C_FILES:.c=.o) $(CPP_FILES:.cpp=.o)



all: Blink.elf

Blink.elf: Blink.o core.a
$(CC) $(CPPFLAGS) $(LDFLAGS) -o $@ $^ -lm
#$(CC) $(CPPFLAGS) $(LDFLAGS) -o $@ Blink.o $(OBJS) -lm
$(OBJDUMP) -d -S -C $@ > Blink.lst
$(OBJDUMP) -t -C $@ > Blink.sym
$(OBJCOPY) -O ihex -R .eeprom $@ Blink.hex
$(SIZE) $@

core.a: $(OBJS)
$(AR) rcs core.a $(OBJS)


clean:
rm -f *.o *.d core.a *.elf *.lst *.sym *.hex

