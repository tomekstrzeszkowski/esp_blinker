#include "connectivity.h"
#include <Arduino.h>

Connectivity* Connectivity::instance = nullptr;

Connectivity::Connectivity(
    const char* ssid, 
    const char* pass,          
    const char* mqttHost, 
    const char* mqttUser, 
    const char* mqttPass,                   
    std::function<void()> onGpio4Command, 
    std::function<bool()> getState
)
    : ssid(ssid), pass(pass), mqttHost(mqttHost),
      mqttUser(mqttUser), mqttPass(mqttPass),
      onGpio4Command(onGpio4Command), getState(getState), mqtt(wifiClient)
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
    mqtt.publish("esp32/gpio4/state", on ? "ON" : "OFF", true);  // retained
}

void Connectivity::connectWifi() {
    WiFi.mode(WIFI_AP);
    WiFi.begin(ssid, pass);
    unsigned short attempt = 5;
    while (WiFi.status() != WL_CONNECTED && attempt > 0) {
        delay(1000);
        attempt--;
    }
}

 

void Connectivity::mqttCallback(char* topic, byte* payload, unsigned int length) {
    if (String(topic) == "esp32/gpio4/set") {
        instance->onGpio4Command();
        instance->publishState(instance->getState());
    }
}
void Connectivity::connectMqtt() {
    if (mqtt.connected()) return;

    if (WiFi.status() != WL_CONNECTED) {
        return;
    }
    if (mqtt.connect("esp32-s3-blinker", mqttUser, mqttPass,
                     "esp32/gpio4/state", 0, true, "OFFLINE")) {
        mqtt.subscribe("esp32/gpio4/set");
    }
}