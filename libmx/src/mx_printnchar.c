#include "../inc/libmx.h"

void mx_printnchar(char character, size_t count) {
    for (size_t i = 0; i < count; i++) {
        mx_printchar(character);
    }
}

