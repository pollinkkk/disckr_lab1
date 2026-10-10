#ifndef LOGIC_H
#define LOGIC_H

#include <stdio.h>

#define MAX_SIZE 256
#define VAR_COUNT 26

int apply_binary(int a, int b, char op);
int get_prioritet(char operator);
int read_string(FILE *file, char *str, size_t size);
char inverse(char el);
int compare (const void *a, const void *b);
int remove_spaces(char *str, char* sub_str);
void to_lower(char *str);
int is_operator(char c);
int var_not_in_mas(char *vars, char el, int size);
int count_variables(char *str, char *vars, int size);
void to_rpn(char *string, char *tokens);
char calculate(char *str, int size);
char result(char *str, int *vals);
void generate(int *vals, int cnt, int num, char *vars);
void write_to_file(int **tabl, int rows, int columns);
#endif
