#include "../inc/libmx.h"

char *mx_strcpy(char* dst, const char* src) {
    if (dst == NULL) {
        return NULL; 
    }

    char* res = dst; 

    while (*src != '\0') {
        *dst = *src;
        dst++;
        src++;
    }

    *dst = '\0'; 

    return res; 
}
