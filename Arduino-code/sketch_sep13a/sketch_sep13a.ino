#include "static_config.h"
#include "grow_pot.h"
#include <EEPROM.h>

void establishConnection();




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
      waterPump  (EEPROM_opts.H2OPump_time_on, EEPROM_opts.H2OPump_time_off, EEPROM_opts.H2OPump_usingTime, EEPROM_opts.H2OPumpPin),
      foodPump   (EEPROM_opts.FoodPump_time_on, EEPROM_opts.FoodPump_time_off, EEPROM_opts.FoodPump_usingTime, EEPROM_opts.FoodPumpPin)
    {
      this->waterPump.setSensorPtr(&h2oSensor);
      this->foodPump.setSensorPtr(&ppmSensor);
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
      retString[retString.length() - 1] = '\n';
      return retString;
    }

    String printAllDevices()
    {
      ;
    }

};


byte hasBeenSetup = 0;
DevState *devState = NULL;
bool line_printed = false;

int address = 0;


bool connected = false;
char commandString[MAXCOMMAND];



void sendAllOptions()
{

  Serial.print(EEPROM_opts.H2OSensorPin);
  Serial.print(' ');
  Serial.print(EEPROM_opts.H2OPumpPin);
  Serial.print(' ');
  Serial.print(EEPROM_opts.H2OSensor_threshold);
  Serial.print(' ');
  Serial.print(EEPROM_opts.H2OPump_usingTime);
  Serial.print(' ');
  Serial.print(EEPROM_opts.H2OPump_time_on);
  Serial.print(' ');
  Serial.print(EEPROM_opts.H2OPump_time_off);
  Serial.print(' ');
  Serial.print(EEPROM_opts.PPMSensorPin);
  Serial.print(' ');
  Serial.print(EEPROM_opts.FoodPumpPin);
  Serial.print(' ');
  Serial.print(EEPROM_opts.PPMSensor_threshold);
  Serial.print(' ');
  Serial.print(EEPROM_opts.FoodPump_usingTime);
  Serial.print(' ');
  Serial.print(EEPROM_opts.FoodPump_time_on);
  Serial.print(' ');
  Serial.print(EEPROM_opts.FoodPump_time_off);
  Serial.print(' ');
  Serial.print(EEPROM_opts.current_setup);
  Serial.print('\n');
  Serial.flush();
}



void getOptions() //receive initialization options as one space-delim String
{
  char* token;
  char* response;
  bool gotResponse = false;

  establishConnection();
  Serial.print("SENDOPTS\n");
  while (!gotResponse) {
    if (Serial.read() == 'B')
      gotResponse = true;
  }
  while (!Serial.available()) {
    ;
  }

  Serial.readBytesUntil(TERMINATOR, commandString, MAXCOMMAND);
#ifdef DEBUG
  Serial.println(commandString);
#endif


  if ((token = strtok(commandString, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.H2OSensorPin =  atoi(token);

#ifdef DEBUG
  Serial.println(token);
#endif

  if ((token = strtok(NULL, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.H2OPumpPin =  atoi(token);
#ifdef DEBUG
  Serial.println(token);
#endif

  if ((token = strtok(NULL, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.H2OSensor_threshold =  atoi(token);
#ifdef DEBUG
  Serial.println(token);
#endif

  if ((token = strtok(NULL, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.H2OPump_usingTime =  atoi(token);
#ifdef DEBUG
  Serial.println(token);
#endif

  if ((token = strtok(NULL, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.H2OPump_time_on = atoi(token);
#ifdef DEBUG
  Serial.println(token);
#endif
  if ((token = strtok(NULL, " ") ) == NULL)
    goto processing_failed;
  EEPROM_opts.H2OPump_time_off =  atoi(token);
#ifdef DEBUG
  Serial.println(token);
#endif

  if ((token = strtok(NULL, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.PPMSensorPin = atoi(token);
#ifdef DEBUG
  Serial.println(token);
#endif

  if ((token = strtok(NULL, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.FoodPumpPin = atoi(token);
#ifdef DEBUG
  Serial.println(token);
#endif

  if ((token = strtok(NULL, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.PPMSensor_threshold = atoi(token);
#ifdef DEBUG
  Serial.println(token);
#endif

  if ((token = strtok(NULL, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.FoodPump_usingTime = atoi(token);
#ifdef DEBUG
  Serial.println(token);
#endif

  if ((token = strtok(NULL, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.FoodPump_time_on = atoi(token);
#ifdef DEBUG
  Serial.println(token);
#endif

  if ((token = strtok(NULL, " ")) == NULL)
    goto processing_failed;
  EEPROM_opts.FoodPump_time_off = atoi(token);
#ifdef DEBUG
  Serial.println(token);
#endif

  if ((token = strtok(NULL, " ")) == NULL)
    token = "NO_NAME";
  strncpy(EEPROM_opts.current_setup, token, MAXCOMMAND - 1);
#ifdef DEBUG
  Serial.println(token);
#endif

  Serial.print("OK\n");



  hasBeenSetup = true;
  EEPROM.update(0, 1);
  writeDevState();
  return;



processing_failed:
  resetConnection(ERR_SETUP);
  return;

}



void blink3()
{
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
}

static void clearEEPROM()
{
  int  i = 0;
  while (i < EEPROM.length())
    EEPROM.update(i++, 0);
  blink3();
}


void setup() {
  Serial.begin(BAUDRATE);
  hasBeenSetup = EEPROM.read(0);
  Serial.print(hasBeenSetup, HEX);
  if (hasBeenSetup) {
    readDevState();
  }
  else {
    while (!hasBeenSetup) {
      getOptions();
    }
  }
  devState = new DevState();
}



void loop() {
  if (!connected )
  {
    establishConnection();
    if (!line_printed && connected)
    {
      Serial.print("WAITCOM\n");
      line_printed = true;
    }
  }
  else
  {
    processCommands();
  }
  blink3();
  devState->processTime();
  blink3();

}






void readDevState() //READ OPTIONS FROM EEPROM
{
  int i = 1;
  memset(&EEPROM_opts, 0, sizeof(EEPROM_opts));
  byte* optsptr = (byte*) (&EEPROM_opts);
  while (i < sizeof(OPTS) + 1) {
    *optsptr++ = EEPROM.read(i++);
  }
#ifdef DEBUG
  sendAllOptions();
#endif


}

void writeDevState()  //WRITE OPTIONS TO EEPROM
{
  byte* optsptr = (byte *) &EEPROM_opts;
  for (int i = 1; i < sizeof(OPTS) + 1; i++) {
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
  Serial.print('\n');
  Serial.flush();
  connected = false;
  line_printed = false;
  serialFlush();

}


void processSensor(COM_TYPE com_type)
{
  char* token;

  token = strtok(NULL, " ");

  if (com_type == GET)
  {
    if (strcmp(token, "ALL") == 0)
    {
      Serial.print(devState->printAllSensors());
      //Serial.print('\n');
    }
    else if (strcmp(token, "H2OS") == 0)
    {
      Serial.print(devState->h2oSensor.asString());
      Serial.print('\n');
    }
    else if (strcmp(token, "PPMS") == 0)
    {
      Serial.print(devState->ppmSensor.asString());
      Serial.print('\n');
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
  char *token, *device; // token2 splits token by " "
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
    devToSet->setUsingTime((bool)value, &EEPROM_opts);
  }
  else if (devSetting == ONTIME)
  {
    value = atoi(token);
    devToSet->setOnTime(value, &EEPROM_opts);
  }
  else if (devSetting == OFFTIME)
  {
    value = atoi(token);
    devToSet->setOffTime(value, &EEPROM_opts);
  }
  else if (devSetting == THRESHOLD)
  {
    value = atoi(token);
    if (devToSet->setThreshold(value, &EEPROM_opts) == false)
      resetConnection(ERR_PARAM);
    return;
  }

  writeDevState();

}



void processCommands()
{
  char* token;
  COM_TYPE com_type;
  if (Serial.available() > 0)
  {
    Serial.readBytesUntil(TERMINATOR, commandString, MAXCOMMAND);
#ifdef DEBUG
    Serial.println(commandString);
#endif

    token = strtok(commandString, " ");
#ifdef DEBUG
    Serial.println(token);
#endif
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
    else if (strcmp(token, "GETOPTS") == 0)
    {
      sendAllOptions();
      return;
    }
    else if (strcmp(token, "ERASE") == 0)
    {
      clearEEPROM();
      Serial.print("RESETPLS\n");
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
  if (!connected) {
    Serial.print('A');
    //delay(300);
    if (Serial.available() > 0) {
      if (Serial.read() == 'A') {
        Serial.print("B\n");
        connected = true;
      }
    }
  }
}
