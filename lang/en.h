/* English Language for ESPMessenger
  By Rémi Sarrailh <@m4dnerd>
  Licence : MIT
*/
#include <Arduino.h>

//banner.h
const String TEXT_INTRO = "|An Connected 8x8 Matrix Display by usini|";
const String TEXT_CONNECTION = "~~~~~~~~~~  Connection ~~~~~~~~~~~~~~~~~~~";

//matrix.h
const String TEXT_MESSAGE = "--> Message : ";
const String TEXT_MESSAGE_TOO_LONG = "--> ERROR Message Too Long!";

//settings.h
const String TEXT_MOUNT_SPIFFS = "*ST: Mounted File System";
const String TEXT_READ_CONFIG_FILE = "*ST: Reading settings.json";
const String TEXT_OPEN_CONFIG_FILE = "*ST: Opened settings.json";
const String TEXT_PARSING_JSON = "\n*ST:Parsing JSON";
const String TEXT_NB_MATRIX = "*ST: Matrix: ";
const String TEXT_LOAD_CONFIG_FILE = "*ST: Failed to load JSON";
const String TEXT_MOUNT_FS = "*ST: Failed to mount File System";

const char* TEXT_NAME = "Name";
const char* TEXT_USER = "User";
const char* TEXT_PASSWORD = "Password";
const char* TEXT_MATRIX = "Matrix";

const String TEXT_TIMEOUT = "*ST: failed to connect and hit timeout";
const String TEXT_CONNECTED = "*ST: Connected :-)";
const String TEXT_SAVING_CONFIG = "*ST: Saving settings.json";
const String TEXT_FAILED_OPEN_CONFIG = "*ST: Failed to write settings.json";

//web.h
const String TEXT_COMMAND = "command:";
const String TEXT_WEB_MESSAGE = "Web Message:";
const String TEXT_MESSAGE_SENT = "Message sent";
const String TEXT_NOT_FOUNDED = "404 Page not found";
const String TEXT_OPEN_UPDATE = "Open update.bin";
const String TEXT_ERROR_UPDATE = "ERROR can't open update.bin";
const String TEXT_BEGIN_UPDATE = "Update begin...";
const String TEXT_UPDATE_SUCCESS = "Update Success: \nRebooting...\n";
const String TEXT_UPDATE_END = "Update ended...";
const String TEXT_UPDATE_FINISHED = "Upload finished updating started...";
const String TEXT_UPDATE_FAILED = "Upload failed ...";
const String TEXT_WEB_STARTED = "... Starting web server .. ";
