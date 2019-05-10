/*
  ESPManager
  Connected Text Matrix for ESP8266
  By Sarrailh Rémi
  Licence : MIT
  https://github.com/maditnerd/espMessenger

  Library required:
  --> Arduino Thread
  --> MD_MAX72xx
  --> ArduinoJSON (v6)
*/
// Global message buffers shared by Serial and Scrolling functions
#define BUF_SIZE  75
char curMessage[BUF_SIZE];
const char * newMessage = "Wifi";
bool newMessageAvailable = false;
#include <MD_MAX72xx.h>
#include <Thread.h>
#include "settings.h"
#include "wifi.h"
#include "web.h"


#define CLK_PIN   D5  // or SCK
#define DATA_PIN  D6  // or MOSI
#define CS_PIN    D7  // or SS
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CHAR_SPACING  1 // pixels between characters
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);



uint16_t  scrollDelay = 50;  // in milliseconds
Thread matrixThread = Thread();

void setup() {
  Serial.begin(74880);
  Serial.println("~~~~~~~~~~  Text Matrix ~~~~~~~~~~");
  mx.begin();
  mx.setShiftDataInCallback(scrollDataSource);
  mx.setShiftDataOutCallback(scrollDataSink);

  matrixThread.onRun(displayMessage);
  matrixThread.setInterval(scrollDelay);
  //Check if Settings exists or create it
  if(checkSettings()){
    settings_state = readSettings(); //Read settings
  }

  if(settings_state){ //If settings was performs correctly
    connect();  //Connect to WiFi (or create Accesss Point)
    webStart(); //Start WebServer
  }
  strcpy(curMessage, WiFi.localIP().toString().c_str());
  //newMessage[0] = '\0';
}

void loop() {
  if(settings_state){ // Check if settings was performs correctly or do nothing
    server.handleClient(); //Manage Web Server
    MDNS.update(); // Manage Bonjour Name (textmatrix.local by default)
    if(matrixThread.shouldRun()){
      matrixThread.run(); //Manage Message Routine
    }
  }
}


void displayMessage(){
  mx.transform(MD_MAX72XX::TSL);  // scroll along - the callback will load all the data
}

void scrollDataSink(uint8_t dev, MD_MAX72XX::transformType_t t, uint8_t col)
// Callback function for data that is being scrolled off the display
{
}

uint8_t scrollDataSource(uint8_t dev, MD_MAX72XX::transformType_t t)
// Callback function for data that is required for scrolling into the display
{
  static char   *p = curMessage;
  static uint8_t  state = 0;
  static uint8_t  curLen, showLen;
  static uint8_t  cBuf[8];
  uint8_t colData;

  // finite state machine to control what we do on the callback
  switch(state)
  {
    case 0: // Load the next character from the font table
      showLen = mx.getChar(*p++, sizeof(cBuf)/sizeof(cBuf[0]), cBuf);
      curLen = 0;
      state++;

      // if we reached end of message, reset the message pointer
      if (*p == '\0')
      {
        p = curMessage;     // reset the pointer to start of message
        if (newMessageAvailable)  // there is a new message waiting
        {
          strcpy(curMessage, newMessage);	// copy it in
          newMessageAvailable = false;
        }
      }
      // !! deliberately fall through to next state to start displaying

    case 1: // display the next part of the character
      colData = cBuf[curLen++];
      if (curLen == showLen)
      {
        showLen = CHAR_SPACING;
        curLen = 0;
        state = 2;
      }
      break;

    case 2: // display inter-character spacing (blank column)
      colData = 0;
      if (curLen == showLen)
        state = 0;
      curLen++;
      break;

    default:
      state = 0;
  }

  return(colData);
}


