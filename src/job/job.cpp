#include "job.h"
#include <HardwareSerial.h>

void Job::setup() {
    this->blinker.setup();

    //test blink
    int blinkTimes = 10;
    bool state = this->isDayMode;
    unsigned long lastToggle = millis();

    while (blinkTimes > 0) {
        if (millis() - lastToggle >= 500) {
            lastToggle = millis();
            this->blinker.switchLight(state);
            state = !state;
            blinkTimes--;
        }
    }

    this->blinker.switchLight(!this->isDayMode);
}

void Job::tick() {
    std::time_t nowT = time(nullptr);
    std::tm *now = localtime(&nowT);

    if(now->tm_min % 15 == 0 && now->tm_sec <= 25) {
        this->blinker.tick();
    } else {
        this->blinker.switchLight(!this->isDayMode);
    }
}