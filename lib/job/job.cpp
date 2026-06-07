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
    if (this->manualOverride) {
        this->switchDayMode(); // todo: switch should read current day mode in HA
        this->manualOverride = false;
        return;
    }

    std::time_t nowT = time(nullptr);
    std::tm *now = localtime(&nowT);

    if(now->tm_min % 15 == 0 && now->tm_sec <= 25) {
        this->blinker.tick();
    } else {
        this->blinker.switchLight(!this->isDayMode);
    }
}

void Job::switchDayMode() {
    this->isDayMode = !this->isDayMode;
}

void Job::setManualOverride(bool active, bool state) {
    this->manualOverride = active;
    this->manualState = state;
}