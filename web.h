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

//Generate endpoints
void webStart() {
    Serial.println("... [WEB] Starting web server .. ");
    server.on("/",webIndex); //Display main Page
    server.on("/message",webMessage); //Send Message (json)
    server.on("/settings",webSettings); //Display Settings page
    server.on("/save",webSaveSettings); //Send Settings to SPIFFS (json)
    server.on("/load",webLoadSettings); //Load Settings from SPIFFS (json)
    server.on("/reboot",webReboot); //Reboot ESP
    server.on("/description.xml", HTTP_GET, []() {
      SSDP.schema(server.client());
    });
    server.begin();

    //Simple Service Discovery Protocol : Display ESP in Windows Network Tab
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
