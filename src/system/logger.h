/*
 * Module: In-memory ring logger
 * Responsibilities:
 * - collect short diagnostic lines for serial output and the web UI
 * - keep a fixed-size rolling buffer to avoid heap growth
 */

#pragma once

#include <Arduino.h>

#ifndef LOG_BUFFER_SIZE
#define LOG_BUFFER_SIZE 32
#endif

#ifndef LOG_LINE_LENGTH
#define LOG_LINE_LENGTH 88
#endif

class Logger {
public:
    static void log(const String& msg);
    static void exportLogs(String& out);

private:
    static char buffer[LOG_BUFFER_SIZE][LOG_LINE_LENGTH];
    static int index;
    static bool wrapped;
};
