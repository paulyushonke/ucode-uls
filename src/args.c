#include "../inc/uls.h"

t_args mx_convert_to_args(c_int argc, c_str_arr argv) {
    t_args new_args = mx_create_args();

    int argv_index = 1;
    for (; argv_index < argc && argv[argv_index][0] == '-' && argv[argv_index][1] != '\0'; argv_index++) {
        char *flag = mx_strjoin(new_args.flags_str, argv[argv_index] + 1);
        free(new_args.flags_str);
        new_args.flags_str = flag;
    }

    for (; argv_index < argc; argv_index++) {
        mx_push_front(&new_args.entry_names_list, mx_strdup(argv[argv_index]));
    }

    if (new_args.entry_names_list == NULL) {
        mx_push_front(&new_args.entry_names_list, mx_strdup("."));
    }

    return new_args;
}

t_args mx_create_args(void) {
    t_args args = {
        .entry_names_list = NULL,
        .flags_str = NULL
    };
    return args;
}

void mx_free_args(t_args args) {
    for (t_list *node = args.entry_names_list; node != NULL; node = node->next) {
        char *ent = (char *)node->data;
        free(ent);
    }
    mx_clear_list(&args.entry_names_list);
    free(args.flags_str);
}

static bool sort_strings_by_name(void *a, void *b) {
    char *a_str = (char *)a;
    char *b_str = (char *)b;
    return mx_strcmp(a_str, b_str) > 0;
}

void mx_print_args_error(t_args_error args_error, const char *existing_flags) {
    if (args_error.error_code == no_ext_entr) {
        mx_sort_list(args_error.invalid_entry_names_list, sort_strings_by_name);
        for (t_list *node = args_error.invalid_entry_names_list; node != NULL; node = node->next) {
            mx_printerr("uls");
            mx_printerr(": ");
            mx_printerr(node->data);
            mx_printerr(": ");
            mx_printerr("No such file or directory");
            mx_printerr("\n");
        }
    } else if (args_error.error_code == err_flag) {
        mx_printerr("uls: illegal option -- ");
        mx_printerrch(args_error.illegal_flag);
        mx_printerrch('\n');
        mx_printerr("usage: uls [-");
        mx_printerr(existing_flags);
        mx_printerr("] [file ...]\n");
    }
}
