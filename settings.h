/* JSON Settings Manager
  By Rémi Sarrailh <@m4dnerd>
  Licence : MIT
*/

#include <ArduinoJson.h>
#include "FS.h"
#include "settings_values.h"
bool settings_state = false;

//Initialize Settings with Default Value
bool initSettings() {
  Serial.println("... [SETTINGS] Generating settings");
  StaticJsonDocument<JSON_BUFFER_SIZE> settings;
  settings = getInitSettings(settings); //get default value from settings_values.h

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

// Start SPIFFS and Check if Settings exists
bool checkSettings(){
  //Open SPIFFS, format it if it needed to be format (can take some time)
  if (!SPIFFS.begin()){
    Serial.println("... [SETTINGS] Can't open flash memory [FAIL]");
    return false;
  }

  //Check if Json file exists
  bool settings_exists = SPIFFS.exists(settings_file);
  if(!settings_exists){
    Serial.println("... [SETTINGS] Settings doesn't exists");
    return initSettings(); //Initialize settings
  } else {
    return true;
  }
  //Shouldn't happens
  return false;
}

//Read Settings file and setup variables
bool readSettings(){
  Serial.println("... [SETTINGS] Opening settings.json ...");
  File file = SPIFFS.open(settings_file, "r");
  StaticJsonDocument<JSON_BUFFER_SIZE> doc;
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

  setSettings(settings); // Set value from settings_value.h
  file.close();
  return true;
}