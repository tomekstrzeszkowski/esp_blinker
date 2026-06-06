#ifndef JOB_H
#define JOB_H
#include <blinker/blinker.h>

class Job {
    public:
        Job(Blinker &blinker) : blinker(blinker), isDayMode(true) {}
        void setup();
        void tick();
    protected:
        Blinker &blinker;
        bool isDayMode;
};
#endif