# CFLAGS = -Wall -Wextra -Werror -std=c11

all: s21_sprintf

s21_sprintf: s21_sprintf.o additional_funcs.o
	gcc s21_sprintf.o additional_funcs.o -o s21_sprintf

s21_sprintf.o: s21_sprintf.c
	gcc -c s21_sprintf.c -o s21_sprintf.o

additional_funcs.o: additional_funcs.c
	gcc -c additional_funcs.c -o additional_funcs.o

clean:
	DEL *.o *.exe

rebuild: clean all