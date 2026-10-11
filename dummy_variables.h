#ifndef DUMMY_VARIABLES_H
#define DUMMY_VARIABLES_H

#include <stdio.h>

void dum_vars(int **table, int rows, int columns, char *dumm_vars, int count_vars, char* vars);
int pow_2(int st);
void to_bin(int *bin, int num);
int count_bin(int num);
void print_dum_vars(char* vars);

#endif