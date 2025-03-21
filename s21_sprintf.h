#ifndef S21_SPRINTF_H
#define S21_SPRINTF_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <wchar.h>
#include <locale.h>

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
int define_length_unsigned(unsigned long long int num);
void number_to_string(long long int num , char *str);
void number_to_string_unsigned(unsigned long long int num , char *str);
int count_fraction_digits(double num, int max_digits);
void double_to_string(double num, char *str, int max_digits);
void define_sett(const char *format, int *i, Settings *settings);
void process_minus_flag(char *str, int *output, Settings *settings, int argum_length);
void process_d(char *str, int *output, Settings *settings, long long int argum);
void process_s(char *str, int *output, Settings *settings, char *argum);
void process_ls(char *str, int *output, Settings *settings, wchar_t *argum);
void process_c(char *str, int *output, Settings *settings, int argum);
void process_lc(char *str, int *output, Settings *settings, wchar_t argum);
void process_f(char *str, int *output, Settings *settings, double argum);
void process_u(char *str, int *output, Settings *settings, unsigned long long int argum);

#endif // S21_SPRINTF_H