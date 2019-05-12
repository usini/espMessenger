#include "web/web_index.h"
#include "web/web_settings.h"

ESP8266WebServer server(80); //Web Server on port 80

// Display index.html (/)
void webIndex() {
  server.send(200, "text/html", HTTP_INDEX);
}

// Display settings.html (/settings)
void webSettings() {
  server.send(200, "text/html", HTTP_SETTINGS);
}

// If copy body request into settings.json (/save)
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

// Restart ESP
void webReboot() {
  ESP.restart();
}

// Display Message on Led Matrix
void webMessage() {
  if(server.hasArg("plain") == false){
    server.send(200, "text/html", "no message");
  } else {
    StaticJsonDocument<2048> doc;
    DeserializationError error = deserializeJson(doc, server.arg("plain"));
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      //String error_string = "{\"error\": \"" + String(error) + "\"}";
      server.send(200, "text/json", "error");
      return;
    }

    matrixText(doc["message"].as<String>());
    server.send(200, "text/json", doc["message"]);
  }
}

//Generate endpoints
void webStart() {
    Serial.println("... [WEB] Starting web server .. ");
    server.on("/",webIndex);
    server.on("/message",webMessage);
    server.on("/settings",webSettings);
    server.on("/save",webSaveSettings);
    server.on("/load",webLoadSettings);
    server.on("/reboot",webReboot);
    server.on("/description.xml", HTTP_GET, []() {
      SSDP.schema(server.client());
    });
    server.begin();

    SSDP.setSchemaURL("description.xml");
    SSDP.setHTTPPort(80);
    SSDP.setName(name);
    SSDP.setDeviceType("upnp:rootdevice");
    SSDP.setSerialNumber("000000000001");
    SSDP.setURL("/");
    SSDP.setModelName("ESP Messenger");
    SSDP.setModelNumber("0000000000001");
    SSDP.setModelURL("http://usini.eu/espmessenger");
    SSDP.setManufacturer("Usini");
    SSDP.setManufacturerURL("http://usini.eu");
    SSDP.begin();
}