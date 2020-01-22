/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-22 11:21:06 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-22 12:09:52
 */

#ifndef STORAGE_H
#define STORAGE_H

#include <avr/eeprom.h>

#define NUMBER_OF_CELLS_PER_BYTE 20

// storage function writing to multiple cells to extend lifespan of eeprom
void storage_write_byte(uint8_t* address, uint8_t value, uint8_t numberOfCells = NUMBER_OF_CELLS_PER_BYTE) {
    eeprom_update_byte(address, value);
}

// storage reading function using multiple cells to extend lifespan of eprom
uint8_t storage_read_byte(uint8_t* address, uint8_t defaultValue = 0, uint8_t numberOfCells = NUMBER_OF_CELLS_PER_BYTE) {
    uint8_t val = eeprom_read_byte(address);
    return (val == 0xFF) ? defaultValue : val;
}

#endif