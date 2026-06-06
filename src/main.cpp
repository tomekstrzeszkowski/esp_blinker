#include <Arduino.h>
#include <blinker.h>
#include <job.h>

#define LIGHT_CHANNEL 4

Blinker blinker(LIGHT_CHANNEL);
Job job{blinker};

void setup() {
  job.setup();
}

void loop() {
  job.tick();
}
