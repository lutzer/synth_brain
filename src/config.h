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

#define DAC_CS_PIN PB1
#define DAC_CS_PIN_PORT PORTB
#define DAC_CS_PIN_DDR DDRB

#define ENCODER_PIN1 PD2
#define ENCODER_PIN1_PORT PORTD
#define ENCODER_PIN1_DDR DDRD
#define ENCODER_PIN1_REG PIND
#define ENCODER_PIN1_INT_ENABLE { PCMSK0 |= _BV(PCINT3); PCICR |= _BV(PCIE0); }

#define ENCODER_PIN2 PD3
#define ENCODER_PIN2_PORT PORTD
#define ENCODER_PIN2_DDR DDRD
#define ENCODER_PIN2_REG PIND

// set trigger pulse length
#define TRIGGER_PULSE_LENGTH 10 // in ms

// set baudrate for uart
#define BAUD 31250
#define UART_BUFFER_SIZE 128

// full pitch bend +- semitones
#define PITCH_BEND_SEMITONES 2

#endif