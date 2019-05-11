#include <MD_MAX72xx.h>
#include <Thread.h>

#define CLK_PIN   D5  // or SCK
#define DATA_PIN  D6  // or MOSI
#define CS_PIN    D7  // or SS
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CHAR_SPACING  1 // pixels between characters
#define BUF_SIZE  75

char curMessage[BUF_SIZE];
const char * newMessage = "Wifi";
bool newMessageAvailable = false;
uint16_t  scrollDelay = 50;  // in milliseconds

MD_MAX72XX matrix = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
Thread matrixThread = Thread();

void displayMessage(){
  matrix.transform(MD_MAX72XX::TSL);  // scroll along - the callback will load all the data
}

void scrollDataSink(uint8_t dev, MD_MAX72XX::transformType_t t, uint8_t col) {
  // Callback function for data that is being scrolled off the display
}

uint8_t scrollDataSource(uint8_t dev, MD_MAX72XX::transformType_t t) {
  // Callback function for data that is required for scrolling into the display
  static char   *p = curMessage;
  static uint8_t  state = 0;
  static uint8_t  curLen, showLen;
  static uint8_t  cBuf[8];
  uint8_t colData;

  // finite state machine to control what we do on the callback
  switch(state)
  {
    case 0: // Load the next character from the font table
      showLen = matrix.getChar(*p++, sizeof(cBuf)/sizeof(cBuf[0]), cBuf);
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

void initMatrix(){
  //Settings Matrix
  matrix.begin();
  matrix.setShiftDataInCallback(scrollDataSource);
  matrix.setShiftDataOutCallback(scrollDataSink);
  matrixThread.onRun(displayMessage);
  matrixThread.setInterval(scrollDelay);
}