#include "../inc/libmx.h"

int mx_strlen(const char *s) {
if (s == NULL) {
        return 0;
}
int m = 0;

while(s[m] != '\0') {
m++;
}

return m;
}
