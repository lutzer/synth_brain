/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-28 21:03:40 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-29 13:33:25
 */

#include "calibration_table.h"
#include "utils/math.h"

#define LOWEST_A_NOTE 33

const uint16_t CalibrationTable::CALIBRATION_DEFAULT_VALUES[] = {
    0,      // A0 0mV
    800,    // A1 1000mV
    1600,   // A2 2000mV
    2399,   // A3 3000mV
    3199,   // A4 4000mV
    3999    // A5 5000mV
};

const float fractionsOf12[] = {
    0.0, 0.0833333, 0.1666666, 0.25, 
    0.3333333, 0.4166666, 0.5, 0.5833333, 
    0.6666666, 0.75, 0.8333333, 0.9166666
};

CalibrationTable::CalibrationTable() {
    // init values with default
    for (uint8_t i = 0; i < NUMBER_OF_CALIBRATION_VALUES; i++) {
        calibrationValues[i] = CalibrationTable::CALIBRATION_DEFAULT_VALUES[i];
    }
};

void CalibrationTable::setCalibrationOffsets(const uint8_t *values) {
    for (uint8_t i = 0; i < NUMBER_OF_CALIBRATION_VALUES; i++)
        calibrationValues[i] = CalibrationTable::CALIBRATION_DEFAULT_VALUES[i] + values[i] - 50;
}

uint16_t CalibrationTable::getValue(uint8_t note, uint16_t pitchbend) {

    // lowest note is A0
    note = max(note - LOWEST_A_NOTE, 0);

    uint8_t octaveFraction = note % 12;
    uint8_t index = 0;
    while (note > 11) {
        note -= 12;
        index++;
    };
    
    // highest note is A5
    if ((index + 1) >= NUMBER_OF_CALIBRATION_VALUES)
        return calibrationValues[NUMBER_OF_CALIBRATION_VALUES - 1];

    uint16_t lowerA = calibrationValues[index];
    uint16_t upperA = calibrationValues[index+1];

    return lowerA + (upperA - lowerA) * fractionsOf12[octaveFraction];
}