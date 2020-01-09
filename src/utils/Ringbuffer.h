/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:14:20 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-07 16:34:48
 */

#ifndef RINGBUFFER_H
#define RINGBUFFER_H

// typedef char buffer_type;
typedef unsigned char uchar;

template<typename T> 
class RingBuffer {
    uchar maxSize;

    volatile uchar start;
    volatile uchar end;
    volatile T *buffer;

    public:
        RingBuffer<T>(uchar maxSize);

        void push(T e);
        T pop();

        bool empty();
        uchar size();
};

#endif
