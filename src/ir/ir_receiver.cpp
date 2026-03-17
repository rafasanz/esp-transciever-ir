/*
 * Module: IR receiver wrapper
 * Responsibilities:
 * - start the receiver hardware
 * - expose decoded results and resume capture after processing
 */

#include "ir_receiver.h"

IRReceiver::IRReceiver(uint16_t pin) : _irrecv(pin), _results{} {}

void IRReceiver::begin() {
    _irrecv.enableIRIn();
}

bool IRReceiver::available() {
    return _irrecv.decode(&_results);
}

decode_results* IRReceiver::getResults() {
    return &_results;
}

void IRReceiver::resume() {
    _irrecv.resume();
}
