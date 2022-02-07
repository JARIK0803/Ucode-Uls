#include "../inc/uls.h"

static void output_name_for_m(t_file* file, t_flags* flags, bool is_last_file)
{
    flags->G && isatty(1) ? mx_print_color_file_name(file->name, file->stat.st_mode) : mx_printstr(file->name);
    flags->F ? mx_output_for_F(file->stat.st_mode) : (void) 0;
    !is_last_file ? mx_printstr(", ") : mx_printstr("\n");
}

void mx_output_for_m(t_file** files, t_flags* flags)
{
    int out_width;

    if (isatty(1))
    {
        struct winsize win_size;
        ioctl(1, TIOCGWINSZ, &win_size);
        out_width = win_size.ws_col;
    } else
        out_width = CAT_WIDTH;
    

    t_file* file = *files;
    int counter = 0;

    while (file) {

        counter += file->next == NULL ? mx_strlen(file->name) : mx_strlen(file->name) + 2;
        if (counter >= out_width)
        {
            counter = mx_strlen(file->name) + 2;
            mx_printstr("\n");
        }
        
        output_name_for_m(file, flags, file->next == NULL);
        file = file->next;

    }
}
