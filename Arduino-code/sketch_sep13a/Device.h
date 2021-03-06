#ifndef DEVICE_H
#define DEVICE_H

#include <Arduino.h>
#include "static_config.h"




class Device {
  protected:
    const uint32_t       maxThresh;
    uint32_t             onTime = 0; //on Time in seconds
    uint32_t             offTime = 0;
    uint32_t             lastSwitch = 0;
    bool                 currentState = false;  //false = off, true = on
    //bool               usingSensor    = true;
    bool                 usingTime      = true;
    unsigned int         threshold;
  public:
    Device()  {}
    Device(unsigned int onTime, unsigned int offTime, bool usingTime) : onTime(onTime), offTime(offTime), usingTime(usingTime) {};
    bool    getState() {
      return this->currentState;
    }
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
    virtual bool setThreshold(uint32_t threshold, OPTS* EEPROM_opts);
    unsigned int getThreshold() {
      return threshold;
    }
    virtual void setUsingTime(bool usingTime, OPTS* EEPROM_opts);
    virtual void setOnTime(uint32_t onTime,  OPTS* EEPROM_opts);
    virtual void setOffTime(uint32_t offTime, OPTS* EEPROM_opts);
    virtual void changeUsingTime();

  private:
    virtual bool react() = 0;

};

#endif
