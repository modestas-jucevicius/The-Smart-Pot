import serial
from enum import Enum
import logging
import re

logging.basicConfig(level=logging.DEBUG)




class WhatToSet(Enum):
	H2OS_Threshold  = 'H2O_sensor_threshold'
	H2OP_UseTime    = 'H2O_pump_using_time' 
	H2OP_TimeOn     = 'H2O_pump_time_on'
	H2OP_TimeOff    = 'H2O_pump_time_off'
	PPMS_Threshold  = 'PPM_sensor_threshold'
	FoodP_UseTime   = 'food_pump_using_time'
	FoodP_TimeOn    = 'food_pump_time_on'
	FoodP_TimeOff   = 'food_pump_time_off'
	Setup_Name		= 'setup_name'

class WhatToGet(Enum):
	H2O_Sensor       = 'H2OS'
	PPM_Sensor       = 'PPMS'
	ALL				 = 'ALL'
	
	



class SmartPot:
	connection  = None
	connected   = False
	is_setup 	= False
	settings    = {}
	stringFunc  = {}
	
	

	def __init__(self):
		self.connected  = False
		self.connection = None
		self.settings  = {
					'H2O_sensor_pin' 		: 10,
					'H2O_pump_pin'   		: 12,
					'H2O_sensor_threshold'	: 0,
					
					'H2O_pump_using_time'   : 0,
					'H2O_pump_time_on'		: 0,
					'H2O_pump_time_off'		: 0,
					
					
					'PPM_sensor_pin'		: 11,
					'food_pump_pin'			: 13,
					'PPM_sensor_threshold'	: 0,
					
					'food_pump_using_time'  : 0,
					'food_pump_time_on'		: 0,
					'food_pump_time_off'	: 0,
					
					'setup_name'			: ''
				}
		self.stringFunc = {
					'H2O_sensor_pin' 		: None,
					'H2O_pump_pin'   		: None,
					'H2O_sensor_threshold'	: self.H2O_sensor_threshold_str,
					
					'H2O_pump_using_time'   : self.H2O_pump_using_time_str,
					'H2O_pump_time_on'		: self.H2O_pump_time_on_str,
					'H2O_pump_time_off'		: self.H2O_pump_time_off_str,
					
					
					'PPM_sensor_pin'		: None,
					'food_pump_pin'			: None,
					'PPM_sensor_threshold'	: self.PPM_sensor_threshold_str,
					
					'food_pump_using_time'  : self.food_pump_using_time_str,
					'food_pump_time_on'		: self.food_pump_time_on_str,
					'food_pump_time_off'	: self.food_pump_time_off_str,
					
					'setup_name'			: self.setup_name_str
				}
	
	def openConnection(self, name="COM5", baudrate=9600, timeout=1):
		self.connection = serial.Serial(name, baudrate)# timeout=timeout)
		
	
	
	def startConnection(self, settings=None):

		buf = str()
		if self.connected:
			return True
		else:
			if self.connection == None:
				self.openConnection()
			while not self.connected:
				readch = self.connection.read()
				logging.debug(readch)
				if readch == b'A':
					logging.debug("Inside connection if")
					#self.connection.reset_input_buffer()
					self.connection.write('A'.encode())
					#self.connection.flush()
					response = self.connection.readline()
					logging.debug(str(response))
					if b'B' in response:
						self.connected = True
						logging.debug("Connected to serial")
						if self.connection.readline() == "SENDOPTS":
							logging.debug("Arduino needs options")
							if settings == None or self.checkSettings(settings) == False:
								return False
							if self.sendAll(settings) == False:
								return False
							self.settings = settings
						else:
							logging.debug("Getting settings")
							self.settings = self.getSettings()
						if self.settings == None:
							return False
						self.is_setup = True
						return True
	
			
	#def H2O_pump_pin_str(self):
#		return 'H2OP ' + str(self.settings['H2O_pump_pin'])
	#def H2O_sensor_pin_str(self):
	#	return 'H2OS' + ' ' + str(self.settings['H2O_sensor_pin'])
	
	def H2O_pump_using_time_str(self):
		if self.settings['H2O_pump_using_time'] == True:
			return 'H2OP USETIME 1'
		else:
			return 'H2OP USETIME 0'
	def H2O_pump_time_on_str(self):
		return 'H2OP ONTIME ' + str(self.settings['H2O_pump_time_on'])
	def H2O_pump_time_off_str(self):
		return 'H2OP OFFTIME ' + str(self.settings['H2O_pump_time_off'])
	def H2O_sensor_threshold_str(self):
		return 'H2OP THRESHOLD ' + str(self.settings['H2O_sensor_threshold'])					
	def food_pump_using_time_str(self):
		if self.settings['food_pump_using_time'] == True:
			return 'FOODP USETIME 1'
		else:
			return 'FOODP USETIME 0'
	def food_pump_time_on_str(self):
		return 'FOODP ONTIME ' + str(self.settings['food_pump_time_on'])
	def food_pump_time_off_str(self):
		return 'FOODP OFFTIME ' + str(self.settings['food_pump_time_off'])
	def PPM_sensor_threshold_str(self):
		return 'FOODP THRESHOLD ' + str(self.settings['PPM_sensor_threshold'])
	def setup_name_str(self):
		return 'SETUPNAME ' + self.settings['setup_name']
	
	def sendAll(self):
		if not connected or not is_setup:
			return False
		self.connection.serial.write('B')
		writebuf = 	   str(self.settings['H2O_sensor_pin']) + ' ' +          \
					   str(self.settings['H2O_pump_pin']) + ' ' +			 \
					   str(self.settings['H2O_sensor_threshold']) + ' ' +    \
					   str(self.settings['H2O_pump_using_time']) + ' ' +     \
					   str(self.settings['H2O_pump_time_on']) + ' ' +        \
					   str(self.settings['H2O_pump_time_off']) + ' ' +       \
					   str(self.settings['PPM_sensor_pin']) + ' ' +          \
					   str(self.settings['food_pump_pin']) + ' ' +           \
					   str(self.settings['PPM_sensor_threshold']) + ' ' +    \
					   str(self.settings['food_pump_using_time']) + ' ' +    \
					   str(self.settings['food_pump_time_on']) + ' ' +       \
					   str(self.settings['food_pump_time_off']) + ' ' +      \
					   self.settings['setup_name'] + '\n'
		self.connection.write(writebuf.encode())
		self.connection.flush()
		buf = self.connection.readline()
		if buf == "OK":
			is_setup == True
			return True
		else:
			return False
	
	def setSetting(whatSetting: WhatToSet, value=None):
		if not connected:
			return False
		if whatSetting == None or value == None:
			return False
		newSetting = dict(self.setting)
		newSetting[whatSetting.value] = value
		if checkSettings(newSetting) == False:
				return False
		self.settings = newSetting
		writebuf = "SET DEVICE " + stringFunc[whatSetting.value]() + '\n'
		self.connection.write(writebuf.encode())
		self.connection.flush()
		
	def getData(self,whatSensor: WhatToGet):
		if not self.connected:
			return None
		buf = "GET SENSOR " + whatSensor.value + '\n'
		self.connection.write(buf.encode())
		self.connection.flush()
		return self.connection.readline().decode()
	
	def disconnect(self):
		try:
			self.connection.write("RESET\n".encode())
			self.connection.flush()
		except:
			pass
		self.connection  = None
		if not self.connected:	
			return True
		self.connected = False
		return True
		
		
	
	
	def getSettings(self):
		self.connection.write("GETOPTS\n".encode())
		buffer = self.connection.readline()
		logging.debug("gotString %s", buffer.decode())
		buffer = buffer.decode().split(' ')
		logging.debug("split into %s", buffer)
		if len(buffer) < 13:
			return None
		settings  = {
					'H2O_sensor_pin' 		: int(buffer[0]),
					'H2O_pump_pin'   		: int(buffer[1]),
					'H2O_sensor_threshold'	: int(buffer[2]),
					
					'H2O_pump_using_time'   : int(buffer[3]),
					'H2O_pump_time_on'		: int(buffer[4]),
					'H2O_pump_time_off'		: int(buffer[5]),
					
					
					'PPM_sensor_pin'		: int(buffer[6]),
					'food_pump_pin'			: int(buffer[7]),
					'PPM_sensor_threshold'	: int(buffer[8]),
				
					'food_pump_using_time'  : int(buffer[9]),
					'food_pump_time_on'		: int(buffer[10]),
					'food_pump_time_off'	: int(buffer[11]),
					
					'setup_name'			: buffer[12].rstrip('\n\r'),
				}
		self.settings = settings
		return settings
	
		
	def checkSettings(self,set=None):
		if set == None:
			return False
		if set['H2O_sensor_threshold'] > 100:
			return False
		if set ['PPM_sensor_threshold']  > 100:
			return False
		return True
	

def main():
	pass
	
if __name__ == "__main__":
	main()
	
	
	
		