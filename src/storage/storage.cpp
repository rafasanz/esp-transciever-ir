/*
 * Module: Persistent JSON storage
 * Responsibilities:
 * - provide atomic save semantics for the main data file
 * - recover from mount failures by formatting LittleFS when required
 */

#include "storage.h"
#include <LittleFS.h>
#include "config.h"
#include "system/logger.h"
#include "system/i18n.h"

namespace {
size_t objectSize(const JsonVariantConst& value) {
    return value.is<JsonObjectConst>() ? value.as<JsonObjectConst>().size() : 0;
}
}

bool Storage::begin() {

    if (!LittleFS.begin()) {
        Logger::log(I18n::pick("LittleFS mount failed. Formatting...", "Fallo al montar LittleFS. Formateando...", "Montowanie LittleFS nie powiodlo sie. Formatuje..."));

        if (!LittleFS.format()) {
            Logger::log(I18n::pick("LittleFS format FAILED", "El formateo de LittleFS FALLO", "Formatowanie LittleFS NIE POWIODLO SIE"));
            return false;
        }

        if (!LittleFS.begin()) {
            Logger::log(I18n::pick("LittleFS mount FAILED after format", "LittleFS no monto tras el formateo", "LittleFS nie zamontowal sie po formatowaniu"));
            return false;
        }

        Logger::log(I18n::pick("LittleFS formatted successfully", "LittleFS formateado correctamente", "LittleFS sformatowano poprawnie"));
    }

    Logger::log(I18n::pick("LittleFS mounted", "LittleFS montado", "LittleFS zamontowany"));
    return true;
}

bool Storage::load(JsonDocument& doc) {
    if (!LittleFS.exists(STORAGE_FILE)) {
        Logger::log(I18n::pick("Storage load: file missing.", "Carga de almacenamiento: falta el archivo.", "Odczyt pamieci: brak pliku."));
        return false;
    }

    File file = LittleFS.open(STORAGE_FILE, "r");
    if (!file) {
        Logger::log(I18n::pick("Storage load: open failed.", "Carga de almacenamiento: fallo al abrir.", "Odczyt pamieci: nie udalo sie otworzyc."));
        return false;
    }

    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
        Logger::log(String(I18n::pick("Storage load: deserialize failed: ", "Carga de almacenamiento: fallo al deserializar: ", "Odczyt pamieci: blad deserializacji: ")) + error.c_str());
        return false;
    }

    Logger::log(String(I18n::pick("Storage load: commands=", "Carga de almacenamiento: comandos=", "Odczyt pamieci: komendy=")) + String(objectSize(doc["commands"])) +
                String(I18n::pick(", mappings=", ", mapeos=", ", mapowania=")) + String(objectSize(doc["mappings"])));
    return true;
}

bool Storage::save(JsonDocument& doc) {
    constexpr const char* kTempFile = "/ir_data.tmp";
    File file = LittleFS.open(kTempFile, "w");
    if (!file) {
        Logger::log(I18n::pick("File open for write FAILED.", "Fallo al abrir el archivo para escritura.", "Otwarcie pliku do zapisu nie powiodlo sie."));
        return false;
    }

    if (serializeJson(doc, file) == 0) {
        file.close();
        LittleFS.remove(kTempFile);
        Logger::log(I18n::pick("JSON serialization FAILED.", "La serializacion JSON FALLO.", "Serializacja JSON NIE POWIODLA SIE."));
        return false;
    }

    file.close();

    JsonDocument verifyDoc;
    File verifyFile = LittleFS.open(kTempFile, "r");
    if (!verifyFile) {
        LittleFS.remove(kTempFile);
        Logger::log(I18n::pick("Storage save: verify open FAILED.", "Guardado de almacenamiento: fallo al abrir para verificar.", "Zapis pamieci: otwarcie do weryfikacji nie powiodlo sie."));
        return false;
    }

    DeserializationError verifyError = deserializeJson(verifyDoc, verifyFile);
    verifyFile.close();
    if (verifyError) {
        LittleFS.remove(kTempFile);
        Logger::log(String(I18n::pick("Storage save: verify failed: ", "Guardado de almacenamiento: verificacion fallida: ", "Zapis pamieci: weryfikacja nie powiodla sie: ")) + verifyError.c_str());
        return false;
    }

    LittleFS.remove(STORAGE_FILE);
    if (!LittleFS.rename(kTempFile, STORAGE_FILE)) {
        LittleFS.remove(kTempFile);
        Logger::log(I18n::pick("Atomic rename FAILED.", "El renombrado atomico FALLO.", "Atomowa zmiana nazwy NIE POWIODLA SIE."));
        return false;
    }

    Logger::log(String(I18n::pick("Storage save: commands=", "Guardado de almacenamiento: comandos=", "Zapis pamieci: komendy=")) + String(objectSize(verifyDoc["commands"])) +
                String(I18n::pick(", mappings=", ", mapeos=", ", mapowania=")) + String(objectSize(verifyDoc["mappings"])));
    return true;
}

bool Storage::wipe() {
    LittleFS.end();

    if (!LittleFS.format()) {
        Logger::log(I18n::pick("LittleFS full wipe FAILED.", "El borrado completo de LittleFS FALLO.", "Pelne czyszczenie LittleFS NIE POWIODLO SIE."));
        return false;
    }

    if (!LittleFS.begin()) {
        Logger::log(I18n::pick("LittleFS mount FAILED after full wipe.", "LittleFS no monto tras el borrado completo.", "LittleFS nie zamontowal sie po pelnym czyszczeniu."));
        return false;
    }

    Logger::log(I18n::pick("LittleFS fully wiped.", "LittleFS borrado por completo.", "LittleFS wyczyszczony calkowicie."));
    return true;
}
