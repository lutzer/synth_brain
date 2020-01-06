#ifndef RINGBUFFER_H
#define RINGBUFFER_H

typedef char buffer_type;
typedef unsigned char uchar;

class RingBuffer {

    uchar start;
    uchar end;
    uchar maxSize;

    char *buffer;

    public:
        RingBuffer(uchar maxSize);

        void push(buffer_type e) volatile;
        buffer_type pop() volatile;

        bool empty() volatile;
        uchar size() volatile;
};

#endif
