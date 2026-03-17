/*
 * Module: Command repository and configuration store
 * Responsibilities:
 * - own the in-memory JSON document mirrored to LittleFS
 * - provide validation, serialization, and import/export for bridge data
 */

#include "command_manager.h"
#include "system/logger.h"
#include "system/i18n.h"

CommandManager::CommandManager(Storage& storage)
    : _storage(storage) {}

void CommandManager::ensureSchema() {
    if (!_doc["commands"].is<JsonObjectConst>()) {
        _doc["commands"].to<JsonObject>();
    }
    if (!_doc["mappings"].is<JsonObjectConst>()) {
        _doc["mappings"].to<JsonObject>();
    }
    if (!_doc["sequences"].is<JsonObjectConst>()) {
        _doc["sequences"].to<JsonObject>();
    }
    if (!_doc["settings"].is<JsonObjectConst>()) {
        JsonObject settings = _doc["settings"].to<JsonObject>();
        settings["language"] = "en";
    } else if (!_doc["settings"]["language"].is<const char*>()) {
        _doc["settings"]["language"] = "en";
    }

    JsonObject settings = _doc["settings"].as<JsonObject>();
    if (!settings["startup"].is<JsonObjectConst>()) {
        JsonObject startup = settings["startup"].to<JsonObject>();
        startup["enabled"] = false;
        startup["type"] = "none";
        startup["target"] = "";
        startup["delay_ms"] = 1200;
    } else if (!isValidStartupAction(settings["startup"].as<JsonObjectConst>())) {
        JsonObject startup = settings["startup"].to<JsonObject>();
        startup["enabled"] = false;
        startup["type"] = "none";
        startup["target"] = "";
        startup["delay_ms"] = 1200;
    }
}

bool CommandManager::isValidName(const String& name) const {
    return name.length() > 0 && name.length() <= 48;
}

bool CommandManager::isValidNetworkConfig(JsonObjectConst net) const {
    if (!net["mode"].is<const char*>()) {
        return false;
    }

    const String mode = net["mode"].as<String>();
    if (mode != "dhcp" && mode != "static") {
        return false;
    }

    if (mode == "dhcp") {
        return true;
    }

    return net["ip"].is<const char*>() &&
           net["gateway"].is<const char*>() &&
           net["subnet"].is<const char*>();
}

bool CommandManager::isValidDeviceState(JsonObjectConst device) const {
    return device["ssid"].is<const char*>() &&
           device["mac_device"].is<const char*>() &&
           device["status"].is<const char*>() &&
           device["ip"].is<const char*>() &&
           device["gateway"].is<const char*>() &&
           device["subnet"].is<const char*>() &&
           device["rssi"].is<int32_t>();
}

bool CommandManager::isValidSequence(JsonObjectConst sequence) const {
    if (!sequence["steps"].is<JsonArrayConst>() ||
        !sequence["target"].is<const char*>() ||
        !sequence["timeout_ms"].is<uint16_t>() ||
        !sequence["enabled"].is<bool>()) {
        return false;
    }

    JsonArrayConst steps = sequence["steps"].as<JsonArrayConst>();
    if (steps.isNull() || steps.size() < 2 || steps.size() > 8) {
        return false;
    }

    for (JsonVariantConst step : steps) {
        if (!step.is<const char*>()) {
            return false;
        }
        const String stepName = step.as<String>();
        if (!isValidName(stepName)) {
            return false;
        }
    }

    const String target = sequence["target"].as<String>();
    return isValidName(target);
}

bool CommandManager::isValidStartupAction(JsonObjectConst startup) const {
    if (!startup["enabled"].is<bool>() ||
        !startup["type"].is<const char*>() ||
        !startup["target"].is<const char*>() ||
        !startup["delay_ms"].is<uint16_t>()) {
        return false;
    }

    const String type = startup["type"].as<String>();
    if (type != "none" && type != "command" && type != "sequence") {
        return false;
    }

    const uint16_t delayMs = startup["delay_ms"].as<uint16_t>();
    if (delayMs > 30000) {
        return false;
    }

    const String target = startup["target"].as<String>();
    if (type == "none") {
        return target.length() == 0;
    }

    return isValidName(target);
}

bool CommandManager::sequenceReferencesCommand(JsonObjectConst sequence, const String& commandName) const {
    if (sequence["target"].is<const char*>() && sequence["target"].as<String>() == commandName) {
        return true;
    }

    if (!sequence["steps"].is<JsonArrayConst>()) {
        return false;
    }

    for (JsonVariantConst step : sequence["steps"].as<JsonArrayConst>()) {
        if (step.is<const char*>() && step.as<String>() == commandName) {
            return true;
        }
    }

    return false;
}

bool CommandManager::begin() {
    if (!_storage.load(_doc)) {
        Logger::log(I18n::pick(
            "No existing JSON. Creating new file.",
            "No existía JSON. Se crea un archivo nuevo.",
            "Brak pliku JSON. Tworze nowy plik."
        ));
        _doc.clear();
        ensureSchema();
        _storage.save(_doc);
    } else {
        ensureSchema();
    }

    I18n::setLanguage(getLanguage());
    Logger::log(String(I18n::pick(
        "CommandManager live: commands=",
        "CommandManager activo: comandos=",
        "CommandManager aktywny: komendy="
    )) + String(commandCount()) +
                String(I18n::pick(", mappings=", ", mapeos=", ", mapowania=")) + String(mappingCount()) +
                String(I18n::pick(", sequences=", ", secuencias=", ", sekwencje=")) + String(sequenceCount()));

    Logger::log(I18n::pick("Loaded JSON:", "JSON cargado:", "Zaladowany JSON:"));
    serializeJsonPretty(_doc, Serial);
    Logger::log("");
    return true;
}

bool CommandManager::saveCommand(const IRCommandData& command) {
    if (!isValidName(command.name)) {
        Logger::log(I18n::pick("Invalid command name.", "Nombre de comando inválido.", "Nieprawidlowa nazwa komendy."));
        return false;
    }

    if (command.protocol == UNKNOWN && !command.hasRaw) {
        Logger::log(I18n::pick(
            "Unknown command without raw timings rejected.",
            "Comando desconocido sin tiempos raw rechazado.",
            "Odrzucono nieznana komende bez danych raw."
        ));
        return false;
    }

    if (command.bits == 0 && !command.hasRaw) {
        Logger::log(I18n::pick("Invalid command payload.", "Payload de comando inválido.", "Nieprawidlowy ladunek komendy."));
        return false;
    }

    JsonObject commands = _doc["commands"].as<JsonObject>();
    JsonObject cmd = commands[command.name].to<JsonObject>();
    commandToJson(command, cmd);
    return _storage.save(_doc);
}

bool CommandManager::renameCommand(const String& oldName, const String& newName) {
    if (!hasCommand(oldName) || !isValidName(newName) || hasCommand(newName)) {
        Logger::log(I18n::pick("Rename rejected.", "Renombrado rechazado.", "Zmiana nazwy odrzucona."));
        return false;
    }

    IRCommandData command;
    if (!getCommand(oldName, command)) {
        return false;
    }

    command.name = newName;

    JsonObject commands = _doc["commands"].as<JsonObject>();
    JsonObject renamed = commands[newName].to<JsonObject>();
    commandToJson(command, renamed);
    commands.remove(oldName);

    JsonObject mappings = _doc["mappings"].as<JsonObject>();
    JsonDocument remapped;
    JsonObject remappedMappings = remapped.to<JsonObject>();
    for (JsonPair kv : mappings) {
        const String source = String(kv.key().c_str()) == oldName ? newName : String(kv.key().c_str());
        const String target = kv.value().as<String>() == oldName ? newName : kv.value().as<String>();
        remappedMappings[source] = target;
    }
    mappings.clear();
    for (JsonPair kv : remappedMappings) {
        mappings[String(kv.key().c_str())] = kv.value().as<String>();
    }

    JsonObject sequences = _doc["sequences"].as<JsonObject>();
    for (JsonPair kv : sequences) {
        JsonObject sequence = kv.value().as<JsonObject>();
        if (sequence["target"].is<const char*>() && sequence["target"].as<String>() == oldName) {
            sequence["target"] = newName;
        }
        if (sequence["steps"].is<JsonArray>()) {
            JsonArray steps = sequence["steps"].as<JsonArray>();
            for (JsonVariant step : steps) {
                if (step.is<const char*>() && step.as<String>() == oldName) {
                    step.set(newName);
                }
            }
        }
    }

    JsonObject startup = _doc["settings"]["startup"].as<JsonObject>();
    if (startup["type"].is<const char*>() &&
        startup["type"].as<String>() == "command" &&
        startup["target"].is<const char*>() &&
        startup["target"].as<String>() == oldName) {
        startup["target"] = newName;
    }

    if (!_storage.save(_doc)) {
        Logger::log(I18n::pick("Rename save failed.", "Fallo al guardar el renombrado.", "Nie udalo sie zapisac zmiany nazwy."));
        return false;
    }

    Logger::log(String(I18n::pick("Command renamed: ", "Comando renombrado: ", "Zmieniono nazwe komendy: ")) + oldName + " -> " + newName);
    return true;
}

bool CommandManager::getCommand(const String& name, IRCommandData& command) const {
    JsonObjectConst commands = _doc["commands"].as<JsonObjectConst>();
    return commandFromJson(name, commands[name], command);
}

bool CommandManager::deleteCommand(const String& name) {
    JsonObject commands = _doc["commands"].as<JsonObject>();
    if (!commands[name].is<JsonObject>()) {
        Logger::log(I18n::pick("Command not found.", "Comando no encontrado.", "Nie znaleziono komendy."));
        return false;
    }

    commands.remove(name);

    JsonObject mappings = _doc["mappings"].as<JsonObject>();
    bool removedDanglingMapping = true;
    while (removedDanglingMapping) {
        removedDanglingMapping = false;
        for (JsonPair kv : mappings) {
            if (kv.value().as<String>() == name || String(kv.key().c_str()) == name) {
                mappings.remove(kv.key().c_str());
                removedDanglingMapping = true;
                break;
            }
        }
    }

    JsonObject sequences = _doc["sequences"].as<JsonObject>();
    bool removedDanglingSequence = true;
    while (removedDanglingSequence) {
        removedDanglingSequence = false;
        for (JsonPair kv : sequences) {
            JsonObject sequence = kv.value().as<JsonObject>();
            if (sequenceReferencesCommand(sequence, name)) {
                sequences.remove(kv.key().c_str());
                removedDanglingSequence = true;
                break;
            }
        }
    }

    JsonObject startup = _doc["settings"]["startup"].as<JsonObject>();
    if (startup["target"].is<const char*>() &&
        startup["target"].as<String>() == name &&
        startup["type"].is<const char*>() &&
        startup["type"].as<String>() == "command") {
        startup["enabled"] = false;
        startup["type"] = "none";
        startup["target"] = "";
    }

    if (!_storage.save(_doc)) {
        Logger::log(I18n::pick("Error saving after delete.", "Error al guardar tras borrar.", "Blad zapisu po usunieciu."));
        return false;
    }

    Logger::log(String(I18n::pick("Command deleted: ", "Comando borrado: ", "Usunieto komende: ")) + name);
    return true;
}

bool CommandManager::addMapping(const String& source, const String& target) {
    if (!hasCommand(source) || !hasCommand(target)) {
        Logger::log(I18n::pick(
            "Mapping rejected: source/target not found.",
            "Mapeo rechazado: origen o destino no encontrado.",
            "Mapowanie odrzucone: nie znaleziono zrodla lub celu."
        ));
        return false;
    }
    if (source == target) {
        Logger::log(I18n::pick(
            "Mapping rejected: source and target are equal.",
            "Mapeo rechazado: origen y destino son iguales.",
            "Mapowanie odrzucone: zrodlo i cel sa takie same."
        ));
        return false;
    }

    JsonObject mappings = _doc["mappings"].as<JsonObject>();
    mappings[source] = target;
    return _storage.save(_doc);
}

bool CommandManager::getMapping(const String& source, String& target) {
    return resolveMapping(source, target);
}

bool CommandManager::addSequence(const String& name,
                                 JsonArrayConst steps,
                                 const String& target,
                                 uint16_t timeoutMs,
                                 bool enabled) {
    if (!isValidName(name) || !hasCommand(target) || timeoutMs < 100 || timeoutMs > 5000 ||
        steps.size() < 2 || steps.size() > 8) {
        Logger::log(I18n::pick("Sequence rejected: invalid payload.", "Secuencia rechazada: payload inválido.", "Sekwencja odrzucona: nieprawidlowy ladunek."));
        return false;
    }

    JsonDocument temp;
    JsonObject sequence = temp.to<JsonObject>();
    JsonArray outSteps = sequence["steps"].to<JsonArray>();
    for (JsonVariantConst step : steps) {
        if (!step.is<const char*>()) {
            Logger::log(I18n::pick("Sequence rejected: invalid step.", "Secuencia rechazada: paso inválido.", "Sekwencja odrzucona: nieprawidlowy krok."));
            return false;
        }
        const String stepName = step.as<String>();
        if (!hasCommand(stepName)) {
            Logger::log(I18n::pick(
                "Sequence rejected: step command not found.",
                "Secuencia rechazada: no existe un comando del paso.",
                "Sekwencja odrzucona: nie znaleziono komendy kroku."
            ));
            return false;
        }
        outSteps.add(stepName);
    }
    sequence["target"] = target;
    sequence["timeout_ms"] = timeoutMs;
    sequence["enabled"] = enabled;

    JsonObject sequences = _doc["sequences"].as<JsonObject>();
    for (JsonPair kv : sequences) {
        if (String(kv.key().c_str()) == name) {
            continue;
        }
        JsonObjectConst existing = kv.value().as<JsonObjectConst>();
        if (!existing["steps"].is<JsonArrayConst>()) {
            continue;
        }
        JsonArrayConst existingSteps = existing["steps"].as<JsonArrayConst>();
        if (existingSteps.size() != outSteps.size()) {
            continue;
        }

        bool same = true;
        for (size_t i = 0; i < existingSteps.size(); ++i) {
            if (existingSteps[i].as<String>() != outSteps[i].as<String>()) {
                same = false;
                break;
            }
        }
        if (same) {
            Logger::log(I18n::pick("Sequence rejected: duplicate steps.", "Secuencia rechazada: pasos duplicados.", "Sekwencja odrzucona: zduplikowane kroki."));
            return false;
        }
    }

    JsonObject saved = sequences[name].to<JsonObject>();
    saved["target"] = target;
    saved["timeout_ms"] = timeoutMs;
    saved["enabled"] = enabled;
    JsonArray savedSteps = saved["steps"].to<JsonArray>();
    savedSteps.clear();
    for (JsonVariantConst step : outSteps) {
        savedSteps.add(step.as<String>());
    }

    return _storage.save(_doc);
}

bool CommandManager::removeSequence(const String& name) {
    JsonObject sequences = _doc["sequences"].as<JsonObject>();
    if (!sequences[name].is<JsonObjectConst>()) {
        return false;
    }

    sequences.remove(name);
    JsonObject startup = _doc["settings"]["startup"].as<JsonObject>();
    if (startup["target"].is<const char*>() &&
        startup["target"].as<String>() == name &&
        startup["type"].is<const char*>() &&
        startup["type"].as<String>() == "sequence") {
        startup["enabled"] = false;
        startup["type"] = "none";
        startup["target"] = "";
    }

    if (!_storage.save(_doc)) {
        return false;
    }

    Logger::log(String(I18n::pick("Sequence removed: ", "Secuencia eliminada: ", "Usunieto sekwencje: ")) + name);
    return true;
}

bool CommandManager::resolveSequence(const String* steps, size_t stepCount, SequenceMatchResult& result) const {
    result = SequenceMatchResult();
    if (steps == nullptr || stepCount == 0) {
        return false;
    }

    JsonObjectConst sequences = _doc["sequences"].as<JsonObjectConst>();
    for (JsonPairConst kv : sequences) {
        JsonObjectConst sequence = kv.value().as<JsonObjectConst>();
        if (!isValidSequence(sequence) || !sequence["enabled"].as<bool>()) {
            continue;
        }

        JsonArrayConst sequenceSteps = sequence["steps"].as<JsonArrayConst>();
        if (stepCount > sequenceSteps.size()) {
            continue;
        }

        bool prefixMatch = true;
        for (size_t i = 0; i < stepCount; ++i) {
            if (sequenceSteps[i].as<String>() != steps[i]) {
                prefixMatch = false;
                break;
            }
        }

        if (!prefixMatch) {
            continue;
        }

        result.timeoutMs = max<uint16_t>(result.timeoutMs, sequence["timeout_ms"] | 700);
        if (sequenceSteps.size() > stepCount) {
            result.hasPrefix = true;
        } else {
            result.hasExact = true;
            result.sequenceName = String(kv.key().c_str());
            result.targetName = sequence["target"].as<String>();
        }
    }

    return result.hasPrefix || result.hasExact;
}

bool CommandManager::commandParticipatesInSequences(const String& commandName) const {
    JsonObjectConst sequences = _doc["sequences"].as<JsonObjectConst>();
    for (JsonPairConst kv : sequences) {
        JsonObjectConst sequence = kv.value().as<JsonObjectConst>();
        if (!sequence["enabled"].as<bool>() || !sequence["steps"].is<JsonArrayConst>()) {
            continue;
        }
        for (JsonVariantConst step : sequence["steps"].as<JsonArrayConst>()) {
            if (step.is<const char*>() && step.as<String>() == commandName) {
                return true;
            }
        }
    }
    return false;
}

size_t CommandManager::maxSequenceLength() const {
    size_t maxLength = 0;
    JsonObjectConst sequences = _doc["sequences"].as<JsonObjectConst>();
    for (JsonPairConst kv : sequences) {
        JsonObjectConst sequence = kv.value().as<JsonObjectConst>();
        if (!sequence["enabled"].as<bool>() || !sequence["steps"].is<JsonArrayConst>()) {
            continue;
        }
        maxLength = max(maxLength, sequence["steps"].as<JsonArrayConst>().size());
    }
    return maxLength;
}

bool CommandManager::resolveMapping(const String& sourceName, String& targetName) const {
    JsonObjectConst mappings = _doc["mappings"].as<JsonObjectConst>();
    if (!mappings[sourceName].is<const char*>()) {
        return false;
    }
    targetName = mappings[sourceName].as<String>();
    return targetName.length() > 0;
}

bool CommandManager::resolveMappedCommand(const String& sourceName,
                                         IRCommandData& targetCommand,
                                         String& targetName) const {
    if (!resolveMapping(sourceName, targetName)) {
        return false;
    }
    return getCommand(targetName, targetCommand);
}

void CommandManager::listCommands() {
    Logger::log(I18n::pick("---- STORED COMMANDS ----", "---- COMANDOS GUARDADOS ----", "---- ZAPISANE KOMENDY ----"));
    JsonObjectConst commands = _doc["commands"].as<JsonObjectConst>();
    for (JsonPairConst kv : commands) {
        Logger::log(kv.key().c_str());
    }
    Logger::log(I18n::pick("-------------------------", "---------------------------", "--------------------------"));
    Logger::log("");
}

void CommandManager::listMappings() {
    Logger::log(I18n::pick("---- MAPPINGS ----", "---- MAPEOS ----", "---- MAPOWANIA ----"));
    JsonObjectConst mappings = _doc["mappings"].as<JsonObjectConst>();
    for (JsonPairConst kv : mappings) {
        Logger::log(kv.key().c_str() + String(" -> ") + kv.value().as<String>());
    }
    Logger::log(I18n::pick("-------------------------", "-------------------------", "-------------------------"));
    Logger::log("");
}

void CommandManager::listSequences() {
    Logger::log(I18n::pick("---- SEQUENCES ----", "---- SECUENCIAS ----", "---- SEKWENCJE ----"));
    JsonObjectConst sequences = _doc["sequences"].as<JsonObjectConst>();
    for (JsonPairConst kv : sequences) {
        Logger::log(kv.key().c_str());
    }
    Logger::log(I18n::pick("-------------------", "-------------------", "-------------------"));
    Logger::log("");
}

void CommandManager::exportCommands(JsonDocument& doc) {
    doc["commands"] = _doc["commands"];
}

void CommandManager::exportMappings(JsonDocument& doc) {
    doc["mappings"] = _doc["mappings"];
}

void CommandManager::exportSequences(JsonDocument& doc) const {
    doc["sequences"] = _doc["sequences"];
}

void CommandManager::serializeCommandSummaries(Print& out) const {
    out.print("{\"commands\":{");

    JsonObjectConst commands = _doc["commands"].as<JsonObjectConst>();
    bool first = true;
    for (JsonPairConst kv : commands) {
        if (!first) {
            out.print(",");
        }
        first = false;

        JsonDocument keyDoc;
        keyDoc.set(String(kv.key().c_str()));
        serializeJson(keyDoc, out);
        out.print(":");

        JsonObjectConst command = kv.value().as<JsonObjectConst>();
        out.print("{\"protocol\":");
        serializeJson(command["protocol"], out);
        out.print(",\"value\":");
        serializeJson(command["value"], out);
        out.print(",\"bits\":");
        serializeJson(command["bits"], out);
        out.print(",\"signature\":");
        serializeJson(command["signature"], out);
        out.print(",\"frequency\":");
        serializeJson(command["frequency"], out);
        out.print(",\"repeats\":");
        serializeJson(command["repeats"], out);
        out.print(",\"hasRaw\":");
        serializeJson(command["hasRaw"], out);
        out.print(",\"rawLength\":");
        if (command["raw"].is<JsonArrayConst>()) {
            out.print(command["raw"].as<JsonArrayConst>().size());
        } else {
            out.print("0");
        }
        out.print("}");
    }

    out.print("}}");
}

void CommandManager::serializeCommands(Print& out) const {
    out.print("{\"commands\":");
    serializeJson(_doc["commands"], out);
    out.print("}");
}

void CommandManager::serializeMappings(Print& out) const {
    out.print("{\"mappings\":");
    serializeJson(_doc["mappings"], out);
    out.print("}");
}

void CommandManager::serializeSequences(Print& out) const {
    out.print("{\"sequences\":");
    serializeJson(_doc["sequences"], out);
    out.print("}");
}

void CommandManager::serializeCommandsAndMappings(Print& out) const {
    out.print("{\"commands\":");
    serializeJson(_doc["commands"], out);
    out.print(",\"mappings\":");
    serializeJson(_doc["mappings"], out);
    out.print(",\"sequences\":");
    serializeJson(_doc["sequences"], out);
    out.print(",\"startup\":");
    serializeJson(_doc["settings"]["startup"], out);
    out.print("}");
}

bool CommandManager::serializeCommandDetail(const String& name, Print& out) const {
    JsonObjectConst commands = _doc["commands"].as<JsonObjectConst>();
    JsonObjectConst command = commands[name].as<JsonObjectConst>();
    if (command.isNull()) {
        return false;
    }

    out.print("{\"name\":");
    JsonDocument nameDoc;
    nameDoc.set(name);
    serializeJson(nameDoc, out);
    out.print(",\"command\":");
    serializeJson(command, out);
    out.print("}");
    return true;
}

size_t CommandManager::commandCount() const {
    return _doc["commands"].is<JsonObjectConst>() ? _doc["commands"].as<JsonObjectConst>().size() : 0;
}

size_t CommandManager::mappingCount() const {
    return _doc["mappings"].is<JsonObjectConst>() ? _doc["mappings"].as<JsonObjectConst>().size() : 0;
}

size_t CommandManager::sequenceCount() const {
    return _doc["sequences"].is<JsonObjectConst>() ? _doc["sequences"].as<JsonObjectConst>().size() : 0;
}

bool CommandManager::removeMapping(const String& source) {
    JsonObject mappings = _doc["mappings"].as<JsonObject>();
    if (mappings[source].isNull()) {
        return false;
    }

    mappings.remove(source);
    if (!_storage.save(_doc)) {
        return false;
    }

    Logger::log(String(I18n::pick("Mapping removed: ", "Mapeo eliminado: ", "Usunieto mapowanie: ")) + source);
    return true;
}

bool CommandManager::setLanguage(const String& language) {
    if (!I18n::setLanguage(language)) {
        Logger::log(I18n::pick("Invalid language.", "Idioma inválido.", "Nieprawidlowy jezyk."));
        return false;
    }

    JsonObject settings = _doc["settings"].to<JsonObject>();
    settings["language"] = I18n::getLanguage();
    return _storage.save(_doc);
}

String CommandManager::getLanguage() const {
    return _doc["settings"]["language"].is<const char*>() ? _doc["settings"]["language"].as<String>() : String("en");
}

void CommandManager::getSettings(JsonDocument& doc) const {
    JsonObject settings = doc["settings"].to<JsonObject>();
    settings["language"] = getLanguage();
    JsonObject startup = settings["startup"].to<JsonObject>();
    JsonObjectConst current = _doc["settings"]["startup"].as<JsonObjectConst>();
    startup["enabled"] = current["enabled"] | false;
    startup["type"] = current["type"] | "none";
    startup["target"] = current["target"] | "";
    startup["delay_ms"] = current["delay_ms"] | 1200;
}

bool CommandManager::setStartupAction(const StartupActionConfig& config) {
    JsonDocument temp;
    JsonObject startup = temp.to<JsonObject>();
    startup["enabled"] = config.enabled;
    startup["type"] = config.type;
    startup["target"] = config.target;
    startup["delay_ms"] = config.delayMs;

    if (!isValidStartupAction(startup)) {
        Logger::log(I18n::pick("Invalid startup action.", "Acción de arranque inválida.", "Nieprawidlowa akcja startowa."));
        return false;
    }

    const String type = startup["type"].as<String>();
    const String target = startup["target"].as<String>();
    if (type == "command" && target.length() > 0 && !hasCommand(target)) {
        Logger::log(I18n::pick("Startup command not found.", "No existe el comando de arranque.", "Nie znaleziono komendy startowej."));
        return false;
    }
    if (type == "sequence" && target.length() > 0) {
        JsonObjectConst sequences = _doc["sequences"].as<JsonObjectConst>();
        if (!sequences[target].is<JsonObjectConst>()) {
            Logger::log(I18n::pick("Startup sequence not found.", "No existe la secuencia de arranque.", "Nie znaleziono sekwencji startowej."));
            return false;
        }
    }

    JsonObject saved = _doc["settings"]["startup"].to<JsonObject>();
    saved["enabled"] = startup["enabled"].as<bool>();
    saved["type"] = type;
    saved["target"] = target;
    saved["delay_ms"] = startup["delay_ms"].as<uint16_t>();
    return _storage.save(_doc);
}

void CommandManager::getStartupAction(JsonDocument& doc) const {
    JsonObject startup = doc["startup"].to<JsonObject>();
    JsonObjectConst current = _doc["settings"]["startup"].as<JsonObjectConst>();
    startup["enabled"] = current["enabled"] | false;
    startup["type"] = current["type"] | "none";
    startup["target"] = current["target"] | "";
    startup["delay_ms"] = current["delay_ms"] | 1200;
}

StartupActionConfig CommandManager::getStartupAction() const {
    StartupActionConfig config;
    JsonObjectConst startup = _doc["settings"]["startup"].as<JsonObjectConst>();
    config.enabled = startup["enabled"] | false;
    config.type = startup["type"] | "none";
    config.target = startup["target"] | "";
    config.delayMs = startup["delay_ms"] | 1200;
    return config;
}

bool CommandManager::setNetworkConfig(const String& mode,
                                      const String& ip,
                                      const String& gateway,
                                      const String& subnet) {
    if (mode != "dhcp" && mode != "static") {
        Logger::log(I18n::pick("Invalid network mode.", "Modo de red inválido.", "Nieprawidlowy tryb sieci."));
        return false;
    }

    if (mode == "static" &&
        (ip.length() == 0 || gateway.length() == 0 || subnet.length() == 0)) {
        Logger::log(I18n::pick("Static network config incomplete.", "Configuración de red estática incompleta.", "Niepelna statyczna konfiguracja sieci."));
        return false;
    }

    JsonObject net = _doc["network"].to<JsonObject>();
    net["mode"] = mode;
    net["ip"] = ip;
    net["gateway"] = gateway;
    net["subnet"] = subnet;
    return _storage.save(_doc);
}

bool CommandManager::setDeviceState(const String& ssid,
                                    const String& mac,
                                    const String& status,
                                    const String& ip,
                                    const String& gateway,
                                    const String& subnet,
                                    int32_t rssi) {
    JsonObject device = _doc["device"].to<JsonObject>();
    device["ssid"] = ssid;
    device["mac_device"] = mac;
    device["status"] = status;
    device["ip"] = ip;
    device["gateway"] = gateway;
    device["subnet"] = subnet;
    device["rssi"] = rssi;
    return _storage.save(_doc);
}

void CommandManager::getNetworkConfig(JsonDocument& doc) {
    doc["network"] = _doc["network"];
}

void CommandManager::getDeviceState(JsonDocument& doc) {
    doc["device"] = _doc["device"];
}

bool CommandManager::importConfig(JsonDocument& doc) {
    if (!doc["commands"].is<JsonObject>() || !doc["mappings"].is<JsonObject>()) {
        Logger::log(I18n::pick("Import rejected: invalid schema.", "Importación rechazada: esquema inválido.", "Import odrzucony: nieprawidlowy schemat."));
        return false;
    }

    if (doc["network"].is<JsonObjectConst>() &&
        !isValidNetworkConfig(doc["network"].as<JsonObjectConst>())) {
        Logger::log(I18n::pick("Import rejected: invalid network config.", "Importación rechazada: configuración de red inválida.", "Import odrzucony: nieprawidlowa konfiguracja sieci."));
        return false;
    }

    if (doc["device"].is<JsonObjectConst>() &&
        !isValidDeviceState(doc["device"].as<JsonObjectConst>())) {
        Logger::log(I18n::pick("Import rejected: invalid device state.", "Importación rechazada: estado de dispositivo inválido.", "Import odrzucony: nieprawidlowy stan urzadzenia."));
        return false;
    }

    _doc.clear();
    _doc.set(doc);
    ensureSchema();
    return _storage.save(_doc);
}

void CommandManager::exportConfig(JsonDocument& doc) const {
    doc.set(_doc);
}

bool CommandManager::importCommandsAndMappings(JsonDocument& doc) {
    if (!doc["commands"].is<JsonObject>() || !doc["mappings"].is<JsonObject>()) {
        Logger::log(I18n::pick("Import rejected: invalid commands/mappings schema.", "Importación rechazada: esquema inválido de comandos o mapeos.", "Import odrzucony: nieprawidlowy schemat komend lub mapowan."));
        return false;
    }

    _doc["commands"] = doc["commands"];
    _doc["mappings"] = doc["mappings"];
    ensureSchema();
    return _storage.save(_doc);
}

bool CommandManager::importCommandsAndMappingsJson(const String& json) {
    JsonDocument incoming;
    if (deserializeJson(incoming, json) != DeserializationError::Ok ||
        !incoming["commands"].is<JsonObjectConst>() ||
        !incoming["mappings"].is<JsonObjectConst>() ||
        !incoming["sequences"].is<JsonObjectConst>()) {
        Logger::log(I18n::pick("Import rejected: invalid commands/mappings/sequences payload.", "Importación rechazada: payload inválido de comandos, mapeos o secuencias.", "Import odrzucony: nieprawidlowy ladunek komend, mapowan lub sekwencji."));
        return false;
    }

    const bool hasNetwork = _doc["network"].is<JsonObjectConst>();
    const String mode = hasNetwork ? _doc["network"]["mode"].as<String>() : "";
    const String networkIp = hasNetwork ? _doc["network"]["ip"].as<String>() : "";
    const String networkGateway = hasNetwork ? _doc["network"]["gateway"].as<String>() : "";
    const String networkSubnet = hasNetwork ? _doc["network"]["subnet"].as<String>() : "";

    const bool hasDevice = _doc["device"].is<JsonObjectConst>();
    const String ssid = hasDevice ? _doc["device"]["ssid"].as<String>() : "";
    const String mac = hasDevice ? _doc["device"]["mac_device"].as<String>() : "";
    const String status = hasDevice ? _doc["device"]["status"].as<String>() : "";
    const String deviceIp = hasDevice ? _doc["device"]["ip"].as<String>() : "";
    const String gateway = hasDevice ? _doc["device"]["gateway"].as<String>() : "";
    const String subnet = hasDevice ? _doc["device"]["subnet"].as<String>() : "";
    const int32_t rssi = hasDevice ? (_doc["device"]["rssi"] | 0) : 0;
    const String language = getLanguage();
    const StartupActionConfig currentStartup = getStartupAction();

    for (JsonPairConst kv : incoming["commands"].as<JsonObjectConst>()) {
        IRCommandData command;
        if (!commandFromJson(String(kv.key().c_str()), kv.value(), command)) {
            Logger::log(String(I18n::pick("Import rejected: invalid command ", "Importación rechazada: comando inválido ", "Import odrzucony: nieprawidlowa komenda ")) + String(kv.key().c_str()));
            return false;
        }
    }

    for (JsonPairConst kv : incoming["mappings"].as<JsonObjectConst>()) {
        if (!kv.value().is<const char*>()) {
            Logger::log(I18n::pick("Import rejected: invalid mapping target.", "Importación rechazada: destino de mapeo inválido.", "Import odrzucony: nieprawidlowy cel mapowania."));
            return false;
        }

        const String source = String(kv.key().c_str());
        const String target = kv.value().as<String>();
        if (source.length() == 0 || target.length() == 0 || source == target ||
            !incoming["commands"][source].is<JsonObjectConst>() ||
            !incoming["commands"][target].is<JsonObjectConst>()) {
            Logger::log(String(I18n::pick("Import rejected: dangling mapping ", "Importación rechazada: mapeo colgante ", "Import odrzucony: wiszace mapowanie ")) + source + " -> " + target);
            return false;
        }
    }

    for (JsonPairConst kv : incoming["sequences"].as<JsonObjectConst>()) {
        JsonObjectConst sequence = kv.value().as<JsonObjectConst>();
        if (!isValidSequence(sequence)) {
            Logger::log(String(I18n::pick("Import rejected: invalid sequence ", "Importación rechazada: secuencia inválida ", "Import odrzucony: nieprawidlowa sekwencja ")) + String(kv.key().c_str()));
            return false;
        }

        if (!hasCommand(sequence["target"].as<String>()) && incoming["commands"][sequence["target"].as<String>()].isNull()) {
            Logger::log(I18n::pick("Import rejected: sequence target missing.", "Importación rechazada: falta el objetivo de la secuencia.", "Import odrzucony: brak celu sekwencji."));
            return false;
        }

        for (JsonVariantConst step : sequence["steps"].as<JsonArrayConst>()) {
            if (incoming["commands"][step.as<String>()].isNull()) {
                Logger::log(I18n::pick("Import rejected: sequence step missing.", "Importación rechazada: falta un paso de la secuencia.", "Import odrzucony: brakuje kroku sekwencji."));
                return false;
            }
        }
    }

    StartupActionConfig importedStartup = currentStartup;
    if (incoming["startup"].is<JsonObjectConst>()) {
        if (!isValidStartupAction(incoming["startup"].as<JsonObjectConst>())) {
            Logger::log(I18n::pick("Import rejected: invalid startup action.", "Importación rechazada: acción de arranque inválida.", "Import odrzucony: nieprawidlowa akcja startowa."));
            return false;
        }

        importedStartup.enabled = incoming["startup"]["enabled"] | false;
        importedStartup.type = incoming["startup"]["type"] | "none";
        importedStartup.target = incoming["startup"]["target"] | "";
        importedStartup.delayMs = incoming["startup"]["delay_ms"] | 1200;

        if (importedStartup.type == "command" &&
            !incoming["commands"][importedStartup.target].is<JsonObjectConst>()) {
            Logger::log(I18n::pick("Import rejected: startup command missing.", "Importación rechazada: falta el comando de arranque.", "Import odrzucony: brak komendy startowej."));
            return false;
        }

        if (importedStartup.type == "sequence" &&
            !incoming["sequences"][importedStartup.target].is<JsonObjectConst>()) {
            Logger::log(I18n::pick("Import rejected: startup sequence missing.", "Importación rechazada: falta la secuencia de arranque.", "Import odrzucony: brak sekwencji startowej."));
            return false;
        }
    }

    _doc.clear();
    ensureSchema();

    if (hasNetwork) {
        JsonObject network = _doc["network"].to<JsonObject>();
        network["mode"] = mode;
        network["ip"] = networkIp;
        network["gateway"] = networkGateway;
        network["subnet"] = networkSubnet;
    }

    if (hasDevice) {
        JsonObject device = _doc["device"].to<JsonObject>();
        device["ssid"] = ssid;
        device["mac_device"] = mac;
        device["status"] = status;
        device["ip"] = deviceIp;
        device["gateway"] = gateway;
        device["subnet"] = subnet;
        device["rssi"] = rssi;
    }

    _doc["settings"]["language"] = language;
    JsonObject startup = _doc["settings"]["startup"].to<JsonObject>();
    startup["enabled"] = importedStartup.enabled;
    startup["type"] = importedStartup.type;
    startup["target"] = importedStartup.target;
    startup["delay_ms"] = importedStartup.delayMs;

    JsonObject commands = _doc["commands"].as<JsonObject>();
    for (JsonPairConst kv : incoming["commands"].as<JsonObjectConst>()) {
        IRCommandData command;
        if (!commandFromJson(String(kv.key().c_str()), kv.value(), command)) {
            Logger::log(String(I18n::pick("Import aborted while rebuilding command ", "Importación abortada al reconstruir el comando ", "Import przerwany podczas odbudowy komendy ")) + String(kv.key().c_str()));
            _doc.clear();
            _storage.load(_doc);
            ensureSchema();
            return false;
        }

        JsonObject out = commands[String(kv.key().c_str())].to<JsonObject>();
        commandToJson(command, out);
    }

    JsonObject mappings = _doc["mappings"].as<JsonObject>();
    for (JsonPairConst kv : incoming["mappings"].as<JsonObjectConst>()) {
        mappings[String(kv.key().c_str())] = kv.value().as<String>();
    }

    JsonObject sequences = _doc["sequences"].as<JsonObject>();
    for (JsonPairConst kv : incoming["sequences"].as<JsonObjectConst>()) {
        JsonObjectConst incomingSequence = kv.value().as<JsonObjectConst>();
        JsonObject saved = sequences[String(kv.key().c_str())].to<JsonObject>();
        saved["target"] = incomingSequence["target"].as<String>();
        saved["timeout_ms"] = incomingSequence["timeout_ms"] | 700;
        saved["enabled"] = incomingSequence["enabled"] | true;
        JsonArray savedSteps = saved["steps"].to<JsonArray>();
        for (JsonVariantConst step : incomingSequence["steps"].as<JsonArrayConst>()) {
            savedSteps.add(step.as<String>());
        }
    }

    if (!_storage.save(_doc)) {
        Logger::log(I18n::pick("Import save FAILED.", "Fallo al guardar la importación.", "Nie udalo sie zapisac importu."));
        _doc.clear();
        _storage.load(_doc);
        ensureSchema();
        return false;
    }

    return true;
}

void CommandManager::exportCommandsAndMappings(JsonDocument& doc) const {
    doc["commands"] = _doc["commands"];
    doc["mappings"] = _doc["mappings"];
    doc["sequences"] = _doc["sequences"];
    doc["startup"] = _doc["settings"]["startup"];
}

bool CommandManager::factoryReset() {
    _doc.clear();
    return _storage.wipe();
}

String CommandManager::getNameBySignature(const String& signature) const {
    if (signature.length() == 0) {
        return "";
    }

    JsonObjectConst commands = _doc["commands"].as<JsonObjectConst>();
    for (JsonPairConst kv : commands) {
        IRCommandData command;
        if (commandFromJson(kv.key().c_str(), kv.value(), command) &&
            commandSignature(command) == signature) {
            return String(kv.key().c_str());
        }
    }

    return "";
}

String CommandManager::getNameByCommand(const IRCommandData& command) const {
    return getNameBySignature(commandSignature(command));
}

bool CommandManager::hasCommand(const String& name) const {
    JsonObjectConst commands = _doc["commands"].as<JsonObjectConst>();
    return commands[name].is<JsonObjectConst>();
}

bool CommandManager::getSequenceDefinition(const String& name, JsonDocument& doc) const {
    JsonObjectConst sequences = _doc["sequences"].as<JsonObjectConst>();
    JsonObjectConst sequence = sequences[name].as<JsonObjectConst>();
    if (!isValidSequence(sequence)) {
        return false;
    }

    doc.clear();
    JsonObject out = doc["sequence"].to<JsonObject>();
    out["target"] = sequence["target"].as<String>();
    out["timeout_ms"] = sequence["timeout_ms"] | 700;
    out["enabled"] = sequence["enabled"] | true;
    JsonArray steps = out["steps"].to<JsonArray>();
    for (JsonVariantConst step : sequence["steps"].as<JsonArrayConst>()) {
        steps.add(step.as<String>());
    }
    return true;
}
