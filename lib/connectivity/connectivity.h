#pragma once
#include <WiFi.h>
#include <PubSubClient.h>

class Connectivity {
public:
    Connectivity(const char* ssid, const char* pass,
                 const char* mqttHost, const char* mqttUser, const char* mqttPass,
                 std::function<void()> onGpio4Command, 
                 std::function<bool()> getState
                );
    void setup();
    void tick();
    void publishState(bool on);
    std::function<bool()> getState;

private:
    const char* ssid;
    const char* pass;
    const char* mqttHost;
    const char* mqttUser;
    const char* mqttPass;
    std::function<void()> onGpio4Command;

    WiFiClient wifiClient;
    PubSubClient mqtt;

    void connectWifi();
    void connectMqtt();
    static void mqttCallback(char* topic, byte* payload, unsigned int length);
    static Connectivity* instance;  // for static callback
};