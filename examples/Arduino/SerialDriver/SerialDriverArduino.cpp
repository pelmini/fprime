// ====================================================================== 
// \title  SerialDriverImpl.cpp
// \author lestarch
// \brief  cpp file for SerialDriver component implementation class
// ====================================================================== 


#include <examples/Arduino/SerialDriver/SerialDriver.hpp>
#include "Fw/Types/BasicTypes.hpp"
#include <Arduino.h>

namespace Arduino {

  void SerialDriverComponentImpl ::
    init(
        const NATIVE_INT_TYPE instance
    )
  {
      SerialDriverComponentBase::init(instance);
      Serial.begin(115200);
  }


  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void SerialDriverComponentImpl ::
    write_data(
        Fw::Buffer &fwBuffer
    )
  {
      Serial.write(reinterpret_cast<U8*>(fwBuffer.getdata()),fwBuffer.getsize());
  }

  void SerialDriverComponentImpl ::
    read_data(Fw::Buffer& fwBuffer)
  {
      U32 i = 0;
      int read = 0;
      for (i = 0; i < fwBuffer.getsize() && read != -1; i++) {
          read = Serial.read();
          if (read != -1) {
              reinterpret_cast<U8*>(fwBuffer.getdata())[i] = static_cast<U8>(read);
          }
      }
      fwBuffer.setsize(i);
  }
} // end namespace Svc
