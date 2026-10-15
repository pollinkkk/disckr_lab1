#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int len(char *str)
{
    int cnt = 0;
    while (str[cnt] != '\0')
    {
        cnt++;
    }
    return cnt;
}
int len_bas(char *str)
{
    int *stack = malloc(len(str) * sizeof(int)); int ind = 0;

    if (stack == NULL)
        return -1;

    for (int i=0; str[i] != '\0'; i++)
    {
        if (isalpha(str[i]))
            stack[ind++] = 1;

        else if(str[i] == '!') 
            stack[ind-1] = 2 * stack[ind-1] + 3;

        else
        {
            stack[ind - 2] = stack[ind-2] * 2 + stack[ind-1]*2 + 9;
            ind--;
        }
    }
    int rez = stack[0];
    free(stack);
    return rez;
}

int current_str(char * stack, char *str, int ind)
{
    if (stack[ind] != ')')
    {
        str[0] = stack[ind];
        return 1;
    }

    int i=0;
    int staples = 1;
    str[i++] = stack[ind--];

    while (staples)
    {
        char current = stack[ind--];
        str[i++] = current;
        if (current == '(')
            staples--;

        else if (current == ')')
            staples++;
    }

    str[i] = '\0';
    return i;
}

void print_bazis(char *bazis2)
{
    for (int i=0; bazis2[i] != '\0'; i++)
    {
        printf("%c", bazis2[i]);
    }
    printf("\n");
}
int check_operators(char *str)
{
    for(int i=0; str[i] != '\0'; i++)
    {
        if (str[i] == '^' ||
            str[i] == '>' ||
            str[i] == '+' ||
            str[i] == '-' ||
            str[i] == '#' ||
            str[i] == '=' ||
            str[i] == '0' ||
            str[i] == '1')
            return 0;
    }
    return 1;
}

void bazis_2(char *bazis, char *bazis2)
{
    int size_bas = len_bas(bazis);
    char *stack = malloc(size_bas+1);
    int ind = 0;

    for (int j=0; bazis[j] != '\0'; j++)
    {
        char current_el = bazis[j];

        if (isalpha(current_el))
            stack[ind++] = current_el;

        else if (current_el == '!')
        {
            char *sub_str = malloc(size_bas+1);
            int size;
            size = current_str(stack, sub_str, ind-1);
            ind = ind - size;

            stack[ind++] = '(';
            for (int i=size-1; i>=0; i--)
            {
                stack[ind++] = sub_str[i];
            }
            stack[ind++] = '-';
            for (int i=size-1; i>=0; i--)
            {
                stack[ind++] = sub_str[i];
            }
            stack[ind++] = ')';
            free(sub_str);

        }

        else if (current_el == '|')
        {
            char *sub_str1 = malloc(size_bas+1);
            char *sub_str2 = malloc(size_bas+1);
            int size1, size2;
            size1 = current_str(stack, sub_str1, ind-1);
            size2 = current_str(stack, sub_str2, ind-size1-1);
            ind = ind - size1 - size2 ;

            stack[ind++] = '(';
            stack[ind++] = '(';
            for (int i=size1-1; i>=0; i--)
            {
                stack[ind++] = sub_str1[i];
            }
            stack[ind++] = '-';
            for (int i=size2-1; i>=0; i--)
            {
                stack[ind++] = sub_str2[i];
            }
            stack[ind++] = ')';
            stack[ind++] = '-';
            stack[ind++] = '(';
            for (int i=size1-1; i>=0; i--)
            {
                stack[ind++] = sub_str1[i];
            }
            stack[ind++] = '-';
            for (int i=size2-1; i>=0; i--)
            {
                stack[ind++] = sub_str2[i];
            }
            stack[ind++] = ')';
            stack[ind++] = ')';
            
            free(sub_str1);
            free(sub_str2);
        }

        else if (current_el == '&')
        {
            char *sub_str1 = malloc(size_bas+1);
            char *sub_str2 = malloc(size_bas+1);
            int size1, size2;
            size1 = current_str(stack, sub_str1, ind-1);
            size2 = current_str(stack, sub_str2, ind-size1-1);
            ind = ind - size1 - size2 ;

            stack[ind++] = '(';
            stack[ind++] = '(';
            for (int i=size1-1; i>=0; i--)
            {
                stack[ind++] = sub_str1[i];
            }
            stack[ind++] = '-';
            for (int i=size1-1; i>=0; i--)
            {
                stack[ind++] = sub_str1[i];
            }
            stack[ind++] = ')';
            stack[ind++] = '-';
            stack[ind++] = '(';
            for (int i=size2-1; i>=0; i--)
            {
                stack[ind++] = sub_str2[i];
            }
            stack[ind++] = '-';
            for (int i=size2-1; i>=0; i--)
            {
                stack[ind++] = sub_str2[i];
            }
            stack[ind++] = ')';
            stack[ind++] = ')';
            
            free(sub_str1);
            free(sub_str2);
        }
        stack[ind] = '\0';
    }

    for (int i=0; i<ind; i++)
    {
        bazis2[i] = stack[i];
    }
    bazis2[ind] = '\0';
    free(stack);
}

void bazis_3(char *bazis, char *bazis3)
{
    int size_bas = len_bas(bazis);
    char *stack = malloc(size_bas+1);
    int ind = 0;

    for (int j=0; bazis[j] != '\0'; j++)
    {
        char current_el = bazis[j];

        if (isalpha(current_el))
            stack[ind++] = current_el;

        else if (current_el == '!')
        {
            char *sub_str = malloc(size_bas+1);
            int size;
            size = current_str(stack, sub_str, ind-1);
            ind = ind - size;

            stack[ind++] = '(';
            for (int i=size-1; i>=0; i--)
            {
                stack[ind++] = sub_str[i];
            }
            stack[ind++] = '#';
            for (int i=size-1; i>=0; i--)
            {
                stack[ind++] = sub_str[i];
            }
            stack[ind++] = ')';
            free(sub_str);

        }

        else if (current_el == '&')
        {
            char *sub_str1 = malloc(size_bas+1);
            char *sub_str2 = malloc(size_bas+1);
            int size1, size2;
            size1 = current_str(stack, sub_str1, ind-1);
            size2 = current_str(stack, sub_str2, ind-size1-1);
            ind = ind - size1 - size2 ;

            stack[ind++] = '(';
            stack[ind++] = '(';
            for (int i=size1-1; i>=0; i--)
            {
                stack[ind++] = sub_str1[i];
            }
            stack[ind++] = '#';
            for (int i=size2-1; i>=0; i--)
            {
                stack[ind++] = sub_str2[i];
            }
            stack[ind++] = ')';
            stack[ind++] = '#';
            stack[ind++] = '(';
            for (int i=size1-1; i>=0; i--)
            {
                stack[ind++] = sub_str1[i];
            }
            stack[ind++] = '#';
            for (int i=size2-1; i>=0; i--)
            {
                stack[ind++] = sub_str2[i];
            }
            stack[ind++] = ')';
            stack[ind++] = ')';
            
            free(sub_str1);
            free(sub_str2);
        }

        else if (current_el == '|')
        {
            char *sub_str1 = malloc(size_bas+1);
            char *sub_str2 = malloc(size_bas+1);
            int size1, size2;
            size1 = current_str(stack, sub_str1, ind-1);
            size2 = current_str(stack, sub_str2, ind-size1-1);
            ind = ind - size1 - size2 ;

            stack[ind++] = '(';
            stack[ind++] = '(';
            for (int i=size1-1; i>=0; i--)
            {
                stack[ind++] = sub_str1[i];
            }
            stack[ind++] = '#';
            for (int i=size1-1; i>=0; i--)
            {
                stack[ind++] = sub_str1[i];
            }
            stack[ind++] = ')';
            stack[ind++] = '#';
            stack[ind++] = '(';
            for (int i=size2-1; i>=0; i--)
            {
                stack[ind++] = sub_str2[i];
            }
            stack[ind++] = '#';
            for (int i=size2-1; i>=0; i--)
            {
                stack[ind++] = sub_str2[i];
            }
            stack[ind++] = ')';
            stack[ind++] = ')';
            
            free(sub_str1);
            free(sub_str2);
        }
        stack[ind] = '\0';
    }

    for (int i=0; i<ind; i++)
    {
        bazis3[i] = stack[i];
    }
    bazis3[ind] = '\0';
    free(stack);
}


void get_rez(int *mas, int **table, int rows, int columns)
{
    for (int i=0; i<rows; i++)
    {
        mas[i] = table[i][columns];
    }
}

void triangel_Zegalkin(int *mas, int size, int *coeff, int ind)
{
    coeff[ind] = mas[0];
    
    if (size == 1)
        return;

    for (int i=0; i<size-1; i++)
    {
        mas[i] = mas[i] ^ mas[i+1];
    }

    triangel_Zegalkin(mas, size-1, coeff, ind+1);
}

void bazis_4(int *coeff, int size, char *bazis4, int *ind, char *vars, int cnt_vars)
{
    for(int i=0; i<size; i++)
    {
        if (coeff[i] == 1)
        {
            if ((*ind) > 0)
                bazis4[(*ind)++] = '^';

            if (i == 0)
                bazis4[(*ind)++] = '1';
            else
            {
                for (int j=0; j<cnt_vars; j++)
                {
                    int bit = 1 << (cnt_vars -1 -j);
                    if (i & bit)
                    {
                        if ((*ind) > 0 && isalpha(bazis4[(*ind)-1]))
                            bazis4[(*ind)++] = '&';
                        bazis4[(*ind)++] = vars[j];
                    }
                }
            }
        }
    }
    bazis4[*ind] = '\0';
}