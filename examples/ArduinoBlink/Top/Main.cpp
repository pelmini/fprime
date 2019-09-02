#include <examples/ArduinoBlink/Top/Components.hpp>
#include <Fw/Types/Generic/GenericLogAssert.hpp>
#include <Os/Log.hpp>
#ifdef ARDUINO
    #include <Os/Arduino/StreamLog.hpp>
    #include <Arduino.h>
#else
    #include <examples/ArduinoGpsTracker/SerialDriver/SerialDriver.hpp>
#endif
Fw::LogAssertHook assert;
/**
 * Main function.
 */
int main(int argc, char* argv[]) {
	assert.registerHook();
#ifdef ARDUINO
    Serial1.begin(115200);
    Os::setArduinoStreamLogHandler(&Serial1);
#else
    // Set serial port
    FW_ASSERT(argc <= 2);
    if (argc == 2) {
        Arduino::SERIAL_PORT = reinterpret_cast<char**>(&argv[1]);
    }
#endif
    Os::Log::logMsg("[SETUP] Constructing system\n", 0, 0, 0, 0, 0, 0);
    constructApp();
#ifndef ARDUINO
    while (1) {}
#endif
    return 0;
}
