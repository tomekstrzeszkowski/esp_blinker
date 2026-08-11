#include "connectivity.h"
#include <Arduino.h>

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
        const unsigned long RECONNECT_INTERVAL = 10UL * 60UL * 1000UL; // 10 minutes
        if (millis() - lastAttempt >= RECONNECT_INTERVAL) {
            lastAttempt = millis();
            connectMqtt();
        }
        return;  // don't call mqtt.loop() while disconnected
    }
    mqtt.loop();
}

void Connectivity::publishState(bool on) {
    mqtt.publish(MQTT_CHANNEL_STATE, on ? "ON" : "OFF", true);  // retained
}

void Connectivity::connectWifi() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
}

void Connectivity::mqttCallback(char* topic, byte* payload, unsigned int length) {
    if (String(topic) == MQTT_CHANNEL_SET) {
        instance->onGpioCommand();
        instance->publishState(instance->getState());
    }
}
void Connectivity::connectMqtt() {
    if (mqtt.connected()) return;

    if (WiFi.status() != WL_CONNECTED) {
        return;
    }
    if (
        mqtt.connect(MQTT_CLIENT_NAME, mqttUser, mqttPass, MQTT_CHANNEL_STATE, 0, true, "OFFLINE")
    ) {
        mqtt.subscribe(MQTT_CHANNEL_SET);
    }
}

void Connectivity::waitForWifi() {
    unsigned short attempt = 5;
    while (WiFi.status() != WL_CONNECTED && attempt > 0) {
        delay(1000);
        attempt--;
    }
}