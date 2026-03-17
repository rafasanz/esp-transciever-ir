/*
 * Module: IR sender wrapper
 * Responsibilities:
 * - abstract the protocol-specific send calls from IRremoteESP8266
 * - replay stored commands, including raw payloads and repeat counts
 */

#pragma once

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include "ir_command.h"

class IRSender {
public:
    explicit IRSender(uint16_t pin);
    void begin();
    void send(decode_type_t type, uint64_t value, uint16_t bits);
    void send(const IRCommandData& command);

private:
    IRsend _irsend;
};
