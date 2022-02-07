#include "../inc/uls.h"

static void set_front_spaces(char** str, int max_length) {

    int difference = max_length - mx_strlen(*str);

    char spaces[difference + 1];
    mx_memset(spaces, ' ', difference);
    spaces[difference] = '\0';

    mx_strcat(spaces, *str);

    mx_strdel(str);
    *str = mx_strdup(spaces);
}

static void set_back_spaces(char** str, int max_length) {
    int difference = max_length - mx_strlen(*str);

    char spaces[difference + 1];
    mx_memset(spaces, ' ', difference);
    spaces[difference] = '\0';

    char tmp[mx_strlen(*str) + difference + 1];

    mx_strcpy(tmp, *str);
    mx_strcat(tmp, spaces);

    mx_strdel(str);
    *str = mx_strdup(tmp);
}

static void reset_file_num_links_length(t_file** files)
{
    int max_length = 0;
    int len;

    t_file* file = *files;
    while (file)
    {
        len = mx_strlen(file->num_links);
        if (len > max_length)
        {
            max_length = len;
        }
        file = file->next;
    }
    file = *files;

    while (file)
    {
        if (mx_strlen(file->num_links) < max_length)
        {
            set_front_spaces(&file->num_links, max_length);
        }
        file = file->next;
    }
}

static void reset_file_size_length(t_file** files, t_flags* flags)
{
    int max_length = flags->h ? 4 : 0;
    int len;

    t_file* file = *files;
    while (file)
    {
        len = mx_strlen(file->size);
        if (len > max_length)
        {
            max_length = len;
        }
        file = file->next;
    }
    file = *files;

    while (file)
    {
        if (mx_strlen(file->size) < max_length)
        {
            set_front_spaces(&file->size, max_length);
        }
        file = file->next;
    }
}

static void reset_file_group_name_length(t_file** files)
{
    int max_length = 0;
    int len;

    t_file* file = *files;
    while (file)
    {
        len = mx_strlen(file->group_name);
        if (len > max_length)
        {
            max_length = len;
        }
        file = file->next;
    }
    file = *files;

    while (file)
    {
        if (mx_strlen(file->group_name) < max_length)
        {
            set_back_spaces(&file->group_name, max_length);
        }
        file = file->next;
    }
}

static void reset_file_user_name_length(t_file** files)
{
    int max_length = 0;
    int len;

    t_file* file = *files;
    while (file)
    {
        len = mx_strlen(file->user_name);
        if (len > max_length)
        {
            max_length = len;
        }
        file = file->next;
    }
    file = *files;

    while (file)
    {
        if (mx_strlen(file->user_name) < max_length)
        {
            set_back_spaces(&file->user_name, max_length);
        }
        file = file->next;
    }
}

static void reset_file_lm_day_length(t_file** files)
{
    int max_length = 2;
    int len;

    t_file* file = *files;
    while (file)
    {
        len = mx_strlen(file->lm_day);
        if (len > max_length)
        {
            max_length = len;
        }
        file = file->next;
    }
    file = *files;

    while (file)
    {
        if (mx_strlen(file->lm_day) < max_length)
        {
            set_front_spaces(&file->lm_day, max_length);
        }
        file = file->next;
    }
}

static void reset_file_lm_time_length(t_file** files)
{
    int max_length = 5;
    int len;

    t_file* file = *files;
    while (file)
    {
        len = mx_strlen(file->lm_time);
        if (len > max_length)
        {
            max_length = len;
        }
        file = file->next;
    }
    file = *files;

    while (file)
    {
        if (mx_strlen(file->lm_time) < max_length)
        {
            set_front_spaces(&file->lm_time, max_length);
        }
        file = file->next;
    }
}

void mx_reset_file_params_length(t_file** files, t_flags* flags) {

    reset_file_num_links_length(files);
    reset_file_user_name_length(files);
    reset_file_group_name_length(files);
    reset_file_size_length(files, flags);
    reset_file_lm_day_length(files);
    reset_file_lm_time_length(files);

}
