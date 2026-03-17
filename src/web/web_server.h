/*
 * Module: HTTP API and embedded UI server
 * Responsibilities:
 * - expose the REST endpoints used by the frontend and automation clients
 * - serve the embedded web application directly from program memory
 */

#pragma once

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "ir/command_manager.h"
#include "ir/ir_bridge_service.h"
#include "wifi/wifi_manager.h"

class WebServerService {
public:
    WebServerService(CommandManager& manager, IRBridgeService& bridgeService, WiFiService& wifiService);
    void begin();

private:
    AsyncWebServer server;
    CommandManager& commandManager;
    IRBridgeService& bridgeService;
    WiFiService& wifiService;

    void setupRoutes();
    String templateProcessor(const String& var) const;
};
