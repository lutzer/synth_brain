/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-08 01:10:00 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-21 12:35:05
 */

#ifndef DISPLAY_H
#define DISPLAY_H

typedef unsigned char uchar;

#define NUMBER_OF_DIGITS 2

/* 
* Drives an 74HC595 Shift Register connected to a 7 Segment numeric led display
*/ 
class Display {
    uchar data[NUMBER_OF_DIGITS];
    uchar dots;

    public:
        static volatile bool _needsRefresh;
        
        Display();

        // takes first two bits to set dots of lcd display
        void showDot(uchar dot, bool show);
        void setDots(const uchar show);

        void print(const uchar number);
        void print(const char *str);
        void setData(int data);
        void clear();

        void update();

};

#endif