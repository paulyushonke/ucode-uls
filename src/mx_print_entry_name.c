#include "../inc/uls.h"

size_t mx_print_entry_name(t_entry *entry, bool colorized, bool slash_after_dirs) {
    if (colorized) {
        mode_t m = entry->stat.st_mode;
        if (S_ISDIR(m)) {
            mx_printstr(BLUE);
        } else if (S_ISLNK(m)) {
            mx_printstr(PURPLE);
        } else if (m & S_IXUSR || m & S_IXGRP || m & S_IXOTH) {
            mx_printstr(RED);
        } else if (S_ISSOCK(m)) {
            mx_printstr(GREEN);
        }
    }
    size_t size= mx_printstr(entry->dirent ? entry->dirent->d_name : entry->relative_path);
    if (colorized) {
        mx_printstr("\x1b[m");
    }
    if (slash_after_dirs && S_ISDIR(entry->stat.st_mode)) {
        mx_printchar('/');
        size++;
    }
    return size;
}
