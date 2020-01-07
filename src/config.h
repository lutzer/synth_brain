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

// set trigger pulse length
#define TRIGGER_PULSE_LENGTH 10 // in ms

// set baudrate for uart
#define BAUD 31250
#define UART_BUFFER_SIZE 128

#endif