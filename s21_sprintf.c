#include <stdio.h>
#include <stdarg.h>

typedef struct {
    int c_spec;
    int d_spec;
    int f_spec;
    int s_spec;
    int u_spec;
    char plus_flag;
    char minus_flag;
    char space_flag;
    int width;
    int accuracy;
    int is_accuracy;
    int length;
    int no_specs;
} Settings;

int define_length(long long int num);
void number_to_string(long long int num , char *str);
void define_sett(const char *format, int *i, Settings *settings);
void process_minus_flag(char *str, int *output, Settings *settings, int argum_length);
void process_d(char *str, int *output, Settings *settings, long long int argum);
void process_s(char *str, int *output, Settings *settings, char *argum);
void process_c(char *str, int *output, Settings *settings, char *argum);


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
            //int rollback = i;
            Settings settings = {0};
            
            define_sett(format, &i, &settings);
            i--;

            // printf("%d %d %d %d %d %c %c %c %d %d %c %d\n", settings.c_spec, settings.d_spec, settings.f_spec, 
            // settings.s_spec, settings.u_spec, settings.plus_flag, settings.minus_flag, 
            // settings.space_flag, settings.width, settings.accuracy, settings.length, settings.no_specs);

            if (settings.no_specs)
            {
                // str[output++] = '%';
                // i = rollback;
                continue;
            }
            
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
                char *value = va_arg(arg, char *);
                process_s(str, &output, &settings, value);
            }
            else if (settings.c_spec)
            {
                // if (settings.length == 'l')
                // {
                //     wchar_t value = va_arg(arg, wchar_t);
                //     process_c(str, &output, &settings, value);
                // }
                
                {
                    char* value = va_arg(arg, char *);
                    process_c(str, &output, &settings, value);
                }
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
            for (int i = 0; i < settings->width - argum_length - 1; i++)
                str[(*output)++] = ' ';
        }
    }
}

void process_c(char *str, int *output, Settings *settings, char *argum)
{
    int argum_length = 1;

    process_minus_flag(str, output, settings, argum_length);

    str[(*output)++] = argum[0];
}

void process_s(char *str, int *output, Settings *settings, char *argum)
{
    int argum_length = 0;

    for (int i = 0; argum[i] != '\0'; i++)
    {
        argum_length++;
    }
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

    int argum_length = (settings->accuracy > define_length(argum)) ? settings->accuracy : define_length(argum);
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
        for (int i = 0; i < settings->accuracy - define_length(argum); i++)
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
    char mas1[100] = "privet_hi";
    char mas2[100] = "privet_hi";
    char mas3[100] = "privet_hi";
    char mas4[100] = "privet_hi";
    char mas5[100];
    char mas6[100], mas7[100], mas8[100], ma9[100], mas10[100];
    int n1, n2, n3, n4, n5, n6, n7, n8, n9, n10;
    n1 = sprintf(mas1, "%lld", 2147483648);
    n2 = s21_sprintf(mas2, "%lld", 2147483648);
    n3 = sprintf(mas3, "%.2s %s", "priv", "huy");
    n4 = s21_sprintf(mas4, "%.2s %s", "priv", "huy");
    n5 = sprintf(mas5, "%5c", 'A');
    n6 = sprintf(mas6, "%5c", 'A');

    printf("sprintf: %lld, %s\n", n1, mas1);
    printf("s21_sprintf: %lld, %s\n", n2, mas2);
    printf("sprintf: %d, %s\n", n3, mas3);
    printf("s21_sprintf: %d, %s\n", n4, mas4);
    printf("sprintf: %s\n", mas5);
    printf("sprintf: %s", mas6);

    return 0;

}

