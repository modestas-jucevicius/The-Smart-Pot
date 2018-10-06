#ifndef WATERPUMP_H
#define WATERPUMP_H
#include <Arduino.h>
#include "static_config.h"
#include "H2OSensor.h"
#include "Device.h"

class WaterPump : public Device {
  private:
    const uint32_t maxThresh = 100;
    unsigned char pinNo;
    H2OSensor* h2oSensor;
  public:
    WaterPump(unsigned int onTime, unsigned int offTimeS, bool usingTime, unsigned char pinNo);
    void setSensorPtr(H2OSensor* h2oSensor) { this->h2oSensor = h2oSensor; }
    void turnOn();
    void turnOff();
    bool setThreshold(uint32_t threshold, OPTS* EEPROM_opts);
    void setUsingTime(bool usingTime, OPTS* EEPROM_opts);
    void setOnTime(uint32_t onTime,  OPTS* EEPROM_opts);
    void setOffTime(uint32_t offTime, OPTS* EEPROM_opts);
    bool react();
#ifdef DEBUG
    void changeUsingTime();
#endif

};

#endif
