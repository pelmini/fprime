// ====================================================================== 
// \title  ArduinoSerialDriverImplSpecific.cpp
// \author lestarch
// \brief  cpp file for ArduinoSerialDriver component implementation class
// ====================================================================== 

#include <Drv/Arduino/ArduinoSerialDriver/ArduinoSerialDriverComponentImpl.hpp>
#include "Fw/Types/BasicTypes.hpp"
namespace Drv {

  void ArduinoSerialDriverComponentImpl ::
    open()
  {}

  SerialReadStatus ArduinoSerialDriverComponentImpl ::
    read(U8* buffer, U32 size)
  {
      SerialReadStatus status = SER_OK;
      return status;
  }

  void ArduinoSerialDriverComponentImpl ::
    write(U8* buffer, U32 size)
  {}
} // end namespace Drv
