/* Web Manager for ESPMessenger
  By Rémi Sarrailh <@m4dnerd>
  Licence : MIT
*/

// Static Web File (en)
/*
#include "web/web_index.h"
#include "web/web_script.h"
*/
// Static Web File (fr)
#include "web/web_index_fr.h"
#include "web/web_script_fr.h"

#include "web/web_style.h"
#include "web/web_toast.h"

#include <ESP8266WebServer.h> //Library for WebServer
#include <ESP8266SSDP.h> //Library for SSDP (Show ESP in Network on Windows)
#include <FS.h> //Library for FileSystem
#include <ESP8266mDNS.h>
#include <ESP8266NetBIOS.h>

File fsUploadFile;

ESP8266WebServer server(80); //Web Server on port 80

char* string2char(String command){
    if(command.length()!=0){
        char *p = const_cast<char*>(command.c_str());
        return p;
    } else{
      return "";
    }
}

unsigned char h2int(char c)
{
    if (c >= '0' && c <='9'){
        return((unsigned char)c - '0');
    }
    if (c >= 'a' && c <='f'){
        return((unsigned char)c - 'a' + 10);
    }
    if (c >= 'A' && c <='F'){
        return((unsigned char)c - 'A' + 10);
    }
    return(0);
}
String urldecode(String str)
{
    
    String encodedString="";
    char c;
    char code0;
    char code1;
    for (int i =0; i < str.length(); i++){
        c=str.charAt(i);
      if (c == '+'){
        encodedString+=' ';  
      }else if (c == '%') {
        i++;
        code0=str.charAt(i);
        i++;
        code1=str.charAt(i);
        c = (h2int(code0) << 4) | h2int(code1);
        encodedString+=c;
      } else{
        
        encodedString+=c;  
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
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setHostname() {
    MDNS.begin(settings.name);
    MDNS.addService("http","tcp", 80);
    NBNS.begin(settings.name);
}

// Basic Authentication
void webAuth() {
    if (!server.authenticate(settings.user, settings.pass)) {
      return server.requestAuthentication();
    }
}


/* Static EndPoints */

// index.html (/)
void webIndex() {
  webAuth();
  server.sendHeader("content-encoding","gzip");
  server.send_P(200, "text/html", MES_INDEX, sizeof(MES_INDEX));
}

// style.css
void webStyle() {
  webAuth();
  server.sendHeader("content-encoding","gzip");
  server.send_P(200, "text/html", MES_STYLE, sizeof(MES_STYLE));
}

// toast.min.js
void webToast() {
  webAuth();
  server.sendHeader("content-encoding","gzip");
  server.send_P(200, "text/html", MES_TOAST, sizeof(MES_TOAST));
}

// script.js
void webScript() {
  webAuth();
  server.sendHeader("content-encoding","gzip");
  server.send_P(200, "text/html", MES_SCRIPT, sizeof(MES_SCRIPT));
}

/* Dynamic EndPoints */

// Just send 1 to check if website is alive
void webPing() {
  server.send(200, "text/plain", "1");
}

// Restart ESP
void webReboot() {
  webAuth();
  ESP.restart();
}

// Send Name
void webName() {
  webAuth();
  server.send(200, "text/plain", settings.name);
}

void webRestHandler(){
  webAuth();
  Serial.println(server.uri());

  String url = server.uri();
  String command = getValue(url,'/',1);
  Serial.print(TEXT_COMMAND);
  Serial.println(command);
  if(command == "message"){
    Serial.println(TEXT_WEB_MESSAGE);
    String message = getValue(url, '/', 2);
    Serial.println(message);
    matrixText(string2char(urldecode(message)));
    server.send(200, "text/plain", TEXT_MESSAGE_SENT);
  }
  server.send(404, "text/plain", TEXT_NOT_FOUNDED);
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
  webAuth();
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

//Generate endpoints
void webStart() {
    Serial.println();

    server.on("/",webIndex); //Display main Page
    //server.on("/message",webMessage); //Send Message (json)
    server.on("/reboot",webReboot); //Reboot ESP
    server.on("/ping", webPing);
    server.on("/name",webName);
    server.on("/style.css",webStyle);
    server.on("/toast.min.js",webToast);
    server.on("/script.js",webScript);
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
    SSDP.setDeviceType("upnp:rootdevice");
    SSDP.setSerialNumber("000000000001");
    SSDP.setURL("/");
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

