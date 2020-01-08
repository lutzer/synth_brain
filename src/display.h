/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-08 01:10:00 
 * @Last Modified by:   Lutz Reiter - http://lu-re.de 
 * @Last Modified time: 2020-01-08 01:10:00 
 */

#ifndef DISPLAY_H
#define DISPLAY_H

/* 
* Drives an 74HC595 Shift Register connected to a 7 Segment numeric led display
*/ 
class Display {
    public:
        Display();

        void show(int data);
        void clear();
};

#endif