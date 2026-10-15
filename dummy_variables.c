#include <stdio.h>
#include <stdlib.h>

void dum_vars(int **table, int rows, int columns, char *dumm_vars, int count_vars, char* vars)
{
    int ind = 0;
    for (int i=0; i<columns; i++)
    {
        int step = 1 << (count_vars - 1 - i);
        int j=0; int flag = 1;
        while (j+step<rows)
        {
            if ((j & step)  == 0) 
            {
                if (table[j][count_vars] != table[j+step][count_vars]) 
                {
                    flag = 0;
                    break;
                }
            }
            j++;
        }

        if (flag)
        {
            dumm_vars[ind++] = vars[i];
        }
    }
    dumm_vars[ind] = '\0';
}
void print_dum_vars(char* vars)
{
    if (vars[0] == '\0')
        printf("У логического выражения нет фиктивных переменных.\n");

    else 
    {
        printf("Фиктивные переменные:\n");
        for (int i=0; vars[i] != '\0'; i++)
        {
            printf("%c ", vars[i]);
        }
        printf("\n");
    }
}