#ifndef SENSOR_H
#define SENSOR_H
#include <Arduino.h>
#include "static_config.h"
//class Device;

class Sensor {
  public:
    Sensor() { }
    virtual unsigned int readVal() =0;
    virtual bool         react()   =0;
    virtual String       asString() =0;
};

#endif
