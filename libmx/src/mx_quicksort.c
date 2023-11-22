#include "../inc/libmx.h"

int mx_quicksort(char **arr, int left, int right) {
    if (!arr) {
        return -1;
    }
    int l = left;
    int r = right;
    int midl = mx_strlen(arr[(l + r) >> 1]);
    int swaps = 0;
    char* forswap;
    while (l <= r) {
        while (mx_strlen(arr[l]) < midl) 
            l++;
        while (mx_strlen(arr[r]) > midl) 
            r--;
        if (l <= r) {
            if (mx_strlen(arr[l]) != mx_strlen(arr[r])) {
                forswap = arr[l];
                arr[l] = arr[r];
                arr[r] = forswap;
                swaps++;
            }
            l++;
            r--;
        }
    }
    if (left < r)
        swaps += mx_quicksort(arr, left, r);
    if (right > l)
        swaps += mx_quicksort(arr, l, right);
    return swaps;
}
