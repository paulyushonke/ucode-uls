#include "../inc/uls.h"

static size_t *create_empty_size_t_array(size_t size) {
    size_t *arr = malloc(size * sizeof(size_t));
    for (size_t i = 0; i < size; i++) {
        arr[i] = 0;
    }
    return arr;
}

static blkcnt_t get_total(t_list *entries_list) {
    blkcnt_t total_bl = 0;
    for (t_list *i = entries_list; i != NULL; i = i->next) {
        total_bl += ((t_entry *)i->data)->stat.st_blocks;
    }

    return total_bl;
}

static size_t *calc_col_size(t_list *entries) {
    size_t *column_sizes = create_empty_size_t_array(4);
    for (t_list *i = entries; i != NULL; i = i->next) {
        t_entry entry = *(t_entry *)i->data;

        size_t number_of_links_len = mx_get_digits_count(entry.stat.st_nlink);
        if (number_of_links_len > column_sizes[0]) {
            column_sizes[0] = number_of_links_len;
        }

        size_t owner_name_len = mx_strlen(getpwuid(entry.stat.st_uid)->pw_name);
        if (owner_name_len > column_sizes[1]) {
            column_sizes[1] = owner_name_len;
        }

        size_t group_name_len = mx_strlen(getgrgid(entry.stat.st_gid)->gr_name);
        if (group_name_len > column_sizes[2]) {
            column_sizes[2] = group_name_len;
        }

        size_t number_of_bytes_len = mx_get_digits_count(entry.stat.st_size);
        if (number_of_bytes_len > column_sizes[3]) {
            column_sizes[3] = number_of_bytes_len;
        }
    }
    return column_sizes;
}

void mx_print_form_entr(t_list *entries_list, c_time_type time_type, bool print_total_number_of_512_byte_blocks, c_long_format_flags long_format_flags, c_entry_printing_flags entry_printing_flags) {
    if (entries_list == NULL) {
        return;
    }

    if (print_total_number_of_512_byte_blocks) {
        mx_printstr("total ");
        mx_printint(get_total((t_list *)entries_list));
        mx_printchar('\n');
    }

    size_t *column_sizes = calc_col_size((t_list *)entries_list);
    for (const t_list *j = entries_list; j != NULL; j = j->next) {
        t_entry entry = *(t_entry *)j->data;
        mx_print_lon_frmtd_entr(entry, column_sizes, time_type, long_format_flags, entry_printing_flags);
    }
    free(column_sizes);
}
