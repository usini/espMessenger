from bottle import route, post, run, template, static_file, request
import json
import time
#http://bottlepy.org/docs/dev/tutorial.html#generating-content

@route('/')
def index():
    return static_file("index.html","./")

@route('/ping')
def webPing():
    return "1"

@post('/message')
def webMessage():
    time.sleep(1)
    return request.body

@route('/settings')
def webSettings():
    return static_file("settings.html","./")

@post('/save')
def webSave():
    global settings
    text = json.dumps(request.json)
    f = open( 'settings.json', 'w' )
    f.write(json.dumps(request.json))
    f.close()
    with open('settings.json') as f:
        settings = json.load(f)
    return text

@route('/name')
def webName():
    return settings["name"]

@route('/load')
def webLoad():
    return static_file("settings.json","./")

@route('/reboot')
def webReboot():
    return template("Rebooting")

with open('settings.json') as f:
    settings = json.load(f)
run(host='0.0.0.0', port=8080)