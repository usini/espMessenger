/*
  ESPManager
  Connected Text Matrix for ESP8266
  By Sarrailh Rémi
  Licence : MIT
  https://github.com/maditnerd/espMessenger

  Library required:
  --> Arduino Thread
  --> MD_MAX72xx
  --> MD_Parola
  --> ArduinoJSON (v6)
*/
// Global message buffers shared by Serial and Scrolling functions
#include "matrix.h"
#include "settings.h"
#include "wifi.h"
#include "web.h"
#include "mqtt.h"
#include <Ticker.h>

Ticker matrixManager;
Ticker mqttManager;

void setup() {
  Serial.begin(74880);
  Serial.println("~~~~~~~~~~  Text Matrix ~~~~~~~~~~");
  initMatrix();
  matrixManager.attach_ms(1, matrixUpdate);
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
  matrixText(WiFi.localIP().toString());
}

void loop() {
  if(settings_state){ // Check if settings was performs correctly or do nothing
    server.handleClient(); //Manage Web Server
    //mqttUpdate();
    MDNS.update(); // Manage Bonjour Name (textmatrix.local by default)
  }
}
