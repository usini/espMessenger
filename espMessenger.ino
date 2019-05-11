/*
  ESPManager
  Connected Text Matrix for ESP8266
  By Sarrailh Rémi
  Licence : MIT
  https://github.com/maditnerd/espMessenger

  Library required:
  --> Arduino Thread
  --> MD_MAX72xx
  --> ArduinoJSON (v6)
*/
// Global message buffers shared by Serial and Scrolling functions
#include "matrix.h"
#include "settings.h"
#include "wifi.h"
#include "web.h"

void setup() {
  Serial.begin(74880);
  Serial.println("~~~~~~~~~~  Text Matrix ~~~~~~~~~~");
  initMatrix();

  //Check if Settings exists or create it
  if(checkSettings()){
    settings_state = readSettings(); //Read settings
  }

  if(settings_state){ //If settings was performs correctly
    connect();  //Connect to WiFi (or create Accesss Point)
    webStart(); //Start WebServer
  }

  strcpy(curMessage, WiFi.localIP().toString().c_str());
}

void loop() {
  if(settings_state){ // Check if settings was performs correctly or do nothing
    server.handleClient(); //Manage Web Server
    MDNS.update(); // Manage Bonjour Name (textmatrix.local by default)

    if(matrixThread.shouldRun()){
      matrixThread.run(); //Manage Message Routine
    }
  }
}
