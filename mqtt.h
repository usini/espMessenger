#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define AIO_SERVER      "192.168.0.141"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "user"
#define AIO_KEY         "key"
const char * topic = "ledmatrix/message";
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// MQTT No Login
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT);
Adafruit_MQTT_Subscribe mqttMatrix = Adafruit_MQTT_Subscribe(&mqtt, topic);
// MQTT Password
//  Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);


// MQTTS No Login

// MQTTS Password

void mqttTextReceived(char *data, uint16_t len){
  matrixText(String(data));
}

void mqttStart(){
    mqttMatrix.setCallback(mqttTextReceived);
    mqtt.subscribe(&mqttMatrix);
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("[MQTT] Connecting ... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("[MQTT] Retrying MQTT in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      Serial.println("[MQTT] No connection");
    }
  }
  Serial.println("MQTT Connected!");
}

void mqttUpdate(){
  MQTT_connect();
  mqtt.processPackets(10000); //Check for message

  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
}


