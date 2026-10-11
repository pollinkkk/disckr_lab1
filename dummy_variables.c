#include <stdio.h>
#include <stdlib.h>

int count_bin(int num)
{
    int count = 0;

    while (num>0) {
        count++;
        num >>= 1;
    }
    return count;
}
void to_bin(int *bin, int num)
{
    int ind = 0;
    while (num > 0)
    {
    bin[ind++] = (num & 1);
    num >>=1;
    }
}

int pow_2(int st)
{
    int len = count_bin(st);
    int* bin = malloc(len * sizeof(int));
    to_bin(bin, st);
    int rez = 1;
    for (int i=len-1; i>=0; i--)
    {
        if (bin[i])
            rez *= 2;
        if (i>0)
            rez *= rez;

    }
    free(bin);
    return rez;
}
void dum_vars(int **table, int rows, int columns, char *dumm_vars, int count_vars, char* vars)
{
    int ind = 0;
    for (int i=0; i<columns; i++)
    {
        int step = pow_2(count_vars - 1 - i);
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
        printf("У логичсекого выражения нет фиктивных переменных.\n");

    else 
    {
        printf("Фиктивные перемнные:\n");
        for (int i=0; vars[i] != '\0'; i++)
        {
            printf("%c ", vars[i]);
        }
        printf("\n");
    }
}