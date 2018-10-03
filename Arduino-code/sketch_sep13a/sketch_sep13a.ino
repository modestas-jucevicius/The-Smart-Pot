#include "static_config.h"
#include "grow_pot.h"
#include <EEPROM.h>


void establishConnection();



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
struct OPTS EEPROM_opts;



class DevState
{
  public:
    H2OSensor h2oSensor;
    WaterPump waterPump;

    PPMSensor ppmSensor;
    FoodPump foodPump;

    Device* devices[2] = {&waterPump, &foodPump};
    Sensor* sensors[2] = {&h2oSensor, &ppmSensor};

    DevState():
      ppmSensor  (EEPROM_opts.PPMSensorPin, EEPROM_opts.PPMSensor_threshold),
      h2oSensor  (EEPROM_opts.H2OSensorPin, EEPROM_opts.H2OSensor_threshold),
      waterPump  (EEPROM_opts.H2OPump_time_on, EEPROM_opts.H2OPump_time_off, EEPROM_opts.H2OPump_usingTime, EEPROM_opts.H2OPumpPin, h2oSensor),
      foodPump   (EEPROM_opts.FoodPump_time_on, EEPROM_opts.FoodPump_time_off, EEPROM_opts.FoodPump_usingTime, EEPROM_opts.FoodPumpPin, ppmSensor)
    {
      ;
    }

    void processTime()
    {
      for (Device* dev : devices)
      {
        dev->changeUsingTime();
      }
    }
    String printAllSensors()
    {
      String retString = "";
      for (Sensor* sensor : sensors)
      {
        retString += sensor->asString();
        retString += ' ';
      }
      retString[retString.length() - 1] = '\0';
      return retString;
    }

    String printAllDevices()
    {
      ;
    }

};


byte hasBeenSetup = 0;
DevState *devState = NULL;


int address = 0;


bool connected = false;
char commandString[MAXCOMMAND];







void getOptions() //receive initialization options as one space-delim String
{
  char* token;
  bool gotResponse = false;


  Serial.print("SENDOPTS\r");
  while (!gotResponse) {
    switch (char c = Serial.read()) {
      case 'B': //positive response
        gotResponse = true;
        break;
      default: //reset
        resetConnection(ERR_NOT_SETUP);
        return;
        break;

    }
  }
  Serial.readBytesUntil(TERMINATOR, commandString, MAXCOMMAND);
  if ((token = strtok(commandString, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.H2OSensorPin =  atoi(token);

  if ((token = strtok(commandString, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.H2OPumpPin =  atoi(token);

  if ((token = strtok(commandString, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.H2OSensor_threshold =  atoi(token);

  if ((token = strtok(commandString, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.H2OPump_usingTime =  atoi(token);

  if ((token = strtok(commandString, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.H2OPump_time_on = atoi(token);

  if ((token = strtok(commandString, " ") ) == NULL)
    goto processing_failed;
  EEPROM_opts.H2OPump_time_off =  atoi(token);

  if ((token = strtok(commandString, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.PPMSensorPin = atoi(token);

  if ((token = strtok(commandString, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.FoodPumpPin = atoi(token);

  if ((token = strtok(commandString, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.PPMSensor_threshold = atoi(token);

  if ((token = strtok(commandString, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.FoodPump_usingTime = atoi(token);

  if ((token = strtok(commandString, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.FoodPump_time_on = atoi(token);

  if ((token = strtok(commandString, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.FoodPump_time_off = atoi(token);

  if ((token = strtok(commandString, " ")) == NULL)
    goto processing_failed;
  strncpy(EEPROM_opts.current_setup, token, MAXCOMMAND - 1);




  hasBeenSetup = true;
  EEPROM.update(0, 1);
  writeDevState();
  return;



processing_failed:
  resetConnection(ERR_SETUP);
  return;

}





void setup() {
  Serial.begin(BAUDRATE);

  hasBeenSetup = EEPROM.read(0);
  Serial.print(hasBeenSetup);
  if (hasBeenSetup) {
    readDevState();
  }
  else {
    while (!hasBeenSetup) {
      establishConnection();
      getOptions();
    }
  }
  devState = new DevState();
}



void loop() {
  if (!connected )
  {
    establishConnection();
  }
  else
  {
    processCommands();
  }
  devState->processTime();

}






void readDevState() //READ OPTIONS FROM EEPROM
{
  int i;
  memset(&EEPROM_opts, 0, sizeof(EEPROM_opts));
  byte* optsptr = (byte*) (&EEPROM_opts);
  for (i = 1; i < sizeof(EEPROM_opts); i++) {
    *optsptr++ = EEPROM.read(i);
  }

}

void writeDevState()  //WRITE OPTIONS TO EEPROM
{
  byte* optsptr = (byte *) &EEPROM_opts;
  for (int i = 1; i < sizeof(DevState) + 1; i++) {
    EEPROM.update(i, *optsptr++);
  }
}


void serialFlush() {
  while (Serial.available() > 0) {
    char t = Serial.read();
  }
}

void resetConnection(int errorno)
{

  
  Serial.print(" ERR ");
  Serial.print(errorno);
  Serial.print('\r');
  Serial.flush();
  connected = false;
  serialFlush();

}


void processSensor(COM_TYPE com_type)
{
  char* token;

  token = strtok(NULL, "\r");

  if (com_type == GET)
  {
    if (strcmp(token, "ALL") == 0)
    {
      Serial.print(devState->printAllSensors());
      Serial.print('\r');
    }
    else if (strcmp(token, "H2OS") == 0)
    {
      Serial.print(devState->h2oSensor.asString());
      Serial.print('\r');
    }
    else if (strcmp(token, "PPMS") == 0)
    {
      Serial.print(devState->ppmSensor.asString());
      Serial.print('\r');
    }
    else
    {
      resetConnection(ERR_NOSENS);
    }
  }
  else
  {
    resetConnection(ERR_USEDEV);
  }

}

void processDevice(COM_TYPE com_type)
{
  char *token, *device; // token2 splits token by ":
  const char* errStr;
  Device* devToSet;
  uint32_t value;
  SET_TYPE devSetting;

  token = strtok(NULL, " ");
  device = strcpy(device, token);
  token = strtok(NULL, " ");

  if (strcmp(token, "USETIME") == 0) {
    devSetting = USETIME;
  }
  else if (strcmp(token, "ONTIME") == 0) {
    devSetting = OFFTIME;
  }
  else if (strcmp(token, "OFFTIME") == 0) {
    devSetting = ONTIME;
  }
  else if (strcmp(token, "THRESHOLD") == 0) {
    devSetting = THRESHOLD;
  }
  else
  {
    resetConnection(ERR_NOATTR);
    return;
  }

  if (strcmp(device, "H2OP") == 0)
  {
    devToSet = &devState->waterPump;
  }
  else if (strcmp(device, "FOODP") == 0)
  {
    devToSet = &devState->foodPump;
  }
  else
  {
    resetConnection(ERR_NODEV);
    return;
  }

  token = strtok(NULL, " ");
  if (devSetting == USETIME)
  {
    value = atoi(token);
    devToSet->setUsingTime((bool)value);
  }
  else if (devSetting == ONTIME)
  {
    value = atoi(token);
  }
  else if (devSetting == OFFTIME)
  {
    value = atoi(token);
    devToSet->setOffTime(value);
  }
  else if (devSetting == THRESHOLD)
  {
    value = atoi(token);
    if (devToSet->setThreshold(value) == false)
      resetConnection(ERR_PARAM);
  }
  

}



void processCommands()
{
  char* token;
  COM_TYPE com_type;
  if (Serial.available() > 0)
  {
    Serial.readBytesUntil(TERMINATOR, commandString, MAXCOMMAND);

    token = strtok(commandString, " ");
    if (strcmp(token, "RESET") == 0)
    {
      resetConnection(ERR_NOERROR);
      return;
    }
    else if (strcmp(token, "SET") == 0)
    {
      com_type = SET;
    }
    else if (strcmp(token, "GET") == 0)
    {
      com_type = GET;
    }
    else {
      resetConnection(ERR_NOCOMMAND);
      return;
    }


    token = strtok(NULL, " ");
    if (strcmp(token, "SENSOR") == 0)
    {
      processSensor(com_type);
    }
    else if (strcmp(token, "DEVICE") == 0)
    {
      processDevice(com_type);
    }
    else
    {
      resetConnection(ERR_UKOBJ);
      return;
    }
  }
}

void establishConnection() {
  if (Serial.available() > 0) {
    while (!connected) {
      Serial.print('A');
      delay(300);
      if (Serial.read() == 'A') {
        Serial.print('B');
        Serial.print('\r');
        connected = true;
      }
    }
  }
}
