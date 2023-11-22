#include "../inc/uls.h"

static ushort mx_max_elem_size(t_list *entries_list, bool slash_after_dirs) {
    ushort widthcolumn = 0;
    for (t_list *i = entries_list; i != NULL; i = i->next) {
        t_entry *entry = (t_entry *)i->data;
        size_t entry_name_len = mx_strlen(entry->dirent ? entry->dirent->d_name : entry->relative_path);
        if (slash_after_dirs) {
            entry_name_len++;
        }
        if (entry_name_len > widthcolumn) {
            widthcolumn = entry_name_len;
        }
    }
    return widthcolumn;
}

void mx_print_entries_in_columns(t_list *entries_list, c_char colunm_delim, ushort terminal_width, t_entry_printing_flags flags) {
    if (entries_list == NULL) {
        return;
    }

    int number = mx_list_size(entries_list);
    ushort widthcolumn = mx_max_elem_size(entries_list, flags & slash_dir_print);
    size_t tab_size = mx_round_down(widthcolumn / 8.0f + 1) * 8 - widthcolumn;
    widthcolumn += colunm_delim == '\t' ? tab_size : 1;

    ushort numrows = mx_round_up(number / mx_round_down((float)terminal_width / widthcolumn));
    float average_number_row = (float)number / numrows;
    bool average_number = average_number_row == mx_round_up(average_number_row);
    ushort numcolumns = average_number ? number / numrows : terminal_width / widthcolumn;

    for (ushort y = 0; y < numrows; y++) {
        for (ushort x = 0; x < numcolumns; x++) {
            ushort converted_index = x * numrows + y;
            if (converted_index < number) {
                t_entry *entry = (t_entry *)mx_get_by_index(entries_list, converted_index)->data;
                size_t printing_string_length = mx_print_entry_name(entry, flags & in_color, flags & slash_dir_print);
                if ((flags & slash_dir_print) && !S_ISDIR(entry->stat.st_mode)) {
                    printing_string_length++;
                }
                if (x + 1 != numcolumns) {
                    if ((flags & slash_dir_print) && !S_ISDIR(entry->stat.st_mode)) {
                        mx_printchar(' ');
                    }
                    if (colunm_delim == '\t') {
                        mx_printnchar(' ', widthcolumn - printing_string_length - tab_size);
                    } else {
                        mx_printnchar(' ', widthcolumn - printing_string_length - 1);
                    }
                    mx_printchar(colunm_delim);
                }
            }
        }
        mx_printchar('\n');
    }
}
