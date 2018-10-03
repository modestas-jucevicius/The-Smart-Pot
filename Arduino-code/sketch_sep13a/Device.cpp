#include "Device.h"

void Device::changeUsingTime()
{
  if ( usingTime )
  {
    if (currentState == false)
    {
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
