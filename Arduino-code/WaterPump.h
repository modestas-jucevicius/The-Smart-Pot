#ifndef WATERPUMP_H
#define WATERPUMP_H
#include <Arduino.h>
#include "static_config.h"
#include "H2OSensor.h"
#include "Device.h"

class WaterPump : public Device {
  private:
  unsigned char pinNo;
  H2OSensor& h2oSensor;
  public:
     WaterPump(unsigned int onTime, unsigned int offTimeS, bool usingTime, unsigned char pinNo, H2OSensor& h2oSensor);
     void turnOn();
     void turnOff();
     void setThreshold(unsigned int threshold);
     bool react();
  
};

#endif
