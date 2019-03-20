// ====================================================================== 
// \title  SerialDriverImpl.cpp
// \author lestarch
// \brief  cpp file for SerialDriver component implementation class
// ====================================================================== 


#include <examples/Arduino/SerialDriver/SerialDriver.hpp>
#include "Fw/Types/BasicTypes.hpp"
#include <Arduino.h>

namespace Arduino {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction 
  // ----------------------------------------------------------------------

  SerialDriverComponentImpl ::
#if FW_OBJECT_NAMES == 1
    SerialDriverComponentImpl(
        const char *const compName
    ) :
      SerialDriverComponentBase(compName)
#else
    SerialDriverImpl(void)
#endif
  {

  }

  void SerialDriverComponentImpl ::
    init(
        const NATIVE_INT_TYPE instance
    ) 
  {
    SerialDriverComponentBase::init(instance);
    Serial.begin(115200);
  }

  SerialDriverComponentImpl ::
    ~SerialDriverComponentImpl(void)
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void SerialDriverComponentImpl ::
    write_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {
      Serial.write(reinterpret_cast<U8*>(fwBuffer.getdata()),fwBuffer.getsize());
  }

  void SerialDriverComponentImpl ::
    readPoll_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {
    //NOPE
  }

} // end namespace Svc
