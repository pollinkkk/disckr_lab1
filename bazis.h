#ifndef BAZIS
#define BAZIS

#include <stdio.h>

int len(char *str);
int len_bas(char *str);
void bazis_2(char *bazis, char *bazis2);
void bazis_3(char *bazis, char *bazis_3);
void print_bazis(char *bazis2);
int check_operators(char *str);
void get_rez(int *mas, int **table, int rows, int columns);
void triangel_Zegalkin(int *mas, int size, int *coeff, int ind);
void bazis_4(int *coeff, int size, char *bazis4, int *ind, char *vars, int cnt_vars);

#endif