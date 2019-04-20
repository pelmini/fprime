#include <examples/Arduino/Top/Components.hpp>
#ifdef ARDUINO
    #include <Os/Arduino/SteamLog.hpp>
    #include <Arduino.h>
#endif
/**
 * Main function.
 */
int main(int argc, char* argv[]) {
#ifdef ARDUINO
    Serial1.begin(115200);
    setArduinoStreamLogHandler(Serial1);
#endif
    Os::Log::logMsg("System Starting.\n", 0, 0, 0, 0, 0, 0);
    constructApp();
    return 0;
}
