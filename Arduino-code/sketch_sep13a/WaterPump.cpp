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
bool WaterPump::setThreshold(uint32_t threshold, OPTS* EEPROM_opts) \
{
  if (threshold > maxThresh) return false;
  this->h2oSensor.threshold = threshold;
  this->threshold = threshold;
  EEPROM_opts->H2OSensor_threshold = threshold;
  return true;
}


void WaterPump::setOnTime(uint32_t onTime,  OPTS* EEPROM_opts)
{
	this->onTime = onTime;
	EEPROM_opts->H2OPump_time_on = onTime;
}

void WaterPump::setOffTime(uint32_t offTime,  OPTS* EEPROM_opts)
{
	this->offTime = offTime;
	EEPROM_opts->H2OPump_time_off = offTime;
}

void WaterPump::setUsingTime(bool usingTime,OPTS* EEPROM_opts)
{
	this->usingTime = usingTime;
	EEPROM_opts->H2OPump_usingTime = usingTime;
}


bool WaterPump::react() { return h2oSensor.react(); }
