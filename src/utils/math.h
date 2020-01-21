/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-07 18:03:29 
 * @Last Modified by: Lutz Reiter - http://lu-re.de
 * @Last Modified time: 2020-01-22 00:52:47
 */

#ifndef MATH_H
#define MATH_H

template <typename T> 
T min(T a, T b) {
    return (a > b) ? b : a;
}

template <typename T> 
T max(T a, T b) {
    return (a > b) ? a : b;
}


template <typename T> 
int constrain(T val, T minimum, T maximum) {
    return max(min(val, maximum), minimum);
}

#endif