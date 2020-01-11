#ifndef PARAMS_H
#define PARAMS_H

// enable debugging by build flag or here
// #define DEBUG

// define pins
#define GATE_PIN PD7
#define GATE_PIN_PORT PORTD
#define GATE_PIN_DDR DDRD

#define TRIGGER_PIN PD6
#define TRIGGER_PIN_PORT PORTD
#define TRIGGER_PIN_DDR DDRD

#define DAC_CS_PIN PB2
#define DAC_CS_PIN_PORT PORTB
#define DAC_CS_PIN_DDR DDRB

#define ENCODER_PIN1 PC1
#define ENCODER_PIN1_PORT PORTC
#define ENCODER_PIN1_DDR DDRC
#define ENCODER_PIN1_REG PINC

#define ENCODER_PIN2 PC0
#define ENCODER_PIN2_PORT PORTC
#define ENCODER_PIN2_DDR DDRC
#define ENCODER_PIN2_REG PINC

// input pins
#define BUTTONS_REG PINC

#define ENCODER_BUTTON PC2
#define ENCODER_BUTTON_PORT PORTC
#define ENCODER_BUTTON_DDR DDRC
#define ENCODER_BUTTON_REG PINC

// set trigger pulse length
#define TRIGGER_PULSE_LENGTH 10 // in ms

// set baudrate for uart
#define BAUD 31250
#define UART_BUFFER_SIZE 128

// full pitch bend +- semitones
#define PITCH_BEND_SEMITONES 2

#endif