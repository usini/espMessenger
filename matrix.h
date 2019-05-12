#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include "Parola_Fonts_data.h"
#include <Thread.h>

#define CLK_PIN   D5  // or SCK
#define DATA_PIN  D6  // or MOSI
#define CS_PIN    D7  // or SS
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CHAR_SPACING  1 // pixels between characters
#define BUF_SIZE  1024

char curMessage[BUF_SIZE];
char * newMessage = " ";
bool newMessageAvailable = false;
static byte c1;  // Last character buffer
// Scrolling parameters
const uint16_t PAUSE_TIME = 2000;
uint8_t frameDelay = 50;  // default frame delay value
textEffect_t scrollEffect = PA_SCROLL_LEFT;

MD_Parola matrix = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

//Initialize Matrix
void initMatrix(){
  //Settings Matrix
  matrix.begin();
  matrix.setInvert(false);
  matrix.displayClear();
  matrix.displaySuspend(false);
  matrix.setPause(PAUSE_TIME);
  matrix.setFont(ExtASCII);
  matrix.displayScroll(curMessage, PA_LEFT, scrollEffect, frameDelay);
}

//Scroll text on Matrix
void matrixUpdate(){
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

//https://playground.arduino.cc/Main/Utf8ascii/

byte utf8ascii(byte ascii) {
    if ( ascii<128 )   // Standard ASCII-set 0..0x7F handling
    {   c1=0;
        return( ascii );
    }

    // get previous input
    byte last = c1;   // get last char
    c1=ascii;         // remember actual character
    switch (last)     // conversion depending on first UTF8-character
    {   case 0xC2: return  (ascii);  break;
        case 0xC3: return  (ascii | 0xC0);  break;
        case 0x82: if(ascii==0xAC) return(0x80);       // special case Euro-symbol
    }

    return  (0);                                     // otherwise: return zero, if character has to be ignored
}

// convert String object from UTF8 String to Extended ASCII
String utf8ascii(String s) {
        String r="";
        char c;
        for (int i=0; i<s.length(); i++)
        {
                c = utf8ascii(s.charAt(i));
                if (c!=0) r+=c;
        }
        return r;
}

// In Place conversion UTF8-string to Extended ASCII (ASCII is shorter!)
void utf8ascii(char* s) {
        int k=0;
        char c;
        for (int i=0; i<strlen(s); i++)
        {
                c = utf8ascii(s[i]);
                if (c!=0)
                        s[k++]=c;
        }
        s[k]=0;
}

void matrixText(String message) {
  //message = removeNonAscii(message);
  message = ' ' + message;
  message = utf8ascii(message);
  strcpy(newMessage, message.c_str());
  newMessageAvailable = true;
  Serial.println("--> Message " + message);
}
