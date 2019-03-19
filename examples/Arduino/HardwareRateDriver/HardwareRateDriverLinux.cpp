#include <Fw/Types/BasicTypes.hpp>
#include <Fw/Types/Assert.hpp>
#include <Os/Task.hpp>
#include <Fw/Types/EightyCharString.hpp>
#include <examples/Arduino/HardwareRateDriver/HardwareRateDriver.hpp>

namespace Arduino {
Os::Task s_task;
void HardwareRateDriver::start() {
    Fw::EightyCharString fake("FakeArduino");
    s_task.setStarted(true);
    s_task.start(fake, 0xdeafbeef, 255, 0, HardwareRateDriver::s_timer, this, 0);
}
void HardwareRateDriver::stop() {
    s_task.setStarted(false);
}
void HardwareRateDriver::s_timerISR() {}
};
