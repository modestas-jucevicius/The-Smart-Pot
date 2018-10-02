#ifndef DEVICE_H
#define DEVICE_H

#include <Arduino.h>


class Device {
  protected:
      unsigned int         onTime = 0; //on Time in seconds
      unsigned int         offTime = 0;
      unsigned int         lastSwitch = 0;
      bool                 currentState = false;  //false = off, true = on
      //bool                 usingSensor    = true;
      bool                 usingTime      = true;
      unsigned int         threshold;
  public:
      Device()  {}
      Device(unsigned int onTime, unsigned int offTime, bool usingTime) : onTime(onTime), offTime(offTime), usingTime(usingTime) {};
      bool    getState() { return this->currentState; }
      virtual void turnOn()=0;
      virtual void turnOff()=0;
      virtual void setThreshold(unsigned int threshold);
      unsigned int getThreshold() { return threshold; }
      void changeUsingTime();

  private:
      virtual bool react() =0;    
      
};

#endif
