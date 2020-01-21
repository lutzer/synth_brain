/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-21 09:44:45 
 * @Last Modified by:   Lutz Reiter - http://lu-re.de 
 * @Last Modified time: 2020-01-21 09:44:45 
 */

#include "timers.h"

TimerCallback Timer2::callbacks[MAX_CALLBACKS];
uchar Timer2::numberOfCallbacks = 0;

void Timer2::initTOI() {
    // setup timer2 for debouncing
    TCNT2 = 0x0; // set timer to 0
    TCCR2A = 0; // set to overflow mode
    TIMSK2 = (1 << TOIE2);  // enable overflow interrupt

    // start timer with clock/256 divider = 3,3 ms per overflow
    TCCR2B = _BV(CS22) | _BV(CS21);
}

void Timer2::addCallback(TimerCallback cb) {
    Timer2::callbacks[Timer2::numberOfCallbacks++] = cb;
}

ISR (TIMER2_OVF_vect) {
    for(int i=0; i< Timer2::numberOfCallbacks; i++)
        Timer2::callbacks[i]();
}