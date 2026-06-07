#include "connectivity.h"
#include <Arduino.h>

Connectivity* Connectivity::instance = nullptr;

Connectivity::Connectivity(const char* ssid, const char* pass,
                           const char* mqttHost, const char* mqttUser, const char* mqttPass,
                           std::function<void(bool)> onGpio4Command)
    : ssid(ssid), pass(pass), mqttHost(mqttHost),
      mqttUser(mqttUser), mqttPass(mqttPass),
      onGpio4Command(onGpio4Command), mqtt(wifiClient)
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
    Serial.print("Connecting WiFi");
    wl_status_t stat = WiFi.status();
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" connected: " + WiFi.localIP().toString());
}

 

void Connectivity::mqttCallback(char* topic, byte* payload, unsigned int length) {
    String msg;
    for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];

    if (String(topic) == "esp32/gpio4/set") {
        bool on = (msg == "ON");
        if (instance->onGpio4Command) {
            instance->onGpio4Command(on);
        }
        instance->publishState(on);
    }
}
void Connectivity::connectMqtt() {
    if (mqtt.connected()) return;

    // Ensure WiFi is up first
    if (WiFi.status() != WL_CONNECTED) {
        connectWifi();
    }

    Serial.print("Connecting MQTT...");
    if (mqtt.connect("esp32-s3-blinker", mqttUser, mqttPass,
                     "esp32/gpio4/state", 0, true, "OFFLINE")) {
        Serial.println("connected");
        mqtt.subscribe("esp32/gpio4/set");
        publishState(false);
    } else {
        Serial.printf("failed, rc=%d\n", mqtt.state());
    }
}