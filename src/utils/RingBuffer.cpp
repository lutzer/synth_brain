#include "ringbuffer.h"

RingBuffer::RingBuffer(uchar maxSize) {
    this->maxSize = maxSize;
    this->start = 0;
    this->end = 0;

    this->buffer = new buffer_type[maxSize];
}

buffer_type RingBuffer::pop() {
    return buffer[start++];
}

void RingBuffer::push(const buffer_type e) {
    buffer[end] = e;
    end = (end + 1) % maxSize;
}

char RingBuffer::size() {
    return (end - start) > 0 ? end - start : end + maxSize - start;
}