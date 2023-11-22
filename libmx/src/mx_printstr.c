#include "../inc/libmx.h"

size_t mx_printstr(const char *const string) {
    size_t string_length = mx_strlen(string);
    write(STDOUT_FILENO, string, string_length);
    return string_length;
}
