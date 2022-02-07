#ifndef ULS_H
#define ULS_H

#include "../libmx/inc/libmx.h"
#include "const.h"
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/xattr.h>
#include <sys/ioctl.h>
#include <sys/acl.h>
#include <string.h>
#include <errno.h>

typedef struct dirent t_dirent;
typedef struct stat t_stat;
typedef struct passwd t_passwd;
typedef struct group t_group;

typedef struct s_file {
    char* num_links;
    char* user_name;
    char* group_name;
    char* size;
    char* lm_day;
    char* lm_time;
    char* lm_month;
    char* perms;
    char* linked_file;
    char* acl_str;

    char* name;
    char* path;
    char type;
    t_stat stat;

    struct s_file* next;
}              t_file;

typedef struct s_flags {
    int a;
    int A;
    int G;
    int l;
    int r;
    int t;
    int c;
    int u;
    int S;
    int T;
    int h;
    int at;
    int e;
    int R;
    int C;
    int one;
    int F;
    int f;
    int m;
}              t_flags;

#define IS_R(m) (m & S_IRUSR && m & S_IRGRP && m & S_IROTH)
#define IS_X(m) (m & S_IXUSR && m & S_IXGRP && m & S_IXOTH)
#define IS_DIR(m) (((m) & S_IFMT) == S_IFDIR)
#define IS_LNK(m) (((m) & S_IFMT) == S_IFLNK)
#define IS_REG(m) (((m) & S_IFMT) == S_IFREG)


// FILE UTILITIES

int mx_uls_init(char** files, int file_count, t_flags* flags);
bool mx_is_implied_dir(const char* dir_name);
bool mx_is_uls_file(const char* file, const char* dir);

int mx_get_total_blocks(t_file** files);
char* mx_get_file_permissions(t_file* file, t_flags* flags);
char mx_get_file_type(mode_t mode);
char* mx_get_file_path(const char* dir_name, const char* file_name);
char* mx_get_file_size(long int f_size);
void mx_get_file_lm_date(t_file **file, t_flags* flags);
char* mx_get_linked_file(t_file* file);

// FILES & FILE ARRAY

t_file* mx_create_file_obj(const char* path, const char* name, t_flags* flags);
t_file* mx_create_default_file_obj(const char* dir_name, const char* name);

void mx_handle_file_array(t_file** files, const char* dir_name, bool is_dir, bool is_single, t_flags* flags);
int mx_handle_dir(const char* dir_name, t_flags* flags, bool is_single);

// PRINT FILE INFO

void mx_output_files_l(t_file** files, bool is_dir, t_flags* flags);
void mx_output_file_l(t_file* file, t_flags* flags);
void mx_output_for_F(mode_t mode);
void mx_output_default(t_file** files, t_flags* flags);
void mx_output_for_m(t_file** files, t_flags* flags);
void mx_print_dir_name(const char* dir_name);
void mx_print_color_file_name(char* file_name, mode_t mode);
void mx_output_acl_info(const char* acl_str);
void mx_output_for_F(mode_t mode);
void mx_output_file_xattr(const char* path, bool is_h_on);

// FLAG UTIL

t_flags* mx_get_flags(int argc,const char** argv, int* flag_count);
bool mx_a_flags_applied(t_flags* flags, char* file_name);

// ERROR HANDLING

void mx_print_flag_err(char flag);
void mx_print_dir_error(const char* error, const char* dir_name);
void mx_print_perm_dir_error(const char* error, const char* dir_path, bool is_single);

// MISC 

void mx_reset_file_params_length(t_file** files, t_flags* flags);

// LIST

void mx_clear_list(t_file **list);
void mx_push_back(t_file **list, const char* dir_name, const char* file_name, t_flags* flags);
int mx_list_size(t_file *list);
t_file* mx_swap_nodes(t_file* node1, t_file* node2);

// SORTING

void mx_sort_init(t_file** files, t_flags* flags);
void mx_sort_list(t_file **lst, bool (*cmp)(t_file* a, t_file* b));
void mx_reverse_list(t_file** list);
bool compare_by_name(t_file* first, t_file* second);

//UTILS
float mx_round(float num, int precision);
char *mx_dtoa(float d, int precision);

#endif
