#ifndef BUTTON_H
#define BUTTON_H
#include <Arduino.h>

class Button {
  public:
    Button(int buttonChannel): buttonChannel(buttonChannel), isPressed(false) {};
    void setup();
    void IRAM_ATTR handlePress();
    bool isPressed;

  protected:
    int buttonChannel;
    unsigned long lastPressTime;
};
#endif