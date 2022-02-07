#include "../inc/uls.h"

int mx_handle_dir(const char* dir_name, t_flags* flags, bool is_single) {

    static int error_code = 0;
    DIR* dir;
    if ((dir = opendir(dir_name)) == NULL) {
        if (errno == EACCES) {
            mx_print_perm_dir_error(strerror(errno), dir_name, is_single);
            return (error_code = 1);
        }

        mx_print_dir_error(strerror(errno), dir_name);
        return (error_code = 1);
    }
    t_dirent* dp;
    t_file* inner_files = NULL;

    while ((dp = readdir(dir)) != NULL) {

        if (!mx_a_flags_applied(flags, dp->d_name) || mx_is_uls_file(dp->d_name, dir_name))
            continue;  

        mx_push_back(&inner_files, dir_name, dp->d_name, flags);

    }
    closedir(dir);
    mx_handle_file_array(&inner_files, dir_name, true, is_single, flags);

    t_file* curr_dir_file = inner_files;
    while (curr_dir_file && flags->R) {

        if (IS_DIR(curr_dir_file->stat.st_mode) &&
            !mx_is_implied_dir(curr_dir_file->name)) {
            
            mx_printstr("\n");
            mx_handle_dir(curr_dir_file->path, flags, false);
        }
        curr_dir_file = curr_dir_file->next;

    }
    mx_clear_list(&inner_files);
    return error_code;

}
