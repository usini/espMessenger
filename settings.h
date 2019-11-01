/* WiFi Manager Code for ESPMessenger
  By Rémi Sarrailh <@m4dnerd>
  Licence : MIT
*/

#include <WiFiManager.h>
#include <ArduinoJson.h>
#include "FS.h"

const char *settings_file = "/settings.json";
bool shouldSaveSettings = false;

// Based on this example : https://github.com/bblanchon/ArduinoJson/blob/6.x/examples/JsonConfigFile/JsonConfigFile.ino
struct Settings {
  char name[128];
  char user[128];
  char pass[128];
  int devices;
};

Settings settings;
WiFiManager wifiManager;

//callback notifying us of the need to save settings
void saveSettingsCallback () {
  shouldSaveSettings = true;
}

void startWiFiManager() {
  if(SPIFFS.begin()){
    Serial.println(TEXT_MOUNT_SPIFFS);
  }
  if (SPIFFS.exists(settings_file)){
    Serial.println();
    File configFile = SPIFFS.open(settings_file,"r");
    if (configFile){
      Serial.println(TEXT_OPEN_CONFIG_FILE);
      size_t size = configFile.size();
      std::unique_ptr<char[]> buf(new char[size]);
      configFile.readBytes(buf.get(), size);
      DynamicJsonDocument json(1024);
      auto deserializeError = deserializeJson(json, buf.get());
      //serializeJson(json, Serial);
      if(! deserializeError ) {
        Serial.println(TEXT_PARSING_JSON);
        strcpy(settings.name, json["name"]);
        strcpy(settings.user, json["user"]);
        strcpy(settings.pass, json["pass"]);
        settings.devices = json["devices"].as<unsigned int>();
        Serial.print(TEXT_MATRIX);
        Serial.println(settings.devices);
      } else {
        Serial.println(TEXT_LOAD_CONFIG_FILE);
      }
      configFile.close();
    }
  } else {
    Serial.println(TEXT_MOUNT_FS);
  }


  WiFiManagerParameter custom_name("name", TEXT_NAME, "messenger", 128);
  WiFiManagerParameter custom_user("user", TEXT_USER, "admin", 128);
  WiFiManagerParameter custom_pass("pass", TEXT_PASSWORD, "admin", 128);
  WiFiManagerParameter custom_devices("devices", TEXT_MATRIX, "4", 2);

  
  WiFiManager wifiManager;
  wifiManager.setSaveConfigCallback(saveSettingsCallback);

  wifiManager.addParameter(&custom_name);
  wifiManager.addParameter(&custom_user);
  wifiManager.addParameter(&custom_pass);
  wifiManager.addParameter(&custom_devices);

  if(!wifiManager.autoConnect("espMessenger","espMessenger")){
    Serial.println(TEXT_TIMEOUT);
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  Serial.println(TEXT_CONNECTED);

  if(shouldSaveSettings){
    strcpy(settings.name, custom_name.getValue());
    strcpy(settings.user, custom_user.getValue());
    strcpy(settings.pass, custom_pass.getValue());
    settings.devices = atoi(custom_devices.getValue());
    Serial.println(TEXT_SAVING_CONFIG);
    DynamicJsonDocument json(1024);
    json["name"] = settings.name;
    json["user"] = settings.user;
    json["pass"] = settings.pass;
    json["devices"] = settings.devices;

    File configFile = SPIFFS.open(settings_file, "w");
    if(!configFile){
      Serial.println(TEXT_FAILED_OPEN_CONFIG);
    }
    //serializeJson(json, Serial);
    serializeJson(json, configFile);
    configFile.close();
  }
}
