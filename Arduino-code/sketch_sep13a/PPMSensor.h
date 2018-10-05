#ifndef PPM_SENSOR_H
#define PPM_SENSOR_H

#include "Sensor.h"


class PPMSensor : public Sensor {
  private:
    unsigned char inputPin;

  public:
    unsigned int threshold;

    PPMSensor( unsigned char inputPin, unsigned int threshold) : inputPin(inputPin), threshold(threshold) {
      //pinMode(inputPin, INPUT);
    };
    unsigned int readVal();
    bool         react();
    String       asString();
};

#endif
