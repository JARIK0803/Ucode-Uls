#include "../inc/uls.h"

float mx_round(float num, int precision)
{
    float base = 1;

    for (int i = 0; i < precision; i++)
    {
        base *= 10;
    }
    
    num *= base;

    if (num < 0.0)
        return ((int)(num - 0.5)) / base;
    else
        return ((int)(num + 0.5) ) / base;
    
}

char *mx_dtoa(float d, int precision)
{
    float tmp = d;
    for (int i = 0; i < precision; i++)
    {
        tmp *= 10;
    }

    char *d_str = mx_itoa((int)tmp);

    mx_realloc(d_str, 1);
    int len = mx_strlen(d_str);

    for (int i = len + 1; i >= len - precision; i--)
    {
        if (i == len - precision)
        {
            d_str[i] = '.';
            break;

        } else {

        d_str[i] = d_str[i - 1];

        }
    }
    
    return d_str;
}
