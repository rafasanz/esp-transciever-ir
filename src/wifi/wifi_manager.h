/*
 * Module: WiFi connection service
 * Responsibilities:
 * - connect through WiFiManager using DHCP or stored static settings
 * - expose the current device network state to the rest of the firmware
 */

#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

class WiFiService {
public:
    void begin();
    bool isConnected();
    void exportDeviceState(JsonObject out) const;

private:
    String status = "not_connected";
    String ssid;
    String macAddress;
    String ip;
    String gateway;
    String subnet;
    int32_t rssi = 0;

    void captureDeviceState(const String& nextStatus);
};
