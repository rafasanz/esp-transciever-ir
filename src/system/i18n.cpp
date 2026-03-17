/*
 * Module: Runtime language selector
 * Responsibilities:
 * - switch the current language code used by the firmware
 * - provide lightweight text selection without dynamic allocation
 */

#include "i18n.h"

namespace {
enum class Language {
    English,
    Spanish,
    Polish,
};

Language currentLanguage = Language::English;
}

namespace I18n {

bool setLanguage(const String& code) {
    if (code == "en") {
        currentLanguage = Language::English;
        return true;
    }
    if (code == "es") {
        currentLanguage = Language::Spanish;
        return true;
    }
    if (code == "pl") {
        currentLanguage = Language::Polish;
        return true;
    }
    return false;
}

const char* getLanguage() {
    switch (currentLanguage) {
        case Language::Spanish:
            return "es";
        case Language::Polish:
            return "pl";
        case Language::English:
        default:
            return "en";
    }
}

const char* pick(const char* en, const char* es, const char* pl) {
    switch (currentLanguage) {
        case Language::Spanish:
            return es;
        case Language::Polish:
            return pl;
        case Language::English:
        default:
            return en;
    }
}

}
