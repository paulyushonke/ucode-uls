#include "../inc/libmx.h"

char *mx_strdup(const char *str) {

char *arr = mx_strnew(mx_strlen(str));
    
mx_strcpy(arr, str);

return arr;
}
