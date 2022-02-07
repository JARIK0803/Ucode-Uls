#include "../inc/uls.h"

static char** get_file_array(int argc, const char** argv, int flag_count) {

    int file_count = argc - flag_count + 1; 
    char** files = malloc(sizeof(char*) * file_count);

    int idx = 0;
    for (int i = flag_count; i < argc; ++i) {

        files[idx++] = mx_strdup(argv[i]);

    }
    files[idx] = NULL;

    return files;

}

static char** get_root_file_array() {

    char** files = malloc(sizeof(char*) * 2);

    int idx = 0;
    files[idx++] = mx_strdup(".");
    files[idx] = NULL;

    return files;

}

int main(int argc, char const *argv[]) {

    char** files = NULL;
    int flag_count = 1;
    t_flags* flags = mx_get_flags(argc, argv, &flag_count);
    int file_count = argc - flag_count;
        
    if (file_count == 0) {
        files = get_root_file_array();
    } else {
        files = get_file_array(argc, argv, flag_count);
    }
    
    int error_code = mx_uls_init(files, file_count, flags);
    
    free(flags);
    mx_del_strarr(&files);
    
    return error_code;

}
