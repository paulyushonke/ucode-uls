#include "../inc/uls.h"

bool mx_sort_name(void *a, void *b) {
    t_entry aa = *(t_entry *)a;
    t_entry bb = *(t_entry *)b;
    if (aa.dirent == NULL  || bb.dirent == NULL) {
        return mx_strcmp(aa.relative_path, bb.relative_path) > 0;
    }
    return mx_strcmp(aa.dirent->d_name, bb.dirent->d_name) > 0;
}

bool mx_sort_name_rev(void *a, void *b) {
    t_entry aa = *(t_entry *)a;
    t_entry bb = *(t_entry *)b;
    if (aa.dirent == NULL  || bb.dirent == NULL) {
        return mx_strcmp(aa.relative_path, bb.relative_path) > 0;
    }
    return mx_strcmp(aa.dirent->d_name, bb.dirent->d_name) < 0;
}

bool mx_sort_size(void *a, void *b) {
    time_t sizeofa = (*(t_entry *)a).stat.st_size;
    time_t sizeofb = (*(t_entry *)b).stat.st_size;
    return  sizeofa == sizeofb ?
            mx_sort_name(a, b) :
            sizeofa < sizeofb;
}

bool mx_sort_modify(void *a, void *b) {
    time_t lasta = (*(t_entry *)a).stat.st_mtime;
    time_t lastb = (*(t_entry *)b).stat.st_mtime;
    return  lasta == lastb ?
            mx_sort_name_rev(a, b) :
            lasta < lastb;
}

bool mx_sort_access(void *a, void *b) {
    time_t lasta = (*(t_entry *)a).stat.st_atime;
    time_t lastb = (*(t_entry *)b).stat.st_atime;
    return  lasta == lastb ?
            mx_sort_name(a, b) :
            lasta < lastb;
}

bool mx_sort_time(void *a, void *b) {
    time_t a_creation_time = (*(t_entry *)a).stat.st_birthtimespec.tv_sec;
    time_t b_creation_time = (*(t_entry *)b).stat.st_birthtimespec.tv_sec;
    
    return  a_creation_time == b_creation_time ?
            mx_sort_name(a, b) :
            a_creation_time < b_creation_time;
}

