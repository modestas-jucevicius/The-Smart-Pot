#include "WaterPump.h"

WaterPump::WaterPump(unsigned int onTime, unsigned int offTime, bool usingTime, unsigned char pinNo) : Device(onTime, offTime, usingTime), pinNo(pinNo)
{

  //pinMode(pinNo, OUTPUT);
}

#ifdef DEBUG
void WaterPump::changeUsingTime() {
  Serial.print("WaterPump class ");
  Serial.flush();
  Device::changeUsingTime();
}
#endif



void WaterPump::turnOn()
{
  if (!currentState)
  {
    //digitalWrite(pinNo, HIGH); // just for example
    currentState = true;
  }
}
void WaterPump::turnOff()
{
  if (currentState)
  {
    //digitalWrite(pinNo, LOW);
    currentState = false;
  }
}
bool WaterPump::setThreshold(uint32_t threshold, OPTS* EEPROM_opts) \
{
  if (threshold > maxThresh) return false;
  this->h2oSensor->threshold = threshold;
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

void WaterPump::setUsingTime(bool usingTime, OPTS* EEPROM_opts)
{
  this->usingTime = usingTime;
  EEPROM_opts->H2OPump_usingTime = usingTime;
}


bool WaterPump::react() {
#ifdef DEBUG
  Serial.print(" WaterPump::react() ");
  Serial.flush();
  Serial.print((unsigned long)this->h2oSensor);
  Serial.flush();
#endif
  
  return this->h2oSensor->react();
}
