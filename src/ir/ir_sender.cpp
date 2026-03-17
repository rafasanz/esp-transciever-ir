/*
 * Module: IR sender wrapper
 * Responsibilities:
 * - map stored command metadata to the correct send routine
 * - log outgoing IR activity for debugging and verification
 */

#include "ir_sender.h"
#include <Arduino.h>
#include "system/logger.h"
#include "system/i18n.h"

IRSender::IRSender(uint16_t pin) : _irsend(pin) {}

void IRSender::begin() {
    _irsend.begin();
}

void IRSender::send(decode_type_t type, uint64_t value, uint16_t bits) {
    switch (type) {
        case NEC:
            Logger::log(I18n::pick("Sending IR as NEC", "Enviando IR como NEC", "Wysylanie IR jako NEC"));
            _irsend.sendNEC(value, bits);
            break;
        case SONY:
            Logger::log(I18n::pick("Sending IR as SONY", "Enviando IR como SONY", "Wysylanie IR jako SONY"));
            _irsend.sendSony(value, bits);
            break;
        case SAMSUNG:
            Logger::log(I18n::pick("Sending IR as SAMSUNG", "Enviando IR como SAMSUNG", "Wysylanie IR jako SAMSUNG"));
            _irsend.sendSAMSUNG(value, bits);
            break;
        default:
            Logger::log(I18n::pick("Sending IR with generic sender", "Enviando IR con emisor generico", "Wysylanie IR przez nadawce generycznego typu"));
            _irsend.send(type, value, bits);
            break;
    }
}

void IRSender::send(const IRCommandData& command) {
    if (command.hasRaw && (command.protocol == UNKNOWN || command.bits == 0)) {
        Logger::log(I18n::pick("Sending IR as RAW", "Enviando IR como RAW", "Wysylanie IR jako RAW"));
        _irsend.sendRaw(command.raw, command.rawLength, command.frequency);
        return;
    }

    const uint8_t repeats = max<uint8_t>(command.repeats, 1);
    for (uint8_t i = 0; i < repeats; ++i) {
        send(command.protocol, command.value, command.bits);
        if (i + 1 < repeats) {
            delay(40);
        }
    }
}
