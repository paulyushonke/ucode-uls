#include "../inc/uls.h"

t_list *mx_get_entr_dir(t_entry directory, t_find_entries_flags find_entries_flags) {
    t_list *read_entries_list = NULL;
    DIR *dirstream = opendir(directory.relative_path);

    for (struct dirent *i = readdir(dirstream); i != NULL; i = readdir(dirstream)) {
        if (!(find_entries_flags & start_point) && i->d_name[0] == '.') {
            continue;
        }
        if ((find_entries_flags & ignr_dir) && (mx_strcmp(i->d_name, ".") == 0 || mx_strcmp(i->d_name, "..") == 0)) {
            continue;
        }
        char *new_entry_path = mx_strnew(mx_strlen(directory.relative_path) + mx_strlen("/") + mx_strlen(i->d_name));
        mx_strcat(mx_strcat(mx_strcat(new_entry_path, directory.relative_path), "/"), i->d_name);
        t_entry *new_entry = mx_create_entry_ptr(new_entry_path);
        free(new_entry_path);
        new_entry->dirent = malloc(sizeof(struct dirent));
        *(new_entry->dirent) = *i;
        mx_push_back(&read_entries_list, new_entry);
    }

    closedir(dirstream);

    return read_entries_list;
}

t_list *mx_get_entr_dir_rec(t_entry directory, t_find_entries_flags find_entries_flags) {
    t_list *read_entries_list = mx_get_entr_dir(directory, find_entries_flags);
    for (t_list *i = read_entries_list; i != NULL; i = i->next) {
        t_entry *entry = (t_entry *)i->data;
        if (S_ISDIR(entry->stat.st_mode) && mx_strcmp(entry->dirent->d_name, ".") != 0 && mx_strcmp(entry->dirent->d_name, "..") != 0) {
            entry->entries_list = mx_get_entr_dir_rec(*entry, find_entries_flags);
        }
    }
    return read_entries_list;
}
