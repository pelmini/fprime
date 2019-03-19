// ====================================================================== 
// \title  LedBlinkerImpl.cpp
// \author lestarch
// \brief  cpp file for LedBlinker component implementation class
// ====================================================================== 


#include <examples/Arduino/LedBlinker/LedBlinker.hpp>
#include "Fw/Types/BasicTypes.hpp"

#include <Arduino.h>
#define LED_PIN 13
namespace Arduino {

  // ----------------------------------------------------------------------
  // Construction, initialization, and destruction 
  // ----------------------------------------------------------------------

  LedBlinkerComponentImpl ::
#if FW_OBJECT_NAMES == 1
    LedBlinkerComponentImpl(
        const char *const compName
    ) :
      LedBlinkerComponentBase(compName), 
#else
    LedBlinkerComponentBase(void),
#endif
    m_counter(0),
    m_cap(0),
    m_state(false)
  {
//      pinMode(LED_PIN, OUTPUT);
//      digitalWrite(LED_PIN, HIGH);
  }

  void LedBlinkerComponentImpl ::
    init(
        const NATIVE_INT_TYPE instance
    ) 
  {
    LedBlinkerComponentBase::init(instance);
  }

  LedBlinkerComponentImpl ::
    ~LedBlinkerComponentImpl(void)
  {

  }

  // ----------------------------------------------------------------------
  // Handler implementations for user-defined typed input ports
  // ----------------------------------------------------------------------

  void LedBlinkerComponentImpl ::
    schedIn_handler(
        const NATIVE_INT_TYPE portNum,
        NATIVE_UINT_TYPE context
    )
  {
	  digitalWrite(13, m_state);
	  m_state = !m_state;
  }

} // end namespace Arduino
