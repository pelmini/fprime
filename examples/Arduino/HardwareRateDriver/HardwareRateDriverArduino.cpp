#include <Fw/Types/BasicTypes.hpp>
#include <Fw/Types/Assert.hpp>
#include <examples/Arduino/HardwareRateDriver/HardwareRateDriver.hpp>
#include <Arduino.h>

namespace Arduino {
IntervalTimer s_itimer;

void HardwareRateDriver::start() {
    s_itimer.begin(HardwareRateDriver::s_timerISR, s_itimer * 1000);
}

void HardwareRateDriver::stop() {
    s_itimer.end();
}

void HardwareRateDriver::s_timerISR() {
    s_timer(s_driver);
}

};
