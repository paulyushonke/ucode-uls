#include "../inc/libmx.h"

void mx_printnstr(const char *string, size_t count) {
    write(STDOUT_FILENO, string, count);
}

