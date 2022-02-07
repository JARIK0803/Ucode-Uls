#include "../inc/uls.h"

static bool is_flag_found(char flag) {

    for (size_t i = 0; i < FLAG_COUNT; ++i) {

        if (flag == const_flags[i])
            return true;

    }
    return false;

}

static void add_flag(t_flags** flags, char flag) {

    switch (flag) {

    case 'l':
        (*flags)->l = 1;
        (*flags)->C = 0;
        (*flags)->m = 0;
        (*flags)->one = 0;
        break;

    case 'a':
        (*flags)->a = 1;
        (*flags)->A = 0;
        break;

    case 'A':
        (*flags)->a = 0;
        (*flags)->A = 1;
        break;

    case 'G':
        (*flags)->G = 1;
        break;

    case 'T':
        (*flags)->T = 1;
        break;

    case 'r':
        (*flags)->r = 1;
        break;

    case 't':
        (*flags)->t = 1;
        break;

    case 'u':
        (*flags)->u = 1;
        (*flags)->c = 0;
        break;

    case 'c':
        (*flags)->c = 1;
        (*flags)->u = 0;
        break;

    case 'S':
        (*flags)->S = 1;
        break;

    case 'R':
        (*flags)->R = 1;
        break;

    case 'h':
        (*flags)->h = 1;
        break;

    case '@':
        (*flags)->at = 1;
        break;

    case 'e':
        (*flags)->e = 1;
        break;

    case '1':
        (*flags)->one = 1;
        (*flags)->l = 0;
        (*flags)->C = 0;
        (*flags)->m = 0;
        break;

    case 'C':
        (*flags)->C = 1;
        (*flags)->l = 0;
        (*flags)->one = 0;
        break;
    
    case 'F':
        (*flags)->F = 1;
        break;
    
    case 'm':
        (*flags)->m = 1;
        (*flags)->one = 0;
        (*flags)->l = 0;
        break;

    case 'f':
        (*flags)->f = 1;
        (*flags)->a = 1;
        (*flags)->A = 0;
        break;

    default:
        break;

    }

}

static void flag_init(t_flags* flags) {

    for (int i = 0; i < FLAG_COUNT; ++i) {

        ((&flags->a)[i]) = 0;

    }

}

t_flags* mx_get_flags(int argc, const char** argv, int* flag_count) {

    t_flags* flags = malloc(sizeof(t_flags));
    flag_init(flags);
    for (int i = 1; i < argc; ++i) {

        if ((argv[i][0] == '-') && (mx_strlen(argv[i]) > 1)) {
            
            if (mx_isalpha(argv[i][1]) || argv[i][1] == '1' || argv[i][1] == '@') {

                ++(*flag_count);
                for (int j = 1; argv[i][j] != '\0'; j++) {

                    if (is_flag_found(argv[i][j])) {
                        add_flag(&flags, argv[i][j]);
                    } else {
                        mx_print_flag_err(argv[i][j]);
                        exit(1);
                    }
                
                }

            } else {
                mx_print_flag_err(argv[i][1]);
                exit(1);
            }

        } else break;
    }
    return flags;
}
