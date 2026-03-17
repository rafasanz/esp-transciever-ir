/*
 * Module: HTTP API and embedded UI server
 * Responsibilities:
 * - register the web routes for commands, mappings, sequences, and settings
 * - serve localized responses and the embedded frontend without filesystem reads
 */

#include "web_server.h"
#include <memory>
#include <ESP8266WiFi.h>
#include <LittleFS.h>
#include "system/logger.h"
#include "system/i18n.h"
#include "config.h"
#include "embedded_ui.h"

bool pendingReboot = false;

namespace {
void addNoStoreHeaders(AsyncWebServerResponse* response) {
    if (response == nullptr) {
        return;
    }

    response->addHeader("Cache-Control", "no-store, no-cache, must-revalidate");
    response->addHeader("Pragma", "no-cache");
    response->addHeader("Expires", "0");
}

bool isValidIpv4(const String& value) {
    IPAddress address;
    return address.fromString(value);
}

void sendJson(AsyncWebServerRequest* request, const JsonDocument& doc) {
    AsyncResponseStream* response = request->beginResponseStream("application/json");
    addNoStoreHeaders(response);
    serializeJson(doc, *response);
    request->send(response);
}

void sendText(AsyncWebServerRequest* request, int code, const char* en, const char* es, const char* pl) {
    AsyncWebServerResponse* response = request->beginResponse(code, "text/plain", I18n::pick(en, es, pl));
    addNoStoreHeaders(response);
    request->send(response);
}

}

WebServerService::WebServerService(CommandManager& manager, IRBridgeService& bridge, WiFiService& wifi)
    : server(80), commandManager(manager), bridgeService(bridge), wifiService(wifi) {}

String WebServerService::templateProcessor(const String& var) const {
    if (var == "APP_VERSION") {
        return VERSION;
    }
    return "";
}

void WebServerService::begin() {
    setupRoutes();
    server.begin();
    Logger::log(I18n::pick("Web server started", "Servidor web iniciado", "Serwer WWW uruchomiony"));
}

void WebServerService::setupRoutes() {
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest* request) {
        AsyncWebServerResponse* response = request->beginResponse_P(200, "text/html", EMBEDDED_INDEX_HTML,
            [this](const String& var) {
                return templateProcessor(var);
            });
        addNoStoreHeaders(response);
        request->send(response);
    });

    server.on("/app.css", HTTP_GET, [](AsyncWebServerRequest* request) {
        AsyncWebServerResponse* response = request->beginResponse_P(200, "text/css", EMBEDDED_APP_CSS);
        addNoStoreHeaders(response);
        request->send(response);
    });

    server.on("/app.js", HTTP_GET, [](AsyncWebServerRequest* request) {
        AsyncWebServerResponse* response = request->beginResponse_P(200, "application/javascript", EMBEDDED_APP_JS);
        addNoStoreHeaders(response);
        request->send(response);
    });

    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest* request) {
        static const char kFavicon[] PROGMEM =
            "<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 64 64'>"
            "<rect width='64' height='64' rx='16' fill='%23152124'/>"
            "<path d='M20 18h8v28h-8zm16 0c9 0 16 6 16 14s-7 14-16 14h-4v-8h4c4 0 8-2 8-6s-4-6-8-6h-4v-8z' fill='%2356c5bc'/>"
            "</svg>";
        AsyncWebServerResponse* response = request->beginResponse_P(200, "image/svg+xml", kFavicon);
        addNoStoreHeaders(response);
        request->send(response);
    });

    server.on("/api/commands", HTTP_GET, [this](AsyncWebServerRequest* request) {
        AsyncResponseStream* response = request->beginResponseStream("application/json");
        addNoStoreHeaders(response);
        commandManager.serializeCommandSummaries(*response);
        request->send(response);
    });

    server.on("/api/command", HTTP_GET, [this](AsyncWebServerRequest* request) {
        if (!request->hasParam("name")) {
            sendText(request, 400, "Missing name", "Falta el nombre", "Brak nazwy");
            return;
        }

        String name = request->getParam("name")->value();
        name.trim();
        if (!commandManager.hasCommand(name)) {
            sendText(request, 404, "Command not found", "Comando no encontrado", "Nie znaleziono komendy");
            return;
        }

        AsyncResponseStream* response = request->beginResponseStream("application/json");
        addNoStoreHeaders(response);
        commandManager.serializeCommandDetail(name, *response);
        request->send(response);
    });

    server.on("/api/mappings", HTTP_GET, [this](AsyncWebServerRequest* request) {
        AsyncResponseStream* response = request->beginResponseStream("application/json");
        addNoStoreHeaders(response);
        commandManager.serializeMappings(*response);
        request->send(response);
    });

    server.on("/api/sequences", HTTP_GET, [this](AsyncWebServerRequest* request) {
        AsyncResponseStream* response = request->beginResponseStream("application/json");
        addNoStoreHeaders(response);
        commandManager.serializeSequences(*response);
        request->send(response);
    });

    auto uiStateHandler = [this](AsyncWebServerRequest* request) {
        JsonDocument doc;
        doc["version"] = VERSION;
        doc["ip"] = WiFi.localIP().toString();
        doc["heap"] = ESP.getFreeHeap();
        doc["maxBlock"] = ESP.getMaxFreeBlockSize();
        doc["fragmentation"] = ESP.getHeapFragmentation();
        doc["uptimeMs"] = millis();
        JsonObject counts = doc["counts"].to<JsonObject>();
        counts["commands"] = commandManager.commandCount();
        counts["mappings"] = commandManager.mappingCount();
        counts["sequences"] = commandManager.sequenceCount();
        bridgeService.exportStatus(doc["bridge"].to<JsonObject>());

        JsonObject device = doc["device"].to<JsonObject>();
        wifiService.exportDeviceState(device);

        JsonDocument networkDoc;
        commandManager.getNetworkConfig(networkDoc);
        doc["network"] = networkDoc["network"];
        device["mode"] = networkDoc["network"]["mode"] | "dhcp";

        JsonDocument settingsDoc;
        commandManager.getSettings(settingsDoc);
        doc["settings"] = settingsDoc["settings"];

        sendJson(request, doc);
    };
    server.on("/api/ui-state", HTTP_GET, uiStateHandler);
    server.on("/api/bootstrap", HTTP_GET, uiStateHandler);

    server.on("/api/status", HTTP_GET, [this](AsyncWebServerRequest* request) {
        JsonDocument doc;
        doc["version"] = VERSION;
        doc["ip"] = WiFi.localIP().toString();
        doc["heap"] = ESP.getFreeHeap();
        doc["maxBlock"] = ESP.getMaxFreeBlockSize();
        doc["fragmentation"] = ESP.getHeapFragmentation();
        doc["uptimeMs"] = millis();
        JsonObject counts = doc["counts"].to<JsonObject>();
        counts["commands"] = commandManager.commandCount();
        counts["mappings"] = commandManager.mappingCount();
        counts["sequences"] = commandManager.sequenceCount();
        bridgeService.exportStatus(doc["bridge"].to<JsonObject>());
        sendJson(request, doc);
    });

    server.on("/api/system", HTTP_GET, [this](AsyncWebServerRequest* request) {
        JsonDocument doc;
        JsonObject device = doc["device"].to<JsonObject>();
        wifiService.exportDeviceState(device);

        JsonDocument networkDoc;
        commandManager.getNetworkConfig(networkDoc);
        device["mode"] = networkDoc["network"]["mode"] | "dhcp";

        sendJson(request, doc);
    });

    server.on("/api/network", HTTP_GET, [this](AsyncWebServerRequest* request) {
        JsonDocument doc;
        commandManager.getNetworkConfig(doc);
        sendJson(request, doc);
    });

    server.on("/api/settings", HTTP_GET, [this](AsyncWebServerRequest* request) {
        JsonDocument doc;
        commandManager.getSettings(doc);
        sendJson(request, doc);
    });

    server.on("/api/settings", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (!request->hasParam("language", true)) {
            sendText(request, 400, "Missing language", "Falta el idioma", "Brak jezyka");
            return;
        }

        String language = request->getParam("language", true)->value();
        language.trim();
        if (!commandManager.setLanguage(language)) {
            sendText(request, 400, "Invalid language", "Idioma inválido", "Nieprawidlowy jezyk");
            return;
        }

        sendText(request, 200, "Language saved", "Idioma guardado", "Zapisano jezyk");
    });

    server.on("/api/startup", HTTP_GET, [this](AsyncWebServerRequest* request) {
        JsonDocument doc;
        commandManager.getStartupAction(doc);
        sendJson(request, doc);
    });

    server.on("/api/startup", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (!request->hasParam("type", true)) {
            sendText(request, 400, "Missing type", "Falta el tipo", "Brak typu");
            return;
        }

        StartupActionConfig config;
        config.type = request->getParam("type", true)->value();
        config.type.trim();
        config.target = request->hasParam("target", true) ? request->getParam("target", true)->value() : "";
        config.target.trim();
        config.enabled = !request->hasParam("enabled", true) ||
            request->getParam("enabled", true)->value() != "false";
        config.delayMs = request->hasParam("delay_ms", true)
            ? static_cast<uint16_t>(request->getParam("delay_ms", true)->value().toInt())
            : 1200;

        if (config.type == "none") {
            config.enabled = false;
            config.target = "";
        }

        if (!commandManager.setStartupAction(config)) {
            sendText(request, 400, "Startup save failed", "Fallo al guardar el arranque", "Nie udalo sie zapisac startu");
            return;
        }

        bridgeService.refreshStartupAction();
        sendText(request, 200, "Startup action saved", "Acción de arranque guardada", "Zapisano akcje startowa");
    });

    server.on("/api/network", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (!request->hasParam("mode", true)) {
            sendText(request, 400, "Missing mode", "Falta el modo", "Brak trybu");
            return;
        }

        String mode = request->getParam("mode", true)->value();
        String ip = request->hasParam("ip", true) ? request->getParam("ip", true)->value() : "";
        String gateway = request->hasParam("gateway", true) ? request->getParam("gateway", true)->value() : "";
        String subnet = request->hasParam("subnet", true) ? request->getParam("subnet", true)->value() : "";
        mode.trim();
        ip.trim();
        gateway.trim();
        subnet.trim();

        if (mode == "static" &&
            (!isValidIpv4(ip) || !isValidIpv4(gateway) || !isValidIpv4(subnet))) {
            sendText(request, 400, "Invalid static network", "Red estática inválida", "Nieprawidlowa siec statyczna");
            return;
        }

        if (!commandManager.setNetworkConfig(mode, ip, gateway, subnet)) {
            sendText(request, 500, "Save failed", "Fallo al guardar", "Nie udalo sie zapisac");
            return;
        }

        pendingReboot = true;
        sendText(request, 200, "Saved. Device will reboot.", "Guardado. El dispositivo se reiniciará.", "Zapisano. Urzadzenie uruchomi sie ponownie.");
    });

    server.on("/api/record", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (!request->hasParam("name", true)) {
            sendText(request, 400, "Missing name", "Falta el nombre", "Brak nazwy");
            return;
        }

        String name = request->getParam("name", true)->value();
        name.trim();

        if (!bridgeService.startRecording(name)) {
            sendText(request, 400, "Invalid name", "Nombre inválido", "Nieprawidlowa nazwa");
            return;
        }

        sendText(request, 200, "Recording started", "Grabación iniciada", "Rozpoczeto nagrywanie");
    });

    server.on("/api/delete", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (!request->hasParam("name", true)) {
            sendText(request, 400, "Missing name", "Falta el nombre", "Brak nazwy");
            return;
        }

        String name = request->getParam("name", true)->value();
        name.trim();

        if (!commandManager.deleteCommand(name)) {
            sendText(request, 400, "Delete failed", "Fallo al borrar", "Nie udalo sie usunac");
            return;
        }

        bridgeService.refreshStartupAction();
        sendText(request, 200, "Deleted", "Borrado", "Usunieto");
    });

    server.on("/api/rename", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (!request->hasParam("old_name", true) || !request->hasParam("new_name", true)) {
            sendText(request, 400, "Missing params", "Faltan parámetros", "Brak parametrow");
            return;
        }

        String oldName = request->getParam("old_name", true)->value();
        String newName = request->getParam("new_name", true)->value();
        oldName.trim();
        newName.trim();

        if (!commandManager.renameCommand(oldName, newName)) {
            sendText(request, 400, "Rename failed", "Fallo al renombrar", "Nie udalo sie zmienic nazwy");
            return;
        }

        bridgeService.refreshStartupAction();
        sendText(request, 200, "Renamed", "Renombrado", "Zmieniono nazwe");
    });

    server.on("/api/map", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (!request->hasParam("source", true) || !request->hasParam("target", true)) {
            sendText(request, 400, "Missing params", "Faltan parámetros", "Brak parametrow");
            return;
        }

        String source = request->getParam("source", true)->value();
        String target = request->getParam("target", true)->value();
        source.trim();
        target.trim();

        if (!commandManager.addMapping(source, target)) {
            sendText(request, 400, "Mapping save failed", "Fallo al guardar el mapeo", "Nie udalo sie zapisac mapowania");
            return;
        }

        sendText(request, 200, "Mapping saved", "Mapeo guardado", "Zapisano mapowanie");
    });

    server.on("/api/unmap", HTTP_POST, [this](AsyncWebServerRequest* request) {
        String source;

        if (request->hasParam("source", true)) {
            source = request->getParam("source", true)->value();
        } else if (request->hasParam("source")) {
            source = request->getParam("source")->value();
        } else {
            sendText(request, 400, "Missing source", "Falta el origen", "Brak zrodla");
            return;
        }

        source.trim();

        if (!commandManager.removeMapping(source)) {
            sendText(request, 400, "Remove failed", "Fallo al eliminar", "Nie udalo sie usunac");
            return;
        }

        sendText(request, 200, "Mapping removed", "Mapeo eliminado", "Usunieto mapowanie");
    });

    server.on("/api/sequence", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (!request->hasParam("name", true) ||
            !request->hasParam("target", true) ||
            !request->hasParam("steps", true)) {
            sendText(request, 400, "Missing params", "Faltan parámetros", "Brak parametrow");
            return;
        }

        const String name = request->getParam("name", true)->value();
        const String target = request->getParam("target", true)->value();
        const uint16_t timeoutMs = request->hasParam("timeout_ms", true)
            ? static_cast<uint16_t>(request->getParam("timeout_ms", true)->value().toInt())
            : 800;
        const bool enabled = !request->hasParam("enabled", true) ||
            request->getParam("enabled", true)->value() != "false";

        JsonDocument stepsDoc;
        if (deserializeJson(stepsDoc, request->getParam("steps", true)->value()) != DeserializationError::Ok ||
            !stepsDoc.is<JsonArrayConst>()) {
            sendText(request, 400, "Invalid steps", "Pasos inválidos", "Nieprawidlowe kroki");
            return;
        }

        if (!commandManager.addSequence(name, stepsDoc.as<JsonArrayConst>(), target, timeoutMs, enabled)) {
            sendText(request, 400, "Sequence save failed", "Fallo al guardar la secuencia", "Nie udalo sie zapisac sekwencji");
            return;
        }

        sendText(request, 200, "Sequence saved", "Secuencia guardada", "Zapisano sekwencje");
    });

    server.on("/api/delete-sequence", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (!request->hasParam("name", true)) {
            sendText(request, 400, "Missing name", "Falta el nombre", "Brak nazwy");
            return;
        }

        String name = request->getParam("name", true)->value();
        name.trim();
        if (!commandManager.removeSequence(name)) {
            sendText(request, 400, "Sequence delete failed", "Fallo al borrar la secuencia", "Nie udalo sie usunac sekwencji");
            return;
        }

        bridgeService.refreshStartupAction();
        sendText(request, 200, "Sequence deleted", "Secuencia eliminada", "Usunieto sekwencje");
    });

    server.on("/api/resolve", HTTP_GET, [this](AsyncWebServerRequest* request) {
        if (!request->hasParam("source")) {
            sendText(request, 400, "Missing source", "Falta el origen", "Brak zrodla");
            return;
        }

        String source = request->getParam("source")->value();
        source.trim();

        JsonDocument doc;
        doc["source"] = source;

        String targetName;
        IRCommandData targetCommand;
        if (commandManager.resolveMappedCommand(source, targetCommand, targetName)) {
            doc["mapped"] = true;
            doc["target"] = targetName;
            JsonObject cmd = doc["command"].to<JsonObject>();
            commandToJson(targetCommand, cmd);
        } else {
            doc["mapped"] = false;
        }

        sendJson(request, doc);
    });

    server.on("/api/update", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (!request->hasParam("name", true) ||
            !request->hasParam("protocol", true) ||
            !request->hasParam("value", true) ||
            !request->hasParam("bits", true)) {
            sendText(request, 400, "Missing params", "Faltan parámetros", "Brak parametrow");
            return;
        }

        IRCommandData command;
        command.name = request->getParam("name", true)->value();
        command.name.trim();
        command.protocol = (decode_type_t)request->getParam("protocol", true)->value().toInt();
        command.value = strtoull(request->getParam("value", true)->value().c_str(), nullptr, 10);
        command.bits = (uint16_t)request->getParam("bits", true)->value().toInt();
        command.frequency = request->hasParam("frequency", true) ? (uint16_t)request->getParam("frequency", true)->value().toInt() : IR_RAW_FREQUENCY;
        command.repeats = request->hasParam("repeats", true) ? (uint8_t)request->getParam("repeats", true)->value().toInt() : ((command.protocol == NEC) ? 2 : 1);

        if (request->hasParam("raw", true)) {
            JsonDocument rawDoc;
            if (deserializeJson(rawDoc, request->getParam("raw", true)->value()) == DeserializationError::Ok &&
                rawDoc.is<JsonArray>()) {
                JsonArray raw = rawDoc.as<JsonArray>();
                command.rawLength = min<uint16_t>(raw.size(), IR_RAW_MAX_SAMPLES);
                for (uint16_t i = 0; i < command.rawLength; ++i) {
                    command.raw[i] = raw[i] | 0;
                }
                command.hasRaw = command.rawLength > 0;
            }
        }

        if (!commandManager.saveCommand(command)) {
            sendText(request, 500, "Update failed", "Fallo al actualizar", "Nie udalo sie zaktualizowac");
            return;
        }

        sendText(request, 200, "Updated", "Actualizado", "Zaktualizowano");
    });

    server.on("/api/execute", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (!request->hasParam("name", true)) {
            sendText(request, 400, "Missing name", "Falta el nombre", "Brak nazwy");
            return;
        }

        String name = request->getParam("name", true)->value();
        name.trim();

        if (!bridgeService.executeCommand(name)) {
            sendText(request, 404, "Command not found", "Comando no encontrado", "Nie znaleziono komendy");
            return;
        }

        sendText(request, 200, "Executed", "Ejecutado", "Wykonano");
    });

    server.on("/api/export", HTTP_GET, [this](AsyncWebServerRequest* request) {
        const String scope = request->hasParam("scope") ? request->getParam("scope")->value() : "all";
        AsyncResponseStream* response = request->beginResponseStream("application/json");
        addNoStoreHeaders(response);
        if (scope == "commands") {
            commandManager.serializeCommands(*response);
        } else if (scope == "mappings") {
            commandManager.serializeMappings(*response);
        } else if (scope == "sequences") {
            commandManager.serializeSequences(*response);
        } else {
            commandManager.serializeCommandsAndMappings(*response);
        }
        request->send(response);
    });

    server.on(
        "/api/import",
        HTTP_POST,
        [this](AsyncWebServerRequest* request) {
            if (request->_tempObject == nullptr) {
                sendText(request, 400, "Missing payload", "Falta el payload", "Brak ladunku");
                return;
            }

            std::unique_ptr<String> body(reinterpret_cast<String*>(request->_tempObject));
            request->_tempObject = nullptr;

            if (!commandManager.importCommandsAndMappingsJson(*body)) {
                sendText(request, 400, "Import failed", "Fallo en la importación", "Import nie powiodl sie");
                return;
            }

            bridgeService.refreshStartupAction();
            sendText(request, 200, "Imported", "Importado", "Zaimportowano");
        },
        nullptr,
        [](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
            String* body = reinterpret_cast<String*>(request->_tempObject);
            if (index == 0) {
                body = new String();
                body->reserve(total);
                request->_tempObject = body;
            }

            if (body != nullptr) {
                body->concat(reinterpret_cast<const char*>(data), len);
            }
        });

    server.on("/api/factory", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (!commandManager.factoryReset()) {
            sendText(request, 500, "Factory reset failed", "Fallo en el reseteo de fábrica", "Reset fabryczny nie udal sie");
            return;
        }

        pendingReboot = true;
        sendText(request, 200, "Factory reset. Rebooting...", "Reseteo de fábrica. Reiniciando...", "Reset fabryczny. Ponowne uruchamianie...");
    });

    server.on("/api/logs", HTTP_GET, [](AsyncWebServerRequest* request) {
        String logs;
        Logger::exportLogs(logs);
        AsyncWebServerResponse* response = request->beginResponse(200, "text/plain", logs);
        addNoStoreHeaders(response);
        request->send(response);
    });
    server.onNotFound([](AsyncWebServerRequest* request) {
        sendText(request, 404, "Not found", "No encontrado", "Nie znaleziono");
    });
}
