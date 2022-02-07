#include "../inc/uls.h"

t_file* mx_create_file_obj(const char* dir_name, const char* name, t_flags* flags) {

    t_file* file = malloc(sizeof(t_file));
    file->path = mx_get_file_path(dir_name, name);
    file->name = mx_strndup(name, mx_strlen(name));
    
    if (lstat(file->path, &(file->stat)) == 0) {
        
        t_passwd* passwd = getpwuid(file->stat.st_uid);
        t_group* group = getgrgid(file->stat.st_gid);

        file->type = mx_get_file_type(file->stat.st_mode);
        file->perms = mx_get_file_permissions(file, flags);
        file->num_links = mx_itoa(file->stat.st_nlink);
        file->user_name = passwd ? mx_strdup(passwd->pw_name) : mx_itoa(file->stat.st_uid);
        file->group_name = group ? mx_strdup(group->gr_name) : mx_itoa(file->stat.st_gid);
        file->size = flags->h ? mx_get_file_size(file->stat.st_size) : mx_itoa(file->stat.st_size);
        mx_get_file_lm_date(&file, flags);

        char* linked_file = mx_get_linked_file(file);
        file->linked_file = linked_file ? mx_strdup(linked_file) : NULL;
        mx_strdel(&linked_file);

    }
    file->next = NULL;

    return file;

}

t_file* mx_create_default_file_obj(const char* dir_name, const char* name) {

    t_file* file = malloc(sizeof(t_file));
    file->path = mx_get_file_path(dir_name, name);
    file->name = mx_strndup(name, mx_strlen(name));
    lstat(file->path, &(file->stat));

    for (int i = 0; i < 10; ++i) {

        ((&file->num_links)[i]) = NULL;

    }
    file->next = NULL;
    return file;

}

void mx_push_back(t_file **list, const char* dir_name, const char* file_name, t_flags* flags)
{
    t_file* new_node;
    if (!flags->l) 
        new_node = mx_create_default_file_obj(dir_name, file_name);
    else 
        new_node = mx_create_file_obj(dir_name, file_name, flags);
        
    t_file *last_node = *list;

    if (*list == NULL)
    {
        *list = new_node;
        return;
    }

    while (last_node->next != NULL)
    {
        last_node = last_node->next;
    }

    last_node->next = new_node;
}

t_file* mx_swap_nodes(t_file* node1, t_file* node2) {

    t_file* tmp = node2->next;
    node2->next = node1;
    node1->next = tmp;
    return node2;

}

int mx_list_size(t_file *list)
{
    t_file *node = list;
    int size = 0;

    while (node != NULL)
    {
        node = node->next;
        size++;
    }

    return size;
}

void mx_clear_node(t_file* node) {

    mx_strdel(&node->path);
    mx_strdel(&node->name);
    mx_strdel(&node->perms);
    mx_strdel(&node->num_links);
    mx_strdel(&node->user_name);
    mx_strdel(&node->group_name);
    mx_strdel(&node->size);
    mx_strdel(&node->lm_month);
    mx_strdel(&node->lm_day);
    mx_strdel(&node->lm_time);
    mx_strdel(&node->linked_file);

    free(node);

}

void mx_clear_list(t_file **list)
{
    t_file *node = *list;
    t_file *next = NULL;

    while (node != NULL)
    {
        next = node->next;
        mx_clear_node(node);
        node = next;
    }
    
    *list = NULL;
}
