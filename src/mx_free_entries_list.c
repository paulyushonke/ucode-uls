#include "../inc/uls.h"

void mx_free_entry(t_entry entry) {
    free(entry.relative_path);
    free(entry.dirent);
    for (t_list *node = entry.entries_list; node != NULL; node = node->next) {
        mx_free_entry(*(t_entry *)node->data);
        free(node->data);
    }
    mx_clear_list(&entry.entries_list);
}

void mx_free_entry_ptr(t_entry **entry) {
    mx_free_entry(**entry);
    free(*entry);
    *entry = NULL;
}

void mx_free_entries_list(t_list *entries_list) {
    for (t_list *node = entries_list; node != NULL; node = node->next) {
        t_entry *entry = (t_entry *)node->data;
        mx_free_entry_ptr(&entry);
    }
    mx_clear_list(&entries_list);
}
