#include "encoder.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/atomic.h>

#define MAX_INT_DIV2 INT32_MAX/2

enum EncoderState : unsigned char {
    CW_MIDDLE = 2, //10
    CW_STEP = 3, //11
    CCW_MIDDLE = 0, //00
    CCW_STEP = 2 //10
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

    // of reading is in bounds
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
    static uchar prevState = 0;

    uchar currentState = (PIND & _BV(PD3)) >> (PD3 - 1) | (PIND & _BV(PD2)) >> PD2;

    if (currentState == EncoderState::CW_STEP && prevState == EncoderState::CW_MIDDLE) {
        absolutePosition++;
    } else if (currentState == EncoderState::CCW_STEP && prevState == EncoderState::CCW_MIDDLE) {
        absolutePosition--;
    }

    prevState = currentState;
}

ISR(INT0_vect)
{
    ENCODER_OBJECT->_processInterrupt();
}

ISR(INT1_vect) 
{ 
    ENCODER_OBJECT->_processInterrupt();
}