#ifndef NORMAL_FORMS
#define NORMAL_FORMS

#include <stdio.h>

void build_scnf(int ** table, int rows, int columns, char *vars, char *scnf, int cnt_vars);
void build_sdnf(int ** table, int rows, int columns, char *vars, char *sdnf, int cnt_vars);
void print_scnf(char *scnf);
void print_sdnf(char *sdnf);

#endif