#include "s21_sprintf.h"


int s21_sprintf(char *str, const char *format, ...)
{
    int output = 0;
    int i = 0;

    va_list arg;
    va_start(arg, format);

    for (; format[i] != '\0'; i++)
    {
        if (format[i] == '%')
        {
            if (format[i + 1] == '%')
            {
                str[output++] = '%';
                i++;
            }
            else if (format[i + 1] == '\0')
                break;

            i++;
            Settings settings = {0};
            define_sett(format, &i, &settings);
            i--;

            // printf("%d %d %d %d %d %c %c %c %d %d %c %d\n", settings.c_spec, settings.d_spec, settings.f_spec, 
            // settings.s_spec, settings.u_spec, settings.plus_flag, settings.minus_flag, 
            // settings.space_flag, settings.width, settings.accuracy, settings.length, settings.no_specs);

            if (settings.no_specs)
                continue;

            if (settings.d_spec)
            {
                long long int value;
                if (settings.length == 'h')
                    value = (short int) va_arg(arg, int);
                else if (settings.length == 'l')
                    value = (long int) va_arg(arg, int);
                else if (settings.length == 'L')
                    value = (long long int) va_arg(arg, int);
                else
                    value = va_arg(arg, int);
                process_d(str, &output, &settings, value);
            }
            else if (settings.s_spec)
            {
                if (settings.length == 'l')
                {
                    wchar_t *value = (wchar_t *) va_arg(arg, char *);
                    process_ls(str, &output, &settings, value);
                }
                else
                {
                    char *value = va_arg(arg, char *);
                    process_s(str, &output, &settings, value);
                }
            }
            else if (settings.c_spec)
            {

                if (settings.length == 'l')
                {
                    wchar_t value = (wchar_t) va_arg(arg, int);
                    process_lc(str, &output, &settings, value);
                }
                else
                {
                    int value = va_arg(arg, int);
                    process_c(str, &output, &settings, value);
                }
            }
            else if (settings.f_spec)
            {
                double value = va_arg(arg, double);
                process_f(str, &output, &settings, value);
            }
            else if (settings.u_spec)
            {
                unsigned long long value;
                if (settings.length == 'h')
                    value = (unsigned short int) va_arg(arg, unsigned int);
                else if (settings.length == 'l')
                    value = (unsigned long int) va_arg(arg, unsigned int);
                else if (settings.length == 'L')
                    value = (unsigned long long int) va_arg(arg, unsigned int);
                else
                    value = va_arg(arg, unsigned int);
                process_u(str, &output, &settings, value);
            }
        }
        else
        {
            str[output++] = format[i];
        }
    }

    str[output] = '\0';
    va_end(arg);
    return output;
}

void process_minus_flag(char *str, int *output, Settings *settings, int argum_length)
{
    if (settings->minus_flag == 0)
    {
        if (settings->width > argum_length)
        {
            for (int i = 0; i < settings->width - argum_length; i++)
                str[(*output)++] = ' ';
        }
    }
}

void process_u(char *str, int *output, Settings *settings, unsigned long long int argum)
{
    int def_len_uns = define_length_unsigned(argum);
    int argum_length = (settings->accuracy > def_len_uns) ? settings->accuracy : def_len_uns;
    printf("acc: %d, len: %d", settings->accuracy, argum_length);
    process_minus_flag(str, output, settings, argum_length);

    if (settings->plus_flag && argum > 0)
        str[(*output)++] = '+';

    if (settings->space_flag && str[(*output)] != ' ')
        str[(*output)++] = ' ';

    if (settings->accuracy)
    {
        for (int i = 0; i < settings->accuracy - def_len_uns; i++)
            str[(*output)++] = '0';
    }

    char buffer[100];

    number_to_string_unsigned(argum, buffer);

    for (int i = 0; buffer[i] != '\0'; i++)
        str[(*output)++] = buffer[i];
}

void process_f(char *str, int *output, Settings *settings, double argum)
{
    int is_negative = 0;
    if (argum < 0)
    {
        is_negative = 1;
        argum = -argum;
    }

    char buffer[100];
    if (settings->is_accuracy)
    {
        double_to_string(argum, buffer, settings->accuracy);
    }
    else
    {
        int argum_length = define_length((int)argum);
        double_to_string(argum, buffer, argum_length);
    }

    int len = 0;
    for (; ; len++)
    {
        if (buffer[len] == '\0')
            break;
    }
    
    process_minus_flag(str, output, settings, len);

    if (settings->plus_flag)
    {
        if (argum > 0)
            str[(*output)++] = '+';
        else
            str[(*output)++] = '-';
    }

    if (settings->space_flag && str[(*output)] != ' ' && is_negative == 0)
        str[(*output)++] = ' ';

    for (int i = 0; buffer[i] != '\0'; i++)
        str[(*output)++] = buffer[i];
}

void process_lc(char *str, int *output, Settings *settings, wchar_t argum) {
    char buffer[MB_LEN_MAX];
    int len = wctomb(buffer, argum); 

    if (len < 0) return;

    process_minus_flag(str, output, settings, len);
    for (int i = 0; i < len; i++) {
        str[(*output)++] = buffer[i];
    }
}

void process_c(char *str, int *output, Settings *settings, int argum)
{
    int argum_length = 1;
    process_minus_flag(str, output, settings, argum_length);
    str[(*output)++] = (char)argum;
}

void process_ls(char *str, int *output, Settings *settings, wchar_t *argum)
{
    int len = wcstombs(((void *)0), argum, 0) + 1;
    char *converted = (char *)malloc(len);
    if (!converted) return;

    wcstombs(converted, argum, len);
    int argum_length = 0;

    for (int i = 0; converted[i] != '\0'; i++)
        argum_length++;
    process_minus_flag(str, output, settings, argum_length);

    if (settings->is_accuracy && settings->accuracy < argum_length)
    {
        for (int i = 0; i < settings->accuracy; i++)
            str[(*output)++] = converted[i];
    }
    else
    {
        for (int i = 0; argum[i] != '\0'; i++)
            str[(*output)++] = converted[i];
    }

    free(converted);
}

void process_s(char *str, int *output, Settings *settings, char *argum)
{
    int argum_length = 0;

    for (int i = 0; argum[i] != '\0'; i++)
        argum_length++;

    process_minus_flag(str, output, settings, argum_length);
    
    //printf("%d, %d, %d\n", settings->is_accuracy, settings->accuracy, argum_length);
    if (settings->is_accuracy && settings->accuracy < argum_length)
    {
        for (int i = 0; i < settings->accuracy; i++)
            str[(*output)++] = argum[i];
    }
    else
    {
        for (int i = 0; argum[i] != '\0'; i++)
            str[(*output)++] = argum[i];
    }
}

void process_d(char *str, int *output, Settings *settings, long long int argum)
{
    int is_negative = 0;
    if (argum < 0)
    {
        is_negative = 1;
        argum = -argum;
    }

    int def_len = define_length(argum);
    int argum_length = (settings->accuracy > def_len) ? settings->accuracy : def_len;
    process_minus_flag(str, output, settings, argum_length);
    
    if (settings->plus_flag)
    {
        if (argum > 0)
            str[(*output)++] = '+';
        else
            str[(*output)++] = '-';
    }

    if (settings->space_flag && str[(*output)] != ' ' && is_negative == 0)
        str[(*output)++] = ' ';

    if (settings->accuracy)
    {
        for (int i = 0; i < settings->accuracy - def_len; i++)
            str[(*output)++] = '0';
    }

    char buffer[100];

    number_to_string(argum, buffer);

    for (int i = 0; buffer[i] != '\0'; i++)
        str[(*output)++] = buffer[i];
}

void define_sett(const char *format, int *i, Settings *settings)
{
    int exit = 0;

    while (format[*i] == '+' || format[*i] == '-' || format[*i] == ' ' || (format[*i] >= '0' && format[*i] <= '9'))
    {
        if (format[*i] == '+')
            settings->plus_flag = format[*i];
        else if (format[*i] == '-')
            settings->minus_flag = format[*i];
        else if (format[*i] == ' ')
            settings->space_flag = format[*i];
        else if (format[*i] >= '0' && format[*i] <= '9')
            settings->width = settings->width * 10 + (format[*i] - '0');
        (*i)++;
    }

    if (format[*i] == '.')
    {
        settings->is_accuracy = 1;
        (*i)++;
        while (format[*i] >= '0' && format[*i] <= '9') {
            settings->accuracy = settings->accuracy * 10 + (format[*i] - '0');  
            (*i)++;
        }
    }


    for (; format[*i] != '\0' && exit == 0; (*i)++)
    {
        if (format[*i] == ' ')
            continue;
        else if ((format[*i] == 'h') || (format[*i] == 'l'))
        {
            if (format[*i + 1] == 'l')
            {
                settings->length = 'L'; 
                (*i)++;
            }
            else
                settings->length = 'l';
        }
        else if (format[*i] == 'c')
        {
            settings->c_spec++;
            exit = 1;
        }
        else if (format[*i] == 'd')
        {
            settings->d_spec++;
            exit = 1;
        }
        else if (format[*i] == 'f')
        {
            settings->f_spec++;
            exit = 1;
        }
        else if (format[*i] == 's')
        {
            settings->s_spec++;
            exit = 1;
        }
        else if (format[*i] == 'u')
            {
            settings->u_spec++;
            exit = 1;
        }
        else
        {
            settings->no_specs++;
            exit = 1;
        }
    }
}


int main(void)
{
    setlocale(LC_ALL, ""); // Позволяет корректно работать с Unicode
    char mas1[100] = "privet_hi";
    char mas2[100] = "privet_hi";
    char mas3[100] = "privet_hi";
    char mas4[100] = "privet_hi";
    char mas5[100];
    char mas6[100], mas7[100], mas8[100], mas9[100], mas10[100], mas11[100], mas12[100];
    int n1, n2, n3, n4, n5, n6, n7, n8, n9 = 0, n10 = 0, n11 = 0, n12 = 0;
    n1 = sprintf(mas1, "%6u", 1234);
    n2 = s21_sprintf(mas2, "%6u", 1234);
    n3 = sprintf(mas3, "%.2s %s", "priv", "huy");
    n4 = s21_sprintf(mas4, "%.2s %s", "priv", "huy");
    n5 = sprintf(mas5, "%5c", 'A');
    n6 = s21_sprintf(mas6, "%5c", 'A');
    n7 = sprintf(mas7, "% .5f", 12.123456789);
    n8 = s21_sprintf(mas8, "% .5f", 12.123456789);
    n9 = sprintf(mas9, "%6u", 1234);
    n10 = s21_sprintf(mas10, "%6u", 1234);
    

    printf("sprintf: %d, %s\n", n1, mas1);
    printf("s21_sprintf: %d, %s\n", n2, mas2);
    printf("sprintf: %d, %s\n", n3, mas3);
    printf("s21_sprintf: %d, %s\n", n4, mas4);
    printf("sprintf: %d, %s\n", n5, mas5);
    printf("s21_sprintf: %d, %s\n", n6, mas6);
    printf("sprintf: %d, %s\n", n7, mas7);
    printf("s21_sprintf: %d, %s\n", n8, mas8);
    printf("sprintf: %d, %s\n", n9, mas9);
    printf("s21_sprintf: %d, %s\n", n10, mas10);

    char buf[100];
    // s21_sprintf(buf, "%lc", L"Ёm");
    // printf("%s\n", buf);

    sprintf(buf, "%lc", L"Привет, мир!");
    printf("%s\n", buf);
    return 0;

}

