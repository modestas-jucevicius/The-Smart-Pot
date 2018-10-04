from smartpot_control import SmartPot, WhatToGet, WhatToSet
from flask import Flask
import json
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
		except:
			return json.dumps({'error' :'Failed to open serial connection'})
	buf = smartpot.getData(WhatToGet.H2O_Sensor)
	if buf:
		return json.loads({'H2OS:' : buf})
	else:
		return json.dumps({'error' :"Failed to get Data"})



def main():
	 smartpot = SmartPot()

	
if __name__ == "__main__":
	main()
	app.run(host="localhost", port=5000)