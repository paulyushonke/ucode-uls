#include "../inc/uls.h"

void mx_sort_entr_lst_rec(t_list *entries_list, t_flags *flags) {
    if (flags->f) {
        return;
    } else if (flags->S) {
        mx_sort_list(entries_list, mx_sort_size);
    } else if (flags->t) {
        if (flags->u > flags->U && flags->u > flags->c) {
            mx_sort_list(entries_list, mx_sort_access);
        } else if (flags->U > flags->u && flags->U > flags->c) {
            mx_sort_list(entries_list, mx_sort_time);
        } else if (flags->c) {
            mx_sort_list(entries_list, mx_sort_access);
        } else {
            mx_sort_list(entries_list, mx_sort_modify);
        }
    } else {
        mx_sort_list(entries_list, mx_sort_name);
    }

    if (flags->r) {
        mx_reverse_list(entries_list);
    }

    for (t_list *i = entries_list; i != NULL; i = i->next) {
        mx_sort_entr_lst_rec(((t_entry *)i->data)->entries_list, flags);
    }
}
