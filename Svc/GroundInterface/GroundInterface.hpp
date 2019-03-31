// ====================================================================== 
// \title  GroundInterfaceImpl.hpp
// \author lestarch
// \brief  hpp file for GroundInterface component implementation class
// ====================================================================== 
#include <Fw/Types/Serializable.hpp>
#ifndef GroundInterface_HPP
#define GroundInterface_HPP

#define GND_BUFFER_SIZE 512
#define TOKEN_TYPE U32

#include "Svc/GroundInterface/GroundInterfaceComponentAc.hpp"

namespace Svc {

  class GroundInterfaceComponentImpl :
    public GroundInterfaceComponentBase
  {

    public:
      /**
       * ProcessingState:
       *
       * State of the processing of incoming data.
       */
      enum ProcessingState {
          START, //! Searching for a start-bit
          SIZE,  //! Reading 4 byte size
          DATA,  //! Reading all data
          CHECK, //! Reading 4 byte checksum
          MAX_PROCESSING_STATE
      };

      static const U32 MAX_DATA_SIZE = 2048;
      static const TOKEN_TYPE START_WORD = static_cast<TOKEN_TYPE>(0xdeadbeef);

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object GroundInterface
      //!
      GroundInterfaceComponentImpl(
#if FW_OBJECT_NAMES == 1
          const char *const compName /*!< The component name*/
#else
          void
#endif
      );

      //! Initialize object GroundInterface
      //!
      void init(
          const NATIVE_INT_TYPE instance = 0 /*!< The instance number*/
      );

      //! Destroy object GroundInterface
      //!
      ~GroundInterfaceComponentImpl(void);

    PRIVATE:

      // ----------------------------------------------------------------------
      // Handler implementations for user-defined typed input ports
      // ----------------------------------------------------------------------

      //! Handler implementation for downlinkPort
      //!
      void downlinkPort_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::ComBuffer &data, /*!< Buffer containing packet data*/
          U32 context /*!< Call context value; meaning chosen by user*/
      );

      //! Handler implementation for fileDownlinkBufferSendIn
      //!
      void fileDownlinkBufferSendIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &fwBuffer 
      );

      //! Handler implementation for readCallback
      //!
      void readCallback_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          Fw::Buffer &fwBuffer 
      );

      //! Handler implementation for schedIn
      //!
      void schedIn_handler(
          const NATIVE_INT_TYPE portNum, /*!< The port number*/
          NATIVE_UINT_TYPE context /*!< The call order*/
      );

      //! Process a data buffer containing a read from the serial port
      void processData(Fw::Buffer& data /*!< Data to process */);

      // Output variables
      Fw::Buffer m_ext_buffer;
      U8 m_buffer[GND_BUFFER_SIZE];
      // Input variables used when reading/processing
      Fw::ExternalSerializeBuffer m_in_wrapper;
      ProcessingState m_state; //!< Processing state for incoming data
      NATIVE_UINT_TYPE m_remaining; //!< Data remaining in the input buffer
      NATIVE_UINT_TYPE m_in_offset; //!< Offset into in-buffer where reading occurs
      TOKEN_TYPE m_data_size; //!< Data size expected in incoming data
      U8 m_in_buffer[GND_BUFFER_SIZE];
    };

} // end namespace Svc

#endif
