#include "button.h"
#include <Arduino.h>

static Button *buttonInstance = nullptr;

void IRAM_ATTR handlePressWrapper() {
  if (buttonInstance) {
    buttonInstance->handlePress();
  }
}

void Button::setup() {
  buttonInstance = this;
  pinMode(this->buttonChannel, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(this->buttonChannel), handlePressWrapper, RISING);
}

void Button::handlePress() {
  unsigned long currentTime = millis();
  if (currentTime - this->lastPressTime < 200) return;
  this->lastPressTime = currentTime;
  this->isPressed = true;
}