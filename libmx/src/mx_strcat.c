#include "../inc/libmx.h"

char *mx_strcat(char *restrict s1, const char *restrict s2) {
    int length_s1 = mx_strlen(s1);
    int length_s2 = mx_strlen(s2);
    int counter = length_s1;
    
    for(int i = 0; counter < length_s1 + length_s2; i++) {
        s1[counter] = s2[i];
        counter++;
    }
    s1[counter++] = '\0';
    return s1;
}
