#include <stdio.h>
#include <stdlib.h>

void build_scnf(int **table, int rows, int columns, char *vars, char *scnf, int cnt_vars)
{
    if (cnt_vars == 0)
    {
        if (table[0][0] == 0) 
        {
            scnf[0] = '0';
            scnf[1] = '\0';
            return;
        }

        scnf[0] = '\0';
        return;
    }
    int current_ind = 0;
    for (int i=0; i<rows; i++)
    {
        if (table[i][columns] == 0)
        {
            if (current_ind>0)
            {
                scnf[current_ind++] = '&';
                scnf[current_ind++] = '(';
            }
            else
                scnf[current_ind++] = '(';

            for (int j=0; j<columns; j++)
            {
                if (table[i][j] == 0)
                    scnf[current_ind++] = vars[j];
                else {
                    scnf[current_ind++] = '!';
                    scnf[current_ind++] = vars[j];
                }
                if (j+1 < columns)
                    scnf[current_ind++] = '|';
            }
            scnf[current_ind++] = ')';
        }
    }
    scnf[current_ind] = '\0';
}

void build_sdnf(int **table, int rows, int columns, char *vars, char *sdnf, int cnt_vars)
{
    if (cnt_vars == 0)
    {
        if (table[0][0] == 1) 
        {
            sdnf[0] = '1';
            sdnf[1] = '\0';
            return;
        }

        sdnf[0] = '\0';
        return;
    }
    int current_ind = 0;
    for (int i=0; i<rows; i++)
    {
        if (table[i][columns] == 1)
        {
            if (current_ind>0)
            {
                sdnf[current_ind++] = '|';
                sdnf[current_ind++] = '(';
            }
            else
                sdnf[current_ind++] = '(';

            for (int j=0; j<columns; j++)
            {
                if (table[i][j] == 1)
                    sdnf[current_ind++] = vars[j];
                else {
                    sdnf[current_ind++] = '!';
                    sdnf[current_ind++] = vars[j];
                }
                if (j+1 < columns)
                    sdnf[current_ind++] = '&';
            }
            sdnf[current_ind++] = ')';
        }
    }
    sdnf[current_ind] = '\0';
}

void print_scnf(char *scnf)
{
    int i = 0;
    while (scnf[i] != '\0') 
    {
        printf("%c", scnf[i++]);
    }
    if (i==0)
        printf("Не удалось построить СКНФ - для этого логического выражения ее не существует.");
    printf("\n");
}

void print_sdnf(char *sdnf)
{
    int i = 0;
    while (sdnf[i] != '\0') 
    {
        printf("%c", sdnf[i++]);
    }
    if (i==0)
        printf("Не удалось построить СДНФ - для этого логического выражения ее не существует.");
    printf("\n");
}