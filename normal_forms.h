#ifndef NORMAL_FORMS
#define NORMAL_FORMS

#include <stdio.h>

#define MAX_SIZE_2 512

void build_scnf(int ** table, int rows, int columns, char *vars, char *scnf);
void build_sdnf(int ** table, int rows, int columns, char *vars, char *sdnf);
void print_scnf(char *scnf);
void print_sdnf(char *sdnf);

#endif