#include "../inc/uls.h"

void mx_print_dir_name(const char* dir_name) {

    mx_printstr(dir_name);
    mx_printstr(":");
    mx_printstr("\n");

}

static void print_linked_file(const char* name) {

    mx_printstr(" -> ");
    mx_printstr(name);

}

static void print_str_single_space(char* str) {

    mx_printstr(str);
    mx_printstr(" ");

}

static void print_str_double_space(char* str) {

    mx_printstr(str);
    mx_printstr("  ");

}

void mx_output_file_l(t_file* file, t_flags* flags) {
    
    mx_printchar(file->type);
    print_str_single_space(file->perms);
    print_str_single_space(file->num_links);
    print_str_double_space(file->user_name);
    print_str_double_space(file->group_name);
    flags->h ? mx_printchar(' ') : (void) 0;
    print_str_single_space(file->size);
    print_str_single_space(file->lm_month);
    print_str_single_space(file->lm_day);
    print_str_single_space(file->lm_time);
    
    flags->G && isatty(1) ? mx_print_color_file_name(file->name, file->stat.st_mode) : mx_printstr(file->name);
    flags->F ? mx_output_for_F(file->stat.st_mode) : (void) 0;
    file->linked_file ? print_linked_file(file->linked_file) : (void) 0;
    
    mx_printstr("\n");
    if (flags->at && flags->l && listxattr(file->path, NULL, 0, XATTR_NOFOLLOW) > 0) {
        mx_output_file_xattr(file->path, flags->h);
    }
    if (flags->e && file->acl_str) {
        mx_output_acl_info(file->acl_str);
    }

}

static void print_total_blocks(t_file** files) {

    mx_printstr("total ");
    mx_printint(mx_get_total_blocks(files));
    mx_printstr("\n");

}

void mx_output_files_l(t_file** files, bool is_dir, t_flags* flags) {

    is_dir && *files ? print_total_blocks(files) : (void) 0;

    t_file* file = *files;
    while (file) {
        
        mx_output_file_l(file, flags);
        file = file->next;

    }

}
