#include "../inc/libmx.h"

char *mx_strstr(const char *haystack, const char *needle) {
    if (*needle == '\0') {
        return (char *) haystack;
    }

    while (*haystack != '\0') {
        const char *h = haystack;
        const char *n = needle;

        while (*n != '\0' && *h == *n) {
            h++;
            n++;
        }

        if (*n == '\0') {
            return (char *) haystack;
        }

        haystack++;
    }

    return NULL;
}
