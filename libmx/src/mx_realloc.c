#include "../inc/libmx.h"

void *mx_realloc(void *ptr, size_t size) {

    
    size_t cur;
    void *new_ptr;

    if (ptr == 0)
	    return malloc(size);
    cur = malloc_size(ptr);
    if (size <= cur)
	    return ptr;
    new_ptr = malloc(size);
    mx_memcpy(new_ptr, ptr, cur);
    free(ptr);
    return new_ptr;
}
