/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:14:10 
 * @Last Modified by:   Lutz Reiter - http://lu-re.de 
 * @Last Modified time: 2020-01-06 19:14:10 
 */

#include "encoder.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/atomic.h>

#define MAX_INT_DIV2 INT32_MAX/2

enum EncoderState : uchar {
    CW_START = 0x01,
    CW_STEP1 = 0x02,
    CW_STEP2 = 0x03,
    CW_FINAL = 0x10,

    CCW_START = 0x04,
    CCW_STEP1 = 0x05,
    CCW_STEP3 = 0x06,
    CCW_FINAL = 0x20,

    UNDEFINED = 0x40,
};

uchar transitionTable[7][4] = {
    { UNDEFINED, CCW_START, CW_START, UNDEFINED },  // init state -> 1,2

    { CW_STEP1, UNDEFINED, UNDEFINED, UNDEFINED }, // cw start -> 0
    { UNDEFINED, CW_STEP2, CW_START, UNDEFINED }, // cw step1 -> 1, <- 2
    { CW_STEP1, UNDEFINED, UNDEFINED, CW_FINAL }, // cw step2 -> 3, <- 0

    { CCW_STEP1, UNDEFINED, UNDEFINED, UNDEFINED }, // ccw start -> 0
    { UNDEFINED, CCW_START, CCW_STEP3, UNDEFINED }, // ccw step1 -> 2, <- 0
    { UNDEFINED, CCW_STEP1, UNDEFINED , CCW_FINAL } // ccw step2 -> 3, <- 2
};

Encoder *ENCODER_OBJECT = 0;

Encoder::Encoder(EncoderEventHandlerPtr handler) {

    ENCODER_OBJECT = this;

    // set PD2 and PD3 as input
    DDRD &= ~(1 << PD2);				
    DDRD &= ~(1 << PD3); 

    // enable pullup
    PORTD |= (1 << PD3) | (1 << PD2);

    // react to raise and fall on both interrupts
    EICRA |= (1<<ISC00);
    EICRA |= (1<<ISC10);

    // enable interrupts
    EIMSK |= (1<<INT0) | (1<<INT1);

    // register change handler
    encoderChangeHandler = handler;

    // init encoder position
    absolutePosition = 0;
    lastRead = absolutePosition;

}

void Encoder::update() {
    if (int change = this->getChange())
        encoderChangeHandler(change);
}

int Encoder::getChange() {
    long delta = 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        delta = absolutePosition - lastRead;
    }   
    lastRead += delta;

    // if reading is in bounds
    if (abs(delta) < MAX_INT_DIV2)
        return delta;

    // if the maximum bounds of long is reached, reset position
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        absolutePosition = 0;
        lastRead = 0;
    }
    return delta > 0 ? -1 : 1;
}

int Encoder::getAbsolute() {
    return this->absolutePosition;
}

void Encoder::_processInterrupt() {
    static uchar state = 0;

    uchar pins = (PIND & _BV(PD3)) >> (PD3 - 1) | (PIND & _BV(PD2)) >> PD2;

    state = transitionTable[state & 0x0F][pins];

    if (state == 0x10)
        absolutePosition++;
    if (state == 0x20)
        absolutePosition--;
}

ISR(INT0_vect)
{
    ENCODER_OBJECT->_processInterrupt();
}

ISR(INT1_vect) 
{ 
    ENCODER_OBJECT->_processInterrupt();
}