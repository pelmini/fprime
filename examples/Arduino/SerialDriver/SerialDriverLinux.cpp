// ====================================================================== 
// \title  SerialDriverImpl.cpp
// \author lestarch
// \brief  cpp file for SerialDriver component implementation class
// ====================================================================== 


#include <examples/Arduino/SerialDriver/SerialDriver.hpp>
#include "Fw/Types/BasicTypes.hpp"
#include <fcntl.h>
#include <unistd.h>

int fd = -1;
#define SERIAL_FILE_LINUX "/dev/pts/3"
namespace Arduino {

  void SerialDriverComponentImpl ::
    init(
        const NATIVE_INT_TYPE instance
    ) 
  {
    SerialDriverComponentBase::init(instance);
    fd = open(SERIAL_FILE_LINUX, O_RDWR);
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void SerialDriverComponentImpl ::
    write_data(
        Fw::Buffer &fwBuffer
    )
  {
      if (fd != -1) {
          write(fd, reinterpret_cast<U8*>(fwBuffer.getdata()),fwBuffer.getsize());
      }
  }

  void SerialDriverComponentImpl ::
    read_data(Fw::Buffer &fwBuffer)
  {
      NATIVE_INT_TYPE result;
      if ((fd != -1) && (-1 != (result = read(fd, reinterpret_cast<U8*>(fwBuffer.getdata()), fwBuffer.getsize())))) {
          fwBuffer.setsize(result);
	  printf("Read: %d bytes\n", fwBuffer.getsize());
      }
      else {
          fwBuffer.setsize(0);
      }
  }



} // end namespace Svc
