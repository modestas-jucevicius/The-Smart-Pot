#include "H2OSensor.h"


bool H2OSensor::react() {
  return readVal() < threshold;
}

unsigned int H2OSensor::readVal()
{
  return  1000; //placeholder
}

String H2OSensor::asString()
{

  String retString = "H2OS:";
  retString += readVal();
  return retString;

}
