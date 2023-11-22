#include "../inc/uls.h"

void mx_free_args_error(t_args_error args_error) {
    for (t_list *n = args_error.invalid_entry_names_list; n != NULL; n = n->next) {
        free(n->data);
    }
    mx_clear_list(&args_error.invalid_entry_names_list);
}

t_args_error mx_validate_args(c_args_ptr args, c_str ext_flag) {
    t_args_error ar_er = {
        .error_code = without_err,
        .invalid_entry_names_list = NULL,
        .illegal_flag = '\0'
    };

    if (args->flags_str != NULL) {
        for (int i = 0; args->flags_str[i] != '\0'; i++) {
            if (mx_strchr(ext_flag, args->flags_str[i]) == NULL) {
                ar_er.error_code = err_flag;
                ar_er.illegal_flag = args->flags_str[i];
                return ar_er;
            }
        }
    }

    struct stat stat_buffer;
    for (t_list *n = args->entry_names_list; n != NULL; n = n->next) {
        errno = 0;
        stat(n->data, &stat_buffer);
        if (errno != 0) {
            ar_er.error_code = no_ext_entr;
            mx_push_front(&ar_er.invalid_entry_names_list, mx_strdup(n->data));
        }
    }

   return ar_er;
}
