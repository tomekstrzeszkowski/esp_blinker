#include <Arduino.h>
#include <blinker.h>
#include <job.h>
#include <button.h>
#include <connectivity.h>

#define LIGHT_CHANNEL 4
#define BUTTON_CHANNEL 5

Blinker blinker(LIGHT_CHANNEL);
Button button(BUTTON_CHANNEL);
Job job{blinker};


Connectivity connectivity(
    WIFI_SSID, WIFI_PASS, MQTT_HOST, MQTT_USER, MQTT_PASS,
    []() {
        job.switchDayMode();
    },
    []() -> bool {
      return digitalRead(LIGHT_CHANNEL) == LOW;
    }
);

void setup() {
  connectivity.setup();
  button.setup();
  job.setup();
}

void loop() {
  connectivity.tick();
  job.tick();
  if (button.isPressed) {
    button.isPressed = false;
    job.switchDayMode();
    connectivity.publishState(connectivity.getState());
  } 
}
