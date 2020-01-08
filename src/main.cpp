/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:13:57 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-07 17:51:02
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

#ifdef DEBUG
#include "utils/debug.h"
#endif

State *state;
MidiReader *midiIn;
Encoder *encoder;
Voice *voice[2];
Dac *dac;
OneShotTrigger *trigger;

void onMidiMessage(MidiMessage message) {
    #ifdef DEBUG
    debug_print("m:%01X,%u %u,%u\n", message.command(), message.channel(), message.data[0], message.data[1]);
    #endif

    byte cmd = message.command();

    for (int i = 0; i < 2; i++) {
        if (cmd == MidiCommand::System_Reset) {
            voice[i]->stopAll();
        } else if (message.channel() == voice[i]->channel) {
            switch (cmd) {
                case MidiCommand::Note_On:
                    voice[i]->playNote(message.data[0]);
                    break;
                case MidiCommand::Note_Off:
                    voice[i]->stopNote(message.data[0]);
                    break;
                case MidiCommand::Pitch_Bend:
                    voice[i]->setPitchBend(0);
                default:
                    break;
            }  
        }
    }
}

void onGateChange(bool enabled) {
    if (enabled) {
        set_pin_high(GATE_PIN);
        trigger->fire();
    } else
        set_pin_low(GATE_PIN);
}

void onEncoderChange(int change) {
    #ifdef DEBUG
    debug_print("ec:%i\n", change);
    #endif

    voice[0]->setChannel(constrain((int)voice[0]->channel + change, 0, 15));
    voice[1]->setChannel(constrain((int)voice[1]->channel + change, 0, 15));

    #ifdef DEBUG
    debug_print("midi c1:%i, c2:%i\n", voice[0]->channel, voice[1]->channel);
    #endif
}

int main(void) {
    // load settings
    state = new State();

    // configure single pins
    configure_output(GATE_PIN);

    // configure inputs
    encoder = new Encoder(&onEncoderChange);
    midiIn = new MidiReader(&onMidiMessage);

    // configure outputs
    dac = new Dac();
    trigger = new OneShotTrigger(TRIGGER_PULSE_LENGTH);

    // init the two oscillators
    voice[0] = new Voice(dac, 0, &onGateChange);
    voice[1] = new Voice(dac, 1, &onGateChange);

    // init serial
    uart_init();

    // enable global interrupts
    sei();

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
    }
    return 0;
}