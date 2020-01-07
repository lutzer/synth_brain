#ifndef MATH_H
#define MATH_H

int min(int a, int b) {
    return (a > b) ? b : a;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int constrain(int val, int minimum, int maximum) {
    return max(min(val, maximum), minimum);
}

#endif