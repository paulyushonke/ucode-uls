#include "../inc/libmx.h"

char *mx_itoa(int number) {
	int length = 0;
	int temp = number;

	while (temp) {
		temp /= 10;
		length++;
	}

	temp = number;
	char *str_number = mx_strnew(length);

	if (number == 0)
		return mx_strcpy(str_number, "0");
	if (number == -2147483648)
		return mx_strcpy(str_number, "-2147483648");
	for (int i = 0; i < length; i++) {
		if (temp < 0) {
			str_number[length] = '-';
			temp = -temp;
		}
		str_number[i] = (temp % 10) + '0';
		temp /= 10;
	}
	mx_str_reverse(str_number);
	return str_number;
}
