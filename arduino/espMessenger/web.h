/* Web Manager for ESPMessenger
  By Rémi Sarrailh <@m4dnerd>
  Licence : MIT
*/

#include "web/web_index.h"
#include "web/web_script.h"
#include "web/web_style.h"
#include "web/web_range.h"
#include "web/web_toast.h"

#include <ESP8266WebServer.h> //Library for WebServer
#include <ESP8266SSDP.h> //Library for SSDP (Show ESP in Network on Windows)
#include <FS.h> //Library for FileSystem
#include <ESP8266mDNS.h>
#include <ESP8266NetBIOS.h>

File fsUploadFile;

ESP8266WebServer server(80); //Web Server on port 80

char* string2char(String command) {
  if (command.length() != 0) {
    char *p = const_cast<char*>(command.c_str());
    return p;
  } else {
    return "";
  }
}

unsigned char h2int(char c)
{
  if (c >= '0' && c <= '9') {
    return ((unsigned char)c - '0');
  }
  if (c >= 'a' && c <= 'f') {
    return ((unsigned char)c - 'a' + 10);
  }
  if (c >= 'A' && c <= 'F') {
    return ((unsigned char)c - 'A' + 10);
  }
  return (0);
}
String urldecode(String str)
{

  String encodedString = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (c == '+') {
      encodedString += ' ';
    } else if (c == '%') {
      i++;
      code0 = str.charAt(i);
      i++;
      code1 = str.charAt(i);
      c = (h2int(code0) << 4) | h2int(code1);
      encodedString += c;
    } else {

      encodedString += c;
    }

    yield();
  }

  return encodedString;
}

// https://stackoverflow.com/questions/9072320/split-string-into-string-array
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setHostname() {
  MDNS.begin(settings.name);
  MDNS.addService("http", "tcp", 80);
  NBNS.begin(settings.name);
}

// Basic Authentication
bool webAuth() {
  bool authOK = server.authenticate(settings.user, settings.pass);
  if (!authOK) {
    server.requestAuthentication();
  }
  return authOK;
}


/* Static EndPoints */

// index.html (/)
void webIndex() {
  if (webAuth()) {
    server.sendHeader("content-encoding", "gzip");
    server.send_P(200, "text/html", MES_INDEX, sizeof(MES_INDEX));
  }
}

// style.css
void webStyle() {
  if (webAuth()) {
    server.sendHeader("content-encoding", "gzip");
    server.send_P(200, "text/css", MES_STYLE, sizeof(MES_STYLE));
  }
}

void webRange() {
  if (webAuth()) {
    server.sendHeader("content-encoding", "gzip");
    server.send_P(200, "text/css", MES_RANGE, sizeof(MES_RANGE));
  }
}

// toast.min.js
void webToast() {
  if (webAuth()) {
    server.sendHeader("content-encoding", "gzip");
    server.send_P(200, "application/javascript", MES_TOAST, sizeof(MES_TOAST));
  }
}

// script.js
void webScript() {
  if (webAuth()) {
    server.sendHeader("content-encoding", "gzip");
    server.send_P(200, "application/javascript", MES_SCRIPT, sizeof(MES_SCRIPT));
  }
}

void webLang() {
  if (webAuth()) {
    server.sendHeader("content-encoding", "gzip");
    server.send_P(200, "application/javascript", MES_SCRIPT_LANG, sizeof(MES_SCRIPT_LANG));
  }
}

/* Dynamic EndPoints */

// Just send 1 to check if website is alive
void webPing() {
  server.send(200, "text/plain", "1");
}

// Restart ESP
void webReboot() {
  if (webAuth()) {
    ESP.restart();
  }
}

// Send Name
void webName() {
  if (webAuth()) {
    server.send(200, "text/plain", settings.name);
  }
}

void webRestHandler() {
  if (webAuth()) {
    Serial.println(server.uri());

    String url = server.uri();
    String command = getValue(url, '/', 1);
    Serial.print(TEXT_COMMAND);
    Serial.println(command);

    if (command == "message") {
      Serial.println(TEXT_WEB_MESSAGE);
      String message = getValue(url, '/', 2);
      Serial.println(message);
      matrixText(string2char(urldecode(message)), true);
      server.send(200, "text/plain", TEXT_MESSAGE_SENT);

    } else if (command == "messageAfter") {
      Serial.println(TEXT_WEB_MESSAGE);
      String message = getValue(url, '/', 2);
      Serial.println(message);
      matrixText(string2char(urldecode(message)), false);
      server.send(200, "text/plain", TEXT_MESSAGE_SENT);

    } else if (command == "speed") {
      String value = getValue(url, '/', 2);
      settings.speed = value.toInt();
      matrix->setSpeed(settings.speed);
      saveSettings();
      server.send(200, "text/plain", "Speed:" + value);

    } else if (command == "align") {
      String align = getValue(url, '/', 2);
      if (align == "LEFT") {
        settings.textPosition = PA_LEFT;
      } else if (align == "RIGHT") {
        settings.textPosition  = PA_RIGHT;
      } else if (align == "CENTER") {
        settings.textPosition = PA_CENTER;
      }
      matrix->displayClear();
      resetScrolling();
      saveSettings();
      server.send(200, "text/plain", "Align: " + align);

    } else if (command == "pause") {
      String value = getValue(url, '/', 2);
      settings.pause = value.toInt();
      matrix->setPause(settings.pause);
      saveSettings();
      server.send(200, "text/plain", "Pause:" + value);

    } else if (command == "intensity") {
      String value = getValue(url, '/', 2);
      int valInt = value.toInt();
      if (valInt < 0) {
        valInt = 0;
      } else if (valInt > 15) {
        valInt = 15;
      }
      settings.intensity = valInt;
      matrix->setIntensity(settings.intensity);
      saveSettings();
      server.send(200, "text/plain", "Intensity:" + value);
    } else if (command == "invert") {
      String value = getValue(url, '/', 2);
      if (value == "true") {
        settings.invert = true;
        matrix->setInvert(true);
      } else if (value == "false") {
        settings.invert = true;
        matrix->setInvert(false);
      }
      saveSettings();
      server.send(200, "text/plain", "Invert:" + value);
    }
    else if (command == "effect") {
      String entry = getValue(url, '/', 2);
      String effect = getValue(url, '/', 3);
      textEffect_t newEffect;
      if (effect == "NO_EFFECT") {
        newEffect = PA_NO_EFFECT;
      } else if (effect == "PRINT") {
        newEffect = PA_PRINT;
      } else if (effect == "SCROLL_UP") {
        newEffect = PA_SCROLL_UP;
      } else if (effect == "SCROLL_DOWN") {
        newEffect = PA_SCROLL_DOWN;
      } else if (effect == "SCROLL_LEFT") {
        newEffect = PA_SCROLL_LEFT;
      } else if (effect == "SCROLL_RIGHT") {
        newEffect = PA_SCROLL_RIGHT;
      } else if (effect == "SLICE") {
        newEffect = PA_SLICE;
      } else if (effect == "MESH") {
        newEffect = PA_MESH;
      } else if (effect == "FADE") {
        newEffect = PA_FADE;
      } else if (effect == "DISSOLVE") {
        newEffect = PA_DISSOLVE;
      } else if (effect == "BLINDS") {
        newEffect = PA_BLINDS;
      } else if (effect == "RANDOM") {
        newEffect = PA_RANDOM;
      } else if (effect == "WIPE") {
        newEffect = PA_WIPE;
      } else if (effect == "WIPE_CURSOR") {
        newEffect = PA_WIPE_CURSOR;
      } else if (effect == "SCAN_HORIZ") {
        newEffect = PA_SCAN_HORIZ;
      } else if (effect == "SCAN_HORIZX") {
        newEffect = PA_SCAN_HORIZX;
      } else if (effect == "SCAN_VERT") {
        newEffect = PA_SCAN_VERT;
      } else if (effect == "SCAN_VERTX") {
        newEffect = PA_SCAN_VERTX;
      } else if (effect == "OPENING") {
        newEffect = PA_OPENING;
      } else if (effect == "OPENING_CURSOR") {
        newEffect = PA_OPENING_CURSOR;
      } else if (effect == "CLOSING") {
        newEffect = PA_CLOSING;
      } else if (effect == "CLOSING_CURSOR") {
        newEffect = PA_CLOSING_CURSOR;
      } else if (effect == "SCROLL_UP_LEFT") {
        newEffect = PA_SCROLL_UP_LEFT;
      } else if (effect == "SCROLL_UP_RIGHT") {
        newEffect = PA_SCROLL_UP_RIGHT;
      } else if (effect == "SCROLL_DOWN_LEFT") {
        newEffect = PA_SCROLL_DOWN_LEFT;
      } else if (effect == "SCROLL_DOWN_RIGHT") {
        newEffect = PA_SCROLL_DOWN_RIGHT;
      } else if (effect == "GROW_UP") {
        newEffect = PA_GROW_UP;
      } else if (effect == "GROW_DOWN") {
        newEffect = PA_GROW_DOWN;
      }
      if (entry == "in") {
        settings.effectIn = newEffect;
      } else if (entry == "out") {
        settings.effectOut = newEffect;
      }
      matrix->displayClear();
      resetScrolling();
      saveSettings();
      server.send(200, "text/plain", entry + ": " + effect);
    }
    server.send(404, "text/plain", TEXT_NOT_FOUNDED);
  }
}

void webSettings() {
  if (webAuth()) {
    File configFile = SPIFFS.open(settings_file, "r");
    server.streamFile(configFile, "text/plain");
    configFile.close();
  }
}

void webSave() {
  if (webAuth()) {
    saveSettings();
    server.send(200, "text/plain", "Settings saved");
  }
}

// Manage update from SPIFFS
void updateESP() {
  Serial.println(TEXT_OPEN_UPDATE);
  File file = SPIFFS.open("/update.bin", "r");
  uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;

  if (!Update.begin(maxSketchSpace, U_FLASH)) { //start with max available size
    Update.printError(Serial);
    Serial.println(TEXT_ERROR_UPDATE);
  }
  Serial.println(TEXT_BEGIN_UPDATE);
  while (file.available()) {
    uint8_t ibuffer[128];
    file.read((uint8_t *)ibuffer, 128);
    //Serial.println((char *)ibuffer);
    Update.write(ibuffer, sizeof(ibuffer));
  }
  if (Update.end(true)) { //true to set the size to the current progress
    Serial.println(TEXT_UPDATE_SUCCESS);
  } else {
    Update.printError(Serial);
  }
  file.close();
  Serial.println(TEXT_UPDATE_END);
  ESP.restart();
}

// Manage Upload of binary to SPIFFS
void webUpload() {
  if (webAuth()) {
    HTTPUpload& upload = server.upload();

    if (upload.status == UPLOAD_FILE_START) {
      String filename = "/update.bin";
      fsUploadFile = SPIFFS.open(filename, "w");
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      //Serial.print("handleFileUpload Data: ");
      //Serial.println(upload.currentSize);
      if (fsUploadFile) {
        fsUploadFile.write(upload.buf, upload.currentSize);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (fsUploadFile) {
        fsUploadFile.close();
        Serial.println(TEXT_UPDATE_FINISHED);
        updateESP();
      } else {
        Serial.println(TEXT_UPDATE_FAILED);
      }
    }
  }
}

//Generate endpoints
void webStart() {
  Serial.println();

  server.on("/", webIndex); //Display main Page
  //server.on(" / message",webMessage); //Send Message (json)
  server.on("/reboot", webReboot); //Reboot ESP
  server.on("/ping", webPing);
  server.on("/name", webName);
  server.on("/style.css", webStyle);
  server.on("/range.css", webRange);
  server.on("/toast.min.js", webToast);
  server.on("/script.js", webScript);
  server.on("/lang.js", webLang);
  server.on("/settings", webSettings);
  server.on("/save", webSave);
  server.on("/update", HTTP_POST, []() {
    server.send(200, "text/plain", "");
  }, webUpload);
  server.on("/description.xml", HTTP_GET, []() {
    SSDP.schema(server.client());
  });
  server.onNotFound(webRestHandler); //Handle Rest Message

  server.begin();

  //Simple Service Discovery Protocol : Display ESP in Windows Network Tab
  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(80);
  SSDP.setName(settings.name);
  SSDP.setDeviceType("upnp: rootdevice");
  SSDP.setSerialNumber("000000000001");
  SSDP.setURL(" / ");
  SSDP.setModelName("ESP Messenger");
  SSDP.setModelNumber("0000000000001");
  SSDP.setModelURL("https://github.com/usini/espMessenger");
  SSDP.setManufacturer("Usini");
  SSDP.setManufacturerURL("http://usini.eu");
  SSDP.begin();
}

// Manage Web Server in Loop
void webUpdate() {
  server.handleClient(); //Manage Web Server
}
