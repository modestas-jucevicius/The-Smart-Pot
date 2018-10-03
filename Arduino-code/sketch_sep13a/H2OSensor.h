#ifndef H2O_SENSOR_H
#define H2O_SENSOR_H

#include "Sensor.h"

class H2OSensor : public Sensor {
   private:
      unsigned char     inputPin;
  public:
      unsigned int      threshold = 0; //if val < threshold, add water until OK;



      
      
   public:
      H2OSensor( unsigned char inputPin, unsigned int threshold) : inputPin(inputPin), threshold(threshold) {};
      unsigned int readVal();
      bool react();
      String asString();
};

#endif
