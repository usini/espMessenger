#include <PubSubClient.h>

//TODO Make it changeable
#define MQTT_SERVER      "192.168.0.141"
#define MQTT_SERVERPORT  1883
#define MQTT_USERNAME    "user"
#define MQTT_KEY         "key"
const char * topic = "ledmatrix/message";

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient espClient;
PubSubClient mqttClient(espClient);

// TODO
// MQTT Password
// MQTTS No Login
// MQTTS Password

void mqttTextReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("MQTT --> ");
  char payloadChar[1024];

  int k = 0;
  char c;
  for (int i = 0; i < length; i++)
  {
    c = payload[i];
    Serial.println(c);
    payloadChar[k++] = payload[i];
  }
  Serial.println(payloadChar);
  matrixText(payloadChar);
}

void mqttStart() {
  Serial.println("... [MQTT] Starting MQTT Server");
  randomSeed(micros());
  mqttClient.setServer(MQTT_SERVER, MQTT_SERVERPORT);
  mqttClient.setCallback(mqttTextReceived);
}

void mqttUpdate() {
  if (!mqttClient.connected()) {
    while (!mqttClient.connected()) {
      String clientId = "ESP8266Client-";
      clientId += String(random(0xffff), HEX);

      if (mqttClient.connect(clientId.c_str())) {
        Serial.println("... [MQTT] Connected");
        mqttClient.subscribe(topic);
      } else {
        Serial.println("... [MQTT] Connection...");
        delay(1000);
      }
    }
  } else {
    mqttClient.loop();
  }
}
