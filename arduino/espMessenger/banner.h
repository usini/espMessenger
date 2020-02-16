void introBanner(){
  Serial.println("~~~~~~~~~~  ESP Messenger ~~~~~~~~~~~~~~~~");
  Serial.println(TEXT_INTRO);
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  Serial.println("|https://github.com/usini/espmessenger   |");
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
  Serial.println(VERSION);
  Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
}

void connectionBanner(){
   Serial.println(TEXT_CONNECTION);
   Serial.print("http://");
   Serial.println(settings.name);
   Serial.print("http://");
   Serial.print(settings.name);
   Serial.println(".local");
   Serial.print("http://");
   Serial.println(WiFi.localIP().toString());
   Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
}
