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
    SerialDriverComponentImpl(
#if FW_OBJECT_NAMES == 1
        const char *const compName,
#endif
        NATIVE_UINT_TYPE portNum
    ) :
      SerialDriverComponentBase(compName),
      m_port_number(portNum),
      m_port_pointer(static_cast<POINTER_CAST>(NULL)),
      m_local_buffer(0xfeedfeed, 0xdeeddeed, reinterpret_cast<POINTER_CAST>(m_data), SERIAL_BUFFER_SIZE)
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
      m_local_buffer.setsize(SERIAL_BUFFER_SIZE);
      read_data(m_local_buffer);
      if (m_local_buffer.getsize() > 0) {
          readCallback_out(0, m_local_buffer);
      }
  }

} // end namespace Svc
