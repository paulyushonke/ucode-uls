#include "../inc/libmx.h"

void mx_print_unicode(wchar_t c) {
    if (c < 128) {
        char result = c;
        write(1, &result, sizeof(result));
    }
    else if (c < 2048) {
        unsigned char res[2];
        res[0] = (c >> 6 & 0x1f) | 0xc0 ;
        res[1] = (c >> 0 & 0x3f) | 0x80;
        write(1, res ,sizeof(res));
    }
    else if (c < 65536) {
        unsigned char res[3];
        res[0] = (c >> 12 & 0x0f) | 0xe0;
        res[1] = (c >> 6 & 0x3f) | 0x80;
        res[2] = (c >> 0 & 0x3f) | 0x80;
        write(1, res, sizeof(res));
    }
    else {
        unsigned char res[4];
        res[0] = (c >> 18 & 0x07) | 0xf0;
        res[1] = (c >> 12 & 0x0f) | 0x80;
        res[2] = (c >> 6 & 0x3f) | 0x80;
        res[3] = (c >> 0 & 0x3f) | 0x80;
        write(1, res, sizeof(res));
    }
}
