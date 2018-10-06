#ifndef FOODPUMP_H
#define FOODPUMP_H

#include <Arduino.h>
#include "Device.h"
#include "PPMSensor.h"

class FoodPump : public Device {
  private:
    const uint32_t maxThresh = 100;
    unsigned char pinNo; //placeholder
    PPMSensor* ppmSensor;
  public:
    FoodPump(unsigned int onTime, unsigned int offTimeS, bool usingTime, unsigned char pinNo);
    void setSensorPtr(PPMSensor* ppmSensor) { this->ppmSensor = ppmSensor; }
    void turnOn();
    void turnOff();
    bool react();
    bool setThreshold(uint32_t threshold, OPTS* EEPROM_opts);
    void setUsingTime(bool usingTime, OPTS* EEPROM_opts);
    void setOnTime(uint32_t onTime,  OPTS* EEPROM_opts);
    void setOffTime(uint32_t offTime, OPTS* EEPROM_opts);
#ifdef DEBUG
    void changeUsingTime();
#endif
};
#endif
