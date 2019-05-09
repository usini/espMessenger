#include "FS.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "settings.h"
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
ESP8266WiFiMulti network;

bool connection = false;
ESP8266WebServer server(80); //Server on port 80

const char HTTP_INDEX[] PROGMEM = R"=====(
<html>

<title> Text Matrix </title>
<h1> Text Matrix </h1>
<form id="display_form">
    Text: <input type="text" name="message"><br>
    <input type="submit">
</form>

<h1> Settings </h1>
TO DO
<h1> History </h1>
1 -->
2 -->
3 -->
4 -->
5 -->
<script>
document.getElementById('display_form').addEventListener('submit', (e) => {
  e.preventDefault();
  const formData = new FormData(e.target);
  const data = Array.from(formData.entries()).reduce((memo, pair) => ({
    ...memo,
    [pair[0]]: pair[1],
  }), {});
  fetch("/message", {
      headers: {
        "Accept": "application/json",
        "Content-Type": "application/json"
      },
      method: "POST",
      body: JSON.stringify(data)
  });
});
</script>
</html>
)=====";

void setup() {
  Serial.begin(74880);
  Serial.println("~~~~~~~~~~  Text Matrix ~~~~~~~~~~");
  //Check Settings
  settings_state = checkSettings();
  if(settings_state){
    settings_state = readSettings();
  }

  if(settings_state){
    connect();
    server.on("/",webIndex);
    server.on("/message",WebMessage);
    server.begin();
  }
}

void loop() {
  if(settings_state){
    server.handleClient();
  }
}

void WebMessage(){
  if(server.hasArg("plain") == false){
    server.send(200, "text/html", "no message");
  } else {
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, server.arg("plain"));
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
    server.send(200, "text/json", doc["message"]);
  }
}

void webIndex() {
  server.send(200, "text/html", HTTP_INDEX);
}

void connect(){
  Serial.println("... [WIFI] Connection ...");
    WiFi.mode(WIFI_STA);
    network.addAP(ssid1,pass1);
    network.addAP(ssid2,pass2);
    network.addAP(ssid3,pass3);
    network.addAP(ssid4,pass4);
    if(network.run() == WL_CONNECTED){
      Serial.print("--> Network Mode : ");
      Serial.println(WiFi.localIP());
    } else{
      WiFi.mode(WIFI_AP);
      WiFi.softAP(ap_ssid,ap_pass);
      Serial.print("--> Access Point Mode : ");
      Serial.println(WiFi.softAPIP());
    }
    if (!MDNS.begin(name)) {
      Serial.println("--> No network name :-(");
    } else{
      Serial.print("--> Network Name : ");
      Serial.println(name);
      MDNS.addService("http","tcp", 80);
    }
}
