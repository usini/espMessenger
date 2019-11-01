/*
  ESPMessenger
  Connected Text Matrix for ESP8266
  By µsini
  Licence : MIT
  https://github.com/usini/espMessenger

  Library required:
  --> MD_MAX72xx (matrix.h)
  --> MD_Parola (matrix.h)
  --> ArduinoJSON (v6) (settings.h / web.h)
  --> WiFiManager (settings.h)
*/

//#include "lang/en.h"
#include "lang/fr.h"

#include "settings.h"
#include "matrix.h"
#include "web.h"
#include "banner.h"

const int LED = D4;

void setup() {
  Serial.begin(74880);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  introBanner();

  startWiFiManager();

  initMatrix();
  matrixText((char*)settings.name);
  digitalWrite(LED, HIGH);

  setHostname();
  webStart(); // Start WebServer
  connectionBanner();
}

void loop() {
  webUpdate();
  MDNS.update(); // Manage Bonjour Name
}
