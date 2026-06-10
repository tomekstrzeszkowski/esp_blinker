#pragma once
#include <PubSubClient.h>

#ifdef ESP8266
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#else
  #error "Unknown platform — add your target here"
#endif

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