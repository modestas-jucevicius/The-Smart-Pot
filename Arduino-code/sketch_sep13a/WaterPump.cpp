#include "WaterPump.h"

WaterPump::WaterPump(unsigned int onTime, unsigned int offTime, bool usingTime, unsigned char pinNo, H2OSensor& sensor) : Device(onTime, offTime, usingTime), pinNo(pinNo), h2oSensor(h2oSensor)
{
  
  pinMode(pinNo, OUTPUT);
}


void WaterPump::turnOn()
{
  if (!currentState)
  {
    digitalWrite(pinNo, HIGH); // just for example
    currentState = true;
  }
}
void WaterPump::turnOff()
{
  if (currentState)
  {
    digitalWrite(pinNo, LOW);
    currentState = false;
  }
}

bool WaterPump::setThreshold(uint32_t threshold)
{
  if (threshold > maxThresh) return false;
  this->h2oSensor.threshold = threshold;
  this->threshold = threshold;
  return true;
}

bool WaterPump::react() { return h2oSensor.react(); }
