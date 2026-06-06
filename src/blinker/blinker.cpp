#include "blinker.h"
#include <Arduino.h>
#include <chrono>

Blinker::Blinker(int lightChannel)  {
    this->lightChannel = lightChannel;
    this->duration = std::chrono::milliseconds(3000);
    this->offDuration = std::chrono::milliseconds(100);
    this->current = std::chrono::steady_clock::now();
    this->cycle = false;
};

void Blinker::setup() {
    pinMode(this->lightChannel, OUTPUT);
}

std::chrono::steady_clock::time_point Blinker::getNextTick() {
    if (this->cycle) {
        return this->current + this->duration;
    }
    return this->current + this->offDuration;
}

void Blinker::tick() {
    std::chrono::steady_clock::time_point nextCycle = this->getNextTick();
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    if (now > nextCycle) {
        this->current = now;
        this->cycle = !this->cycle;
        this->switchLight(this->cycle);
    }
}

void Blinker::switchLight(bool on) {
    if (on) {
        digitalWrite(this->lightChannel, HIGH);
    } else {
        digitalWrite(this->lightChannel, LOW);
    }
}