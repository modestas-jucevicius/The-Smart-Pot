#ifndef FOODPUMP_H
#define FOODPUMP_H

#include <Arduino.h>
#include "Device.h"
#include "PPMSensor.h"

class FoodPump : public Device {
  private:
   unsigned char pinNo; //placeholder
   PPMSensor& ppmSensor;
  public:
    FoodPump(unsigned int onTime, unsigned int offTimeS, bool usingTime, unsigned char pinNo, PPMSensor& ppmSensor);
    void turnOn();
    void turnOff();
    bool react();
    void setThreshold(unsigned int threshold);
};
#endif
