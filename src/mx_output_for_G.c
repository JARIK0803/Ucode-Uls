#include "../inc/uls.h"

void mx_print_color_file_name(char* file_name, mode_t mode)
{
    mode_t modified_mode = mode & S_IFMT;

    if (modified_mode == S_IFDIR)
    {
        if (S_ISVTX & mode && mode & S_IWOTH)
        {
            mx_printstr(COLOR_BLACK_GREEN);
        }
        else if (!(S_ISVTX & mode) && mode & S_IWOTH)
        {   
            mx_printstr(COLOR_BLACK_YELLOW);
        }
        else 
        {
            mx_printstr(COLOR_BLUE_DFLT);
        }
    }
    else if (modified_mode == S_IFLNK)
    {
        mx_printstr(COLOR_MAGENTA_DFLT);
    }
    else if (modified_mode == S_IFSOCK)
    {
        mx_printstr(COLOR_GREEN_DFLT);
    }
    else if (modified_mode == S_IFIFO)
    {
        mx_printstr(COLOR_YELLOW_DFLT);
    }
    else if (IS_X(mode) && IS_R(mode))
    {
        if (S_ISUID & mode)
        {
            mx_printstr(COLOR_BLACK_RED);
        }
        else if (S_ISGID & mode)
        {
            mx_printstr(COLOR_BLACK_CYAN);
        }
        else 
        {
            mx_printstr(COLOR_RED_DFLT); 
        }
    }
    else if (modified_mode == S_IFBLK)
    {
        mx_printstr(COLOR_BLUE_CYAN);
    }
    else if (modified_mode == S_IFCHR)
    {
        mx_printstr(COLOR_BLUE_YELLOW);
    }

    mx_printstr(file_name);
    mx_printstr(COLOR_DEFAULT);
}
