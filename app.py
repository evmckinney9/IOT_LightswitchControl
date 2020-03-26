from flask import Flask
from flask_restful import Api, Resource, reqparse

app = Flask(__name__)
api = Api(app)

lights = [
    {
        "name": "overhead",
        "status" : "off"
    }
]

class LightSwitch(Resource):
    def get(self, name, param):
	if (param != "get") :
		return "Invalid param (not set /get)", 400
        for light in lights:
            if(name == light["name"]):
                return light, 200
        return "User not found", 404

    def post(self, name, param): 
	if ("on" in param):
		param = "on"
	elif ("off" in param):
		param = "off"
        for light in lights:
            if(name == light["name"] and param == "on" or param == "off"):
                light["status"] = param

        	light = {
        	    "name": name,
         	   "status": param
       	 	}
        	return light, 202
     	return "invalid status (not set on/off)", 400

api.add_resource(LightSwitch, "/lightswitch/<string:name>/<string:param>")

app.run(host="0.0.0.0")
