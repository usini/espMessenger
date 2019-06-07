/* Web Manager
  By Rémi Sarrailh <@m4dnerd>
  Licence : MIT
*/

// Static Web File
#include "web/web_index.h"
#include "web/web_settings.h"
#include "web/web_script.h"
#include "web/web_style.h"
#include "web/web_toast.h"

#include <ESP8266WebServer.h> //Library for WebServer
#include <ESP8266WebServerSecure.h> //Library for WebServer (SSL)
#include <ESP8266SSDP.h> //Library for SSDP (Show ESP in Network on Windows)
#include <FS.h> //Library for FileSystem

//TODO Captive Portal

File fsUploadFile;

ESP8266WebServer server(80); //Web Server on port 80
BearSSL::ESP8266WebServerSecure server_ssl(443);

const char *web_cert_file = "/webcert.pem";
const char *web_key_file = "/webkey.pem";
bool isWebSSL = false;

// Basic Authentication
void webAuth(){
  if(web_auth_enabled){
    if(isWebSSL){
      if (!server_ssl.authenticate(web_user, web_pass)) {
        Serial.println(web_user);
        Serial.println(web_pass);
        return server_ssl.requestAuthentication();
      }
    } else {
      if (!server.authenticate(web_user, web_pass)) {
        Serial.println(web_user);
        Serial.println(web_pass);
        return server.requestAuthentication();
      }
    }
  }
}

/* Static EndPoints */

// index.html (/)
void webIndex() {
  webAuth();
  if(isWebSSL){
    Serial.println("Sending SSL Index");
    server_ssl.sendHeader("content-encoding","gzip");
    server_ssl.send_P(200, "text/html", HTTP_INDEX, sizeof(HTTP_INDEX));
  } else {
        server.sendHeader("content-encoding","gzip");
    server.send_P(200, "text/html", HTTP_INDEX, sizeof(HTTP_INDEX));
  }
}

// settings.html (/settings)
void webSettings() {
  webAuth();
  if(isWebSSL){
  server_ssl.sendHeader("content-encoding","gzip");
  server_ssl.send_P(200, "text/html", HTTP_SETTINGS, sizeof(HTTP_SETTINGS));
  } else {
    server.sendHeader("content-encoding","gzip");
    server.send_P(200, "text/html", HTTP_SETTINGS, sizeof(HTTP_SETTINGS));
  }
}

// style.css
void webStyle(){
  webAuth();
  if(isWebSSL){
  server_ssl.sendHeader("content-encoding","gzip");
  server_ssl.send_P(200, "text/html", HTTP_STYLE, sizeof(HTTP_STYLE));
  } else {
    server.sendHeader("content-encoding","gzip");
    server.send_P(200, "text/html", HTTP_STYLE, sizeof(HTTP_STYLE));
  }
}

// toast.min.js
void webToast(){
  webAuth();
  if(isWebSSL){
  server_ssl.sendHeader("content-encoding","gzip");
  server_ssl.send_P(200, "text/html", HTTP_TOAST, sizeof(HTTP_TOAST));
  } else {
    server.sendHeader("content-encoding","gzip");
    server.send_P(200, "text/html", HTTP_TOAST, sizeof(HTTP_TOAST));
  }
}

// script.js
void webScript(){
  webAuth();
  if(isWebSSL){
  server_ssl.sendHeader("content-encoding","gzip");
  server_ssl.send_P(200, "text/html", HTTP_SCRIPT, sizeof(HTTP_SCRIPT));
  } else {
    server.sendHeader("content-encoding","gzip");
    server.send_P(200, "text/html", HTTP_SCRIPT, sizeof(HTTP_SCRIPT));
  }
}

/* Dynamic EndPoints */

// Copy body request into settings.json (/save)
void webSaveSettings() {
  webAuth();
  if(server.hasArg("plain") == false){
    if(isWebSSL){
      server.send(200, "text/html", "no message");
    } else {
      server_ssl.send(200, "text/html", "no message");
    }
  } else {
    File file = SPIFFS.open(settings_file,"w");
    file.println(server.arg("plain"));
    file.close();
    if(isWebSSL){
      server_ssl.send(200, "text/json", server.arg("plain"));
    } else {
      server.send(200, "text/json", server.arg("plain"));
    }
  }
}

// Load settings.json and display it
void webLoadSettings() {
  webAuth();
  File file = SPIFFS.open(settings_file, "r");
  String settings_json;
  while (file.available()){
    settings_json += char(file.read());
  }
  file.close();
  if(isWebSSL){
    server_ssl.send(200, "text/json", settings_json);
  } else {
    server.send(200, "text/json", settings_json);
  }
}

// Just send 1 to check if website is alive
void webPing(){
  if(isWebSSL){
    server_ssl.send(200, "text/plain", "1");
  } else {
    server.send(200, "text/plain", "1");
  }
}

// Restart ESP
void webReboot() {
  webAuth();
  ESP.restart();
}

// Send Name
void webName(){
  webAuth();
  if(isWebSSL){
    server_ssl.send(200, "text/plain", name);
  } else {
    server.send(200, "text/plain", name);
  }
}

// Display Message on Led Matrix
void webMessage() {
  webAuth();
  bool isMessage = false;

  // Check if message exists
  if(isWebSSL){ // SSL
    if(server_ssl.hasArg("plain") == false){
      server_ssl.send(200, "text/html", "no message");
    } else {
      isMessage = true;
    }
  } else { // NON SSL
    if(server.hasArg("plain") == false){
      server.send(200, "text/html", "no message");
    } else {
      isMessage = true;
    }
  }

  if(isMessage){
    StaticJsonDocument<2048> doc;
    DeserializationError error;
    if(isWebSSL){
      error = deserializeJson(doc, server_ssl.arg("plain"));
    } else {
      error = deserializeJson(doc, server.arg("plain"));
    }
    if (error) {
      Serial.println("[MESSAGE] : Deserialize Json failed");
      //String error_string = "{\"error\": \"" + String(error) + "\"}";
      if(isWebSSL){
        server_ssl.send(200, "text/json", "error");
      } else {
        server.send(200, "text/json", "error");
      }
      return;
    } else {
      Serial.print("--> WEB : ");
      String serialMessage = doc["message"].as<String>();
      Serial.println(serialMessage);
      matrixText((char *)doc["message"].as<char*>());
      if(isWebSSL){
        server_ssl.send(200, "text/json", doc["message"]);
      } else {
        server.send(200, "text/json", doc["message"]);
      }
    }
  }
}

// Redirect http to https
void webRedirect(){
    if(ap){
      server.sendHeader("Location", String("https://") + WiFi.localIP().toString(), true);
    } else {
      server.sendHeader("Location", String("https://") + WiFi.softAPIP().toString(), true);
    }
    server.send(302, "text/plain", "");   // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.client().stop(); // Stop is needed because we sent no content length
}

// Manage Certificates for SSL
void webCheckCerts(){
  if(web_ssl_enabled){
    bool cert_exists = SPIFFS.exists(web_cert_file);

    Serial.println("AFTER SPIFFS OPEN");
    Serial.println(web_user);
    Serial.println(web_pass);
    // If web cert exists
    if(cert_exists){
      File file = SPIFFS.open(web_cert_file, "r");
      String web_cert;
      String web_key;

      Serial.println("... [CERTS] Reading web cert ...");

      while (file.available()){
        web_cert += char(file.read());
      }
      file.close();

      //If web key exists
      Serial.println("... [CERTS] Reading web key ...");
      bool key_exists = SPIFFS.exists(web_key_file);
      if(key_exists){
        file = SPIFFS.open(web_key_file, "r");
        while (file.available()){
          web_key += char(file.read());
        }
        // Copy String to const char*
        server_ssl.setRSACert(new BearSSL::X509List(web_cert.c_str()), new BearSSL::PrivateKey(web_key.c_str()));
        isWebSSL = true;
      } else {
        isWebSSL = false; //No key so no SSL
      }
      file.close();
    } else {
      isWebSSL = false; // No certs so no SSL
    }
  } else {
    isWebSSL = false;
  }
}

// Manage update from SPIFFS
void updateESP(){
    Serial.println("Open update.bin");
    File file = SPIFFS.open("/update.bin", "r");
    Serial.println("File open");
    uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;

    if (!Update.begin(maxSketchSpace, U_FLASH)) { //start with max available size
      Update.printError(Serial);
      Serial.println("ERROR");
    }
    Serial.println("Update begin...");
    while (file.available()) {
      uint8_t ibuffer[128];
      file.read((uint8_t *)ibuffer, 128);
      //Serial.println((char *)ibuffer);
      Update.write(ibuffer, sizeof(ibuffer));
    }
    if (Update.end(true)) { //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n");
    } else {
          Update.printError(Serial);
    }
    file.close();
    Serial.println("Update ended...");
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
      Serial.println("Upload finished updating started...");
      updateESP();
    } else {
      Serial.println("Upload failed ...");
    }
  }
}

//Generate endpoints
void webStart() {
    Serial.println("... [WEB] Starting web server .. ");
    webCheckCerts();

    if(isWebSSL){
      Serial.println("... [WEB] Secured website enable ...");
      server_ssl.on("/",webIndex); //Display main Page
      server_ssl.on("/message",webMessage); //Send Message (json)
      server_ssl.on("/settings",webSettings); //Display Settings page
      server_ssl.on("/save",webSaveSettings); //Send Settings to SPIFFS (json)
      server_ssl.on("/load",webLoadSettings); //Load Settings from SPIFFS (json)
      server_ssl.on("/reboot",webReboot); //Reboot ESP
      server_ssl.on("/ping", webPing);
      server_ssl.on("/name",webName);
      server_ssl.on("/style.css",webStyle);
      server_ssl.on("/toast.min.js",webToast);
      server_ssl.on("/script.js",webScript);
      server_ssl.on("/update", HTTP_POST, []() {
      server_ssl.send(200, "text/plain", "");
      }, webUpload);

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
      server.on("/style.css",webStyle);
      server.on("/toast.min.js",webToast);
      server.on("/script.js",webScript);
      server.on("/update", HTTP_POST, []() {
        server.send(200, "text/plain", "");
      }, webUpload);
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

// Manage Web Server in Loop
void webUpdate(){
   server.handleClient(); //Manage Web Server
  if(isWebSSL){
    server_ssl.handleClient();
  }
}
