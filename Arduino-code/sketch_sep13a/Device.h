#ifndef DEVICE_H
#define DEVICE_H

#include <Arduino.h>





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
      bool    getState() { return this->currentState; }
      virtual void turnOn()=0;
      virtual void turnOff()=0;
      virtual bool setThreshold(uint32_t threshold);
      unsigned int getThreshold() { return threshold; }
      void         setUsingTime(bool usingTime) { this->usingTime = usingTime; }
      void         setOnTime(uint32_t onTime)   { this->onTime = onTime; }
      void         setOffTime(uint32_t offTime) { this->offTime = offTime; }
      void         changeUsingTime();

  private:
      virtual bool react() =0;    
      
};

#endif
