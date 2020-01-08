/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-07 18:05:09 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-07 19:29:13
 */

#include <avr/io.h>

#include "spi.h"

void spi_init_master() {
     //set spi pins as output (MOSI,SCK,SS)
    DDRB |= (1<<PB2) | (1<<PB3) | (1<<PB5);

    // Enable SPI, Master, clock rate F_CPU/2, (msb first)
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPI2X) /* | (1<<DORD) */;
}

void spi_transmit(char data) {
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
}