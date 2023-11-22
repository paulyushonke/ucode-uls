#include "../inc/libmx.h"

double mx_pow(double n, unsigned int pow) {
double m = 1;

if (pow == 0) { 
return 1.0;
}
else {
for ( ; pow >= 1; pow--) 
m *= n;
return m;
}
}
