/*
 * Module: Firmware entrypoint
 * Responsibilities:
 * - initialize storage, IR IO, WiFi, and HTTP services
 * - run the bridge loop, serial maintenance commands, and heap diagnostics
 */

#include <Arduino.h>
#include <LittleFS.h>
#include "config.h"
#include "ir/ir_bridge_service.h"
#include "ir/ir_receiver.h"
#include "ir/ir_sender.h"
#include "ir/command_manager.h"
#include "storage/storage.h"
#include "wifi/wifi_manager.h"
#include "web/web_server.h"
#include "system/logger.h"
#include "system/i18n.h"

void handleSerialCommands();
void logBootDiagnostics();

IRReceiver irReceiver(IR_RECEIVE_PIN);
IRSender irSender(IR_SEND_PIN);
Storage storage;
CommandManager commandManager(storage);
IRBridgeService bridgeService(commandManager, irReceiver, irSender);
WiFiService wifiService;
WebServerService webServer(commandManager, bridgeService, wifiService);
extern bool pendingReboot;

unsigned long lastMemoryCheck = 0;
const unsigned long memoryInterval = 30000;
const uint32_t MIN_HEAP_WARNING = 12000;
const unsigned long rebootGracePeriod = 2200;

void setup() {
    Serial.begin(SERIAL_BAUD);
    Serial.setTimeout(25);
    delay(500);

    Serial.println("\n=== IR Bridge Boot ===");

    irReceiver.begin();
    irSender.begin();

    if (!storage.begin()) {
        Serial.println("LittleFS mount FAILED");
    } else {
        Serial.println("LittleFS mount SUCCESS");
    }

    commandManager.begin();
    bridgeService.begin();
    wifiService.begin();
    delay(500);
    webServer.begin();
    logBootDiagnostics();
}

void loop() {
    const unsigned long currentMillis = millis();
    if (currentMillis - lastMemoryCheck >= memoryInterval) {
        lastMemoryCheck = currentMillis;
        const uint32_t freeHeap = ESP.getFreeHeap();
        if (freeHeap < MIN_HEAP_WARNING) {
            Logger::log(String(I18n::pick("Low heap warning: ", "Aviso de heap bajo: ", "Ostrzezenie o niskim heapie: ")) + String(freeHeap) + String(I18n::pick(" bytes", " bytes", " bajtow")));
        }
    }

    handleSerialCommands();
    bridgeService.process();

    if (pendingReboot) {
        delay(rebootGracePeriod);
        ESP.restart();
    }
}

void handleSerialCommands() {
    if (!Serial.available()) {
        return;
    }

    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.startsWith("record ")) {
        String name = input.substring(7);
        name.trim();

        if (!bridgeService.startRecording(name)) {
            Logger::log(I18n::pick("Invalid name.", "Nombre inválido.", "Nieprawidlowa nazwa."));
            return;
        }

        Logger::log(I18n::pick("Waiting for IR signal (15s timeout)...", "Esperando señal IR (timeout de 15 s)...", "Oczekiwanie na sygnal IR (timeout 15 s)..."));
        return;
    }

    if (input == "list") {
        commandManager.listCommands();
        commandManager.listMappings();
        return;
    }

    if (input.startsWith("delete ")) {
        String name = input.substring(7);
        name.trim();

        if (!commandManager.deleteCommand(name)) {
            Logger::log(I18n::pick("Delete failed.", "Fallo al borrar.", "Usuwanie nie powiodlo sie."));
        }
        return;
    }

    if (input.startsWith("map ")) {
        const int firstSpace = input.indexOf(' ');
        const int secondSpace = input.indexOf(' ', firstSpace + 1);

        if (secondSpace == -1) {
            Logger::log(I18n::pick("Usage: map SOURCE_NAME TARGET_NAME", "Uso: map SOURCE_NAME TARGET_NAME", "Uzycie: map SOURCE_NAME TARGET_NAME"));
            return;
        }

        String source = input.substring(firstSpace + 1, secondSpace);
        String target = input.substring(secondSpace + 1);
        source.trim();
        target.trim();

        if (commandManager.addMapping(source, target)) {
            Logger::log(I18n::pick("Mapping saved!", "Mapeo guardado", "Zapisano mapowanie"));
        } else {
            Logger::log(I18n::pick("Error saving mapping.", "Error al guardar el mapeo.", "Blad zapisu mapowania."));
        }
        return;
    }

    if (input.startsWith("send ")) {
        String name = input.substring(5);
        name.trim();
        if (!bridgeService.executeCommand(name)) {
            Logger::log(I18n::pick("Send failed.", "Fallo al enviar.", "Wysylanie nie powiodlo sie."));
        }
        return;
    }

    if (input == "reboot") {
        Logger::log(I18n::pick("Rebooting cleanly...", "Reiniciando limpiamente...", "Bezpieczny restart..."));
        delay(100);
        ESP.restart();
    }

    Logger::log(I18n::pick("Unknown command.", "Comando desconocido.", "Nieznana komenda."));
}

void logBootDiagnostics() {
    Logger::log(String(I18n::pick("Flash chip size: ", "Tamaño de flash del chip: ", "Rozmiar flash ukladu: ")) + String(ESP.getFlashChipSize()));
    Logger::log(String(I18n::pick("Real flash size: ", "Tamaño real de flash: ", "Rzeczywisty rozmiar flash: ")) + String(ESP.getFlashChipRealSize()));

    FSInfo info;
    if (LittleFS.info(info)) {
        Logger::log(String(I18n::pick("FS total bytes: ", "Bytes totales del FS: ", "Calkowite bajty FS: ")) + String(info.totalBytes));
        Logger::log(String(I18n::pick("FS used bytes: ", "Bytes usados del FS: ", "Uzyte bajty FS: ")) + String(info.usedBytes));
    }

    Logger::log(String(I18n::pick("Free heap: ", "Heap libre: ", "Wolny heap: ")) + String(ESP.getFreeHeap()));
    Logger::log(String(I18n::pick("Max alloc heap: ", "Heap máximo asignable: ", "Maksymalny przydzial heap: ")) + String(ESP.getMaxFreeBlockSize()));
    Logger::log(String(I18n::pick("Heap fragmentation: ", "Fragmentación del heap: ", "Fragmentacja heap: ")) + String(ESP.getHeapFragmentation()) + "%");
}
