/*
 * Module: Persistent JSON storage
 * Responsibilities:
 * - mount and format LittleFS when needed
 * - load, save, and fully wipe the project data file
 */

#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

class Storage {
public:
    bool begin();
    bool load(JsonDocument& doc);
    bool save(JsonDocument& doc);
    bool wipe();
};
