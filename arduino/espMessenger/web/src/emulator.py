from bottle import route, post, get, run, template, static_file, request
import json
import time
#http://bottlepy.org/docs/dev/tutorial.html#generating-content

name = "espmessenger"

@route('/')
def index():
    return static_file("index.html","./")

@route('/ping')
def webPing():
    return "1"

@get('/message/<message>')
def webMessage(message):
    time.sleep(1) #Simulate delay from ESP
    return message

@route('/name')
def webName():
    return name

@route('/style.css')
def style():
    return static_file("style.css","./")

@route('/toast.min.js')
def toast_script():
    return static_file("toast.min.js","./")

@route('/script.js')
def script():
    return static_file("script.js","./")

@route('/reboot')
def webReboot():
    return template("Rebooting")

run(host='0.0.0.0', port=8080)