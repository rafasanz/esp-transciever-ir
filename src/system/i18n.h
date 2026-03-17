/*
 * Module: Runtime language selector
 * Responsibilities:
 * - store the active UI language for backend messages
 * - pick localized strings for English, Spanish, and Polish
 */

#pragma once

#include <Arduino.h>

namespace I18n {

bool setLanguage(const String& code);
const char* getLanguage();
const char* pick(const char* en, const char* es, const char* pl);

}
