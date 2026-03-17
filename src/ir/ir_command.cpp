/*
 * Module: IR command model
 * Responsibilities:
 * - derive stable command signatures for matching and deduplication
 * - serialize raw and decoded IR payloads for persistence and transport
 */

#include "ir_command.h"

namespace {
String toHexValue(uint64_t value) {
    char buffer[19];
    snprintf(buffer, sizeof(buffer), "%llX", value);
    return String(buffer);
}

uint32_t computeRawHash(const uint16_t* raw, uint16_t rawLength) {
    uint32_t hash = 2166136261UL;
    for (uint16_t i = 0; i < rawLength; ++i) {
        hash ^= raw[i];
        hash *= 16777619UL;
    }
    return hash;
}

void copyRawTimings(const decode_results& results, IRCommandData& out) {
    if (results.rawbuf == nullptr || results.rawlen <= 1) {
        out.hasRaw = false;
        out.rawLength = 0;
        return;
    }

    const uint16_t rawLength = min<uint16_t>(results.rawlen - 1, IR_RAW_MAX_SAMPLES);
    out.rawLength = rawLength;
    out.hasRaw = rawLength > 0;

    for (uint16_t i = 0; i < rawLength; ++i) {
        out.raw[i] = results.rawbuf[i + 1] * kRawTick;
    }
}
}

bool commandFromResults(const String& name, const decode_results& results, IRCommandData& out) {
    out = IRCommandData();
    out.name = name;
    out.protocol = results.decode_type;
    out.value = results.value;
    out.bits = results.bits;
    out.repeats = (results.decode_type == NEC) ? 2 : 1;
    copyRawTimings(results, out);

    if (out.protocol == UNKNOWN && !out.hasRaw) {
        return false;
    }

    return true;
}

String commandSignature(const IRCommandData& command) {
    if (command.protocol == UNKNOWN && command.hasRaw) {
        return String("raw:") + String(command.rawLength) + ":" + String(computeRawHash(command.raw, command.rawLength), HEX);
    }

    return String((int)command.protocol) + ":" + String(command.bits) + ":" + toHexValue(command.value);
}

String commandSignature(const decode_results& results) {
    IRCommandData command;
    if (!commandFromResults("", results, command)) {
        return "";
    }
    return commandSignature(command);
}

void commandToJson(const IRCommandData& command, JsonObject obj) {
    obj["protocol"] = (int)command.protocol;
    obj["value"] = String(command.value);
    obj["bits"] = command.bits;
    obj["signature"] = commandSignature(command);
    obj["frequency"] = command.frequency;
    obj["repeats"] = command.repeats;
    obj["hasRaw"] = command.hasRaw;

    if (command.hasRaw) {
        JsonArray raw = obj["raw"].to<JsonArray>();
        for (uint16_t i = 0; i < command.rawLength; ++i) {
            raw.add(command.raw[i]);
        }
    }
}

bool commandFromJson(const String& name, JsonVariantConst src, IRCommandData& out) {
    if (!src.is<JsonObjectConst>()) {
        return false;
    }

    JsonObjectConst obj = src.as<JsonObjectConst>();
    out = IRCommandData();
    out.name = name;
    out.protocol = (decode_type_t)(obj["protocol"] | (int)UNKNOWN);
    out.value = strtoull(String(obj["value"] | "0").c_str(), nullptr, 10);
    out.bits = obj["bits"] | 0;
    out.frequency = obj["frequency"] | IR_RAW_FREQUENCY;
    out.repeats = obj["repeats"] | ((out.protocol == NEC) ? 2 : 1);
    out.hasRaw = false;
    out.rawLength = 0;

    if (obj["raw"].is<JsonArrayConst>()) {
        JsonArrayConst raw = obj["raw"].as<JsonArrayConst>();
        out.rawLength = min<uint16_t>(raw.size(), IR_RAW_MAX_SAMPLES);
        for (uint16_t i = 0; i < out.rawLength; ++i) {
            out.raw[i] = raw[i] | 0;
        }
        out.hasRaw = out.rawLength > 0;
    }

    if (out.protocol == UNKNOWN && !out.hasRaw) {
        return false;
    }

    return out.bits > 0 || out.hasRaw;
}
