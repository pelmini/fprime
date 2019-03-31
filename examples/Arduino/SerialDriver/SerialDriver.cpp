// ====================================================================== 
// \title  SerialDriverImpl.cpp
// \author lestarch
// \brief  cpp file for SerialDriver component implementation class
// ====================================================================== 


#include <examples/Arduino/SerialDriver/SerialDriver.hpp>
#include "Fw/Types/BasicTypes.hpp"

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
      write_data(fwBuffer);
  }

  void SerialDriverComponentImpl ::
    readPoll_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {
      read_data(fwBuffer);
  }

  void SerialDriverComponentImpl ::
    schedIn_handler(
        const NATIVE_INT_TYPE portNum, /*!< The port number*/
        NATIVE_UINT_TYPE context /*!< The call order*/
    )
  {
      //TODO: ask for buffer
      Fw::Buffer buffer;
      read_data(buffer);
  }

} // end namespace Svc
