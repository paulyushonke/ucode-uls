#include "../inc/libmx.h"

char *mx_replace_substr(const char *str, const char *sub, const char *replace) {
    if (str == NULL || sub == NULL || replace == NULL) {
        return NULL;
    }

    int str_len = mx_strlen(str);
    int sub_len = mx_strlen(sub);
    int replace_len = mx_strlen(replace);

    int count = 0;
    const char *ptr = str;
    while ((ptr = mx_strstr(ptr, sub)) != NULL) {
        count++;
        ptr += sub_len;
    }

    int newlen = str_len + (replace_len - sub_len) * count + 1;
    char *result = mx_strnew(newlen);

    if (result == NULL) {
        return NULL;
    }

    char *dest = result;
    ptr = str;

    while (*ptr) {
        if (mx_strncmp(ptr, sub, sub_len) == 0) {
            mx_strcpy(dest, replace);
            dest += replace_len;
            ptr += sub_len;
        } else {
            *dest++ = *ptr++;
        }
    }

    *dest = '\0';

    return result;
}
