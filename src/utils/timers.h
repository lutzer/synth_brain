#ifndef TIMERS_H
#define TIMERS_H

#include <avr/interrupt.h>
#include <avr/io.h>

#define MAX_CALLBACKS 5

typedef void (*TimerCallback)();
typedef unsigned char uchar;

class Timer2 {

    public:
        static TimerCallback callbacks[MAX_CALLBACKS];
        static uchar numberOfCallbacks;

        static void initTOI();

        static void addCallback(TimerCallback cb);
};

#endif