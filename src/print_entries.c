#include "../inc/uls.h"

static t_flags *Flags = NULL;
static t_output_format outputform;
static char coldelim = 0;
static ushort terwidth = 0;
static t_time_type timetype;
static bool totnum = false;

static bool PermissionDenied = false;

void mx_printerrn(const char *s, int n) {
    int s_len = mx_strlen(s);
	write(2, s, n > s_len ? s_len : n);
}

void mx_free_dirs(t_files_dirs *files_dirs) {
    mx_clear_list(&files_dirs->files_list);
    mx_clear_list(&files_dirs->dirs_list);
}


static ushort mx_get_terminal_width(void) {
    struct winsize winsize;
    ioctl(0, TIOCGWINSZ, &winsize);
    return winsize.ws_col;
}

void mx_print_sep_com(t_list *entries_list, ushort terminal_width, c_entry_printing_flags flags) {
    if (entries_list == NULL) return;

    size_t count_line = 0;
    for (t_list *i = entries_list; i != NULL; i = i->next) {
        t_entry *entry = (t_entry *)i->data;

        size_t entry_size = mx_strlen(entry->dirent ? entry->dirent->d_name : entry->relative_path);

        if (count_line + entry_size + (i->next ? 2 : 0) >= terminal_width) {
            mx_printchar('\n');
            count_line = 0;
        }

        count_line += mx_print_entry_name(entry, flags & in_color, flags & slash_dir_print);
        count_line += mx_printstr_if(i->next, ", ");
    }
    mx_printchar('\n');
}

static c_time_type get_time_type(c_flags_ptr flags) {
    t_time_type time_type = data_modify_time;
    if (flags->c) {
        time_type = file_stchange_time;
    } else if (flags->u) {
        time_type = access_time;
    } else if (flags->U) {
        time_type = create_file_time;
    }
    return time_type;
}

static t_output_format get_output_format(t_flags *flags) {
    bool output_is_to_terminal = isatty(STDOUT_FILENO);
    int priority_flag = MAX6(flags->C, flags->one, flags->l, flags->o, flags->g, flags->m);
    priority_flag = priority_flag == 0 ? -1 : priority_flag;
    if (output_is_to_terminal) {
        if (priority_flag == (int)flags->one) {
            return one_entr_line_outform;
        } else if (priority_flag == (int)flags->l || priority_flag == (int)flags->o || priority_flag == (int)flags->g) {
            return lon_outform;
        } else if (priority_flag == (int)flags->m) {
            return sep_com_outform;
        } else {
            return some_col_outform;
        }
    } else {
        if (priority_flag == (int)flags->C) {
            return some_col_outform;
        } else if (priority_flag == (int)flags->l || priority_flag == (int)flags->o || priority_flag == (int)flags->g) {
            return lon_outform;
        } else if (priority_flag == (int)flags->m) {
            return sep_com_outform;
        } else {
            return one_entr_line_outform;
        }
    }
}

static t_long_format_flags to_long_flags(t_flags *flags) {
    t_long_format_flags long_format_flags = 0;
    long_format_flags |= (flags->at ? print_ext_atr : 0) |
                         (flags->T ? info : 0) |
                         (flags->h ?  hmn_rdbl_size : 0) |
                         (flags->o ? group_name : 0) |
                         (flags->g ? own_name : 0) |
                         (flags->e ? print_access_list : 0);
    return long_format_flags;
}

static t_entry_printing_flags to_entr_flags(t_flags *flags) {
    t_entry_printing_flags entry_printing_flags = 0;
    entry_printing_flags |= (flags->G && isatty(STDOUT_FILENO) ? in_color : 0) |
                            (flags->p ? slash_dir_print : 0);
    return entry_printing_flags;
}

static void print_entries(t_list *entries_list, bool print_newline_in_the_end) {
    switch (outputform) {
        case one_entr_line_outform:
            
            if (entries_list == NULL) {
        return;
    }
    for (t_list *i = entries_list; i != NULL; i = i->next) {
        t_entry *entry = (t_entry *)i->data;
        mx_print_entry_name(entry, to_entr_flags(Flags) & in_color, to_entr_flags(Flags) & slash_dir_print);
        mx_printchar('\n');
    }
        break;
        case some_col_outform:
            mx_print_entries_in_columns(entries_list, coldelim, terwidth, to_entr_flags(Flags));
        break;
        case lon_outform:
            mx_print_form_entr(entries_list, timetype, totnum, to_long_flags(Flags), to_entr_flags(Flags));
        break;
        case sep_com_outform:
            mx_print_sep_com(entries_list, terwidth, to_entr_flags(Flags));
        break;
    }
    mx_printchar_if_con(print_newline_in_the_end && entries_list, '\n');
}

static void mx_print_dir_rec(t_entry *directory, bool print_dir_name, bool print_newline_in_the_beginning) {
    mx_printchar_if_con(print_newline_in_the_beginning, '\n');
    mx_print_two_strings_if(print_dir_name, directory->relative_path, ":\n");
    if (directory->no_permission) {
        char *path = directory->relative_path;
        size_t last_char_index_of_name = mx_strlen(path) - 1;
        for ( ; path[last_char_index_of_name] == '/' ; last_char_index_of_name--) { }
        int first_char_index_of_name = last_char_index_of_name;
        for ( ; path[first_char_index_of_name] != '/' && first_char_index_of_name != -1; first_char_index_of_name--) { }
        first_char_index_of_name++;

        mx_printerr("uls");
        mx_printerr(": ");
        mx_printerrn(path + first_char_index_of_name, last_char_index_of_name - first_char_index_of_name + 1);
        mx_printerr(": ");
        mx_printerr("Permission denied\n");
        PermissionDenied = true;
        return;
    }
    totnum = true;
    print_entries(directory->entries_list, false);

    t_files_dirs files_dirs = mx_separate_entries(directory->entries_list, false);
    for (t_list *i = files_dirs.dirs_list; i != NULL; i = i->next) {
        t_entry *i_directory = (t_entry *)i->data;
        if (mx_strcmp(i_directory->dirent->d_name, ".") != 0 && mx_strcmp(i_directory->dirent->d_name, "..") != 0) {
            mx_print_dir_rec((t_entry *)i->data, true, true);
        }
    }
    mx_free_dirs(&files_dirs);
}

static void set_global_variables(t_flags *flags) {
    Flags = flags;
    outputform = get_output_format(flags);
    switch (outputform) {
        case one_entr_line_outform:
        break;
        case some_col_outform:
            terwidth = mx_get_terminal_width();
            coldelim = flags->G && isatty(STDOUT_FILENO) ? ' ' : '\t';
        break;
        case lon_outform:
            timetype = get_time_type(flags);
        break;
        case sep_com_outform:
            terwidth = mx_get_terminal_width();
        break;
    }
}

void mx_print_files_dirs(t_list *entries_list, t_flags *flags, c_size_t invalid_paths_to_entries_count) {
    set_global_variables(flags);

    if (flags->d) {
        print_entries(entries_list, false);
    } else {
        t_files_dirs files_dirs = mx_separate_entries(entries_list, !flags->l && !flags->g && !flags->o);
        totnum = files_dirs.dirs_list != NULL;
        print_entries(files_dirs.files_list, files_dirs.dirs_list != NULL);
        if (flags->R) {
            for (t_list *i = files_dirs.dirs_list; i != NULL; i = i->next) {
                t_entry *directory = (t_entry *)i->data;
                mx_print_dir_rec(directory, files_dirs.total_entries_count + invalid_paths_to_entries_count > 1, false);
                mx_printchar_if_con(i->next != NULL, '\n');
            }
        } else {
           
            for (t_list *i = files_dirs.dirs_list; i != NULL; i = i->next) {
                t_entry *directory = (t_entry *)i->data;
                mx_print_two_strings_if(files_dirs.total_entries_count + invalid_paths_to_entries_count > 1, directory->relative_path, ":\n");
                if (directory->no_permission) {
                    char *path = directory->relative_path;
                    size_t last_char_index_of_name = mx_strlen(path) - 1;
                    for ( ; path[last_char_index_of_name] == '/' ; last_char_index_of_name--) { }
                    int first_char_index_of_name = last_char_index_of_name;
                    for ( ; path[first_char_index_of_name] != '/' && first_char_index_of_name != -1; first_char_index_of_name--) { }
                    first_char_index_of_name++;

                    mx_printerr("uls");
                    mx_printerr(": ");
                    mx_printerrn(path + first_char_index_of_name, last_char_index_of_name - first_char_index_of_name + 1);
                    mx_printerr(": ");
                    mx_printerr("Permission denied\n");
                    PermissionDenied = true;
                    mx_printstr_if(i->next != NULL, "\n");
                    continue;
                }
                totnum = true;

            //     if (directory->entries_list == NULL && i->next != NULL ) {
            //         mx_printchar('\n');  
            //    }
                print_entries(directory->entries_list, i->next != NULL);   
            }
        }
        mx_free_dirs(&files_dirs);
    }
}

