/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-28 20:01:17 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-29 13:01:57
 */

#ifndef CALIBRATION_TABLE_H
#define CALIBRATION_TABLE_H

#include <avr/common.h>

#define NUMBER_OF_CALIBRATION_VALUES 6

class CalibrationTable {

    uint16_t calibrationValues[NUMBER_OF_CALIBRATION_VALUES];

    public:
        static const uint16_t CALIBRATION_DEFAULT_VALUES[];

        CalibrationTable();
        void setCalibrationOffsets(const uint8_t *values);
        uint16_t getValue(uint8_t note, uint16_t pitchbend = 0);
};

#endif