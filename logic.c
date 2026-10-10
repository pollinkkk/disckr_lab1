#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "logic.h"

int apply_binary(int a, int b, char op) {
    switch (op) {
        case '&': return a && b;
        case '|': return a || b;
        case '^': return a ^ b;
        case '=': return a == b;         // эквиваленция
        case '>': return (!a) || b;      // импликация
        case '+': return a && (!b);      // коимпликация
        case '-': return !(a || b);      // стрелка Пирса
        case '#': return !(a && b);      // штрих Шеффера
    }

    return 0;
}

int get_prioritet(char operator)
{
    switch (operator)
    {
        case '!': return 0;
        case '&': return 1;

        case '|':
        case '^':
            return 2;

        case '#':
        case '-':
            return 3;

        case '+':
        case '>':
            return 4;

        case '=':
            return 5;

        case '(':
            return 6;
    }

    return -1;
}

int read_string(FILE *file, char *str, size_t size) 
{
    if(fgets(str, size, file) == NULL) 
        return 0;
    return 1;
}
char inverse(char el) 
{
    return el == '0' ? '1' : '0';
}
int compare (const void *a, const void *b) 
{
    char ch1 = *(const char *)a;
    char ch2 = *(const char *)b;
    return (ch1 - ch2);
}
int remove_spaces(char *str, char* sub_str) 
{
    int i=0, ind=0;

    while (str[i] != '\0')
    {
        if (!isspace((unsigned char)str[i]))
            sub_str[ind++] = str[i];
        i++;
    }
    sub_str[ind] = '\0';

    return ind;
}
void to_lower(char *str) 
{
    int i=0;

    while (str[i] != '\0')
    {
        str[i] = tolower(str[i]);
        i++;
    }
}
int is_operator(char c)
{
    return c == '!' || c == '&' || c == '|' ||
           c == '^' || c == '+' || c == '#' ||
           c == '>' || c == '=' || c == '-';
}

int var_not_in_mas(char *vars, char el, int size)
{
    for (int i=0; i<size; i++) 
    {
        if (vars[i] == el) return 0;
    } return 1;
}
int count_variables(char *str, char *vars, int size) 
{
    int cnt = 0;

    for (int i=0; i<size; i++) 
    {
        if (isalpha((unsigned char)str[i]) && var_not_in_mas(vars, str[i], cnt))
        {
            vars[cnt++] = str[i];
        }
    }
    return cnt;
}

void to_rpn(char *string, char *tokens, int len)
{
    char *stack = malloc(len); int stack_index = 0;
    int token_index =0;

    if (stack == NULL)
    {
        printf("Не удалось выделить память для стека.");
        return;
    }

    for (int i=0; string[i] != '\0'; i++)
    {
        char current_token = string[i];

        if (isalpha(current_token) || current_token == '0' || current_token == '1') 
        {
            tokens[token_index++] = current_token;
        }

        else if (current_token == '(') 
        {
            stack[stack_index++] = current_token;
        }

        else if (current_token == ')') {
            while (stack[stack_index-1] != '(')
            {
                tokens[token_index++] = stack[--stack_index];

            } stack_index--;
        }

        else if (is_operator(current_token))
        {
            while (stack_index > 0 && stack[stack_index - 1] != '(')
            {
                int stack_priority = get_prioritet(stack[stack_index - 1]);
                int current_priority = get_prioritet(current_token);

                if (stack_priority < current_priority ||
                    (stack_priority == current_priority && current_token != '!'))
                {
                    tokens[token_index++] = stack[--stack_index];
                }
                else
                {
                    break;
                }
            }

            stack[stack_index++] = current_token;
        }
    }

    while (stack_index>0) 
    {
            tokens[token_index++] = stack[--stack_index];

    }
    tokens[token_index] = '\0';

    free(stack);
}

char calculate(char *str, int size, int len)
{
    char *stack = malloc(len); int ind=0;

    if (stack == NULL)
    {
        printf("Не удалось выделить память для стека.");
        return '1';
    }

    for (int i=0;i<size;i++)
    {
        if (isalnum((unsigned char)str[i]))
            stack[ind++] = str[i];
        else if (str[i] == '!')
        {
            stack[ind-1] = inverse(stack[ind-1]);
        }
        else 
        {
            int op2 = stack[--ind] - '0', op1 = stack[--ind]-'0';
            int res = apply_binary(op1,op2,str[i]);
            stack[ind++] = res+ '0';
        }
    } 
    char rez = stack[0];
    free(stack);
    return rez;
}
char result(char *str, int *vals, int len)
{
    char *substr = malloc(len); int ind=0;

    if (substr == NULL)
    {
        printf("Не удалось выделить память для вспомогательной строки.");
        return '1';
    }

    for (int i=0; str[i]!='\0';i++)
    {
        if (isalpha((unsigned char)str[i])) {
            substr[ind++] = vals[str[i] - 'a'] + '0';
        }
        else 
            substr[ind++] = str[i];
    }
    char rez = calculate(substr,ind, len);
    free(substr);
    return rez;

} 
void generate(int *vals, int cnt, int num, char *vars) 
{
    for (int i=0; i<cnt; i++)
    {
        int bit = (num >> i) & 1;
        vals[vars[cnt-1-i] - 'a'] = bit;
    } return ;
}

void write_to_file(int **tabl, int rows, int columns)
{
    FILE *file = fopen("tabl.txt", "w");

    if (file == NULL)
    {
        printf("Не удалось открыть файл: %s\n", "tabl.txt");
        perror("fopen");
        return;
    }
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<columns; j++)
        {
            fprintf(file, "%d ", tabl[i][j]);
        }
        fprintf(file, "\n");
    }
    
    fclose(file);
}