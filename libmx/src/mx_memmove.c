#include "../inc/libmx.h"

void *mx_memmove(void *dst, const void *src, size_t len) {
	unsigned char *swp = malloc(len);

	swp = mx_memcpy(swp, src, len);
	dst = mx_memcpy(dst, swp, len);
	free(swp);
	return dst;
}
