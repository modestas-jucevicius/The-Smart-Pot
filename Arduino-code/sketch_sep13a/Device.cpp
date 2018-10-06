#include "Device.h"

void Device::changeUsingTime()
{
#ifdef DEBUG
  Serial.print("changeUsingTime() ");
  Serial.flush();
#endif

  if ( usingTime )
  {
#ifdef DEBUG
    Serial.print("changeUsingTime()->usingTime = true ");
    Serial.flush();
#endif
    if (currentState == false)
    {
#ifdef DEBUG
      Serial.print("changeUsingTime()->current_state = false ");
      Serial.flush();
#endif
      if ( lastSwitch + offTime >= millis() )
      {
        lastSwitch = millis();
        if (react() || threshold == 0) //
          turnOn();
      }
    }
    else
    {
      if (lastSwitch + onTime >= millis())
      {
        lastSwitch = millis();
        turnOff();
      }
    }

  }
  else
  {
#ifdef DEBUG
    Serial.print("changeUsingTime()->!usingTime ");
    Serial.flush();
#endif
    if (react() && threshold)
    {
      turnOn();
    }
    else if (currentState == true && !react() && threshold)
    {
      turnOff();
    }
  }
}
