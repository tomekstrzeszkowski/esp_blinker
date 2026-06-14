#ifndef BUTTON_H
#define BUTTON_H
#include <Arduino.h>

class Button {
  public:
    Button(int buttonChannel): buttonChannel(buttonChannel) {};
    void setup();
    void handlePress();
    bool isPressed = false;

  protected:
    int buttonChannel;
    unsigned long lastPressTime;
};
#endif