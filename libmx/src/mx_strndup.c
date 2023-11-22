#include "../inc/libmx.h"

char *mx_strndup(const char * s1, size_t n) {
    char *res = NULL;
    size_t length = mx_strlen(s1);
    if (n < length)
        length = n;
    
    res = mx_strnew(length);    
    mx_strncpy(res, s1, length);
    return res;
}
