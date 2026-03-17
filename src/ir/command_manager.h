/*
 * Module: Command repository and configuration store
 * Responsibilities:
 * - validate and persist commands, mappings, sequences, and settings
 * - serialize project data for the web API, import/export, and runtime use
 */

#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Print.h>
#include "ir_command.h"
#include "../storage/storage.h"

struct SequenceMatchResult {
    bool hasPrefix = false;
    bool hasExact = false;
    String sequenceName;
    String targetName;
    uint16_t timeoutMs = 700;
};

struct StartupActionConfig {
    bool enabled = false;
    String type = "none";
    String target;
    uint16_t delayMs = 1200;
};

class CommandManager {
public:
    CommandManager(Storage& storage);

    bool begin();

    bool saveCommand(const IRCommandData& command);
    bool getCommand(const String& name, IRCommandData& command) const;
    bool renameCommand(const String& oldName, const String& newName);
    
    bool deleteCommand(const String& name);    

    bool addMapping(const String& source, const String& target);
    bool getMapping(const String& source, String& target);
    bool addSequence(const String& name,
                     JsonArrayConst steps,
                     const String& target,
                     uint16_t timeoutMs,
                     bool enabled);
    bool removeSequence(const String& name);
    bool resolveSequence(const String* steps, size_t stepCount, SequenceMatchResult& result) const;
    bool commandParticipatesInSequences(const String& commandName) const;
    size_t maxSequenceLength() const;

    void listCommands();
    void listMappings();
    void listSequences();

    void exportCommands(JsonDocument& doc);
    void exportMappings(JsonDocument& doc);
    void exportSequences(JsonDocument& doc) const;
    void serializeCommandSummaries(Print& out) const;
    void serializeCommands(Print& out) const;
    void serializeMappings(Print& out) const;
    void serializeSequences(Print& out) const;
    void serializeCommandsAndMappings(Print& out) const;
    bool serializeCommandDetail(const String& name, Print& out) const;
    size_t commandCount() const;
    size_t mappingCount() const;
    size_t sequenceCount() const;
    bool removeMapping(const String& source);
    bool setLanguage(const String& language);
    String getLanguage() const;
    void getSettings(JsonDocument& doc) const;
    bool setStartupAction(const StartupActionConfig& config);
    void getStartupAction(JsonDocument& doc) const;
    StartupActionConfig getStartupAction() const;

    bool setNetworkConfig(const String& mode,
                      const String& ip,
                      const String& gateway,
                      const String& subnet);
    bool setDeviceState(const String& ssid,
                        const String& mac,
                        const String& status,
                        const String& ip,
                        const String& gateway,
                        const String& subnet,
                        int32_t rssi);

    void getNetworkConfig(JsonDocument& doc);
    void getDeviceState(JsonDocument& doc);
    bool importConfig(JsonDocument& doc);
    void exportConfig(JsonDocument& doc) const;
    bool importCommandsAndMappings(JsonDocument& doc);
    bool importCommandsAndMappingsJson(const String& json);
    void exportCommandsAndMappings(JsonDocument& doc) const;
    bool factoryReset();

    String getNameBySignature(const String& signature) const;
    String getNameByCommand(const IRCommandData& command) const;
    bool hasCommand(const String& name) const;
    bool resolveMappedCommand(const String& sourceName, IRCommandData& targetCommand, String& targetName) const;
    bool resolveMapping(const String& sourceName, String& targetName) const;
    bool getSequenceDefinition(const String& name, JsonDocument& doc) const;

private:
    Storage& _storage;
    JsonDocument _doc;

    void ensureSchema();
    bool isValidName(const String& name) const;
    bool isValidNetworkConfig(JsonObjectConst net) const;
    bool isValidDeviceState(JsonObjectConst device) const;
    bool isValidSequence(JsonObjectConst sequence) const;
    bool isValidStartupAction(JsonObjectConst startup) const;
    bool sequenceReferencesCommand(JsonObjectConst sequence, const String& commandName) const;
};
