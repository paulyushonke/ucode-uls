#include "../inc/libmx.h"

char *mx_nbr_to_hex(unsigned long nbr) {
char *arr = mx_strnew(30);
    int i = 0;
    int len = 0;
    while(nbr != 0) {
        int k = 0;
        k = nbr % 16;
        if(k < 10) {
            arr[i] = k + 48;
            i++;
        } 
        else {
            arr[i] = k + 87;
            i++;
        }
        nbr = nbr / 16;
        len++;
    }
    char *result = mx_strnew(len);
    for(int i =0; i < len; i++){
        result[i] = arr[len - i - 1];
    }
    return result;
}
