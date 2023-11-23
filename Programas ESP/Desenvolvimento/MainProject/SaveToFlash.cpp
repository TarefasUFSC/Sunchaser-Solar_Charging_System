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
    // checa se o diretorio /cache existe, se não existir, cria
    if (!LittleFS.exists("/cache"))
    {
        Serial.println("Creating /cache directory");
        createDir(LittleFS, "/cache");
    }
    if (WIPE_ON_INITIALIZE)
    {
        Serial.println("Wiping /cache directory");
        deleteFile(LittleFS, "/cache/BatCurrent.dt");
        deleteFile(LittleFS, "/cache/BatVoltage.dt");
        deleteFile(LittleFS, "/cache/PVCurrent.dt");
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

String SaveToFlash::readFilePage(fs::FS &fs, const char *path, int page)
{
    DynamicJsonDocument doc(1024);
    Serial.printf("Reading file: %s page: %d\r\n", path, page);

    File file = fs.open(path);
    if (!file)
    {
        Serial.println("- failed to open file for reading");
        return "";
    }

    int first_line = page * NUM_READINGS;
    int last_line = first_line + NUM_READINGS;

    int i = 0;
    if (!file.available())
    {
        Serial.println("File is empty");
        return "";
    }
    while (file.available() && i <= last_line)
    {
        // read a line and increment the counter
        // if the counter is in the range of the lines to be read, save the line
        String line = file.readStringUntil('\n');
        i++;
        if (i >= first_line && i <= last_line)
        {
            //            Serial.print("Line: ");
            //            Serial.println(line);
            doc["content"][i - first_line - 1] = line;
            //            Serial.printf("Colocado no JS/ON na linha %d\n", (i - first_line - 1));
        }
    }
    file.close();
    String FileContent;
    serializeJson(doc, FileContent);
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
    if (file.println(message))
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
    doc["datetime"] = time;

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

    // // Read the cache files
    // String BatCurrent = readFile(LittleFS, "/cache/BatCurrent.dt");
    // String BatVoltage = readFile(LittleFS, "/cache/BatVoltage.dt");
    // String PVCurrent = readFile(LittleFS, "/cache/PVCurrent.dt");

    // // Save the cache files to long term memory
    // appendFile(LittleFS, "/longterm/BatCurrent.dt", BatCurrent);
    // appendFile(LittleFS, "/longterm/BatVoltage.dt", BatVoltage);
    // appendFile(LittleFS, "/longterm/PVCurrent.dt", PVCurrent);

    // n_longterm_saves++;

    // // Empties the cache files
    // writeFile(LittleFS, "/cache/BatCurrent.dt", "");
    // writeFile(LittleFS, "/cache/BatVoltage.dt", "");
    // writeFile(LittleFS, "/cache/PVCurrent.dt", "");

    // n_cache_saves = 0;

    // // If the long term memory is full, delete the oldest 24 hours of readings
    // if (n_longterm_saves >= long_term_size)
    // {
    //     String LT_BatCurrent = readFile(LittleFS, "/longterm/BatCurrent.dt");
    //     String LT_BatVoltage = readFile(LittleFS, "/longterm/BatVoltage.dt");
    //     String LT_PVCurrent = readFile(LittleFS, "/longterm/PVCurrent.dt");

    //     // Delete the oldest n_cache_saves hours of readings
    //     for (int i = 0; i < n_cache_saves; i++)
    //     {
    //         LT_BatCurrent.remove(0, LT_BatCurrent.indexOf("}") + 1);
    //         LT_BatVoltage.remove(0, LT_BatVoltage.indexOf("}") + 1);
    //         LT_PVCurrent.remove(0, LT_PVCurrent.indexOf("}") + 1);
    //     }

    //     // Save the remaining readings to the long term memory
    //     writeFile(LittleFS, "/longterm/BatCurrent.dt", LT_BatCurrent);
    //     writeFile(LittleFS, "/longterm/BatVoltage.dt", LT_BatVoltage);
    //     writeFile(LittleFS, "/longterm/PVCurrent.dt", LT_PVCurrent);

    //     n_longterm_saves--;
    // }
}

Readings_Lists SaveToFlash::convertReadingJSONToStruct(String batteryLoadCurrent, String batteryVoltage, String pvBatteryCurrent)
{

    Readings_Lists readings{0};
    // Serial.println("\n" + battery_load_current + "\n");
    DynamicJsonDocument battery_load_current_doc(1024);
    deserializeJson(battery_load_current_doc, batteryLoadCurrent);
    JsonObject battery_load_current_obj_list = battery_load_current_doc.as<JsonObject>();

    DynamicJsonDocument battery_voltage_doc(1024);
    deserializeJson(battery_voltage_doc, batteryVoltage);
    JsonObject battery_voltage_obj_list = battery_voltage_doc.as<JsonObject>();

    DynamicJsonDocument pv_battery_current_doc(1024);
    deserializeJson(pv_battery_current_doc, pvBatteryCurrent);
    JsonObject pv_battery_current_obj_list = pv_battery_current_doc.as<JsonObject>();

    for (int i = 0; i < NUM_READINGS; i++)
    {

        DynamicJsonDocument bat_load_current_reading_doc(200);
        // Serial.println(obj_list["content"][i].as<String>());
        deserializeJson(bat_load_current_reading_doc, battery_load_current_obj_list["content"][i].as<String>());
        JsonObject bat_load_current_reading_obj = bat_load_current_reading_doc.as<JsonObject>();
        // Serial.println(obj["value"].as<float>());
        readings.BatteryLoadCurrent[i].value = bat_load_current_reading_obj["value"].as<float>();
        readings.BatteryLoadCurrent[i].datetime = bat_load_current_reading_obj["datetime"].as<String>();

        DynamicJsonDocument bat_voltage_reading_doc(200);
        deserializeJson(bat_voltage_reading_doc, battery_voltage_obj_list["content"][i].as<String>());
        JsonObject bat_voltage_reading_obj = bat_voltage_reading_doc.as<JsonObject>();
        readings.BatteryVoltage[i].value = bat_voltage_reading_obj["value"].as<float>();
        readings.BatteryVoltage[i].datetime = bat_voltage_reading_obj["datetime"].as<String>();

        DynamicJsonDocument pv_current_reading_doc(200);
        deserializeJson(pv_current_reading_doc, pv_battery_current_obj_list["content"][i].as<String>());
        JsonObject pv_current_reading_obj = pv_current_reading_doc.as<JsonObject>();
        readings.PVBatteryCurrent[i].value = pv_current_reading_obj["value"].as<float>();
        readings.PVBatteryCurrent[i].datetime = pv_current_reading_obj["datetime"].as<String>();
    }
    return readings;
}
Readings_Lists SaveToFlash::get_readings_from_cache(int page)
{
    Readings_Lists readings;

    String battery_load_current = readFilePage(LittleFS, "/cache/BatCurrent.dt", page);
    String battery_voltage = readFilePage(LittleFS, "/cache/BatVoltage.dt", page);
    String pv_battery_current = readFilePage(LittleFS, "/cache/PVCurrent.dt", page);

    readings = convertReadingJSONToStruct(battery_load_current, battery_voltage, pv_battery_current);

    return readings;
}

Readings_Lists SaveToFlash::get_readings_from_longterm(int page)
{
    Readings_Lists readings;

    String battery_load_current = readFilePage(LittleFS, "/longterm/BatCurrent.dt", page);
    String battery_voltage = readFilePage(LittleFS, "/longterm/BatVoltage.dt", page);
    String pv_battery_current = readFilePage(LittleFS, "/longterm/PVCurrent.dt", page);

    readings = convertReadingJSONToStruct(battery_load_current, battery_voltage, pv_battery_current);

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
