#include "../inc/uls.h"

t_files_dirs mx_separate_entries(t_list *entries_list, c_bool follow_link) {
    t_files_dirs files_dirs = {
        .files_list = NULL,
        .dirs_list = NULL,
        .total_entries_count = 0
    };

    for (t_list *i = entries_list; i != NULL; i = i->next) {

        t_entry entry = *(t_entry *)i->data;

        if (follow_link) {
            struct stat fol_link;
            stat(entry.relative_path, &fol_link);
            if (S_ISDIR(fol_link.st_mode)) {
                mx_push_back(&files_dirs.dirs_list, i->data);
            }
        } else if (S_ISDIR(entry.stat.st_mode)) {
            mx_push_back(&files_dirs.dirs_list, i->data);
        }
        if (S_ISREG(entry.stat.st_mode)) {
            mx_push_back(&files_dirs.files_list, i->data);
        }
        files_dirs.total_entries_count++;
    }

    return files_dirs;
}
