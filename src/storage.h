/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-22 11:21:06 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-22 14:00:03
 */

#ifndef STORAGE_H
#define STORAGE_H

#include <avr/eeprom.h>

#define NUMBER_OF_COPIES 10

/* 
 * Storage function writing to multiple cells to extend lifespan of eeprom.
 * 
 * First x copies hold status buffers, last x ones hold storage buffer, needs bufferSize*2
 * size in eeprom
 */
void storage_write_byte(uint8_t* address, uint8_t value, uint8_t bufferSize = NUMBER_OF_COPIES) {
    // read status buffer
    uint8_t status_block[bufferSize];
    eeprom_read_block(status_block, address, bufferSize);

    uint8_t i = 0;
    while (i < bufferSize - 1) {
        if (status_block[i] == (uint8_t)(status_block[i+1] + 1) )
            break;
        i++;
    }

    // check if the latest value has changed
    if (eeprom_read_byte((uint8_t*)(address + bufferSize + i)) != value) {
        //update status buffer and storage buffer
        uint8_t writePosition = ((i + 1) % bufferSize);
        eeprom_write_byte((uint8_t*)(address + writePosition), status_block[writePosition]+1);
        eeprom_write_byte((uint8_t*)(address + bufferSize + writePosition), value);
    }
}

/*
 *  storage reading function using multiple cells to extend lifespan of eprom
 */  
uint8_t storage_read_byte(uint8_t* address, uint8_t defaultValue = 0, uint8_t bufferSize = NUMBER_OF_COPIES) {
    // read status buffer
    uint8_t status_block[bufferSize];
    eeprom_read_block(status_block, address, bufferSize);

    uint8_t i = 0;
    while (i < bufferSize - 1) {
        if (status_block[i] == (uint8_t)(status_block[i+1] + 1) )
            break;
        i++;
    }

    uint8_t val = eeprom_read_byte(address + bufferSize + i);
    return (val == 0xFF) ? defaultValue : val;
}

#endif