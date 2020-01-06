/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:13:57 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-06 23:19:07
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "params.h"
#include "utils/macros.h"

#include "uart.h"
#include "midi.h"
#include "encoder.h"
#include "voice.h"
#include "trigger.h"

#ifdef DEBUG
#include <stdio.h>
#endif

MidiReader *midiIn;
Encoder *encoder;
Voice *voice[2];
OneShotTrigger *trigger;

void onMidiMessage(MidiMessage message) {
    #ifdef DEBUG
    char msg[20] = "";
    sprintf(msg, "m:%01X,%u %u,%u\n", message.command(), message.channel(), message.data[0], message.data[1]);
    uart_putstring(msg);
    #endif

    byte cmd = message.command();

    for (int i = 0; i < 2; i++) {
        if (cmd == MidiCommand::System_Reset || cmd == MidiCommand::Stop) {
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
        SET_PIN_HIGH(GATE_PIN);
        trigger->fire();
    } else
        SET_PIN_LOW(GATE_PIN);
}

void onEncoderChange(int change) {
    #ifdef DEBUG
    char msg[10] = "";
    sprintf(msg, "ec:%i\n", change); 
    uart_putstring(msg);
    #endif
}

int main(void) {

    CONFIGURE_OUTPUT(GATE_PIN);
    CONFIGURE_OUTPUT(TRIGGER_PIN);

    encoder = new Encoder(&onEncoderChange);
    midiIn = new MidiReader(&onMidiMessage);
    voice[0] = new Voice(&onGateChange);
    voice[1] = new Voice(&onGateChange);
    
    uart_init(); // init serial

    sei(); // enable global interrupts

    trigger = new OneShotTrigger(TRIGGER_PULSE_LENGTH);

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