/*
 * @Author: Lutz Reiter - http://lu-re.de 
 * @Date: 2020-01-07 18:03:23 
 * @Last Modified by:   Lutz Reiter - http://lu-re.de 
 * @Last Modified time: 2020-01-07 18:03:23 
 */

#include "math.h"

int min(int a, int b) {
    return (a > b) ? b : a;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int constrain(int val, int minimum, int maximum) {
    return max(min(val, maximum), minimum);
}