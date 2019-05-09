#include "FS.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include "settings.h"
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <U8g2lib.h>
ESP8266WiFiMulti network;
U8G2_MAX7219_32X8_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/ D5, /* data=*/ D6, /* cs=*/ D7, /* dc=*/ U8X8_PIN_NONE, /* reset=*/ U8X8_PIN_NONE);
uint16_t pos = 0;
uint16_t len;
int bitpos = 0;
const char * str = "Hello World";

bool connection = false;
ESP8266WebServer server(80); //Server on port 80

const char HTTP_SETTINGS[] PROGMEM = R"=====(
<html>

<title> Text Matrix </title>
<h1> Text Matrix </h1>
<h2> Settings </h2>
<h3> Web Access </h3>
<form id="settings_form">
        Name <input type="text" name="name"><br>
        User      <input type="text" name="web_user"><br>
        Password  <input type="password" name="web_pass"><br>
<h3> Wi-Fi </h3>
    SSID 1      <input type="text" name="ssid1"><br>
    PASSWORD 1 <input type="password" name="pass1"><br>
    SSID 2      <input type="text" name="ssid2"><br>
    PASSWORD 2 <input type="password" name="pass2"><br>
    SSID 3 <input type="text" name="ssid3"><br>
    PASSWORD 3 <input type="password" name="pass3"><br>
    SSID 4 <input type="text" name="ssid4"><br>
    PASSWORD 4 <input type="password" name="pass4"><br>
<h3> Access Point </h3>
        SSID       <input type="text" name="ap_ssid"><br>
        PASSWORD <input type="text" name="ap_pass"><br>
        <input type="submit">
</form>
<button onclick="window.location.href = '/'"> Go back </button>
<script>
fetch("/load")
.then(function(res){return res.json();})
.then(function(data){
    for (form_obj in data){
        document.getElementsByName(form_obj)[0].value = data[form_obj];
    }
});

document.getElementById('settings_form').addEventListener('submit', (e) => {
    e.preventDefault();
    const formData = new FormData(e.target);
    const data = Array.from(formData.entries()).reduce((memo, pair) => ({
        ...memo,
        [pair[0]]: pair[1],
    }), {});
    fetch("/save", {
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

const char HTTP_INDEX[] PROGMEM = R"=====(
<html>

<title> Text Matrix </title>
<h1> Text Matrix </h1>
<form id="display_form">
    Text: <input type="text" name="message"><br>
    <input type="submit">
</form>

<button onclick="window.location.href = '/settings'"> Settings </button>
<h1> History </h1>
<ul>
    <li>MESSAGE1</li>
    <li>MESSAGE2</li>
    <li>MESSAGE3</li>
    <li>MESSAGE4</li>
    <li>MESSAGE5</li>
</ul>
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
  u8g2.begin();
  u8g2.setFont(u8g2_font_victoriabold8_8r);  // choose a 8x8 u8g2 font
  len = strlen(str)*8;
  //Check Settings
  settings_state = checkSettings();
  if(settings_state){
    settings_state = readSettings();
  }

  if(settings_state){
    connect();
    server.on("/",webIndex);
    server.on("/message",webMessage);
    server.on("/settings",webSettings);
    server.on("/save",webSaveSettings);
    server.on("/load",webLoadSettings);
    server.on("/reboot",webReboot);
    server.begin();
  }
}

void loop() {
  if(settings_state){
    server.handleClient();
    MDNS.update();
    displayMessage();
  }
}

void webReboot(){
  ESP.restart();
}

void webSettings(){
  server.send(200, "text/html", HTTP_SETTINGS);
}

void webSaveSettings(){
  if(server.hasArg("plain") == false){
    server.send(200, "text/html", "no message");
  } else {
    File file = SPIFFS.open(settings_file,"w");
    file.println(server.arg("plain"));
    file.close();
    server.send(200, "text/json", server.arg("plain"));
  }
}

void webLoadSettings(){
  File file = SPIFFS.open(settings_file, "r");
  String settings_json;
  while (file.available()){
    settings_json += char(file.read());
  }
  file.close();
  server.send(200, "text/json", settings_json);
}

void webMessage(){
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
    str = doc["message"].as<char*>();
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
    int wait_for_network = 600;
    while (network.run() != WL_CONNECTED){
      Serial.println(wait_for_network);
      wait_for_network--;
      if(wait_for_network == 0){
        break;
      }
      delay(100);
    }
    if(wait_for_network == 0){
      WiFi.mode(WIFI_AP);
      WiFi.softAP(ap_ssid,ap_pass);
      Serial.print("--> Access Point Mode : ");
      Serial.println(WiFi.softAPIP());
    } else {
      Serial.print("--> Network Mode : ");
      Serial.println(WiFi.SSID());
      Serial.println(WiFi.localIP());
    }
    if (!MDNS.begin(name)) {
      Serial.println("--> No network name :-(");
    } else{
      Serial.print("--> Network Name : ");
      Serial.println(name);
      //MDNS.addService("http","tcp", 80);
    }
}
#define BUF_LEN 256
void displayMessage(){
  u8g2.clearBuffer();         // clear the internal memory
  char buf[BUF_LEN];
  uint16_t start = bitpos / 8;
  uint16_t i;
  for( i = 0; i < BUF_LEN-1; i++)
  {
    buf[i] = str[start+i];
    if ( str[start+i] == '\0' )
      break;
  }
  buf[BUF_LEN-1] = '\0';
  u8g2.drawStr( -(bitpos & 7), 8, buf);     // write buffer
  u8g2.sendBuffer();          // transfer internal memory to the display
  pos++;
  if ( pos >= len )
    pos = 0;
}