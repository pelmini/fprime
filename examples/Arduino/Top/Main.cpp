#include <examples/Arduino/Top/Components.hpp>
#include <Os/Log.hpp>
#ifdef ARDUINO
    #include <Os/Arduino/StreamLog.hpp>
    #include <Arduino.h>
#endif
/**
 * Main function.
 */
int main(int argc, char* argv[]) {
#ifdef ARDUINO
    Serial1.begin(115200);
    Os::setArduinoStreamLogHandler(&Serial1);
#endif
    Os::Log::logMsg("System Starting.\n", 0, 0, 0, 0, 0, 0);
    constructApp();
    return 0;
}
