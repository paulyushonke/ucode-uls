#include "../inc/uls.h"

static char take_fts(mode_t file_mode) {
    if (S_ISBLK(file_mode)) {
        return 'b';
    } else if (S_ISCHR(file_mode)) {
        return 'c';
    } else if (S_ISDIR(file_mode)) {
        return 'd';

    } else if (S_ISLNK(file_mode)) {
        return 'l';

#ifdef S_ISSOCK
    } else if (S_ISSOCK(file_mode)) {
        return 's';
#endif 
#ifdef S_ISFIFO
    } else if (S_ISFIFO(file_mode)) {
        return 'p';
#endif 
    } else if (S_ISREG(file_mode)) {
        return '-';
    }
    return '?';
}

static char *get_permissions_str(mode_t file_mode, char *path_to_file) {
    static const char *rwx[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};

    char *permissions_str = mx_strnew(11);

    permissions_str[0] = take_fts(file_mode);
    mx_strcpy(&permissions_str[1], rwx[(file_mode >> 6) & 7]);
    mx_strcpy(&permissions_str[4], rwx[(file_mode >> 3) & 7]);
    mx_strcpy(&permissions_str[7], rwx[file_mode & 7]);
    if (file_mode & S_ISUID)
        permissions_str[3] = (file_mode & S_IXUSR) ? 's' : 'S';
    if (file_mode & S_ISGID)
        permissions_str[6] = (file_mode & S_IXGRP) ? 's' : 'l';
#ifdef S_ISVTX
    if (file_mode & S_ISVTX)
        permissions_str[9] = (file_mode & S_IXOTH) ? 't' : 'T';
#endif 

    char buf[10000];
    bool has_xattr = listxattr(path_to_file, buf, 10000, XATTR_NOFOLLOW) > 0;

    bool has_acl = acl_get_file(path_to_file, ACL_TYPE_EXTENDED);

    if (has_xattr) {
        permissions_str[10] = '@';

    } else if (has_acl) {
        permissions_str[10] = '+';

    } else {
        permissions_str[10] = ' ';
    }

    return permissions_str;
}

static void print_entr_perm(t_entry *entry) {
    char *permissions_str = get_permissions_str(entry->stat.st_mode, entry->relative_path);
    mx_printstr(permissions_str);
    free(permissions_str);
    mx_printchar(' ');
}

static void print_num_entry_links(t_entry *entry, size_t column_size) {
    int indent = column_size - mx_get_digits_count(entry->stat.st_nlink);
    mx_printnchar(' ', indent);
    mx_printint(entry->stat.st_nlink);
    mx_printchar(' ');
}

static void print_owner_name(t_entry *entry, size_t column_size) {
    mx_printstr(getpwuid(entry->stat.st_uid)->pw_name);
    mx_printnchar(' ', column_size - mx_strlen(getpwuid(entry->stat.st_uid)->pw_name));
    mx_printnchar(' ', 1);
}

static void print_group_name(t_entry *entry, size_t column_size) {
    mx_printstr(getgrgid(entry->stat.st_gid)->gr_name);
    mx_printnchar(' ', column_size - mx_strlen(getgrgid(entry->stat.st_gid)->gr_name));
    mx_printnchar(' ', 2);
}

static void print_num_of_bytes(t_entry *entry, size_t column_size) {
    mx_printnchar(' ', column_size - mx_get_digits_count(entry->stat.st_size));
    mx_printint(entry->stat.st_size);
    mx_printchar(' ');
}

static void print_readbl(off_t entry_size) {
    uchar rmvd_entr_size = 0;

    mx_printchar(' ');

    if (entry_size < 1000) {
        mx_printnchar(' ', entry_size < 10 ? 2 : entry_size < 100 ? 1 : 0);
        mx_printint(entry_size);
    } else {
        float conv_entr_size = entry_size;
        for (; conv_entr_size > 1000; rmvd_entr_size++) {
            conv_entr_size /= 1024;
        }

        if (conv_entr_size >= 10) {
            int rounded_entry_size = mx_round(conv_entr_size);
            mx_printchar_if_con(rounded_entry_size < 100, ' ');
            mx_printint(rounded_entry_size);
        } else {
            int number = mx_round(conv_entr_size * 10);
            bool print_float = number % 10 != 0 || number / 10 < 10;
            mx_printchar_if_con(!print_float, ' ');
            mx_printint(number / 10);
            mx_print_char_and_int_if(print_float, '.', number % 10);
        }
    }

    static const char units[] = {'B', 'K', 'M', 'G', 'T', 'P'};
    mx_print_two_chars(units[rmvd_entr_size], ' ');
}

time_t get_time(t_entry *entry, t_time_type time_type) {
    switch (time_type) {
        case data_modify_time:
            return entry->stat.st_mtime;
        case access_time:
            return entry->stat.st_atime;
        case file_stchange_time:
            return entry->stat.st_ctime;
        case create_file_time:
            return entry->stat.st_birthtime;
    }
}

static void print_mnth(c_str hmn_rdbl_tm) {
    mx_printnstr(hmn_rdbl_tm + 4, 3);
    mx_printchar(' ');
}

static void print_mnth_day(c_str hmn_rdbl_tm) {
    mx_printnstr(hmn_rdbl_tm + 8, 2);
    mx_printchar(' ');
}

static void print_hours_minutes(c_str hmn_rdbl_tm) {
    mx_printnstr(hmn_rdbl_tm + 11, 5);
    mx_printchar(' ');
}

static void print_year(c_str hmn_rdbl_tm) {
    mx_printnstr(hmn_rdbl_tm + 19, 5);
    mx_printchar(' ');
}

static void print_full_time(c_str hmn_rdbl_tm) {
    mx_printnstr(hmn_rdbl_tm + 4, 20);
    mx_printchar(' ');
}

static void print_link_content(t_entry entry) {
    char link_content[1024];
    ssize_t link_content_len = readlink(entry.relative_path, link_content, 1024);
    mx_printnstr(link_content, link_content_len);
}


static void print_ext_attrbs(t_entry *entry) {
    size_t xattr_size = 10000;
    char attributes[xattr_size];
    ssize_t attributes_len = listxattr(entry->relative_path, attributes, xattr_size, XATTR_NOFOLLOW);
    for (ssize_t i = 0; i < attributes_len;) {
        mx_printstr("\n\t");
        size_t printed_len = mx_printstr(attributes + i);
        char value[xattr_size];
        ssize_t value_len = getxattr(entry->relative_path, &attributes[i], value, xattr_size, 0, XATTR_NOFOLLOW);
        mx_printchar('\t');
        mx_printint(value_len);
        mx_printchar(' ');
        i += printed_len + 1;
    }
}

static void print_access_cl(t_entry *entry) {
    acl_t acl = acl_get_file(entry->relative_path, ACL_TYPE_EXTENDED);
    if (acl == 0) return;
    c_str desc = acl_to_text(acl, NULL);
    char *pointer = NULL;
    mx_printstr("\n 0: ");
    pointer = mx_strchr(desc, '\n') + 1;
    for (; *pointer != ':'; pointer++) {
        mx_printchar(*pointer);
    }
    pointer = mx_strchr(pointer + 1, ':') + 1;
    mx_printchar(':');
    for (; *pointer != ':'; pointer++) {
        mx_printchar(*pointer);
    }
    pointer = mx_strchr(pointer + 1, ':') + 1;
    mx_printchar(' ');
    for (; *pointer != ':'; pointer++) {
        mx_printchar(*pointer);
    }
    pointer++;
    mx_printchar(' ');
    for (; *pointer != '\n'; pointer++) {
        mx_printchar(*pointer);
    }

    acl_free((void *)desc);
    acl_free(acl);
}

static time_t diff_btw_tms(time_t time1, time_t time2) {
    if (time1 > time2) {
        return time1 - time2;
    } else {
        return time2 - time1;
    }
}

void mx_print_lon_frmtd_entr(t_entry entry, size_t *column_sizes, c_time_type time_type, c_long_format_flags long_format_flags, c_entry_printing_flags entry_printing_flags) {
    print_entr_perm(&entry);
    print_num_entry_links(&entry, column_sizes[0]);

    if (!(long_format_flags & own_name)) {
        print_owner_name(&entry, column_sizes[1]);
        if (!(long_format_flags & group_name)) {
            mx_printchar(' ');
        }
    } else if (long_format_flags & group_name) {
        mx_printchar(' ');
    }
    if (!(long_format_flags & group_name)) {
        print_group_name(&entry, column_sizes[2]);
    } else {
        mx_printchar(' ');
    }
    if (long_format_flags &  hmn_rdbl_size) {
        print_readbl(entry.stat.st_size);
    } else {
        print_num_of_bytes(&entry, column_sizes[3]);
    }
    time_t file_time = get_time(&entry, time_type);
    c_str hmn_rdbl_tm = ctime(&file_time);
    if (long_format_flags & info) {
        print_full_time(hmn_rdbl_tm);
    } else {
        print_mnth(hmn_rdbl_tm);
        print_mnth_day(hmn_rdbl_tm);
        if (diff_btw_tms(file_time, time(NULL)) > 15768000) {
            print_year(hmn_rdbl_tm);
        } else {
            print_hours_minutes(hmn_rdbl_tm);
        }
    }
    mx_print_entry_name(&entry, entry_printing_flags & in_color, entry_printing_flags & slash_dir_print);
    if (S_ISLNK(entry.stat.st_mode)) {
        mx_printstr(" -> ");
        print_link_content(entry);
    }
    if (long_format_flags & print_ext_atr) {
        print_ext_attrbs(&entry);
    }
    if (long_format_flags & print_access_list) {
        print_access_cl(&entry);
    }
    mx_printchar('\n');
}
