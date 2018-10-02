
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

void FoodPump::setThreshold(unsigned int threshold)
{
  this->ppmSensor.threshold = threshold;
  this->threshold = threshold;
}

bool FoodPump::react() { return ppmSensor.react(); }
