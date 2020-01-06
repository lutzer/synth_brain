#include "ringbuffer.h"

RingBuffer::RingBuffer(uchar maxSize) {
    this->maxSize = maxSize;
    this->start = 0;
    this->end = 0;

    this->buffer = new buffer_type[maxSize];
}

buffer_type RingBuffer::pop() {
    char val = buffer[start];
    start = (start + 1) % maxSize;
    return val;
}

void RingBuffer::push(const buffer_type e) {
    buffer[end] = e;
    end = (end + 1) % maxSize;
}

uchar RingBuffer::size() {
    return (end - start + maxSize) % maxSize;
}