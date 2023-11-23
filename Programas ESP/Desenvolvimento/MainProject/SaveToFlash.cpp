#include "SaveToFlash.h"

SaveToFlash::SaveToFlash()
{
    n_cache_saves = 0;
    n_longterm_saves = 0;
}

void SaveToFlash::mountLittleFS()
{
    if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED))
    {
        Serial.println("LittleFS Mount Failed");
        return;
    }
}

void SaveToFlash::listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        Serial.println("- failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels)
            {
                listDir(fs, file.path(), levels - 1);
            }
        }
        else
        {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void SaveToFlash::createDir(fs::FS &fs, const char *path)
{
    Serial.printf("Creating Dir: %s\n", path);
    if (fs.mkdir(path))
    {
        Serial.println("Dir created");
    }
    else
    {
        Serial.println("mkdir failed");
    }
}

void SaveToFlash::removeDir(fs::FS &fs, const char *path)
{
    Serial.printf("Removing Dir: %s\n", path);
    if (fs.rmdir(path))
    {
        Serial.println("Dir removed");
    }
    else
    {
        Serial.println("rmdir failed");
    }
}

String SaveToFlash::readFile(fs::FS &fs, const char *path)
{
    String FileContent = "";
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if (!file || file.isDirectory())
    {
        Serial.println("- failed to open file for reading");
        return String("");
    }

    Serial.println("- read from file:");
    while (file.available())
    {
        FileContent += (char)file.read();
        Serial.write(file.read());
    }
    file.close();
    return FileContent;
}

void SaveToFlash::writeFile(fs::FS &fs, const char *path, String message)
{
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("- failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        Serial.println("- file written");
    }
    else
    {
        Serial.println("- write failed");
    }
    file.close();
}

void SaveToFlash::appendFile(fs::FS &fs, const char *path, String message)
{
    Serial.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        Serial.println("- failed to open file for appending");
        return;
    }
    if (file.print(message))
    {
        Serial.println("- message appended");
    }
    else
    {
        Serial.println("- append failed");
    }
    file.close();
}

void SaveToFlash::renameFile(fs::FS &fs, const char *path1, const char *path2)
{
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2))
    {
        Serial.println("- file renamed");
    }
    else
    {
        Serial.println("- rename failed");
    }
}

void SaveToFlash::deleteFile(fs::FS &fs, const char *path)
{
    Serial.printf("Deleting file: %s\r\n", path);
    if (fs.remove(path))
    {
        Serial.println("- file deleted");
    }
    else
    {
        Serial.println("- delete failed");
    }
}

String SaveToFlash::createJSON(String type, float value, float time)
{
    StaticJsonDocument<200> doc;
    doc["type"] = type;
    doc["value"] = value;
    doc["DateTime"] = time;

    String Serial = "";
    serializeJson(doc, Serial);
    return Serial;
}

int SaveToFlash::getCachesize()
{
    return cache_size;
}

int SaveToFlash::getNCacheSaves()
{
    return n_cache_saves;
}

void SaveToFlash::saveToCache()
{
    // Read the sensors
    float time = random(100.0);
    float BatteryCurrent = random(100.0);
    float BatteryVoltage = random(100.0);
    float PVCurrent = random(100.0);

    // Save the readings to cache
    String JSON = createJSON("BatteryCurrent", BatteryCurrent, time);
    appendFile(LittleFS, "/cache/BatCurrent.dt", JSON);
    JSON = createJSON("BatteryVoltage", BatteryVoltage, time);
    appendFile(LittleFS, "/cache/BatVoltage.dt", JSON);
    JSON = createJSON("PVCurrent", PVCurrent, time);
    appendFile(LittleFS, "/cache/PVCurrent.dt", JSON);

    n_cache_saves++;
}

void SaveToFlash::saveToLongTerm()
{
    // If the long term memory is full, delete the oldest 24 hours of readings
    if (n_longterm_saves >= long_term_size)
    {
        String LT_BatCurrent = readFile(LittleFS, "/longterm/BatCurrent.dt");
        String LT_BatVoltage = readFile(LittleFS, "/longterm/BatVoltage.dt");
        String LT_PVCurrent = readFile(LittleFS, "/longterm/PVCurrent.dt");

        // Delete the oldest n_cache_saves hours of readings
        for (int i = 0; i < n_cache_saves; i++)
        {
            LT_BatCurrent.remove(0, LT_BatCurrent.indexOf("}") + 1);
            LT_BatVoltage.remove(0, LT_BatVoltage.indexOf("}") + 1);
            LT_PVCurrent.remove(0, LT_PVCurrent.indexOf("}") + 1);
        }

        // Save the remaining readings to the long term memory
        writeFile(LittleFS, "/longterm/BatCurrent.dt", LT_BatCurrent);
        writeFile(LittleFS, "/longterm/BatVoltage.dt", LT_BatVoltage);
        writeFile(LittleFS, "/longterm/PVCurrent.dt", LT_PVCurrent);

        n_longterm_saves--;
    }

    // Read the cache files
    String BatCurrent = readFile(LittleFS, "/cache/BatCurrent.dt");
    String BatVoltage = readFile(LittleFS, "/cache/BatVoltage.dt");
    String PVCurrent = readFile(LittleFS, "/cache/PVCurrent.dt");

    // Save the cache files to long term memory
    appendFile(LittleFS, "/longterm/BatCurrent.dt", BatCurrent);
    appendFile(LittleFS, "/longterm/BatVoltage.dt", BatVoltage);
    appendFile(LittleFS, "/longterm/PVCurrent.dt", PVCurrent);

    n_longterm_saves++;

    // Empties the cache files
    writeFile(LittleFS, "/cache/BatCurrent.dt", "");
    writeFile(LittleFS, "/cache/BatVoltage.dt", "");
    writeFile(LittleFS, "/cache/PVCurrent.dt", "");

    n_cache_saves = 0;
}

Readings_Lists SaveToFlash::get_readings_from_cache(int step)
{
    int start = NUM_READINGS * step;
    int end = NUM_READINGS * (step + 1);
    Readings_Lists readings;

    String BatCurrent = readFile(LittleFS, "/cache/BatCurrent.dt");
    String BatVoltage = readFile(LittleFS, "/cache/BatVoltage.dt");
    String PVCurrent = readFile(LittleFS, "/cache/PVCurrent.dt");

    for (int i = start; i < end; i++)
    {
        readings.BatteryCurrent[i] = BatCurrent.substring(BatCurrent.indexOf("value") + 7, BatCurrent.indexOf("DateTime") - 3).toFloat();
        BatCurrent.remove(0, BatCurrent.indexOf("}") + 1);
        readings.BatteryVoltage[i] = BatVoltage.substring(BatVoltage.indexOf("value") + 7, BatVoltage.indexOf("DateTime") - 3).toFloat();
        BatVoltage.remove(0, BatVoltage.indexOf("}") + 1);
        readings.PVCurrent[i] = PVCurrent.substring(PVCurrent.indexOf("value") + 7, PVCurrent.indexOf("DateTime") - 3).toFloat();
        PVCurrent.remove(0, PVCurrent.indexOf("}") + 1);
    }

    return readings;
}

Readings_Lists SaveToFlash::get_readings_from_longterm(int step)
{
    int start = NUM_READINGS * step;
    int end = NUM_READINGS * (step + 1);
    Readings_Lists readings;

    String BatCurrent = readFile(LittleFS, "/longterm/BatCurrent.dt");
    String BatVoltage = readFile(LittleFS, "/longterm/BatVoltage.dt");
    String PVCurrent = readFile(LittleFS, "/longterm/PVCurrent.dt");

    for (int i = start; i < end; i++)
    {
        readings.BatteryCurrent[i] = BatCurrent.substring(BatCurrent.indexOf("value") + 7, BatCurrent.indexOf("DateTime") - 3).toFloat();
        BatCurrent.remove(0, BatCurrent.indexOf("}") + 1);
        readings.BatteryVoltage[i] = BatVoltage.substring(BatVoltage.indexOf("value") + 7, BatVoltage.indexOf("DateTime") - 3).toFloat();
        BatVoltage.remove(0, BatVoltage.indexOf("}") + 1);
        readings.PVCurrent[i] = PVCurrent.substring(PVCurrent.indexOf("value") + 7, PVCurrent.indexOf("DateTime") - 3).toFloat();
        PVCurrent.remove(0, PVCurrent.indexOf("}") + 1);
    }

    return readings;
}

void SaveToFlash::set_newcachesize(int newSize)
{
    cache_size = newSize;
}

void SaveToFlash::set_newlongterm(int newLongTerm)
{
    long_term_size = newLongTerm;
}