#include "../inc/libmx.h"

void* mx_memccpy(void* restrict dst, const void* restrict src, int c, size_t n) {
    unsigned char* d = (unsigned char*)dst;
    const unsigned char* s = (const unsigned char*)src;
    
    size_t i;
    for (i = 0; i < n; i++) {
        d[i] = s[i];
        if (s[i] == (unsigned char)c)
            return (void*)(d + i + 1);
    }
    
    return NULL;
}
