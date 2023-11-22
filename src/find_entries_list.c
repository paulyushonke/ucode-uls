#include "../inc/uls.h"

static t_find_entries_flags to_find_entr_flgs(t_flags *flags) {
    t_find_entries_flags find_entries_flags = 0;
    find_entries_flags |= (flags->a || flags->A || flags->f ? start_point : 0) |
                          (!flags->a && flags->A && !flags->f ? ignr_dir : 0);
    return find_entries_flags;
}

t_list *mx_find_entries_list(t_list *all_inputted_paths_to_entries_list, t_list *invalid_inputted_paths_to_entries_list, t_flags *flags) {
    t_list *li = NULL;

    for (t_list *i = all_inputted_paths_to_entries_list; i != NULL; i = i->next) {
        bool invalid = false;
        for (t_list *j = invalid_inputted_paths_to_entries_list; j != NULL; j = j->next) {
            if (mx_strcmp(i->data, j->data) == 0) {
                invalid = true;
            }
        }
        if (invalid) continue;
        mx_push_front(&li, mx_create_entry_ptr(i->data));
    }

    if (flags->d) {
        return li;
    }

    for (t_list *i = li; i != NULL; i = i->next) {
        t_entry *entry = (t_entry *)i->data;
        if (!S_ISDIR(entry->stat.st_mode) && (!S_ISLNK(entry->stat.st_mode) || (flags->l || flags->o || flags->g))) continue;
        if (flags->R) {
            entry->entries_list = mx_get_entr_dir_rec(entry, to_find_entr_flgs(flags));
        } else {
            entry->entries_list = mx_get_entr_dir(entry, to_find_entr_flgs(flags));
        }
    }

    return li;
}
