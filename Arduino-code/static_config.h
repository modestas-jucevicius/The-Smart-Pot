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

//default values
#define FOOD_PUMP_PIN     13
#define H2O_PUMP_PIN      12
#define H2O_SENSOR_PIN    10
#define PPM_SENSOR_PIN    11




enum COM_TYPE {
  SET = 1,
  GET = 2
};





#endif
