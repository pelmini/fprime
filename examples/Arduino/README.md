# NASA OpenSource on Arduino?  Yes, very much so!

## Teensy32 installation instructions

1. Install teensyduino in standard arduion **with** udev rules
2. Edit linker file to specify 72MHz speed
3. Make the make file in `/opt/arduino-1.8.9/hardware/teensy/avr/cores/teensy3/` as such:

```
make clean
ARDUINOPATH=path/to/arduion make
mv main.o main.o.bak
```
4. Compile with TEENSY32 target in F prime
