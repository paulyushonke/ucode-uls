#include "../inc/uls.h"

t_entry *mx_create_entry_ptr(char *relative_path) {
    t_entry *entry = malloc(sizeof(t_entry));
    
    if (entry) {
        entry->relative_path = mx_strdup(relative_path);
        entry->dirent = NULL;
        lstat(relative_path, &entry->stat);
        entry->entries_list = NULL;
    }

    return entry;
}
