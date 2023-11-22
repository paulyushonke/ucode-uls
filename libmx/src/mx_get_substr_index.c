#include "../inc/libmx.h"

int mx_get_substr_index(const char *str, const char *sub) {
    if (str == NULL || sub == NULL) {
        return -2; 
    }

    int length1 = mx_strlen(str);
    int length2 = mx_strlen(sub);

    if (length2 == 0) {
        return 0; 
    }

    for (int i = 0; i <= length1 - length2; i++) {
        int j;

        for (j = 0; j < length2; j++) {
            if (str[i + j] != sub[j]) {
                break; 
            }
        }

        if (j == length2) {
            return i; 
        }
    }

    return -1; 
}
