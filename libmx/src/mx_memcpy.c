#include "../inc/libmx.h"

void* mx_memcpy(void* restrict dst, const void* restrict src, size_t n) {
    char* d = (char*) dst;
    const char* s = (const char*)src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dst;
}
