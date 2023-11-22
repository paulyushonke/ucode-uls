#ifndef LIBMX_H
#define LIBMX_H

#include <unistd.h>
#include <wchar.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

#define MX_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MX_MIN(x, y) (((x) < (y)) ? (x) : (y))

#ifdef __MACH__
#include <malloc/malloc.h>
#else
#include <malloc.h>
#define malloc_size malloc_usable_size
#endif

typedef struct s_list {
    void *data;
    struct s_list*next;
}           t_list;


//MY
int mx_get_index(char **strarr, char *str);
int mx_atoi(const char *str);
void mx_printerr(const char *s);
int mx_strncmp(const char *s1, const char *s2, int n);
bool mx_isspace(char c);
bool mx_isdigit(int c);
bool mx_isalpha(int c);
bool mx_isupper(int c);
void mx_clear_list(t_list **list);
void mx_printerrch(const char c);
void mx_reverse_list(t_list *head);
char *mx_strchr(const char *s, int c);
char *mx_strchr_last(const char *s, int c);
int mx_get_digits_count(int num);
void mx_printnchar(char character, size_t count);
void mx_printnstr(const char *string, size_t count);
t_list *mx_get_by_index(t_list *head, int index);



void mx_printchar(char c);
void mx_print_unicode(wchar_t c);
size_t mx_printstr(const char *const string);
void mx_print_strarr(char **arr, const char *delim);
void mx_printint(int n);
double mx_pow(double nm, unsigned int pow);
char *mx_nbr_to_hex(unsigned long nbr);
unsigned long mx_hex_to_nbr(const char *hex);
char *mx_itoa(int number);
void mx_foreach(int *arr, int size, void (*f)(int));
int mx_binary_search(char **arr, int size, const char *s, int *count);
int mx_bubble_sort(char **arr, int size);
int mx_quicksort(char **arr, int left, int right);
int mx_strlen(const char *s);
void mx_swap_char(char *s1, char *s2);
void mx_str_reverse(char *s);
void mx_strdel(char **str);
void mx_del_strarr(char ***arr);
int mx_get_char_index(const char *str, char c);
char *mx_strdup(const char *s1);
char *mx_strndup(const char *s1, size_t n);
char *mx_strcpy(char *dst, const char *src);
char *mx_strncpy(char *dst, const char *src, int len);
int mx_strcmp(const char *s1, const char *s2);
char *mx_strcat(char *restrict s1, const char *restrict s2);
char *mx_strstr(const char *haystack, const char *needle);
int mx_get_substr_index(const char *str, const char *sub);
int mx_count_substr(const char *str, const char *sub);
int mx_count_words(const char *str, char c);
char *mx_strnew(const int size);
char* mx_strtrim(const char *str);
char *mx_del_extra_spaces(const char *str);
char **mx_strsplit(const char *s, char c);
char *mx_strjoin(const char *s1, const char *s2);
char *mx_file_to_str(const char *file);
char *mx_replace_substr(const char *str, const char *sub, const char *replace);
int mx_read_line(char **lineptr, size_t buf_size, char delim, const int fd);
void *mx_memset(void *b, int c, size_t len);
void *mx_memcpy(void *restrict dst, const void *restrict src, size_t n);
void *mx_memccpy(void *restrict dst, const void *restrict src, int c, size_t n);
int mx_memcmp(const void *s1, const void *s2, size_t n);
void *mx_memchr(const void *s, int c, size_t n);
void *memrchr(const void *s, int c, size_t n);
void *mx_memmem(const void *big, size_t big_len, const void *little, size_t little_len);
void *mx_memmove(void *dst, const void *src, size_t len);
void *mx_realloc(void *ptr, size_t size);
int mx_sqrt(int x);
t_list *mx_create_node(void *data);
void mx_push_front(t_list **list, void *data);
void mx_push_back(t_list **list, void *data);
void mx_pop_front(t_list **head);
void mx_pop_back(t_list **head);
int mx_list_size(t_list *list);
t_list *mx_sort_list(t_list *lst, bool (*cmp)(void *, void *));

#endif
