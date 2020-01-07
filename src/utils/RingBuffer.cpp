/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-06 19:14:26 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-07 14:59:50
 */

#include "ringbuffer.h"

template <typename T> 
RingBuffer<T>::RingBuffer(uchar maxSize) {
    this->maxSize = maxSize;
    this->start = 0;
    this->end = 0;

    this->buffer = new T[maxSize];
}

template <typename T> 
T RingBuffer<T>::pop() volatile {
    T val = buffer[start];
    start = (start + 1) % maxSize;
    return val;
}

template <typename T> 
void RingBuffer<T>::push(const T e) volatile {
    buffer[end] = e;
    end = (end + 1) % maxSize;
}

template <typename T> 
bool RingBuffer<T>::empty() volatile {
    return end == start;
}

template <typename T>
uchar RingBuffer<T>::size() volatile {
    return (end - start + maxSize) % maxSize;
}

// compile for float
template class RingBuffer<uchar>;