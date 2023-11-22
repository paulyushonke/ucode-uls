#include "../inc/libmx.h"

int mx_bubble_sort(char **arr, int size) {
    int swaps = 0;
    int sort = 0;

    if (arr == NULL || size <= 1)
        return swaps;

    while (!sort) {
        sort = 1;
        for (int i = 0; i < size - 1; i++) {
            if (mx_strcmp(arr[i], arr[i + 1]) > 0) {
                char *swapelem = arr[i];
                arr[i] = arr[i + 1];
                arr[i + 1] = swapelem;
                swaps++;
                sort = 0;
            }
        }
        size--;
    }

    return swaps;
}
