#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

ESP8266WiFiMulti network;
bool connection = false;

//Try to connect to ssid, if fail connect to AP.
//TODO: Should check if ssid exists and go to AP if not directly
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
      MDNS.addService("http","tcp", 80);
    }
}