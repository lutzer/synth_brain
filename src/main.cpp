/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:13:57 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-09 09:50:25
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "config.h"
#include "utils/macros.h"
#include "utils/math.h"

#include "uart.h"
#include "midi.h"
#include "encoder.h"
#include "voice.h"
#include "trigger.h"
#include "dac.h"
#include "state.h"
#include "button.h"

#ifdef DEBUG
#include "utils/debug.h"
#endif

Statemachine *state;
MidiReader *midiIn;
MidiHandler *midiHandler;

Encoder *encoder;
Buttons *buttons;

Voice *voice[2];
Dac *dac;
OneShotTrigger *trigger;

void onMidiMessage(MidiMessage message) {
    #ifdef DEBUG
    debug_print("m:%01X,%u %u,%u\n", message.command(), message.channel(), message.data[0], message.data[1]);
    #endif

    midiHandler->handle(message);
}

void onGateChange(bool enabled) {
    #ifdef DEBUG
    debug_print("gate %i\n", enabled);
    #endif

    if (enabled) {
        set_pin_high(GATE_PIN);
        trigger->fire();
    } else
        set_pin_low(GATE_PIN);
}

void onButtonChange(uchar id, bool pushed) {
    #ifdef DEBUG
    debug_print("button %i:%i\n", id, pushed);
    #endif

    if (id == 0 && pushed)
        state->encoder_push();
}

void onEncoderChange(int change) {
    #ifdef DEBUG
    debug_print("ec:%i\n", change);
    #endif

    state->encoder_turn(change);
}

int main(void) {
    uart_init();

    // load settings
    state = new Statemachine();

    // configure inputs
    encoder = new Encoder(&onEncoderChange);
    buttons = new Buttons(&onButtonChange);
    
    // configure outputs
    configure_output(GATE_PIN);
    trigger = new OneShotTrigger(TRIGGER_PULSE_LENGTH);

    // init the two oscillators
    dac = new Dac();
    voice[0] = new Voice(dac, 0, &onGateChange);
    voice[1] = new Voice(dac, 1, &onGateChange);

    // init midi handler
    midiHandler = new MidiHandler();
    midiHandler->addVoice(voice[0]);
    midiHandler->addVoice(voice[1]);

    // init midi
    midiIn = new MidiReader(&onMidiMessage);

    // enable global interrupts
    sei();

    #ifdef DEBUG
    debug_print("initialized\n");
    #endif

    while (1)
    {
        // read midi data from rx port
        while (uart_data_available()) {
            char c = uart_getchar();
            midiIn->parse(c);
        }

        voice[0]->update();
        voice[1]->update();

        encoder->update();
        buttons->update();
    }
    return 0;
}