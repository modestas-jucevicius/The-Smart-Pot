
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

bool FoodPump::setThreshold(uint32_t threshold,OPTS* EEPROM_opts)
{
  if (threshold > maxThresh) return false;
  this->ppmSensor.threshold = threshold;
  this->threshold = threshold;
  EEPROM_opts->PPMSensor_threshold = threshold;
  return true;
}

void FoodPump::setOnTime(uint32_t onTime,  OPTS* EEPROM_opts)
{
	this->onTime = onTime;
	EEPROM_opts->FoodPump_time_on = onTime;
}

void FoodPump::setOffTime(uint32_t offTime,  OPTS* EEPROM_opts)
{
	this->offTime = offTime;
	EEPROM_opts->FoodPump_time_off = offTime;
}

void FoodPump::setUsingTime(bool usingTime,OPTS* EEPROM_opts)
{
	this->usingTime = usingTime;
	EEPROM_opts->FoodPump_usingTime = usingTime;
}

bool FoodPump::react() { return ppmSensor.react(); }
