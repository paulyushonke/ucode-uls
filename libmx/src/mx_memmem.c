#include "../inc/libmx.h"

void *mx_memmem(const void *big, size_t big_len, const void *little, size_t little_len) {
        unsigned char *big_ptr = NULL;
        unsigned char *little_ptr = NULL;

        if (big_len >= little_len && big_len > 0 && little_len > 0) {
                big_ptr = (unsigned char *)big;
                little_ptr = (unsigned char *)little;
                while (*big_ptr) {
                        if (mx_memcmp(big_ptr, little_ptr, little_len - 1) == 0)
                                return big_ptr;
                        big_ptr++;
                }
        }
        return NULL;
}
