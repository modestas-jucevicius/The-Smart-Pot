#ifndef STATIC_CONF_H
#define STATIC_CONF_H

#define BAUDRATE 9600
#define MAXCOMMAND 255
#define TERMINATOR '\r'


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


#endif
