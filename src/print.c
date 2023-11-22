#include "../inc/uls.h"

void mx_print_char_and_int_if(c_bool condition, c_char character, c_int integer) {
    if (condition) {
        mx_printchar(character);
        mx_printint(integer);
    }
}

size_t mx_printstr_if(c_bool condition, c_str string) {
    if (condition) {
        return mx_printstr(string);
    }
    return 0;
}
void mx_print_two_chars(c_char character1, c_char character2) {
    mx_printchar(character1);
    mx_printchar(character2);
}

void mx_print_two_strings_if(c_bool condition, c_str string1, c_str string2) {
    if (condition) {
        mx_printstr(string1);
        mx_printstr(string2);
    
    }
}

void mx_printchar_if_con(c_bool condition, c_char character) {
    if (condition) {
        mx_printchar(character);
    }
}
