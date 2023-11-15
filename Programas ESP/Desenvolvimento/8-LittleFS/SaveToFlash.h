#ifndef SAVETOFLASH_H
#define SAVETOFLASH_H

#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>
#include <ArduinoJson.h>  

/* You only need to format LittleFS the first time you run a
   test or else use the LITTLEFS plugin to create a partition
   https://github.com/lorol/arduino-esp32littlefs-plugin */

#define FORMAT_LITTLEFS_IF_FAILED true

//create a struct to hold the data
struct SensorsData {
  float BatteryCurrent;
  float BatteryVoltage;
  float PVCurrent;
  float PVVoltage;
};

void mountLittleFS();
void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
void createDir(fs::FS &fs, const char * path);
void removeDir(fs::FS &fs, const char * path);
void readFile(fs::FS &fs, const char * path);
void writeFile(fs::FS &fs, const char * path, String message);
void appendFile(fs::FS &fs, const char * path, String message);
void renameFile(fs::FS &fs, const char * path1, const char * path2);
void deleteFile(fs::FS &fs, const char * path);

String createJSON(String type, float value, float time);

#endif