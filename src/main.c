#include "../inc/uls.h"

int main(c_int argc, c_str_arr argv) {
    c_str FLAGS = "ACGRSTU@acdefghlmoprtu1";

    c_args args = mx_convert_to_args(argc, argv);
    t_args_error args_error = mx_validate_args(&args, FLAGS);
    mx_print_args_error(args_error, FLAGS);
    if (args_error.error_code != err_flag) {
        t_flags f = mx_create_flags(args.flags_str);
        t_list *entries_list = mx_find_entries_list(args.entry_names_list, args_error.invalid_entry_names_list, &f);
        mx_sort_entr_lst_rec(entries_list, &f);
        mx_print_files_dirs(entries_list, &f, mx_list_size(args_error.invalid_entry_names_list));
        mx_free_entries_list(entries_list);
    }

    mx_free_args(args);
    mx_free_args_error(args_error);

    return args_error.error_code ? 1 : 0;
}
