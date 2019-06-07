/* JSON Settings Manager (values)
  By Rémi Sarrailh <@m4dnerd>
  Licence : MIT
*/

// Change this value if your settings.json is bigger.
// https://arduinojson.org/v6/assistant/
const int JSON_BUFFER_SIZE = 1024;

/* settings.json */

// WiFi Settings
const char* ssid1;
const char* pass1;
const char* ssid2;
const char* pass2;
const char* ssid3;
const char* pass3;
const char* ssid4;
const char* pass4;

// Access Point Wifi
const char* ap_ssid;
const char* ap_pass;

// Name
const char* name;

// Authentication Web
const char* web_user;
const char* web_pass;
bool web_auth_enabled;
bool web_ssl_enabled;

// MQTT
const char* mqtt_server;
int mqtt_port;
const char* mqtt_username;
const char* mqtt_password;
bool mqtt_enabled;
bool mqtt_ssl_enabled;

// Settings file
const char *settings_file = "/settings.json";

StaticJsonDocument<JSON_BUFFER_SIZE> getInitSettings(StaticJsonDocument<JSON_BUFFER_SIZE> settings){
  // WiFi
  settings["ssid1"] = "";
  settings["pass1"] = "";
  settings["ssid2"] = "";
  settings["pass2"] = "";
  settings["ssid3"] = "";
  settings["pass3"] = "";
  settings["ssid4"] = "";
  settings["pass4"] = "";

  // Access Point WiFi
  settings["ap_ssid"] = "textmatrix";
  settings["ap_pass"] = "textmatrix";

  // Name
  settings["name"] = "textmatrix";

  // Web
  settings["web_ssl_enabled"] = false;
  settings["web_user"] = "textmatrix";
  settings["web_pass"] = "textmatrix";
  settings["web_auth_enabled"] = false;

  // MQTT
  settings["mqtt_server"] = "";
  settings["mqtt_port"] = 1883;
  settings["mqtt_username"] = "";
  settings["mqtt_password"] = "";
  settings["mqtt_enabled"] = false;
  settings["mqtt_ssl_enabled"] = false;

  return settings;
}

JsonObject setSettings(JsonObject settings){
  //WiFi
  ssid1 = settings["ssid1"].as<char*>();
  pass1 = settings["pass1"].as<char*>();
  ssid2 = settings["ssid2"].as<char*>();
  pass2 = settings["pass2"].as<char*>();
  ssid3 = settings["ssid3"].as<char*>();
  pass3 = settings["pass3"].as<char*>();
  ssid4 = settings["ssid4"].as<char*>();

  // Access Point WiFi
  ap_ssid = settings["ap_ssid"].as<char*>();
  ap_pass = settings["ap_pass"].as<char*>();

  // Name
  name = settings["name"].as<char*>();

  // Web
  web_user = settings["web_user"].as<char*>();
  web_pass = settings["web_pass"].as<char*>();

  // MQTT
  mqtt_server = settings["mqtt_server"].as<char*>();
  mqtt_port = settings["mqtt_port"].as<int>();
  mqtt_username = settings["mqtt_username"].as<char*>();
  mqtt_password = settings["mqtt_password"].as<char*>();
  mqtt_enabled = settings["mqtt_enabled"].as<bool>();
  mqtt_ssl_enabled = settings["mqtt_ssl_enabled"].as<bool>();

  return settings;
}