/*
 * Module: Global build configuration
 * Responsibilities:
 * - expose compile-time constants required by the firmware
 * - fail fast when required build flags are missing
 */

#pragma once

#ifndef SERIAL_BAUD
#define SERIAL_BAUD 115200
#endif

#ifndef IR_SEND_PIN
#error "IR_SEND_PIN must be defined in platformio.ini build_flags"
#endif

#ifndef IR_RECEIVE_PIN
#error "IR_RECEIVE_PIN must be defined in platformio.ini build_flags"
#endif

#ifndef VERSION
#error "VERSION must be defined in platformio.ini build_flags"
#endif

#define IR_LED_FEEDBACK false

#define STORAGE_FILE "/ir_data.json"
