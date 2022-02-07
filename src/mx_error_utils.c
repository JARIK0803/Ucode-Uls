#include "../inc/uls.h"

static void print_err_char(char flag) {
    
    write(2, &flag, 1);

}

void mx_print_flag_err(char flag) {

    mx_printerr(ULS_STR);
    mx_printerr(ILOP_STR);
    print_err_char(flag);
    mx_printerr("\n");
    mx_printerr(USAGE_STR);

}

void mx_print_dir_error(const char* error, const char* dir_name) {

    mx_printerr(ULS_STR);
    mx_printerr(dir_name);
    mx_printerr(": ");
    mx_printerr(error);
    mx_printerr("\n");

}

void mx_print_perm_dir_error(const char* error, const char* dir_path, bool is_single) {

    char* path = mx_strdup(dir_path);
    char* dir_name = mx_strrchr(path, '/');

    if (!is_single) {
        mx_printstr(dir_path);
        mx_printstr(":\n");
    }
    mx_printerr(ULS_STR);
    mx_printerr((dir_name != NULL) ? ++dir_name : dir_path);
    mx_printerr(": ");
    mx_printerr(error);
    mx_printerr("\n");
    mx_strdel(&path);

}
