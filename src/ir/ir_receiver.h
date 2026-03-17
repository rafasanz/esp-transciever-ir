/*
 * Module: IR receiver wrapper
 * Responsibilities:
 * - encapsulate the IRremoteESP8266 receive object
 * - expose a small polling API for the bridge service
 */

#pragma once

#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

class IRReceiver {
public:
    explicit IRReceiver(uint16_t pin);
    void begin();
    bool available();
    decode_results* getResults();
    void resume();

private:
    IRrecv _irrecv;
    decode_results _results;
};
