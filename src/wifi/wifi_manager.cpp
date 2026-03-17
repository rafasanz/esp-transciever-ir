/*
 * Module: WiFi connection service
 * Responsibilities:
 * - apply persisted network settings before auto-connect
 * - capture the current SSID, MAC, IP, and signal state for the web API
 */

#include "wifi_manager.h"
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ArduinoJson.h>
#include "ir/command_manager.h"
#include "system/logger.h"
#include "system/i18n.h"

extern CommandManager commandManager;

WiFiManager wm;

namespace {
void applyDeviceState(JsonObject out,
                      const String& status,
                      const String& ssid,
                      const String& macAddress,
                      const String& ip,
                      const String& gateway,
                      const String& subnet,
                      int32_t rssi) {
    out["status"] = status;
    out["ssid"] = ssid;
    out["mac_device"] = macAddress;
    out["ip"] = ip;
    out["gateway"] = gateway;
    out["subnet"] = subnet;
    out["rssi"] = rssi;
}
}

void WiFiService::captureDeviceState(const String& nextStatus) {
    const bool connected = WiFi.status() == WL_CONNECTED;
    status = nextStatus;
    ssid = connected ? WiFi.SSID() : "";
    macAddress = WiFi.macAddress();
    ip = connected ? WiFi.localIP().toString() : "";
    gateway = connected ? WiFi.gatewayIP().toString() : "";
    subnet = connected ? WiFi.subnetMask().toString() : "";
    rssi = connected ? WiFi.RSSI() : 0;
}

void WiFiService::begin() {
    Logger::log(I18n::pick("Connecting to WiFi...", "Conectando al WiFi...", "Laczenie z WiFi..."));
    WiFi.persistent(false);
    WiFi.mode(WIFI_STA);

    JsonDocument doc;
    commandManager.getNetworkConfig(doc);

    if (doc["network"].is<JsonObject>()) {
        JsonObject net = doc["network"];

        if (net["mode"].is<const char*>() && String(net["mode"]) == "static") {
            Logger::log(I18n::pick("Static mode detected", "Modo estático detectado", "Wykryto tryb statyczny"));

            String ipStr      = net["ip"]      | "";
            String gatewayStr = net["gateway"] | "";
            String subnetStr  = net["subnet"]  | "";

            if (ipStr.length() > 6 &&
                gatewayStr.length() > 6 &&
                subnetStr.length() > 6) {

                IPAddress ip, gateway, subnet;

                if (ip.fromString(ipStr) &&
                    gateway.fromString(gatewayStr) &&
                    subnet.fromString(subnetStr)) {

                    if (WiFi.config(ip, gateway, subnet)) {
                        Logger::log(I18n::pick("Static IP applied", "IP estática aplicada", "Zastosowano statyczny adres IP"));
                    } else {
                        Logger::log(I18n::pick("WiFi.config FAILED", "WiFi.config FALLO", "WiFi.config NIE POWIODL SIE"));
                    }

                } else {
                    Logger::log(I18n::pick("Invalid IP format. Ignoring static config.", "Formato IP inválido. Se ignora la configuración estática.", "Nieprawidlowy format IP. Ignoruje konfiguracje statyczna."));
                }
            } else {
                Logger::log(I18n::pick("Static config incomplete. Using DHCP.", "Configuración estática incompleta. Se usa DHCP.", "Niepelna konfiguracja statyczna. Uzywam DHCP."));
            }
        } else {
            Logger::log(I18n::pick("DHCP mode", "Modo DHCP", "Tryb DHCP"));
        }
    } else {
        Logger::log(I18n::pick("No network config found. Using DHCP.", "No se encontró configuración de red. Se usa DHCP.", "Nie znaleziono konfiguracji sieci. Uzywam DHCP."));
    }

    captureDeviceState("not_connected");

    wm.setWiFiAutoReconnect(true);
    wm.setConfigPortalTimeout(180);

    if (!wm.autoConnect("IR-Bridge-Setup")) {
        Logger::log(I18n::pick("WiFi failed. Restarting...", "Fallo de WiFi. Reiniciando...", "Polaczenie WiFi nie powiodlo sie. Restart..."));
        delay(2000);
        ESP.restart();
    }

    Logger::log(I18n::pick("WiFi connected!", "WiFi conectado", "Polaczono z WiFi"));
    Logger::log(String(I18n::pick("IP: ", "IP: ", "IP: ")) + WiFi.localIP().toString());
    captureDeviceState("connected");
}

bool WiFiService::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiService::exportDeviceState(JsonObject out) const {
    applyDeviceState(out, status, ssid, macAddress, ip, gateway, subnet, rssi);
}
