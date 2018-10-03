
#include "FoodPump.h"


FoodPump::FoodPump(unsigned int onTime, unsigned int offTime,bool usingTime, unsigned char pinNo, PPMSensor& ppmSensor) : Device(onTime, offTime, usingTime), pinNo(pinNo), ppmSensor(ppmSensor)
{
  pinMode(pinNo, OUTPUT);
}


void FoodPump::turnOn()
{
  if (!this->currentState)
  {
    digitalWrite(pinNo, HIGH); // just for example
    this->currentState = true;
  }
}

void FoodPump::turnOff()
{
  if (this->currentState)
  {
    digitalWrite(pinNo, LOW);
    this->currentState = false;
  }
}

bool FoodPump::setThreshold(uint32_t threshold)
{
  if (threshold > maxThresh) return false;
  this->ppmSensor.threshold = threshold;
  this->threshold = threshold;
  return true;
}

bool FoodPump::react() { return ppmSensor.react(); }
