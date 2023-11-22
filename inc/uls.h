#ifndef ULS_H
#define ULS_H

#include "../libmx/inc/libmx.h"
#include <sys/acl.h>
#include <dirent.h>
#include <grp.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <sys/xattr.h>
#include <sys/stat.h>
#include <stddef.h>
#include <sys/ttycom.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <pwd.h>

#define MAX2(a, b) (a > b ? a : b)
#define MAX3(a, b, c) (MAX2(MAX2(a, b), c))
#define MAX4(a, b, c, d) (MAX2(MAX3(a, b, c), d))
#define MAX5(a, b, c, d, e) (MAX2(MAX4(a, b, c, d), e))
#define MAX6(a, b, c, d, e, f) (MAX2(MAX5(a, b, c, d, e), f))

#define BLUE "\x1b[34m"
#define PURPLE "\x1b[35m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"

typedef const ushort c_ushort;
typedef const size_t c_size_t;
typedef const bool c_bool;
typedef const char c_char;
typedef const float c_float;
typedef const int c_int;
typedef const char *const c_str;
typedef const char *const *const c_str_arr;
typedef char *str;
typedef unsigned char uchar;


typedef struct s_args {
    t_list *entry_names_list;
    char *flags_str;
} t_args;
typedef const t_args c_args;
typedef c_args *const c_args_ptr;

typedef enum e_args_error_code {
    without_err,
    err_flag,
    no_ext_entr
} t_args_error_code;

typedef struct s_args_error {
    t_args_error_code error_code;
    t_list *invalid_entry_names_list;
    char illegal_flag;
} t_args_error;
typedef const t_args_error c_args_error;
typedef c_args_error *const c_args_error_ptr;

typedef enum e_output_format {
    one_entr_line_outform,
    some_col_outform,
    lon_outform,
    sep_com_outform
} t_output_format;

typedef struct s_entry {
    char *relative_path;
    struct dirent *dirent;
    struct stat stat;
    t_list *entries_list;
    bool no_permission;
} t_entry;

typedef struct s_files_dirs {
    t_list *files_list;
    t_list *dirs_list;
    size_t total_entries_count;
} t_files_dirs;

typedef enum e_time_type {
    data_modify_time,
    access_time,
    file_stchange_time,
    create_file_time
} t_time_type;
typedef const t_time_type c_time_type;

typedef enum e_entry_printing_flags {
    in_color = 2,
    slash_dir_print = 4
} t_entry_printing_flags;
typedef const t_entry_printing_flags c_entry_printing_flags;

typedef enum e_long_format_flags {
    print_ext_atr = 4,
    info = 8,
    hmn_rdbl_size = 16,
    own_name = 64,
    group_name = 128,
    print_access_list = 256
} t_long_format_flags;
typedef const t_long_format_flags c_long_format_flags;

typedef enum e_find_entries_flags {
    start_point = 2,
    ignr_dir = 4
} t_find_entries_flags;
typedef struct s_flags {
    unsigned int S;
    unsigned int t;
    unsigned int u;
    unsigned int at;
    unsigned int m;
    unsigned int o;
    unsigned int p;
    unsigned int C;
    unsigned int G;
    unsigned int T;
    unsigned int f;
    unsigned int r;
    unsigned int A;
    unsigned int R;
    unsigned int a;
    unsigned int d;
    unsigned int one;
    unsigned int c;
    unsigned int e;
    unsigned int g;
    unsigned int h;
    unsigned int l;
    unsigned int U;
  
} t_flags;
typedef const t_flags c_flags;
typedef const t_flags *const c_flags_ptr;

void mx_free_args(t_args args);
bool mx_sort_name(void *, void *);
bool mx_sort_size(void *, void *);
bool mx_sort_modify(void *, void *);
bool mx_sort_access(void *, void *);
bool mx_sort_time(void *, void *);

void mx_print_char_and_int_if( c_bool condition, c_char character, c_int integer);
size_t mx_printstr_if(c_bool condition, c_str string);
void mx_print_two_chars(c_char character1, c_char character2);
void mx_print_two_strings_if(c_bool condition, c_str string1, c_str string2);
void mx_printchar_if_con(c_bool condition, c_char character);
float mx_round_down(c_float number);
float mx_round_up(c_float number);
float mx_round(c_float number);
void mx_print_args_error(t_args_error args_error, const char *);
void mx_free_args_error(t_args_error args_error);
void mx_free_entries_list(t_list *);
void mx_print_files_dirs(t_list *, t_flags *, c_size_t invalid_paths_to_entries_count);
void mx_sort_entr_lst_rec(t_list *, t_flags *);
void mx_free_entry_ptr(t_entry **);
void mx_print_entries_in_columns(t_list *, c_char column_delimiter, ushort terminal_width, t_entry_printing_flags flags);
void mx_print_form_entr(t_list *, c_time_type time_type, bool print_total_number_of_512_byte_blocks, c_long_format_flags long_format_flags, c_entry_printing_flags entry_printing_flags);
void mx_print_lon_frmtd_entr(t_entry entry, size_t *, c_time_type time_type, c_long_format_flags long_format_flags, c_entry_printing_flags entry_printing_flags);
void mx_free_dirs(t_files_dirs *);
t_files_dirs mx_separate_entries(t_list *, c_bool follow_link);
t_args_error mx_validate_args(c_args_ptr args, c_str ext_flag);
t_list *mx_find_entries_list(t_list *, t_list *, t_flags *);
t_flags mx_create_flags(char *);
t_list *mx_get_entr_dir(t_entry *directory, t_find_entries_flags find_entries_flags);
t_list *mx_get_entr_dir_rec(t_entry *directory, t_find_entries_flags find_entries_flags);
t_entry *mx_create_entry_ptr(char *);
t_args mx_convert_to_args(c_int argc, c_str_arr argv);
t_args mx_create_args(void);
size_t mx_print_entry_name(t_entry *entry, bool colorized, bool slash_after_dirs);

#endif
