// ====================================================================== 
// \title  ArduinoSerialDriverImpl.cpp
// \author lestarch
// \brief  cpp file for ArduinoSerialDriver component implementation class
// ====================================================================== 


#include <Drv/Arduino/ArduinoSerialDriver/ArduinoSerialDriverComponentImpl.hpp>
#include "Fw/Types/BasicTypes.hpp"

namespace Drv {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction 
  // ----------------------------------------------------------------------

  ArduinoSerialDriverComponentImpl ::
#if FW_OBJECT_NAMES == 1
    ArduinoSerialDriverComponentImpl(
        const char *const compName
    ) :
      ArduinoSerialDriverComponentBase(compName)
#else
    ArduinoSerialDriverImpl(void)
#endif
  {
      //Initial pool has no buffers
      for (U32 i = 0; i < DRV_MAX_BUFFERS; i++) {
          m_pool[i].avail = false;
      }
  }

  void ArduinoSerialDriverComponentImpl ::
    init(
        const ArduinoSerialDevice device,
        const ArduinoBaudRate rate,
        const NATIVE_INT_TYPE instance
    ) 
  {
      ArduinoSerialDriverComponentBase::init(instance);
      m_device = device;
      m_rate = rate;
      open();
  }

  ArduinoSerialDriverComponentImpl ::
    ~ArduinoSerialDriverComponentImpl(void)
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void ArduinoSerialDriverComponentImpl ::
    readBufferSend_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {
      //Search through pool looking for "in-use" buffer
      //to replace with returned buffer
      U32 i = 0;
      for (i = 0; i < DRV_MAX_BUFFERS; i++) {
          if (!m_pool[i].avail) {
              m_pool[i].buffer = fwBuffer;
              m_pool[i].avail = false;
              break;
          }
      }
      //Make sure buffer successfully returned
      FW_ASSERT(i != DRV_MAX_BUFFERS);
  }

  void ArduinoSerialDriverComponentImpl ::
    serialSend_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &serBuffer
    )
  {
      write(reinterpret_cast<U8*>(serBuffer.getdata()),
            serBuffer.getsize());
  }

  void ArduinoSerialDriverComponentImpl ::
    serialPoll_handler(
        const NATIVE_INT_TYPE portNum
    )
  {
      SerialReadStatus status = SER_OK;
      Fw::Buffer buffer;
      //Search through pool looking for not "in-use"
      //buffer to replace with returned buffer
      U32 i = 0;
      for (i = 0; i < DRV_MAX_BUFFERS; i++) {
          if (m_pool[i].avail) {
              buffer = m_pool[i].buffer;
              m_pool[i].avail = true;
              break;
          }
      }
      //Make sure buffer successfully returned
      if (i != DRV_MAX_BUFFERS) {
          status = read(reinterpret_cast<U8*>(buffer.getdata()),
                        buffer.getsize());
      } else {
          status = SER_NO_BUFFERS;
      }
      serialRecv_out(portNum, buffer, status);
  }

} // end namespace Drv
