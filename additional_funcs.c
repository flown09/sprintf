#include <stdio.h>

int count_fraction_digits(double num, int max_digits) {
    int count = 0;
    double fraction = num - (int)num;  
    while (count < max_digits) {
        fraction *= 10;
        int digit = (int)fraction;
        if (digit == 0 && count > 0) break; 
        fraction -= digit;
        count++;
    }
    return count;
}

int define_length(long long int num)
{
    int res = 0;
    if (num == 0)
        res = 1;
    else
    {
        while (num > 0)
        {
            num /= 10;
            res++;
        }
    }
    return res;
}

int define_length_unsigned(unsigned long long int num)
{
    int res = 0;
    if (num == 0)
        res = 1;
    else
    {
        while (num > 0)
        {
            num /= 10;
            res++;
        }
    }
    return res;
}

void number_to_string(long long int num , char *str)
{
    num = num < 0 ? num * (-1) : num;

    int i = 0;
    do {
        str[i++] = (num % 10) + '0';
        num /= 10;
    } while (num > 0);

    str[i] = '\0';

    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}

void number_to_string_unsigned(unsigned long long int num , char *str)
{
    // num = num < 0 ? num * (-1) : num;

    int i = 0;
    do {
        str[i++] = (num % 10) + '0';
        num /= 10;
    } while (num > 0);

    str[i] = '\0';

    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }
}

void double_to_string(double num, char *str, int max_digits)
{
    long long int whole = (long long int)num;
    double fraction = num - whole;
    int count_frac_dig = count_fraction_digits(num, 15);

    int i = 0;
    do {
        str[i++] = (whole % 10) + '0';
        whole /= 10;
    } while (whole > 0);

    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }

    if (max_digits > 0)
    {
        str[i++] = '.';
        if (count_frac_dig > max_digits)
        {
            for (int j = 0; j < max_digits; j++)
            {
                fraction *= 10;
                int digit = (int)fraction;
                if ((j == max_digits - 1) && ((int)((fraction - digit) * 10)) >= 5)
                    str[i++] = digit + 1 + '0';
                else
                    str[i++] = digit + '0';
                fraction -= digit;
            }
        }
        else 
        {
            for (int j = 0; j < max_digits; j++)
            {
                fraction *= 10;
                int digit = (int)fraction;
                str[i++] = digit + '0';
                fraction -= digit;
            }

            // if (count_frac_dig < max_digits)
            // {
            //     for (int j = 0; j < max_digits - count_frac_dig - 1; j++)
            //     {
            //         str[i++] = '0';
            //     }
            // }
        }
    }

    str[i] = '\0';
}