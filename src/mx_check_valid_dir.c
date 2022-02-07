#include "../inc/uls.h"

bool mx_is_implied_dir(const char* dir_name) {

    return (!mx_strcmp(dir_name, ".") || !mx_strcmp(dir_name, ".."));
    
}

bool mx_is_hidden_file(const char* name) {

    return (name[0] == '.');

}

bool mx_a_flags_applied(t_flags* flags, char* file_name) {

    if (!flags->a && !flags->A && (mx_is_hidden_file(file_name) || mx_is_implied_dir(file_name)))
        return false;

    if (flags->A && (mx_is_implied_dir(file_name)))
        return false;
    return true;

}

bool mx_is_uls_file(const char* file, const char* dir) {

    bool to_return = false;
    t_stat uls_stat;
    lstat(EXE_STR, &uls_stat);

    char* path = mx_get_file_path(dir, file);
    t_stat stat;
    lstat(path, &stat);

    if (stat.st_ino == uls_stat.st_ino)
        to_return = true;

    mx_strdel(&path);

    return to_return;

}
