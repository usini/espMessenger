#include "web/web_index.h"
#include "web/web_settings.h"
#include <ESP8266WebServer.h>
#include <ESP8266WebServerSecure.h>
#include <FS.h>
ESP8266WebServer server(80); //Web Server on port 80
BearSSL::ESP8266WebServerSecure server_ssl(443);

const char *web_cert_file = "/webcert.pem";
const char *web_key_file = "/webkey.pem";
bool isWebSSL = false;

// Display index.html (/)
void webIndex() {
  server.send(200, "text/html", HTTP_INDEX);
}

// Display settings.html (/settings)
void webSettings() {
  server.send(200, "text/html", HTTP_SETTINGS);
}

// Copy body request into settings.json (/save)
void webSaveSettings() {
  if(server.hasArg("plain") == false){
    server.send(200, "text/html", "no message");
  } else {
    File file = SPIFFS.open(settings_file,"w");
    file.println(server.arg("plain"));
    file.close();
    server.send(200, "text/json", server.arg("plain"));
  }
}

// Load settings.json and display it
void webLoadSettings() {
  File file = SPIFFS.open(settings_file, "r");
  String settings_json;
  while (file.available()){
    settings_json += char(file.read());
  }
  file.close();
  server.send(200, "text/json", settings_json);
}
/*
void webUploadFile(){
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) {
      filename = "/" + filename;
    }
    fsUploadFile = SPIFFS.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    //DBG_OUTPUT_PORT.print("handleFileUpload Data: "); DBG_OUTPUT_PORT.println(upload.currentSize);
    if (fsUploadFile) {
      fsUploadFile.write(upload.buf, upload.currentSize);
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {
      fsUploadFile.close();
    }
}
*/
void webPing(){
  server.send(200, "text/plain", "1");
}

// Restart ESP
void webReboot() {
  ESP.restart();
}

void webName(){
  server.send(200, "text/plain", name);
}



// Display Message on Led Matrix
void webMessage() {
  if(server.hasArg("plain") == false){
    server.send(200, "text/html", "no message");
  } else {
    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc, server.arg("plain"));
    if (error) {
      Serial.println("[MESSAGE] : Deserialize Json failed");
      //String error_string = "{\"error\": \"" + String(error) + "\"}";
      server.send(200, "text/json", "error");
      return;
    } else {
      Serial.print("--> WEB : ");
      String serialMessage = doc["message"].as<String>();
      Serial.println(serialMessage);
      matrixText((char *)doc["message"].as<char*>());
      server.send(200, "text/json", doc["message"]);
    }
  }
}

void webRedirect(){
  server.send(200,"Redirection");
}

void webCheckCerts(){
    File file = SPIFFS.open(web_cert_file, "r");
    String web_cert;
    if(file){
      String web_cert;
      while (file.available()){
        web_cert += char(file.read());
      }
      file.close();
      isWebSSL = true;
    }
    if(isWebSSL){
      file = SPIFFS.open(web_key_file, "r");
      String web_key;
      while (file.available()){
        web_key += char(file.read());
      }
      file.close();
      isWebSSL = true;
    } else {
      isWebSSL = false;
    }
}

void webUpdate(){
   server.handleClient(); //Manage Web Server
  if(isWebSSL){
    server_ssl.handleClient();
  }
}

//Generate endpoints
void webStart() {
    Serial.println("... [WEB] Starting web server .. ");
    webCheckCerts();
    if(isWebSSL){
      server_ssl.on("/",webIndex); //Display main Page
      server_ssl.on("/message",webMessage); //Send Message (json)
      server_ssl.on("/settings",webSettings); //Display Settings page
      server_ssl.on("/save",webSaveSettings); //Send Settings to SPIFFS (json)
      server_ssl.on("/load",webLoadSettings); //Load Settings from SPIFFS (json)
      server_ssl.on("/reboot",webReboot); //Reboot ESP
      server_ssl.on("/ping", webPing);
      server_ssl.on("/name",webName);
      server.on("/description.xml", HTTP_GET, []() {
        SSDP.schema(server.client());
      });
      server.on("/",webRedirect);
      server.begin();
      server_ssl.begin();

    } else {
      server.on("/",webIndex); //Display main Page
      server.on("/message",webMessage); //Send Message (json)
      server.on("/settings",webSettings); //Display Settings page
      server.on("/save",webSaveSettings); //Send Settings to SPIFFS (json)
      server.on("/load",webLoadSettings); //Load Settings from SPIFFS (json)
      server.on("/reboot",webReboot); //Reboot ESP
      server.on("/ping", webPing);
      server.on("/name",webName);
      server.on("/description.xml", HTTP_GET, []() {
        SSDP.schema(server.client());
      });
      server.begin();
    }
    //Simple Service Discovery Protocol : Display ESP in Windows Network Tab
    SSDP.setSchemaURL("description.xml");
    SSDP.setHTTPPort(80);
    SSDP.setName(name);
    SSDP.setDeviceType("upnp:rootdevice");
    SSDP.setSerialNumber("000000000001");
    SSDP.setURL("/");
    SSDP.setModelName("ESP Messenger");
    SSDP.setModelNumber("0000000000001");
    SSDP.setModelURL("https://github.com/maditnerd/espMessenger");
    SSDP.setManufacturer("Usini");
    SSDP.setManufacturerURL("http://usini.eu");
    SSDP.begin();
}
