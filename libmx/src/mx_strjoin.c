#include "../inc/libmx.h"

char *mx_strjoin(const char *s1, char const *s2) {
if (s1 == NULL && s2 == NULL)
        return NULL;
else if (s1 == NULL)
	return mx_strdup(s2);
else if (s2 == NULL)
	return mx_strdup(s1);
else {
int first = mx_strlen(s1), second = mx_strlen(s2);
char *result = mx_strnew(first + second);
result =  mx_strcpy(result, s1);
result =  mx_strcat(result, s2);

return result;
}
}
