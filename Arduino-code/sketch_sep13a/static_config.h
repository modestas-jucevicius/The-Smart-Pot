#ifndef STATIC_CONF_H
#define STATIC_CONF_H

#define BAUDRATE 9600
#define MAXCOMMAND 255
#define TERMINATOR '\n'


#define ERR_NOERROR   0 //for resetting connection with no error
#define ERR_NOCOMMAND 1 //no such command
#define ERR_UKOBJ     2 //uknown object (TIME OR SENSOR)
#define ERR_NOT_SETUP 3 //hasn't received any settings yet
#define ERR_SETUP     4 //error when receiving setup
#define ERR_NOSENS    5 //error no such sensor
#define ERR_NODEV     6 //error no such device
#define ERR_USEDEV    7 //use device to set settings
#define ERR_NOATTR    8 //no such attribute
#define ERR_PARAM     9

//default values
#define FOOD_PUMP_PIN     13
#define H2O_PUMP_PIN      12
#define H2O_SENSOR_PIN    10
#define PPM_SENSOR_PIN    11




enum COM_TYPE {
  SET = 1,
  GET = 2
};

enum SET_TYPE {
  USETIME = 1, //to set if using time
  ONTIME,
  OFFTIME,
  THRESHOLD
};

struct OPTS {
  byte H2OSensorPin;
  byte H2OPumpPin;

  byte H2OSensor_threshold;

  bool H2OPump_usingTime;
  uint32_t H2OPump_time_on; //
  uint32_t H2OPump_time_off;

  /**********************************/
  byte PPMSensorPin;
  byte FoodPumpPin;

  byte PPMSensor_threshold;

  bool     FoodPump_usingTime;
  uint32_t FoodPump_time_on;
  uint32_t FoodPump_time_off;



  char current_setup[256];
};

#endif
