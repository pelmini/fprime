// ====================================================================== 
// \title  SerialDriverImpl.hpp
// \author lestarch
// \brief  hpp file for SerialDriver component implementation class
// ====================================================================== 

#ifndef SerialDriver_HPP
#define SerialDriver_HPP

#include "examples/Arduino/SerialDriver/SerialDriverComponentAc.hpp"

namespace Arduino {

  class SerialDriverComponentImpl :
    public SerialDriverComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object SerialDriver
      //!
      SerialDriverComponentImpl(
#if FW_OBJECT_NAMES == 1
          const char *const compName /*!< The component name*/
#else
          void
#endif
      );

      //! Initialize object SerialDriver
      //!
      void init(
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object SerialDriver
      //!
      ~SerialDriverComponentImpl(void);

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for write
      //!
      void write_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &fwBuffer 
      );

      //! Handler implementation for readPoll
      //!
      void readPoll_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &fwBuffer 
      );


    };

} // end namespace Arduino

#endif
