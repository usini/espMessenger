/* Mini Matrix Manager
  By Rémi Sarrailh <@m4dnerd>
  Licence : MIT
  UTF8 to Extended Ascii : https://playground.arduino.cc/Main/Utf8ascii/
*/

#include <MD_Parola.h>  //Led Matrix Library (advanced)
#include <MD_MAX72xx.h> //Led Matrix Library (basic)
#include "fonts/Parola_Fonts_data.h" //Ascii Extended Font
#include <Ticker.h> //Parralel Manager

#define CLK_PIN   D5  // or SCK
#define DATA_PIN  D6  // or MOSI
#define CS_PIN    D7  // or SS
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4 //TODO Make it changeable
#define CHAR_SPACING  1 // pixels between characters
#define BUF_SIZE  1024
#define CHAR_LIMIT 280

Ticker matrixManager;

char curMessage[BUF_SIZE];
char newMessage[BUF_SIZE];
bool newMessageAvailable = false;
static byte c1;  // Last character buffer
// Scrolling parameters
const uint16_t PAUSE_TIME = 2000;
uint8_t frameDelay = 50;  // default frame delay value
textEffect_t scrollEffect = PA_SCROLL_LEFT;

MD_Parola matrix = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

//Scroll curMessage on Matrix (if newMessage change update curMessage)
void matrixUpdate() {
  if (matrix.displayAnimate())
  {
    if (newMessageAvailable)
    {
      strcpy(curMessage, newMessage);
      newMessageAvailable = false;
    }
    matrix.displayReset();
  }
}

//Initialize Matrix
void initMatrix() {
  //Settings Matrix
  matrix.begin();
  matrix.setInvert(false);
  matrix.displayClear();
  matrix.displaySuspend(false);
  matrix.setPause(PAUSE_TIME);
  matrix.setFont(ExtASCII);
  matrix.displayScroll(curMessage, PA_LEFT, scrollEffect, frameDelay);
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
void matrixText(char *message) {
  if (strlen(message) <= CHAR_LIMIT ) {
    message = utf8ascii(message);
    //Serial.println(strlen(message));
    strcpy(newMessage, message);
    newMessageAvailable = true;
    Serial.print("--> Message : ");
    Serial.println(message);
  } else {
    Serial.println("--> ERROR Message Too Long!");
  }
}
