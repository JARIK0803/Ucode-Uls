#include "../inc/uls.h"

void mx_output_for_F(mode_t mode) {

    mode_t modified_mode = mode & S_IFMT;

    if (modified_mode == S_IFDIR)
    {
        mx_printchar('/');
    }
    else if (modified_mode == S_IFLNK)
    {
        mx_printchar('@');
    }
    else if (modified_mode == S_IFSOCK)
    {
        mx_printchar('=');
    }
    else if (modified_mode == S_IFIFO)
    {
        mx_printchar('|');
    }
    else if (IS_X(mode) && IS_R(mode))
    {
        mx_printchar('*');
    }
    else if (modified_mode == S_IFWHT)
    {
        mx_printchar('%');
    }
    
}

void mx_output_file_xattr(const char* path, bool is_h_on) {
    
    char attr_name[XATTR_LEN];
    char value[XATTR_LEN];
    int list_len = listxattr(path, attr_name, XATTR_LEN, XATTR_NOFOLLOW);
    int value_len = 0;
    for (int idx = 0; idx < list_len;) {
        
        mx_printstr("\t");
        mx_printstr(&attr_name[idx]);
        value_len = getxattr(path, &attr_name[idx], value, XATTR_LEN, 0, 0);

        mx_printstr("\t   ");
        if (value_len != -1)
            mx_printint(value_len);
        else 
            mx_printint(0);

        is_h_on ? mx_printchar('B') : (void) 0;
        mx_printstr(" \n");
        idx += mx_strlen(&attr_name[idx]) + 1;
    }

}

void mx_output_acl_params(char** acl_params, int idx) {

    mx_printchar(' ');
    mx_printint(idx);
    mx_printstr(": ");
    mx_printstr(acl_params[0]);
    mx_printchar(':');
    mx_printstr(acl_params[2]);
    mx_printchar(' ');
    mx_printstr(acl_params[4]);
    mx_printchar(' ');
    mx_printstr(acl_params[5]);
    mx_printchar('\n');

}

void mx_output_acl_info(const char* acl_str) {

    char** acl_lines = mx_strsplit(acl_str, '\n');
    for (int i = 1; acl_lines[i] != NULL; ++i) {

        char** acl_params = mx_strsplit(acl_lines[i], ':');
        mx_output_acl_params(acl_params, i - 1);
        mx_del_strarr(&acl_params);

    }
    mx_del_strarr(&acl_lines);

}
