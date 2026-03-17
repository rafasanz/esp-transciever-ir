/*
 * Module: IR command model
 * Responsibilities:
 * - define the normalized command structure used across the firmware
 * - convert decoded IR frames to and from JSON persistence
 */

#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>

#ifndef IR_RAW_MAX_SAMPLES
#define IR_RAW_MAX_SAMPLES 100
#endif

#ifndef IR_RAW_FREQUENCY
#define IR_RAW_FREQUENCY 38
#endif

struct IRCommandData {
    String name;
    decode_type_t protocol = UNKNOWN;
    uint64_t value = 0;
    uint16_t bits = 0;
    uint16_t raw[IR_RAW_MAX_SAMPLES] = {0};
    uint16_t rawLength = 0;
    uint16_t frequency = IR_RAW_FREQUENCY;
    uint8_t repeats = 1;
    bool hasRaw = false;
};

bool commandFromResults(const String& name, const decode_results& results, IRCommandData& out);
String commandSignature(const IRCommandData& command);
String commandSignature(const decode_results& results);
void commandToJson(const IRCommandData& command, JsonObject obj);
bool commandFromJson(const String& name, JsonVariantConst src, IRCommandData& out);
