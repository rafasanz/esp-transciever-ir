/*
 * Module: In-memory ring logger
 * Responsibilities:
 * - store timestamped log lines in a fixed buffer
 * - export the buffer in chronological order for HTTP clients
 */

#include "logger.h"
#include <cstdio>

char Logger::buffer[LOG_BUFFER_SIZE][LOG_LINE_LENGTH];
int Logger::index = 0;
bool Logger::wrapped = false;

void Logger::log(const String& msg) {
    snprintf(buffer[index], LOG_LINE_LENGTH, "[%lu] %s", millis(), msg.c_str());
    Serial.println(buffer[index]);

    ++index;
    if (index >= LOG_BUFFER_SIZE) {
        index = 0;
        wrapped = true;
    }
}

void Logger::exportLogs(String& out) {
    out = "";
    out.reserve(LOG_BUFFER_SIZE * (LOG_LINE_LENGTH / 2));

    if (!wrapped) {
        for (int i = 0; i < index; i++) {
            out += buffer[i];
            out += "\n";
        }
    } else {
        for (int i = index; i < LOG_BUFFER_SIZE; i++) {
            out += buffer[i];
            out += "\n";
        }
        for (int i = 0; i < index; i++) {
            out += buffer[i];
            out += "\n";
        }
    }
}
