// ====================================================================== 
// \title  ArduinoSerialDriverImplSpecific.cpp
// \author lestarch
// \brief  cpp file for ArduinoSerialDriver component implementation class
// ====================================================================== 


#include <Drv/Arduino/ArduinoSerialDriver/ArduinoSerialDriverComponentImpl.hpp>
#include "Fw/Types/BasicTypes.hpp"
#include <Arduino.h>
namespace Drv {

  void ArduinoSerialDriverComponentImpl ::
    open()
  {
      //Begin Arduino serial
      switch(m_device) {
          case USB_SERIAL_1:
              Serial.begin(m_rate);
              break;
          case HW_SERIAL_1:
              Serial.begin(m_rate);
              break;
          case HW_SERIAL_2:
              Serial.begin(m_rate);
              break;
          default;
              FW_ASSERT(false, m_device);
              break;
      }
  }

  SerialReadStatus ArduinoSerialDriverComponentImpl ::
    read(U8* buffer, U32 size)
  {
      SerialReadStatus status = SER_OK;
      //Read Arduino based on device
      switch(m_device) {
          case USB_SERIAL_1:
              while (Serial.available() > 0 && size > 0) {
                  *buffer = Serial.read();
                  size--;
                  buffer++;
              }
              break;
          case HW_SERIAL_1:
              while (Serial1.available() > 0 && size > 0) {
                  *buffer = Serial1.read();
                  size--;
                  buffer++;
              }
              break;
          case HW_SERIAL_2:
              while (Serial2.available() > 0 && size > 0) {
                  *buffer = Serial2.read();
                  size--;
                  buffer++;
              }
              break;
          default;
              FW_ASSERT(false, m_device);
              break;
      }
      //Check to see if we ran out of buffer
      if (size == 0) {
          status = SER_BUFFER_TOO_SMALL;
      }
      return status;
  }

  void ArduinoSerialDriverComponentImpl ::
    write(U8* buffer, U32 size)
  {
      switch(m_device) {
          case USB_SERIAL_1:
              Serial.write(buffer, size);
              break;
          case HW_SERIAL_1:
              Serial.write(buffer, size);
              break;
          case HW_SERIAL_2:
              Serial.write(buffer, size);
              break;
          default;
              FW_ASSERT(false, m_device);
              break;
      }
  }
} // end namespace Drv
