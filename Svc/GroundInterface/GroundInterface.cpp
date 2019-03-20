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
      GroundInterfaceComponentBase(compName)
#else
    GroundInterfaceImpl(void)
#endif
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
      Fw::Buffer output;
      output.setsize(data.getBuffLength());
      output.setdata(reinterpret_cast<POINTER_CAST>(data.getBuffAddr()));
      write_out(0, output);
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
        Fw::Buffer &fwBuffer
    )
  {}

} // end namespace Svc
