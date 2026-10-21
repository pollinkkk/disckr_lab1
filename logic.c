#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "logic.h"

int apply_binary(int a, int b, char op) {
    // выбираем символ для операции
    switch (op) {
        case '&': return a && b;         // конъюнкция
        case '|': return a || b;         // дизъюнкция
        case '^': return a ^ b;          // XOR
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
    // получаем символ операции и возвращаем ее приоритет
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
    // читаем строку
    if(fgets(str, size, file) == NULL) 
        return 0; // строку прочитать не удалось
    return 1;
}
char inverse(char el) 
{
    // инверсия
    return el == '0' ? '1' : '0';
}
int compare (const void *a, const void *b) 
{
    // функция сравнения двух элементов для qsort
    char ch1 = *(const char *)a;
    char ch2 = *(const char *)b;
    return (ch1 - ch2);
}
int remove_spaces(char *str, char* sub_str) 
{
    // убираем пробельные и тд символы из строки
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
    // перевод букв строки в нижний регистр
    int i=0;

    while (str[i] != '\0')
    {
        str[i] = tolower((unsigned char)str[i]);
        i++;
    }
}
int is_operator(char c)
{
    // проверяем что введенный символ это один из 
    // поддерживаемых операторов
    return c == '!' || c == '&' || c == '|' ||
           c == '^' || c == '+' || c == '#' ||
           c == '>' || c == '=' || c == '-';
}

int var_not_in_mas(char *vars, char el, int size)
{
    // проверяем отсутствие элемента в строке
    for (int i=0; i<size; i++) 
    {
        if (vars[i] == el) return 0;
    } return 1;
}
int count_variables(char *str, char *vars, int size) 
{
    // считаем количество различных переменных в выражении
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

int to_rpn(char *string, char *tokens, int len)
{
    // перевод в обратную польскую запись

    // используем стек
    char *stack = malloc(len);
    int stack_index = 0; // количество элементов в стеке
    int token_index =0; // позиция для записи в выходную строку

    if (stack == NULL)
    {
        printf("Не удалось выделить память для стека.");
        return -1;
    }

    for (int i=0; string[i] != '\0'; i++)
    {
        char current_token = string[i]; // текущий символ

        // переменная или константа
        if (isalpha(current_token) || current_token == '0' || current_token == '1') 
        {
            tokens[token_index++] = current_token;
        }

        // открывающая скобка
        else if (current_token == '(') 
        {
            stack[stack_index++] = current_token;
        }
        // закрывающая скобка
        else if (current_token == ')') {
            // пока верхний элемент стека не открывающая скобка
            while (stack[stack_index-1] != '(')
            {
                // берем оператор с вершины стека
                tokens[token_index++] = stack[--stack_index];

            } stack_index--; // убираем саму открывающую скобку
        }

        // оператор
        else if (is_operator(current_token))
        {
            // пока стек не пуст и не встретили откр скобку
            while (stack_index > 0 && stack[stack_index - 1] != '(')
            {
                // проритет операции на вершине стека
                int stack_priority = get_prioritet(stack[stack_index - 1]);
                // проритет текущей операции         
                int current_priority = get_prioritet(current_token);

                // если в стеке приоритет операции выше или равен 
                // (но текущая операция не отрицание)
                if (stack_priority < current_priority ||
                    (stack_priority == current_priority && 
                        current_token != '!'))
                {
                    // операцию из стека в опз
                    tokens[token_index++] = stack[--stack_index];
                }
                else
                {
                    // остальное оставляем в стеке
                    break;
                }
            }

            // кладем новую операцию в стек
            stack[stack_index++] = current_token;
        }
    }

    // после чтения фомулы достаем, то что осталось в стеке
    while (stack_index>0) 
    {
            tokens[token_index++] = stack[--stack_index];

    }
    tokens[token_index] = '\0';

    free(stack);
    return 0;
}

int calculate(char *str, int size, int len)
{
    // вычисляет готовую опз
    char *stack = malloc(len); int ind=0;

    if (stack == NULL)
    {
        printf("Не удалось выделить память для стека.");
        return -1;
    }

    // перебираем все символы в опз
    for (int i=0;i<size;i++)
    {
        // если встретили букву или цифру
        if (isdigit((unsigned char)str[i]))
            stack[ind++] = str[i];
        // если отрицание
        else if (str[i] == '!')
        {
            stack[ind-1] = inverse(stack[ind-1]);
        }
        // иначе бинарный оператор
        else 
        {
            // снимаем правый операнд
            int op2 = stack[--ind] - '0';
            // снимаем левый операнд
            int op1 = stack[--ind]-'0';
            // вычисляем операцию над двумя числами
            int res = apply_binary(op1,op2,str[i]);
            stack[ind++] = res+ '0';
        }
    } 
    // в стеке остался результат функции
    int rez = stack[0] - '0';
    free(stack);
    return rez;
}
int result(char *str, int *vals, int len)
{
    char *substr = malloc(len); int ind=0;

    if (substr == NULL)
    {
        printf("Не удалось выделить память для вспомогательной строки.");
        return -1;
    }

    // перебираем исходную опз
    for (int i=0; str[i]!='\0';i++)
    {
        // если встретили переменную
        if (isalpha((unsigned char)str[i])) {
            // по букве находим ее значение
            substr[ind++] = vals[str[i] - 'a'] + '0';
        }
        else 
        // операторы и константы без изменений
            substr[ind++] = str[i];
    }
    // вычисляем опз с подставленными значениями
    int rez = calculate(substr,ind, len);
    free(substr);
    return rez;

} 
void generate(int *vals, int cnt, int num, char *vars) 
{
    // генерирует набор знаений для переменных

    // перебираем числа -> их двоичные разряды
    for (int i=0; i<cnt; i++)
    {
        // сдвигаем номер вправо на i разрядов
        // и получаем младший бит
        int bit = (num >> i) & 1;
        // записываем бит в значение соответствующей переменной
        vals[vars[cnt-1-i] - 'a'] = bit;
    } return ;
}
    