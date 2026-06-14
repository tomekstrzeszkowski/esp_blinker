#include "connectivity.h"
#include <Arduino.h>

#define STATE_CHANNEL "esp32/gpio4/state"
#define SET_CHANNEL "esp32/gpio4/set"

Connectivity* Connectivity::instance = nullptr;

Connectivity::Connectivity(
    const char* ssid, 
    const char* pass,          
    const char* mqttHost, 
    const char* mqttUser, 
    const char* mqttPass,                   
    std::function<void()> onGpioCommand, 
    std::function<bool()> getState
)
    : ssid(ssid), pass(pass), mqttHost(mqttHost),
      mqttUser(mqttUser), mqttPass(mqttPass),
      onGpioCommand(onGpioCommand), getState(getState), mqtt(wifiClient)
{
    instance = this;
}

void Connectivity::setup() {
    connectWifi();
    mqtt.setServer(mqttHost, 1883);
    mqtt.setCallback(mqttCallback);
    connectMqtt();
}

void Connectivity::tick() {
    if (!mqtt.connected()) {
        static unsigned long lastAttempt = 0;
        if (millis() - lastAttempt > 3000) {
            lastAttempt = millis();
            connectMqtt();
        }
        return;  // don't call mqtt.loop() while disconnected
    }
    mqtt.loop();
}

void Connectivity::publishState(bool on) {
    mqtt.publish(STATE_CHANNEL, on ? "ON" : "OFF", true);  // retained
}

void Connectivity::connectWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
}

void Connectivity::mqttCallback(char* topic, byte* payload, unsigned int length) {
    if (String(topic) == SET_CHANNEL) {
        instance->onGpioCommand();
        instance->publishState(instance->getState());
    }
}
void Connectivity::connectMqtt() {
    if (mqtt.connected()) return;

    if (WiFi.status() != WL_CONNECTED) {
        return;
    }
    if (mqtt.connect("esp32-s3-blinker", mqttUser, mqttPass,
                     STATE_CHANNEL, 0, true, "OFFLINE")) {
        mqtt.subscribe(SET_CHANNEL);
    }
}

void Connectivity::waitForWifi() {
    unsigned short attempt = 5;
    while (WiFi.status() != WL_CONNECTED && attempt > 0) {
        delay(1000);
        attempt--;
    }
}