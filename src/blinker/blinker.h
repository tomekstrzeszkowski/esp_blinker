#ifndef BLINKER_H
#define BLINKER_H

#include <chrono>

class Blinker {
  public:
    Blinker(int lightChannel);
    void setup();
    void switchLight (bool on);
    void tick();
    std::chrono::steady_clock::time_point getNextTick();

  protected:
    int lightChannel;
    std::chrono::milliseconds duration;
    std::chrono::milliseconds offDuration;
    std::chrono::steady_clock::time_point current;
    bool cycle;
};
#endif