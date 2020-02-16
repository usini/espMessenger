/* Mini Matrix Manager
  By Rémi Sarrailh <@m4dnerd>
  Licence : MIT
  UTF8 to Extended Ascii : https://playground.arduino.cc/Main/Utf8ascii/
*/


#include "fonts/Parola_Fonts_data.h" //Ascii Extended Font
#include <Ticker.h> //Parralel Manager

// PCB PINS
#define CLK_PIN   D7  // or SCK
#define CS_PIN    D6  // or SS
#define DATA_PIN  D5  // or MOSI
// ALT PINS
/*
  #define CLK_PIN   D5  // or SCK
  #define DATA_PIN  D6  // or MOSI
  #define CS_PIN    D7  // or SS
*/

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define CHAR_SPACING  1 // pixels between characters
#define BUF_SIZE  1024
#define CHAR_LIMIT 280
Ticker matrixManager;
bool needScrolling = true;

char curMessage[BUF_SIZE];
char newMessage[BUF_SIZE];
bool newMessageAvailable = false;
static byte c1;  // Last character buffer

MD_Parola* matrix;

//Scroll curMessage on Matrix (if newMessage change update curMessage)
void matrixUpdate() {
  if (needScrolling) {
    if (matrix->displayAnimate())
    {
      if (newMessageAvailable)
      {
        strcpy(curMessage, newMessage);
        newMessageAvailable = false;
      }
      matrix->displayReset();
    }
  } else {
    matrix->displayAnimate();
  }
}

void resetScrolling() {
  matrix->setInvert(settings.invert);
  matrix->setPause(settings.pause);
  matrix->setIntensity(settings.intensity);
  matrix->displayScroll(curMessage, settings.textPosition, settings.effectIn, settings.speed);
  matrix->setTextEffect(settings.effectIn, settings.effectOut);
}


//Initialize Matrix
void initMatrix() {

  //Serial.println(settings.devices);
  matrix = new MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, settings.devices);
  //Settings Matrix
  matrix->begin();
  matrix->displayClear();
  matrix->displaySuspend(false);
  matrix->setFont(ExtASCII);
  resetScrolling();
  matrixManager.attach_ms(10, matrixUpdate);
}




// Convert UTF8 to Extended Ascii (byte by byte)
byte utf8ascii(byte ascii) {
  if ( ascii < 128 ) // Standard ASCII-set 0..0x7F handling
  { c1 = 0;
    return ( ascii );
  }

  // get previous input
  byte last = c1;   // get last char
  c1 = ascii;       // remember actual character
  switch (last)     // conversion depending on first UTF8-character
  { case 0xC2: return  (ascii);  break;
    case 0xC3: return  (ascii | 0xC0);  break;
    case 0x82: if (ascii == 0xAC) return (0x80);   // special case Euro-symbol
  }

  return  (0);                                     // otherwise: return zero, if character has to be ignored
}

// In Place conversion UTF8-string to Extended ASCII (ASCII is shorter!)
char* utf8ascii(char* s) {
  int k = 0;
  char c;
  for (int i = 0; i < strlen(s); i++)
  {
    c = utf8ascii(s[i]);
    if (c != 0)
      s[k++] = c;
  }
  s[k] = 0;
  return s;
}

// Convert UTF8 char pointer to Extended Ascii char pointer and copy it inside newMessage
void matrixText(char *message, bool instant) {
  if(!needScrolling){
    matrix->displayClear();
  }
  
  if (strlen(message) <= settings.devices + 1) {
    needScrolling = false;
  } else {
    needScrolling = true;
  }
  message = utf8ascii(message);
  //Serial.println(strlen(message));
  strcpy(newMessage, message);
  newMessageAvailable = true;
  if(needScrolling){
    if (instant) {
      matrix->displayClear();
      strcpy(curMessage, newMessage);
      resetScrolling();
    }
  } else {
    matrix->displayText(newMessage, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  }

  Serial.print(TEXT_MESSAGE);
  Serial.println(message);
  // } else {
  //  Serial.println(TEXT_MESSAGE_TOO_LONG);
  //}
}
