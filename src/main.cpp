/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:13:57 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-09 14:54:03
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "config.h"
#include "utils/macros.h"
#include "utils/math.h"
#include "utils/timers.h"

#include "uart.h"
#include "midi.h"
#include "encoder.h"
#include "voice.h"
#include "trigger.h"
#include "dac.h"
#include "state.h"
#include "button.h"
#include "display.h"

#ifdef DEBUG
#include "utils/debug.h"
#endif

Statemachine *state;
MidiReader *midiIn;
MidiHandler *midiHandler;

Encoder *encoder;
Buttons *buttons;
Display *display;

Voice *voice[2];
Dac *dac;
OneShotTrigger *trigger;

void onMidiMessage(MidiMessage message) {
    #ifdef DEBUG
    debug_print("m:%01X,%u %u,%u\n", message.command(), message.channel(), message.data[0], message.data[1]);
    #endif

    midiHandler->handle(message);
}

void onGateChange(bool enabled, uchar dacChannel) {
    #ifdef DEBUG
    debug_print("gate %i\n", enabled);
    #endif

    if (enabled) {
        set_pin_high(GATE_PIN);
        trigger->fire();
        display->showDot(dacChannel, true);
    } else {
        set_pin_low(GATE_PIN);
        display->showDot(dacChannel, false);
    }
}

void onButtonChange(uchar changes, uchar pushed) {
    #ifdef DEBUG
    debug_print("button %01X:%01X\n", changes, pushed);
    #endif
    if (changes & _BV(ENCODER_BUTTON) && pushed & _BV(ENCODER_BUTTON)) {
        state->encoder_push();
    }
    if (changes & _BV(MODE_BUTTON) && pushed & _BV(MODE_BUTTON)) {
        state->mode_button_push();
    }
}

void onEncoderChange(int change) {
    #ifdef DEBUG
    debug_print("ec:%i\n", change);
    #endif
    state->encoder_turn(change);
}

void onStateChanged(const State &state) {
    #ifdef DEBUG
    debug_print("state changed");
    #endif

    midiHandler->setMidiMode(state.midiMode, state.midiChannels);

    // switch midi-mode led
    if (state.midiMode == MidiMode::MONOPHONIC) {
        set_pin_high(MODE_LED1);
        set_pin_low(MODE_LED2);
    } else if (state.midiMode == MidiMode::SPLIT) {
        set_pin_low(MODE_LED1);
        set_pin_high(MODE_LED2);
    } else {
        set_pin_high(MODE_LED1);
        set_pin_high(MODE_LED2);
    }

    switch (state.status) {
        case CtrlState::INIT:
            if (state.menuStatus == MenuState::MENU_OFF)
                display->clear();
            else if (state.menuStatus == MenuState::MENU_CHANNEL1)
                display->print("M0");
            else if (state.menuStatus == MenuState::MENU_CHANNEL2)
                display->print("M1");
            else if (state.menuStatus == MenuState::MENU_CALIBRATE_LOW)
                display->print("CL");
            else if (state.menuStatus == MenuState::MENU_CALIBRATE_HIGH)
                display->print("CH");
            break;
        case CtrlState::CONTROL_CHANNEL1:
            display->print(state.midiChannels[0]+1);
            break;
        case CtrlState::CONTROL_CHANNEL2:
            display->print(state.midiChannels[1]+1);
            break;
    } 

    // switch midi-channels
}

int main(void) {
    uart_init();

    // load settings
    state = new Statemachine(&onStateChanged);

    // configure inputs
    encoder = new Encoder(&onEncoderChange);
    buttons = new Buttons(&onButtonChange);
    
    // configure outputs
    configure_output(GATE_PIN);
    configure_output(MODE_LED1);
    configure_output(MODE_LED2);
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

    // enable display
    display = new Display();

    // enable global interrupts
    sei();

    //activate timer2
    Timer2::initTOI();

    #ifdef DEBUG
    debug_print("initialized\n");
    #endif

    state->load();

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

        display->update();

    }
    return 0;
}