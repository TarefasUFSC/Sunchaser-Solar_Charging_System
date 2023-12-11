#include "SaveToFlash.h"
#include <math.h>

SaveToFlash::SaveToFlash(TimeConfigurations &configs)
{
    timeConfigs = configs;
    this->n_cache_saves = 0;
    this->n_longterm_saves = 0;
    this->cache_size = timeConfigs.get_CacheMaxSize();
    this->long_term_size = timeConfigs.get_LTMMazSize();
}

void SaveToFlash::mountLittleFS()
{
    if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED))
    {
        Serial.println("LittleFS Mount Failed");
        return;
    }
    // checa se o diretorio /cache existe, se não existir, cria
    if (!LittleFS.exists("/cache") || !LittleFS.exists("/longterm"))
    {
        Serial.println("Creating /cache directory");
        createDir(LittleFS, "/cache");
        Serial.println("Creating /longterm directory");
        createDir(LittleFS, "/longterm");
    }
    else
    {
        if (WIPE_ON_INITIALIZE)
        {
            Serial.println("Wiping /cache directory");
            deleteFile(LittleFS, "/cache/BatCurrent.dt");
            deleteFile(LittleFS, "/cache/BatVoltage.dt");
            deleteFile(LittleFS, "/cache/PVCurrent.dt");

            deleteFile(LittleFS, "/longterm/BatCurrent.dt");
            deleteFile(LittleFS, "/longterm/BatVoltage.dt");
            deleteFile(LittleFS, "/longterm/PVCurrent.dt");
        }
        else
        {
            // roda o programa para saber quantos arquivos tem no cache e no longterm
            // só precisa rodar um arquivo de cada tipo pq eles tem o mesmo tamanho
            File cache_file = LittleFS.open("/cache/BatCurrent.dt");
            File longterm_file = LittleFS.open("/longterm/BatCurrent.dt");
            if (cache_file.size() > 0)
            {
                // le linha por linha e conta quantas linhas tem
                int n_lines = 0;
                while (cache_file.available())
                {
                    String line = cache_file.readStringUntil('\n');
                    n_lines++;
                }
                this->n_cache_saves = n_lines;
            }

            if (longterm_file.size() > 0)
            {
                // le linha por linha e conta quantas linhas tem
                int n_lines = 0;
                while (longterm_file.available())
                {
                    String line = longterm_file.readStringUntil('\n');
                    n_lines++;
                }
                this->n_longterm_saves = n_lines;
            }

            Serial.printf("Cache has %d items\n", this->n_cache_saves);
            Serial.printf("Longterm has %d items\n", this->n_longterm_saves);
        }
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

    File file = fs.open(path);
    if (!file)
    {
        Serial.print(path);
        Serial.println(" - failed to open file for reading");
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
    if (!file.print(message))

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
    if (!file.println(message))

    {
        Serial.println("- append failed");
    }
    file.close();
}

void SaveToFlash::renameFile(fs::FS &fs, const char *path1, const char *path2)
{
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (!fs.rename(path1, path2))

    {
        Serial.println("- rename failed");
    }
}

void SaveToFlash::deleteFile(fs::FS &fs, const char *path)
{
    Serial.printf("Deleting file: %s\r\n", path);
    if (!fs.remove(path))

    {
        Serial.println("- delete failed");
    }
}

String SaveToFlash::createJSON(String type, float value, String datetime, int isValid)
{
    StaticJsonDocument<200> doc;
    doc["type"] = type;
    doc["value"] = value;
    doc["datetime"] = datetime;
    doc["isValid"] = isValid;

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
    return this->n_cache_saves;
}

void SaveToFlash::saveToCache(String datetime, float BatteryCurrent, float BatteryVoltage, float PVCurrent)
{
    // Save the readings to cache
    String JSON = createJSON("BatteryCurrent", BatteryCurrent, datetime, 1);
    appendFile(LittleFS, "/cache/BatCurrent.dt", JSON);
    JSON = createJSON("BatteryVoltage", BatteryVoltage, datetime, 1);
    appendFile(LittleFS, "/cache/BatVoltage.dt", JSON);
    JSON = createJSON("PVCurrent", PVCurrent, datetime, 1);
    appendFile(LittleFS, "/cache/PVCurrent.dt", JSON);

    this->n_cache_saves++;
}

void SaveToFlash::saveToLongTerm()
{

    int qtd_pages_cache = ceil((float)this->n_cache_saves / (float)NUM_READINGS);
    Serial.printf("O cache tem %d itens, e tem %d paginas\n", this->n_cache_saves, qtd_pages_cache);

    // Read the cache files page by page and save them to long term memory
    for (int i = 0; i < qtd_pages_cache; i++)
    {
        Readings_Lists readings = get_readings_from_cache(i);
        for (int j = 0; j < NUM_READINGS; j++)
        { Serial.printf("value: %f | datetime: %s | isValid: %d\n", readings.BatteryLoadCurrent[j].value, readings.BatteryLoadCurrent[j].datetime, readings.BatteryLoadCurrent[j].isValid);
                
            if (!readings.BatteryLoadCurrent[j].isValid)
            {

                Serial.println("NAO TA VAAAAAAAAAAAAAALIIIIIIIIIIIIDOOOOO");
                break;
            }
            String JSON = createJSON("BatteryCurrent", readings.BatteryLoadCurrent[j].value, readings.BatteryLoadCurrent[j].datetime, readings.BatteryLoadCurrent[j].isValid);
            appendFile(LittleFS, "/longterm/BatCurrent.dt", JSON);
            JSON = createJSON("BatteryVoltage", readings.BatteryVoltage[j].value, readings.BatteryVoltage[j].datetime, readings.BatteryVoltage[j].isValid);
            appendFile(LittleFS, "/longterm/BatVoltage.dt", JSON);
            JSON = createJSON("PVCurrent", readings.PVBatteryCurrent[j].value, readings.PVBatteryCurrent[j].datetime, readings.PVBatteryCurrent[j].isValid);
            appendFile(LittleFS, "/longterm/PVCurrent.dt", JSON);
        }
    }

    this->n_longterm_saves += this->n_cache_saves;

    // Empties the cache files
    deleteFile(LittleFS, "/cache/BatCurrent.dt");
    deleteFile(LittleFS, "/cache/BatVoltage.dt");
    deleteFile(LittleFS, "/cache/PVCurrent.dt");
    this->n_cache_saves = 0;

    this->long_term_size = timeConfigs.get_LTMMazSize();
    // verificacao de tamanho do longterm
    if (this->n_longterm_saves > this->long_term_size)
    {

        // verifica quantas paginas tem o longterm
        Serial.printf("O longterm tem %d itens\n", this->n_longterm_saves);
        int qtd_pages_longterm = ceil((float)this->n_longterm_saves / (float)NUM_READINGS);
        Serial.printf("O longterm tem %d paginas\n", qtd_pages_longterm);

        int items_to_delete = this->n_longterm_saves - this->long_term_size;
        Serial.printf("O longterm deve deletar %d itens\n", items_to_delete);
        int pages_to_delete = ceil((float)items_to_delete / (float)NUM_READINGS);

        Serial.printf("O longterm deve deletar %d paginas, ficando com %d elementos\n", pages_to_delete, this->long_term_size);

        // cria um arquivo temporario com as paginas que não serão deletadas
        int deleted_entries = 0;
        for (int i = 0; i < qtd_pages_longterm; i++)
        {
            Readings_Lists readings = this->get_readings_from_longterm(i);
            for (int j = 0; j < NUM_READINGS; j++)
            {
                
                if (deleted_entries < items_to_delete)
                {
                    deleted_entries++;
                    continue;
                }
                Serial.printf("value: %f | datetime: %s | isValid: %d\n", readings.BatteryLoadCurrent[j].value, readings.BatteryLoadCurrent[j].datetime, readings.BatteryLoadCurrent[j].isValid);
                if (!readings.BatteryLoadCurrent[j].isValid)
                {
                    Serial.println("NAO TA VAAAAAAAAAAAAAALIIIIIIIIIIIIDOOOOO");
                    break;
                }
                String JSON = createJSON("BatteryCurrent", readings.BatteryLoadCurrent[j].value, readings.BatteryLoadCurrent[j].datetime, readings.BatteryLoadCurrent[j].isValid);
                appendFile(LittleFS, "/longterm/BatCurrent_temp.dt", JSON);
                JSON = createJSON("BatteryVoltage", readings.BatteryVoltage[j].value, readings.BatteryVoltage[j].datetime, readings.BatteryVoltage[j].isValid);
                appendFile(LittleFS, "/longterm/BatVoltage_temp.dt", JSON);
                JSON = createJSON("PVCurrent", readings.PVBatteryCurrent[j].value, readings.PVBatteryCurrent[j].datetime, readings.PVBatteryCurrent[j].isValid);
                appendFile(LittleFS, "/longterm/PVCurrent_temp.dt", JSON);
            }
        }

        // deleta o longterm
        deleteFile(LittleFS, "/longterm/BatCurrent.dt");
        deleteFile(LittleFS, "/longterm/BatVoltage.dt");
        deleteFile(LittleFS, "/longterm/PVCurrent.dt");

        // renomeia os temps
        renameFile(LittleFS, "/longterm/BatCurrent_temp.dt", "/longterm/BatCurrent.dt");
        renameFile(LittleFS, "/longterm/BatVoltage_temp.dt", "/longterm/BatVoltage.dt");
        renameFile(LittleFS, "/longterm/PVCurrent_temp.dt", "/longterm/PVCurrent.dt");

        // atualiza o numero de saves
        this->n_longterm_saves -= pages_to_delete * NUM_READINGS;
    }
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
        readings.BatteryLoadCurrent[i].isValid = bat_load_current_reading_obj["isValid"].as<int>();

        DynamicJsonDocument bat_voltage_reading_doc(200);
        deserializeJson(bat_voltage_reading_doc, battery_voltage_obj_list["content"][i].as<String>());
        JsonObject bat_voltage_reading_obj = bat_voltage_reading_doc.as<JsonObject>();
        readings.BatteryVoltage[i].value = bat_voltage_reading_obj["value"].as<float>();
        readings.BatteryVoltage[i].datetime = bat_voltage_reading_obj["datetime"].as<String>();
        readings.BatteryVoltage[i].isValid = bat_voltage_reading_obj["isValid"].as<int>();

        DynamicJsonDocument pv_current_reading_doc(200);
        deserializeJson(pv_current_reading_doc, pv_battery_current_obj_list["content"][i].as<String>());
        JsonObject pv_current_reading_obj = pv_current_reading_doc.as<JsonObject>();
        readings.PVBatteryCurrent[i].value = pv_current_reading_obj["value"].as<float>();
        readings.PVBatteryCurrent[i].datetime = pv_current_reading_obj["datetime"].as<String>();
        readings.PVBatteryCurrent[i].isValid = pv_current_reading_obj["isValid"].as<int>();
    }
    return readings;
}
Readings_Lists SaveToFlash::get_readings_from_cache(int page)
{
    Readings_Lists readings;
    Serial.println("files cache");
    Serial.println(page);
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
    Serial.print("LT");
    Serial.println(battery_load_current);
    String battery_voltage = readFilePage(LittleFS, "/longterm/BatVoltage.dt", page);
    String pv_battery_current = readFilePage(LittleFS, "/longterm/PVCurrent.dt", page);

    readings = convertReadingJSONToStruct(battery_load_current, battery_voltage, pv_battery_current);

    return readings;
}