#include "PPMSensor.h"

unsigned int PPMSensor::readVal() {
  return rand();  //placeholder
}


String PPMSensor::asString() {
  String retString = "PPMS ";
  retString += readVal();
  return retString;
}

bool PPMSensor::react() {
  readVal() < threshold;
}
