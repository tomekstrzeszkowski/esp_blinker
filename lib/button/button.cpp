#include "button.h"
#include <Arduino.h>

static Button *buttonInstance = nullptr;

void Button::setup() {
  buttonInstance = this;
  pinMode(this->buttonChannel, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(this->buttonChannel), [](){
    buttonInstance->handlePress();
  }, RISING);
}

void IRAM_ATTR Button::handlePress() {
  unsigned long currentTime = millis();
  if (currentTime - this->lastPressTime < 200) return;
  this->lastPressTime = currentTime;
  this->isPressed = true;
}