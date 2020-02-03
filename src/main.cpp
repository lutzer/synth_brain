/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:13:57 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-29 13:29:27
 */

#include <avr/io.h>

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
#include "calibration_table.h"

#ifdef DEBUG
#include "utils/debug.h"
#endif

Statemachine *statemachine;
MidiReader *midiIn;

Encoder *encoder;
Buttons *buttons;
Display *display;

VoiceGroup *voiceGroup;
Voice *voice[NUMBER_OF_VOICES];
Dac *dac;
CalibrationTable *calibrationTable;

OneShotTrigger *trigger;

void onMidiMessage(MidiMessage message) {
    #ifdef DEBUG
    debug_print("m:%01X,%u %u,%u\n", message.command(), message.channel(), message.data[0], message.data[1]);
    #endif

    voiceGroup->handle(message);
}

void onGateChange(bool enabled, uchar dacChannel) {
    #ifdef DEBUG
    debug_print("gate c%i %i\n", dacChannel, enabled);
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
        statemachine->encoder_push();
    }
    if (changes & _BV(MODE_BUTTON) && pushed & _BV(MODE_BUTTON)) {
        statemachine->mode_button_push();
    }
}

void onEncoderChange(int change) {
    #ifdef DEBUG
    debug_print("ec:%i\n", change);
    #endif

    statemachine->encoder_turn(change);
}

void onStateChanged(const State &state) {
    #ifdef DEBUG
    debug_print("state: %i:%i\n", state.status, state.midiMode);
    #endif

    // only set calibration on first state change
    static bool loaded = true;
    if (loaded) {
        calibrationTable->setCalibrationOffsets(state.calibration);
        loaded = false;
    }

    // switch midi mode and midi channels and set calibration values
    voiceGroup->setMidiMode(state.midiMode, state.midiChannels);

    // switch midi-mode led
    if (state.midiMode == MidiMode::SPLIT) {
        set_pin_high(MODE_LED1);
        set_pin_low(MODE_LED2);
    } else if (state.midiMode == MidiMode::MONOPHONIC ) {
        set_pin_low(MODE_LED1);
        set_pin_high(MODE_LED2);
    } else if (state.midiMode == MidiMode::PARAPHONIC ){
        set_pin_high(MODE_LED1);
        set_pin_high(MODE_LED2);
    }

    switch (state.status) {
        case CtrlState::INIT:
            if (state.menuStatus == MenuState::MENU_OFF)
                display->clear();
            else if (state.menuStatus == MenuState::MENU_CHANNEL1)
                display->print("C1");
            else if (state.menuStatus == MenuState::MENU_CHANNEL2)
                display->print("C2");
            else if (state.menuStatus == MenuState::MENU_CALIBRATE_A1)
                display->print("A1");
            else if (state.menuStatus == MenuState::MENU_CALIBRATE_A2)
                display->print("A2");
            else if (state.menuStatus == MenuState::MENU_CALIBRATE_A3)
                display->print("A3");
            else if (state.menuStatus == MenuState::MENU_CALIBRATE_A4)
                display->print("A4");
            break;
        case CtrlState::CONTROL_CHANNEL1:
            display->print(state.midiChannels[0]+1);
            break;
        case CtrlState::CONTROL_CHANNEL2:
            display->print(state.midiChannels[1]+1);
            break;
        case CtrlState::CALIBRATE_A1:
            calibrationTable->setCalibrationOffsets(state.calibration);
            display->print(state.calibration[1]);
            voiceGroup->retrigger();
            break;
        case CtrlState::CALIBRATE_A2:
            calibrationTable->setCalibrationOffsets(state.calibration);
            display->print(state.calibration[2]);
            voiceGroup->retrigger();
            break;
        case CtrlState::CALIBRATE_A3:
            calibrationTable->setCalibrationOffsets(state.calibration);
            display->print(state.calibration[3]);
            voiceGroup->retrigger();
            break;
        case CtrlState::CALIBRATE_A4:
            calibrationTable->setCalibrationOffsets(state.calibration);
            display->print(state.calibration[4]);
            voiceGroup->retrigger();
            break;
    }
}

int main(void) {
    uart_init();

    // configure outputs
    configure_output(GATE_PIN);
    configure_output(MODE_LED1);
    configure_output(MODE_LED2);
    trigger = new OneShotTrigger(TRIGGER_PULSE_LENGTH);

    // enable display
    display = new Display();

    // configure inputs
    encoder = new Encoder(&onEncoderChange);
    buttons = new Buttons(&onButtonChange);

    // setup statemachine
    statemachine = new Statemachine(&onStateChanged);

    // init calibration table
    calibrationTable = new CalibrationTable();

    // init the two oscillators
    dac = new Dac();
    voice[0] = new Voice(dac, 0, calibrationTable, &onGateChange);
    voice[1] = new Voice(dac, 1, calibrationTable, &onGateChange);

    // init voice group
    voiceGroup = new VoiceGroup(NUMBER_OF_VOICES);
    voiceGroup->addVoice(voice[0]);
    voiceGroup->addVoice(voice[1]);

    // init midi
    midiIn = new MidiReader(&onMidiMessage);

    // enable global interrupts
    sei();

    //activate timer2
    Timer2::initTOI();

    #ifdef DEBUG
    debug_print("initialized\n");
    #endif

    statemachine->load();

    while (1)
    {
        // read midi data from rx port
        while (uart_data_available()) {
            char c = uart_getchar();
            midiIn->parse(c);
        }

        voiceGroup->update();

        encoder->update();
        buttons->update();
        
        statemachine->update();

        display->update();

    }
    return 0;
}