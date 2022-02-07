#include "../inc/uls.h"

bool compare_by_name(t_file* first, t_file* second) {

    return (mx_strcmp(first->name, second->name) > 0);

}

bool compare_by_size(t_file* first, t_file* second) {

    if (first->stat.st_size == second->stat.st_size) {
        return compare_by_name(first, second);
    }
    
    return (first->stat.st_size < second->stat.st_size);

}

bool compare_by_mtime(t_file* first, t_file* second) {

    if (first->stat.st_mtime == second->stat.st_mtime) {
        return compare_by_name(first, second);
    }

    return (first->stat.st_mtime < second->stat.st_mtime);

}

bool compare_by_atime(t_file* first, t_file* second) {

    if (first->stat.st_atime == second->stat.st_atime) {
        return compare_by_name(first, second);
    }

    return (first->stat.st_atime < second->stat.st_atime);    

}

bool compare_by_ctime(t_file* first, t_file* second) {

    if (first->stat.st_ctime == second->stat.st_ctime) {
        return compare_by_name(first, second);
    }

    return (first->stat.st_ctime < second->stat.st_ctime);    

} 

void mx_sort_list(t_file** list, bool (*cmp)(t_file *a, t_file *b)) {

    t_file** file;
    bool is_swapped;
  
    int count = mx_list_size(*list);

    for (int i = 0; i < count; i++) {
  
        file = list;
        is_swapped = false;
  
        for (int j = 0; j < count - i - 1; j++) {
  
            t_file* f1 = *file;
            t_file* f2 = f1->next;
  
            if (cmp(f1, f2)) {

                *file = mx_swap_nodes(f1, f2);
                is_swapped = true;
            
            }
  
            file = &(*file)->next;
        }
        if (!is_swapped)
            break;

    }

}

void mx_reverse_list(t_file** list) {

    t_file* previous = NULL;
    t_file* current = *list;
    t_file* next;
    while (current != NULL) {

        next  = current->next;  
        current->next = previous;   
        previous = current;
        current = next;
    
    }
    *list = previous;

}

void mx_sort_init(t_file** files, t_flags* flags) {

    if (flags->f) return;

    if (flags->S)
        mx_sort_list(files, compare_by_size);

    else if (flags->t && !flags->u && !flags->c)
        mx_sort_list(files, compare_by_mtime);
    
    else if (flags->u && flags->t && !flags->S)
        mx_sort_list(files, compare_by_atime);

    else if (flags->c && flags->t && !flags->S)
        mx_sort_list(files, compare_by_ctime);

    else if (!flags->t && !flags->S)
        mx_sort_list(files, compare_by_name);

    if (flags->r)
        mx_reverse_list(files);

}
