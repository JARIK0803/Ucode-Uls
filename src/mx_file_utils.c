#include "../inc/uls.h"

char* mx_get_linked_file(t_file* file) {

    int res_size = file->stat.st_size + 1;
    char* result = mx_strnew(res_size - 1);

    int bytes = readlink(file->path, result, res_size);

    if (bytes == -1) {
        mx_strdel(&result);
        return NULL;
    }
    result[bytes] = '\0';

    return result;
    
}

static char get_additional_perms(t_file* file, t_flags* flags) {

    ssize_t buf_len = listxattr(file->path, NULL, 0, XATTR_NOFOLLOW);
    acl_t acl_info = acl_get_file(file->path, ACL_TYPE_EXTENDED);
    flags->e ? file->acl_str = acl_to_text(acl_info, NULL) : NULL;

    if (buf_len > 0) {

        return '@';

    } else if (acl_info != NULL) {

        acl_free(acl_info);
        return '+';

    }
    return ' ';

}

static char get_owner_perms(char old_perm, char new_perm) {

    if (old_perm == '-') {
        return mx_toupper(new_perm);
    } else {
        return new_perm;
    }

}

char* mx_get_file_permissions(t_file* file, t_flags* flags) {

    mode_t mode = file->stat.st_mode;
    const int PERMS_LEN = 10;
    char* file_perms = mx_strnew(PERMS_LEN);
    int i = 0;

    file_perms[i++] = (mode & S_IRUSR) ? 'r' : '-';
    file_perms[i++] = (mode & S_IWUSR) ? 'w' : '-';
    file_perms[i++] = (mode & S_IXUSR) ? 'x' : '-';
    file_perms[i++] = (mode & S_IRGRP) ? 'r' : '-';
    file_perms[i++] = (mode & S_IWGRP) ? 'w' : '-';
    file_perms[i++] = (mode & S_IXGRP) ? 'x' : '-';
    file_perms[i++] = (mode & S_IROTH) ? 'r' : '-';
    file_perms[i++] = (mode & S_IWOTH) ? 'w' : '-';
    file_perms[i++] = (mode & S_IXOTH) ? 'x' : '-';
    file_perms[i++] = get_additional_perms(file, flags);

    if (S_ISUID & mode) 
        file_perms[2] = get_owner_perms(file_perms[2], 's');
    
    if (S_ISGID & mode) 
        file_perms[5] = get_owner_perms(file_perms[5], 's');
    
    if (S_ISVTX & mode) 
        file_perms[8] = get_owner_perms(file_perms[8], 't');

    return file_perms;

}

char mx_get_file_type(mode_t mode) {

    switch (mode & S_IFMT) {

    case S_IFBLK:
        return 'b';
        
    case S_IFCHR:
        return 'c';

    case S_IFDIR:
        return 'd';

    case S_IFIFO:
        return 'p';

    case S_IFLNK:
        return 'l';

    case S_IFREG:
        return '-';

    case S_IFSOCK:
        return 's';

    default:
        return '?';
    
    }

}

char* mx_get_file_path(const char* dir_name, const char* file_name) {

    char* file_path = mx_strnew(mx_strlen(dir_name) + mx_strlen(file_name) + 1);
    mx_strcpy(file_path, dir_name);
    mx_strcat(file_path, "/");
    mx_strcat(file_path, file_name);
    return file_path;

}

static char* format_size_str(float d, int precision, char* letter) {

    char *file_size_str;
    float tmp = d;

    if ((int)tmp / 10 > 0) {
        tmp = mx_round(tmp, 0);
        file_size_str = mx_itoa(tmp);

    } else {
        tmp = mx_round(tmp, precision);
        file_size_str = mx_dtoa(tmp, precision);
    }

    mx_strcat(file_size_str, letter);

    return file_size_str;
}

char* mx_get_file_size(long int f_size) {
    int Kbyte = 1024;
    int Mbyte = 1024 * 1024;
    int Gbyte = 1024 * 1024 * 1024;
    long Tbyte = 1024 * 1024 * (long)1024 * 1024;
    long Pbyte = 1024 * 1024 * (long)1024 * 1024 * 1024;

    long int file_size = f_size;
    char *file_size_str;
    float tmp;

    if (file_size >= Pbyte) {

        tmp = (float)file_size / Pbyte;
        file_size_str = format_size_str(tmp, 1, "P");

    } else if (file_size >= Tbyte) {

        tmp = (float)file_size / Tbyte;
        file_size_str = format_size_str(tmp, 1, "T");

    } else if (file_size >= Gbyte) {
        tmp = (float)file_size / Gbyte;
        file_size_str = format_size_str(tmp, 1, "G");

    } else if (file_size >= Mbyte) {
        tmp = (float)file_size / Mbyte;

        file_size_str = format_size_str(tmp, 1, "M");

    } else if (file_size >= 1000) {
        tmp = (float)file_size / Kbyte;
        file_size_str = format_size_str(tmp, 1, "K");

    } else {

        file_size_str = mx_itoa(file_size);
        mx_strcat(file_size_str, "B");

    }
    
    return mx_strdup(file_size_str);
}

void mx_get_file_lm_date(t_file **file, t_flags* flags) {

    time_t cur_time;
    time(&cur_time);

    time_t time = (*file)->stat.st_mtime;
    if (flags->u)
        time = (*file)->stat.st_atime;

    if (flags->c)
        time = (*file)->stat.st_ctime;

    char *time_str = ctime(&time);
    char **lm_date = mx_strsplit(time_str, ' ');

    (*file)->lm_month = mx_strdup(lm_date[1]);
    (*file)->lm_day = mx_strdup(lm_date[2]);

    if (flags->T)
    {

        (*file)->lm_time = mx_strndup(&time_str[11], 13);

    } else {

        int test_time = cur_time - time;
        if (test_time > SIX_MON_LEN || (test_time < 0 && test_time > (-1 * SIX_MON_LEN))) {

            (*file)->lm_time = mx_strndup(lm_date[4], 4);

        } else {

            (*file)->lm_time = mx_strndup(lm_date[3], 5);
            
        }
    }

    mx_del_strarr(&lm_date);
}

int mx_get_total_blocks(t_file** files) {

    int total = 0;
    t_file* file = *files;
    while (file) {

        total += file->stat.st_blocks;
        file = file->next;

    }
    return total;

}
