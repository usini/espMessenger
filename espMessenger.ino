/*
  ESPManager
  Connected Text Matrix for ESP8266
  By Sarrailh Rémi
  Licence : MIT
  https://github.com/maditnerd/espMessenger

  Library required:
  --> PubSubClient (mqtt.h)
  --> MD_MAX72xx (matrix.h)
  --> MD_Parola (matrix.h)
  --> ArduinoJSON (v6) (settings.h / web.h)
*/

#include "matrix.h"
#include "settings.h"
#include "wifi.h"
#include "web.h"
#include "mqtt.h"

void setup() {
  Serial.begin(74880);
  Serial.println("~~~~~~~~~~  Text Matrix ~~~~~~~~~~");
  initMatrix();
  matrixText("Settings");

  // Check if Settings exists or create it
  if(checkSettings()){
    settings_state = readSettings(); // Read settings
  }

  if(settings_state){ // If settings was performs correctly
    matrixText("WiFi");
    connect();  // Connect to WiFi (or create Accesss Point)

  webStart(); // Start WebServer
  //mqttStart(); //Start MQTT Client
  }

  // Display IP Address on Matrix
  char ip[100];
  String ipString;
  if(ap){
  ipString = WiFi.localIP().toString();
  } else {
  ipString = WiFi.softAPIP().toString();
  }
  ipString.toCharArray(ip, sizeof(ip));
  matrixText(ip); //Todo hostname or IP for Access Point
}

void loop() {
  if(settings_state){ // Check if settings was performs correctly or do nothing
    webUpdate();
    //mqttUpdate();
    MDNS.update(); // Manage Bonjour Name (textmatrix.local by default)
  }
}
