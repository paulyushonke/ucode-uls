#include "../inc/libmx.h"

int mx_binary_search(char **arr, int size, const char *s, int *count) {
    int l = 0;
    int r = size - 1;

    if (arr == NULL || s == NULL || count == NULL || size <= 0) {
        *count = 0;
        return -1;
    }

    while (l <= r) {
        (*count)++;
        int mid = l + (r - l) / 2;
        int k = mx_strcmp(arr[mid], s);

        if (k == 0) {
            return mid;
        } else if (k < 0) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }

    *count = 0;
    return -1;
}
