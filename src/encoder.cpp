#include "encoder.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/atomic.h>

enum EncoderState : unsigned char {
    CW_MIDDLE = 2,
    CW_STEP = 3,
    CCW_MIDDLE = 0,
    CCW_STEP = 2
};

Encoder *globalInterruptReceivingEncoder = 0;

Encoder::Encoder(EncoderEventHandlerPtr handler) {

    // set PD2 and PD3 as input
    DDRD &= ~(1 << PD2);				
    DDRD &= ~(1 << PD3); 

    // enable pullup
    PORTD |= (1 << PD3) | (1 << PD2);

    // INT0 - falling edge, INT1 - raising edge
    EICRA |= (1<<ISC01);
    EICRA |= (1<<ISC11) | (1<<ISC10);

    // enable interrupts
    EIMSK |= (1<<INT0) | (1<<INT1);

    globalInterruptReceivingEncoder = this;

    encoderChangeHandler = handler;

    absolutePosition = 0;
    lastRead = absolutePosition;

}

void Encoder::update() {
    if (int change = this->getChange())
        encoderChangeHandler(change);
}

int Encoder::getChange() {
    int delta = 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        delta = absolutePosition - lastRead;
    }   
    lastRead += delta;

    
    if (abs(delta) < 100)
        return delta;

    // if the maximum bounds of long is reached
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        absolutePosition = 0;
    }
    lastRead = 0;
    return delta > 0 ? -1 : 1;
}

int Encoder::getAbsolute() {
    return this->absolutePosition;
}

void Encoder::_process(char pinState) {
    static EncoderState state = (EncoderState)0;

    if (pinState == EncoderState::CW_STEP && state == EncoderState::CW_MIDDLE) {
        absolutePosition++;
    } else if (pinState == EncoderState::CCW_STEP && state == EncoderState::CCW_MIDDLE) {
        absolutePosition--;
    }

    state = (EncoderState)pinState;
}

ISR(INT0_vect)
{
    char pinState = (PIND & _BV(PD3)) >> (PD3 - 1) | (PIND & _BV(PD2)) >> PD2;
    if (globalInterruptReceivingEncoder)
        globalInterruptReceivingEncoder->_process(pinState);
}

ISR(INT1_vect) 
{ 
    char pinState = (PIND & _BV(PD3)) >> (PD3 - 1) | (PIND & _BV(PD2)) >> PD2;
    if (globalInterruptReceivingEncoder)
        globalInterruptReceivingEncoder->_process(pinState);
}