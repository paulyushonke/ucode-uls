#include "../inc/uls.h"

float mx_round(c_float number) {
    if (number < 0) {
        return (float)((int)(number - 0.5));
    } else {
        return (float)((int)(number + 0.5));
    }
}

float mx_round_up(c_float number) {
    float convnum = (float)((long long)number);
    if (convnum == number || number < 0) {
        return convnum;
    }
    return convnum + 1;
}

float mx_round_down(c_float number) {
    float convnum = (float)((long long)number);
    if (convnum == number || number >= 0) {
        return convnum;
    }
    return convnum - 1;
}
