// ======================================================================
// \title  BodySensorComponentImpl.cpp
// \author pelmini
// \brief  cpp file for BodySensor component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================


#include <DoorBell/BodySensor/BodySensorComponentImpl.hpp>
#include "Fw/Types/BasicTypes.hpp"
//#include <Arduino.h>

namespace DoorBell {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction
  // ----------------------------------------------------------------------

  BodySensorComponentImpl ::
#if FW_OBJECT_NAMES == 1
    BodySensorComponentImpl(
        const char *const compName
    ) :
      BodySensorComponentBase(compName)
#else
    BodySensorComponentImpl(void)
#endif
  {
    count = 0;
  }

  void BodySensorComponentImpl ::
    init(
        const NATIVE_INT_TYPE instance
    )
  {
    BodySensorComponentBase::init(instance);
  }

  BodySensorComponentImpl ::
    ~BodySensorComponentImpl(void)
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void BodySensorComponentImpl ::
    schedIn_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
    printf("I am here\n");
    int val = 0;
    val = 200; //analogRead(A6);

    if(val > 150)
    {
      ON_OFF_out(0, true);
      count = 30;
    }
    else if(count == 0)
    {
      ON_OFF_out(0,false);
    }
    else
    {
      count--;
    }

  }

} // end namespace DoorBell
