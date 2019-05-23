#include <ArduinoJson.h>
#include "FS.h"

const String DEFAULT_SSID1 = "";
const String DEFAULT_PASS1 = "";
const String DEFAULT_SSID2 = "";
const String DEFAULT_PASS2 = "";
const String DEFAULT_SSID3 = "";
const String DEFAULT_PASS3 = "";
const String DEFAULT_SSID4 = "";
const String DEFAULT_PASS4 = "";
const String DEFAULT_NAME = "textmatrix";

const String DEFAULT_AP_SSID = "textmatrix";
const String DEFAULT_AP_PASS = "textmatrix";
const String DEFAULT_WEB_USER = "textmatrix";
const String DEFAULT_WEB_PASS = "textmatrix";

const char* ssid1;
const char* pass1;
const char* ssid2;
const char* pass2;
const char* ssid3;
const char* pass3;
const char* ssid4;
const char* pass4;
const char* ap_ssid;
const char* ap_pass;
const char* name;

String web_user;
String web_pass;

const char *settings_file = "/settings.json";
bool settings_state = false;

//Initialize Settings with Default Value
bool initSettings() {
  Serial.println("... [SETTINGS] Generating settings");
  StaticJsonDocument<2048> settings;
  settings["ssid1"] = DEFAULT_SSID1;
  settings["pass1"] = DEFAULT_PASS1;
  settings["ssid2"] = DEFAULT_SSID2;
  settings["pass2"] = DEFAULT_PASS2;
  settings["ssid3"] = DEFAULT_SSID3;
  settings["pass3"] = DEFAULT_PASS3;
  settings["ssid4"] = DEFAULT_SSID4;
  settings["pass4"] = DEFAULT_PASS4;
  settings["ap_ssid"] = DEFAULT_AP_SSID;
  settings["ap_pass"] = DEFAULT_AP_PASS;
  settings["web_user"] = DEFAULT_WEB_USER;
  settings["web_pass"] = DEFAULT_WEB_PASS;
  settings["name"] = DEFAULT_NAME;

  File file = SPIFFS.open(settings_file, "w");
  if(!file){
    return false;
  }

  if (serializeJson(settings, file) == 0){
    Serial.println("... [SETTINGS] Failed to write JSON to settings file [FAIL]");
  }
  file.close();
  return true;
}

bool checkSettings(){
  //Open SPIFFS, format it if it needed to be format (can take some time)
  if (!SPIFFS.begin()){
    Serial.println("... [SETTINGS] Can't open flash memory [FAIL]");
    return false;
  }

  //Check if Json file exists
  File file = SPIFFS.open(settings_file, "r");
  if(!file){
    Serial.println("... [SETTINGS] Settings doesn't exists");
    return initSettings(); //Initialize settings
  } else {
    file.close(); //Settings exists close it.
    return true;
  }
  //Shouldn't happens
  return false;
}

//Read Settings file and setup variables
bool readSettings(){
  Serial.println("... [SETTINGS] Opening settings.json ...");
  File file = SPIFFS.open(settings_file, "r");
  StaticJsonDocument<2048> doc;
  DeserializationError error = deserializeJson(doc, file);
  JsonObject settings = doc.as<JsonObject>();

  //If JSON cannot be deserialize, we reset the settings file
  if(error){
    Serial.println("... [FAIL] Settings corrupted reinitialisation required");
    Serial.println(error.c_str());
    if(initSettings()){
      return readSettings();
    } else {
      return false;
    }
  }

  ssid1 = settings["ssid1"].as<char*>();
  pass1 = settings["pass1"].as<char*>();
  ssid2 = settings["ssid2"].as<char*>();
  pass2 = settings["pass2"].as<char*>();
  ssid3 = settings["ssid3"].as<char*>();
  pass3 = settings["pass3"].as<char*>();
  ssid4 = settings["ssid4"].as<char*>();
  ap_ssid = settings["ap_ssid"].as<char*>();
  ap_pass = settings["ap_pass"].as<char*>();
  name = settings["name"].as<char*>();
  web_user = settings["web_user"].as<String>();
  web_pass = settings["web_pass"].as<String>();
  file.close();
  return true;
}