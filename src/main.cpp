#include <Arduino.h>
#include <blinker.h>
#include <job.h>
#include <button.h>

#define LIGHT_CHANNEL 4
#define BUTTON_CHANNEL 5

Blinker blinker(LIGHT_CHANNEL);
Button button(BUTTON_CHANNEL);
Job job{blinker};

void setup() {
  job.setup();
  button.setup();
}

void loop() {
  job.tick();
  if (button.isPressed) {
    button.isPressed = false;
    job.switchDayMode();
  } 
}
