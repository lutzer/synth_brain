/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:14:26 
 * @Last Modified by:   Lutz Reiter - http://lu-re.de 
 * @Last Modified time: 2020-01-06 19:14:26 
 */

#include "ringbuffer.h"

RingBuffer::RingBuffer(uchar maxSize) {
    this->maxSize = maxSize;
    this->start = 0;
    this->end = 0;

    this->buffer = new buffer_type[maxSize];
}

buffer_type RingBuffer::pop() volatile {
    char val = buffer[start];
    start = (start + 1) % maxSize;
    return val;
}

void RingBuffer::push(const buffer_type e) volatile {
    buffer[end] = e;
    end = (end + 1) % maxSize;
}

bool RingBuffer::empty() volatile {
    return end == start;
}

uchar RingBuffer::size() volatile {
    return (end - start + maxSize) % maxSize;
}