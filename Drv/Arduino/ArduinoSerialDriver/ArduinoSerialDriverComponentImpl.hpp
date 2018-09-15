// ====================================================================== 
// \title  ArduinoSerialDriverImpl.hpp
// \author lestarch
// \brief  hpp file for ArduinoSerialDriver component implementation class
//
// ====================================================================== 

#ifndef ArduinoSerialDriver_HPP
#define ArduinoSerialDriver_HPP

#include "Drv/Arduino/ArduinoSerialDriver/ArduinoSerialDriverComponentAc.hpp"
#include "Drv/Arduino/ArduinoSerialDriver/ArduinoSerialDriverComponentCfg.hpp"

namespace Drv {
  /**
   * BufferPoolEntry:
   *  Entry for the buffer pool. Will be filled and sent, then
   *  expected to return via another call.
   */
  struct BufferPoolEntry {
      Fw::Buffer buffer;
      bool avail;
  };

  /**
   * ArduinoSerialDevices:
   *  An enumeration of Arduino serial devices
   */
  enum ArduinoSerialDevice {
      USB_SERIAL_1,
      HW_SERIAL_1,
      HW_SERIAL_2,
      MAX_SerialDevices
  };

  /**
   * ArduinoBaudRates:
   *  An enumeration of baud rates supported by this driver.
   */
  enum ArduinoBaudRate {
      B9600 = 9600,
      B115200 = 115200
  };

  class ArduinoSerialDriverComponentImpl :
    public ArduinoSerialDriverComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object ArduinoSerialDriver
      //!
      ArduinoSerialDriverComponentImpl(
#if FW_OBJECT_NAMES == 1
          const char *const compName /*!< The component name*/
#else
          void
#endif
      );

      //! Initialize object ArduinoSerialDriver
      //!
      void init(
          const ArduinoSerialDevice device,
          const ArduinoBaudRate rate,
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object ArduinoSerialDriver
      //!
      ~ArduinoSerialDriverComponentImpl(void);

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for readBufferSend
      //!
      void readBufferSend_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &fwBuffer 
      );

      //! Handler implementation for serialSend
      //!
      void serialSend_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &serBuffer 
      );

      //! Handler implementation for serialPoll
      //!
      void serialPoll_handler(
          const NATIVE_INT_TYPE portNum /*!< The port number*/
      );

      //------------------
      // Helper functions
      //------------------
      //! Opens the uart device
      //!
      void open();

      //! Read from the UART device and get status
      //!
      SerialReadStatus read(U8* buffer, U32 size);

      //! Write out data to UART device
      //!
      void write(U8* buffer, U32 size);

      //------------------
      // Member variables
      //------------------
      BufferPoolEntry m_pool[DRV_MAX_BUFFERS];
      ArduinoSerialDevice m_device;
      ArduinoBaudRate m_rate;
    };

} // end namespace Drv

#endif
