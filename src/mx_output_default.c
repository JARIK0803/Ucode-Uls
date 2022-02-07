#include "../inc/uls.h"

static t_file* get_file_at_index(t_file* file, int index) {

    int i = 0;
    while (file) {

        if (i == index)
            return file;
        
        file = file->next;
        ++i;

    }
    return NULL;

}

static int get_max_name_length(t_file** files)
{
    int max_name_len = 0;
    int len;

    t_file* file = *files;
    while (file)
    {
        len = mx_strlen(file->name);
        if (len > max_name_len)
        {
            max_name_len = len;
        }
        file = file->next;
    }

    return max_name_len;
}

static int calc_col_width(t_file** files)
{
    int col_width = get_max_name_length(files);

    if (col_width % TAB_SIZE == 0) {
        col_width += TAB_SIZE;
    }
    else {
        col_width += TAB_SIZE - (col_width % TAB_SIZE);
    }
    
    return col_width;
}

static int calc_num_tabs(int col_width, int name_len)
{
    int difference = col_width - name_len;

    if (difference % TAB_SIZE != 0) {
        return difference / TAB_SIZE + 1;
    } else { 
        return difference / TAB_SIZE;
    }
}

static void print_tabs(int col_width, int name_len)
{
    int num_tabs = calc_num_tabs(col_width, name_len);
    
    for (int i = 0; i < num_tabs; i++)
    {
        mx_printchar('\t');
    }
}

static int calc_cols(int win_cols, int col_width)
{
    int cols = (win_cols / col_width) == 0 ? 1 : win_cols / col_width;

    if (col_width * cols > win_cols && cols != 1) {
        cols--;
    }

    return cols;
}

static int calc_rows(int cols, int num_names)
{
    int rows = num_names / cols;

    if (rows == 0 || num_names % cols != 0) {
        rows++;
    }

    return rows;
}

static void print_rows(t_file** files, int col_width, int rows, int num_names, t_flags* flags)
{
    for (int i = 0; i < rows; i++) {

        for (int j = 0; i + j < num_names; j += rows) {

            t_file* curr_file = get_file_at_index(*files, i + j);
            if (flags->G && isatty(1))
                mx_print_color_file_name(curr_file->name, curr_file->stat.st_mode);
            else
                mx_printstr(curr_file->name);

            flags->F ? mx_output_for_F(curr_file->stat.st_mode) : (void) 0;

            if (curr_file->next && (i + j + rows < num_names)) {

                print_tabs(col_width, mx_strlen(curr_file->name));

            }

        }
        mx_printchar('\n');
    }
}

static void print_single_row(t_file** files, int col_width, t_flags* flags)
{
    t_file* file = *files;
    while (file)
    {
        if (flags->G && isatty(1))
            mx_print_color_file_name(file->name, file->stat.st_mode);
        else
            mx_printstr(file->name);

        flags->F ? mx_output_for_F(file->stat.st_mode) : (void) 0;
        

        if (file->next)
        {
            print_tabs(col_width, mx_strlen(file->name));
        }
        file = file->next;
    }
    mx_printchar('\n');
}

static void print_file_names(t_file** files, int col_width, int win_cols, t_flags* flags)
{
    int cols = calc_cols(win_cols, col_width);
    int rows;
    int num_names = mx_list_size(*files);

    if (num_names * col_width > win_cols) {

        rows = calc_rows(cols, num_names);
        print_rows(files, col_width, rows, num_names, flags);

    } else if (num_names != 0) {

        print_single_row(files, col_width, flags);

    }
}

void mx_output_default(t_file** files, t_flags* flags)
{
    struct winsize win_size;

    int col_width = calc_col_width(files);
    ioctl(1, TIOCGWINSZ, &win_size);

    if ((!isatty(1) || flags->one) && !flags->C) {
        int max_len = get_max_name_length(files);
        print_file_names(files, col_width, max_len, flags);
    }
    
    else if (isatty(1) && !flags->one && !flags->C)
        print_file_names(files, col_width, win_size.ws_col, flags);
    
    else 
        print_file_names(files, col_width, CAT_WIDTH, flags);
        
}
