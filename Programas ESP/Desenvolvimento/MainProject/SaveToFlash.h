#ifndef SAVETOFLASH_H
#define SAVETOFLASH_H

#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "TimeConfigurations.h"

/* You only need to format LittleFS the first time you run a
   test or else use the LITTLEFS plugin to create a partition
   https://github.com/lorol/arduino-esp32littlefs-plugin */

#define FORMAT_LITTLEFS_IF_FAILED true
#define WIPE_ON_INITIALIZE false
#define NUM_READINGS 10

struct Reading
{
  float value;
  String datetime;
  int isValid;
};

struct Readings_Lists
{
  Reading BatteryLoadCurrent[NUM_READINGS];
  Reading BatteryVoltage[NUM_READINGS];
  Reading PVBatteryCurrent[NUM_READINGS];
};

class SaveToFlash
{
private:
  int n_cache_saves;
  int n_longterm_saves;
  
  TimeConfigurations timeConfigs;

  int cache_size = 2;      // 1 day of readings every hour
  // esse aqui TEM que ser multiplo de NUM_READINGS pq se não a conta fica muito dificil e não quero fazer ela
  int long_term_size = 5; 

  void listDir(fs::FS &fs, const char *dirname, uint8_t levels);
  void createDir(fs::FS &fs, const char *path);
  void removeDir(fs::FS &fs, const char *path);
  String readFilePage(fs::FS &fs, const char *path, int page);
  void writeFile(fs::FS &fs, const char *path, String message);
  void appendFile(fs::FS &fs, const char *path, String message);
  void renameFile(fs::FS &fs, const char *path1, const char *path2);
  void deleteFile(fs::FS &fs, const char *path);

  String createJSON(String type, float value, String datetime, int isValid);
  Readings_Lists convertReadingJSONToStruct(String batteryLoadCurrent, String batteryVoltage, String pvBatteryCurrent);

public:
  SaveToFlash(TimeConfigurations &configs); // Constructor
  void mountLittleFS();
  int getCachesize();
  int getNCacheSaves();
  void saveToCache(String datetime, float BatteryCurrent, float BatteryVoltage, float PVCurrent);
  void saveToLongTerm();
  Readings_Lists get_readings_from_cache(int page);
  Readings_Lists get_readings_from_longterm(int page);
};

#endif
