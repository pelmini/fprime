// ====================================================================== 
// \title  GroundInterface.cpp
// \author lestarch
// \brief  cpp file for GroundInterface component implementation class
// ====================================================================== 


#include <Svc/GroundInterface/GroundInterface.hpp>
#include "Fw/Types/BasicTypes.hpp"

namespace Svc {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction 
  // ----------------------------------------------------------------------

  GroundInterfaceComponentImpl ::
#if FW_OBJECT_NAMES == 1
    GroundInterfaceComponentImpl(
        const char *const compName
    ) :
      GroundInterfaceComponentBase(compName),
#else
  GroundInterfaceComponentBase(void),
#endif
    m_ext_buffer(0xfeedfeed, 0xdeeddeed, reinterpret_cast<POINTER_CAST>(m_buffer), GND_BUFFER_SIZE),

    // Input processing variables
    m_state(START),
    m_data_size(0),
    m_remaining(0),
    m_in_offset(0),
    m_in_wrapper(m_in_buffer, GND_BUFFER_SIZE)
  {

  }

  void GroundInterfaceComponentImpl ::
    init(
        const NATIVE_INT_TYPE instance
    ) 
  {
    GroundInterfaceComponentBase::init(instance);
  }

  GroundInterfaceComponentImpl ::
    ~GroundInterfaceComponentImpl(void)
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void GroundInterfaceComponentImpl ::
    downlinkPort_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::ComBuffer &data,
        U32 context
    )
  {
      // TODO: replace with a call to a buffer manager
      Fw::Buffer buffer = m_ext_buffer;
      Fw::ExternalSerializeBuffer buffer_wrapper(reinterpret_cast<U8*>(m_ext_buffer.getdata()),
                                                 m_ext_buffer.getsize());
      // Serialize data
      buffer_wrapper.serialize(START_WORD);
      buffer_wrapper.serialize(data.getBuffLength());
      buffer_wrapper.serialize(data.getBuffAddr(), data.getBuffLength());
      buffer_wrapper.serialize(static_cast<U32>(0xdeadbeef));

      buffer.setsize(buffer_wrapper.getBuffLength());
      buffer.setdata(reinterpret_cast<POINTER_CAST>(buffer_wrapper.getBuffAddr()));
      write_out(0, buffer);
  }

  void GroundInterfaceComponentImpl ::
    fileDownlinkBufferSendIn_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &fwBuffer
    )
  {}

  void GroundInterfaceComponentImpl ::
    readCallback_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &buffer
    )
  {
      processData(buffer);
  }

  void GroundInterfaceComponentImpl ::
    schedIn_handler(
        const NATIVE_INT_TYPE portNum, /*!< The port number*/
        NATIVE_UINT_TYPE context /*!< The call order*/
    )
  {
      // TODO: replace with a call to a buffer manager
      Fw::Buffer buffer = m_ext_buffer;
      readPoll_out(0, buffer);
      processData(buffer);
  }

  void GroundInterfaceComponentImpl ::
    processData(Fw::Buffer& buffer)
  {
      U8* pointer = reinterpret_cast<U8*>(buffer.getdata());
      // Outer loop: this portion reads the data in the incoming buffer and stops when the data
      // runs out. Any non-processed data remains in the buffer and the remaining variable tracks
      // how much is left-over.
      while (pointer < (reinterpret_cast<U8*>(buffer.getdata()) + buffer.getsize())) {
          // Detect based on state how much data needs to be read into our buffer.
          NATIVE_UINT_TYPE read_size = 0;
          switch (m_state) {
              case DATA:
                  read_size = static_cast<NATIVE_UINT_TYPE>(m_data_size);
                  break;
              case CHECK:
                  read_size = sizeof(U32);
                  break;
              default:
                  read_size = sizeof(TOKEN_TYPE);
                  break;
          }
          // Read the data byte-by-byte into the buffer in preparation for processing
          // of the data. At no time should the remaining data be more that the needed
          // data from this step.
          FW_ASSERT(m_remaining <= read_size, m_remaining, read_size);
          while ((read_size - m_remaining > 0) &&
                 (pointer < reinterpret_cast<U8*>(buffer.getdata()))) {
              m_in_buffer[m_remaining + m_in_offset] = *pointer;
              m_remaining++;
          }
          // Return if not enough data yet
          if ((read_size - m_remaining) > 0) {
              return;
          }
          m_in_wrapper.setBuffLen(m_in_offset + m_remaining);
          // Process the data based on the state. If we are in the start phase, scan for the start
          // token. In size state, read the size, data state, copy the data, and in the check state
          // read the checksum for comparison.
          switch(m_state) {
              case START:
                  TOKEN_TYPE token;
                  m_data_size = 0;
                  m_in_wrapper.deserialize(token);
                  // Correct word, move on to the next state
                  if (token == START_WORD) {
                      m_state = SIZE;
                      m_remaining = m_remaining - sizeof(START_WORD);
                      m_in_offset +=  sizeof(START_WORD);
                  }
                  // Wrong word, shift away bad start byte
                  else {
                      for (NATIVE_UINT_TYPE i = 1; i < m_remaining; i++) {
                          m_in_buffer[m_in_offset + i - 1] = m_in_buffer[m_in_offset + i];
                      }
                      // Three bytes left over after shift
                      m_remaining = m_remaining - 1;
                  }
                  break;
              // When looking for size, ball-park check it
              case SIZE:
                  m_in_wrapper.deserialize(m_data_size);
                  m_remaining =  m_remaining - sizeof(m_data_size);
                  // Bad size, bail and copy over erroneous data
                  if (m_data_size >= MAX_DATA_SIZE) {
                      m_state = START;
                      std::memcpy(m_in_buffer, m_in_buffer + m_in_offset, sizeof(m_data_size));
                      m_in_offset = 0;
                      m_remaining = sizeof(m_data_size);
                  }
                  // Good size, keep processing
                  else {
                      m_state = DATA;
                      m_in_offset += sizeof(m_data_size);
                  }
                  break;
              case DATA:
                  m_remaining -= m_data_size;
                  m_state = CHECK;
                  m_in_offset += m_data_size;
                  break;
              case CHECK:
                  // Read the checksum
                  U32 checksum;
                  m_in_wrapper.deserialize(checksum);
                  // Regardless of success, we will reset here in preparation for the next message.
                  // Checksum failures forfeit previous data.
                  m_state = START;
                  m_in_offset  = 0;
                  m_remaining =  m_remaining - sizeof(checksum);
                  //Recalculate the checksum and ensure it is correct in order to send the data out
                  U32 recalculate = 0xfeebdaed;
                  if (checksum == recalculate) {
                      Fw::ComBuffer com;
                      com.setBuff(m_in_buffer + 2  * sizeof(TOKEN_TYPE), m_data_size);
                      uplinkPort_out(0, com, 0);
                  }
          }
      }
  }
} // end namespace Svc
