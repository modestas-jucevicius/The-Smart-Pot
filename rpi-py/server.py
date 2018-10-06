from smartpot_control import SmartPot, WhatToGet, WhatToSet
from flask import Flask
import json
import logging
import traceback

app = Flask(__name__)

smartpot = SmartPot()


@app.route("/GET/H2OS")
def getH2OSenor():
	if smartpot.connected == False:
		try:
			if smartpot.startConnection() == False:
				return json.dumps({'error' : 'Failed to Connect'})
			if smartpot.is_setup == False:
				return json.dumps({'error' : 'Device has not yet been setup'})
		except Exception as e:
			logging.debug(traceback.print_exc())
			smartpot.disconnect()
			return json.dumps({'error' :str(e)})
	buf = smartpot.getData(WhatToGet.H2O_Sensor)
	if buf:
		return json.dumps({'H2OS' : int(buf.rstrip("\r\n").split(' ')[1])})
	else:
		return json.dumps({'error' :"Failed to get Data"})

@app.route("/GET/PPMS")
def getPPMSenor():
	if smartpot.connected == False:
		try:
			if smartpot.startConnection() == False:
				return json.dumps({'error' : 'Failed to Connect'})
			if smartpot.is_setup == False:
				return json.dumps({'error' : 'Device has not yet been setup'})
		except Exception as e:
			logging.debug(traceback.print_exc())
			smartpot.disconnect()
			return json.dumps({'error' :str(e)})
	buf = smartpot.getData(WhatToGet.PPM_Sensor)
	if buf:
		return json.dumps({'PPMS' : int(buf.rstrip("\r\n").split(' ')[1])})
	else:
		return json.dumps({'error' :"Failed to get Data"})

@app.route("/GET/ALL")
def getAllSensor():
	if smartpot.connected == False:
		try:
			if smartpot.startConnection() == False:
				return json.dumps({'error' : 'Failed to Connect'})
			if smartpot.is_setup == False:
				return json.dumps({'error' : 'Device has not yet been setup'})
		except Exception as e:
			logging.debug(traceback.print_exc())
			smartpot.disconnect()
			return json.dumps({'error' :str(e)})
	buf = smartpot.getData(WhatToGet.ALL)
	if buf:
		return json.dumps({'H2OS' : int(buf.rstrip("\r\n").split(' ')[1]), 'PPMS' : int(buf.rstrip("\r\n").split(' ')[3])})
	else:
		return json.dumps({'error' :"Failed to get Data"})

@app.route("/GET/CONFIG")
def getConfig():
	if smartpot.connected == False:
		try:
			if smartpot.startConnection() == False:
				return json.dumps({'error' : 'Failed to Connect'})
			if smartpot.is_setup == False:
				return json.dumps({'error' : 'Device has not yet been setup'})
		except Exception as e:
			logging.debug(traceback.print_exc())
			smartpot.disconnect()
			return json.dumps({'error' :str(e)})
	smartpot.getSettings()
	return json.dumps(smartpot.settings)


		

def main():
	logging.basicConfig(level=logging.DEBUG)
	smartpot = SmartPot()

	
if __name__ == "__main__":
	main()
	app.run(host="localhost", port=5000)