#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include "bazis.h"

int len(char *str)
{
    // количество символов в строке
    int cnt = 0;
    while (str[cnt] != '\0')
    {
        cnt++;
    }
    return cnt;
}
int len_bas(char *str)
{
    // расчитываем примерную длину базиса
    int *stack = malloc(len(str) * sizeof(int)); int ind = 0;

    if (stack == NULL)
        return -1;

    // берем максимальную длину, которая поместится в int
    const int max_len = INT_MAX - 1;

    // перебираем символы опз  
    for (int i=0; str[i] != '\0'; i++)
    {
        // если переменная/константа
        if (isalpha((unsigned char)str[i]))
            stack[ind++] = 1; // она занимает 1 символ

        // если отрицание
        else if(str[i] == '!') 
        {
            // проверяем на переполнение
            if (stack[ind-1]*2 > (max_len-3) / 2)
            {
                printf("Длина базиса выходит за пределы int.");
                return -1;
            }
            // аргумент повторяется дважды + скобки + операция
            stack[ind-1] = 2 * stack[ind-1] + 3;
        }

        // если бинарная операция
        else
        {
            // проверяем на переполнение
            int limit = (max_len - 9) /2; // лимит суммы аргументов

            if (stack[ind-2] > limit || stack[ind-1] > limit- stack[ind-2])
            {
                printf("Длина базиса выходит за пределы int.");
                return -1;
            }
            // каждый аргумент повторится дважды
            // ещё девять символов: шесть скобок и три операции
            stack[ind - 2] = stack[ind-2] * 2 + stack[ind-1]*2 + 9;

            // вместо двух длин в стеке теперь одна
            ind--;
        }
    }
    // после обработки всей ОПЗ осталась длина итогового выражения
    int rez = stack[0];
    free(stack);
    return rez;
}

int current_str(char * stack, char *str, int ind)
{
    // копирует последнее выражение из стека задом наперед

    // одиночная переменная
    if (stack[ind] != ')')
    {
        str[0] = stack[ind];
        return 1;
    }

    int i=0;
    // количество закрывающих скобок
    int staples = 1;
    // копируем последнюю ')' и сдвигаемся влево по стеку
    str[i++] = stack[ind--];

    // пока закрыты (нашли пару) не все закрывающиеся скобки
    while (staples)
    {
        // берём очередной символ справа налево
        char current = stack[ind--];
        str[i++] = current;

        // одна закрывающая скобка получила пару
        if (current == '(')
            staples--;

        // встретили закрывающую скобку
        else if (current == ')')
            staples++;
    }

    str[i] = '\0';
    return i;
}

void print_bazis(char *bazis2)
{
    // печать базиса
    for (int i=0; bazis2[i] != '\0'; i++)
    {
        printf("%c", bazis2[i]);
    }
    printf("\n");
}

int check_operators(char *str)
{
    // проверяем подходит ли исходная опз для прямого преобразования
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

int bazis_2(char *bazis, char *bazis2)
{
    // строим базис через стрелку пирса
    int size_bas = len_bas(bazis);
    if (size_bas == -1)
    {
        printf("Не удалось корректно посчитать длину базиса.");
        return -1;
    }
    char *stack = malloc(size_bas+1);
    if (stack == NULL)
    {
        printf("Ошибка выделения памяти: не удалось выделить память для стека.");
        return -1;
    }
    int ind = 0;

    // идем по опз
    for (int j=0; bazis[j] != '\0'; j++)
    {
        char current_el = bazis[j]; // текущий символ

        // переменную добавляем в стек
        if (isalpha((unsigned char)current_el))
            stack[ind++] = current_el;

        // если отрицание
        else if (current_el == '!')
        {
            // берем последний аргумент
            char *sub_str = malloc(size_bas+1);
            if (sub_str == NULL)
            {
                printf("Ошибка выделения памяти.");
                return -1;
            }
            // длина аргумента
            int size;
            size = current_str(stack, sub_str, ind-1); // берем сам аргумент

            // убираем его из используемой части стека
            ind = ind - size;

            // записываем полученное новое выражение для отрицания
            stack[ind++] = '(';
            for (int i=size-1; i>=0; i--) // идем с конца тк аргумент получем в обратном порядке
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
        // если |
        else if (current_el == '|')
        {
            // для двух аргументов
            char *sub_str1 = malloc(size_bas+1);
            char *sub_str2 = malloc(size_bas+1);
            if (sub_str1 == NULL)
            {
                printf("Ошибка выделения памяти.");
                return -1;
            }
            if (sub_str2 == NULL)
            {
                printf("Ошибка выделения памяти.");
                return -1;
            }
            // размеры аргументов
            int size1, size2;
            size1 = current_str(stack, sub_str1, ind-1);
            size2 = current_str(stack, sub_str2, ind-size1-1);
            // сдвигаем индекс стека
            ind = ind - size1 - size2 ;

            // записываем новое выражение
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

        // если &
        else if (current_el == '&')
        {
            // для двух аргументов
            char *sub_str1 = malloc(size_bas+1);
            char *sub_str2 = malloc(size_bas+1);
            if (sub_str1 == NULL)
            {
                printf("Ошибка выделения памяти.");
                return -1;
            }
            if (sub_str2 == NULL)
            {
                printf("Ошибка выделения памяти.");
                return -1;
            }
            // размеры аргументов
            int size1, size2;
            size1 = current_str(stack, sub_str1, ind-1);
            size2 = current_str(stack, sub_str2, ind-size1-1);
            // сдвигаем индекс стека
            ind = ind - size1 - size2 ;


            // записываем новое выражение
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

    // копируем готовое выражение из стека в массив
    for (int i=0; i<ind; i++)
    {
        bazis2[i] = stack[i];
    }
    bazis2[ind] = '\0';
    free(stack);
    return 0;
}

int bazis_3(char *bazis, char *bazis3)
{
    // строим базис через штрих Шеффера

    int size_bas = len_bas(bazis);
    if (size_bas == -1)
    {
        printf("Не удалось корректно посчитать длину базиса.");
        return -1;
    }
    char *stack = malloc(size_bas+1);
    if (stack == NULL)
    {
        printf("Ошибка выделения памяти: не удалось выделить память для стека.");
        return -1;
    }
    int ind = 0;

    // идем по опз
    for (int j=0; bazis[j] != '\0'; j++)
    {
        char current_el = bazis[j];

        // переменную добавляем в стек
        if (isalpha(current_el))
            stack[ind++] = current_el;

        // если отрицание
        else if (current_el == '!')
        {
            // берем последний аргумент
            char *sub_str = malloc(size_bas+1);
            if (sub_str == NULL)
            {
                printf("Ошибка выделения памяти.");
                return -1;
            }
            // длина аргумента
            int size;
            size = current_str(stack, sub_str, ind-1); // получаем аргумент
            // сдвигаем индекс стека на рабочую часть
            ind = ind - size;

            // записываем новое выражение
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
        // если &
        else if (current_el == '&')
        {
            // для двух аругментов
            char *sub_str1 = malloc(size_bas+1);
            char *sub_str2 = malloc(size_bas+1);
            if (sub_str1 == NULL)
            {
                printf("Ошибка выделения памяти.");
                return -1;
            }
            if (sub_str2 == NULL)
            {
                printf("Ошибка выделения памяти.");
                return -1;
            }

            // размеры аргументов
            int size1, size2;
            // получаем аргументы + размеры
            size1 = current_str(stack, sub_str1, ind-1);
            size2 = current_str(stack, sub_str2, ind-size1-1);
            // сдвигаем индекс стека
            ind = ind - size1 - size2 ;

            // записываем новое выражение
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
        // если |
        else if (current_el == '|')
        {
            // для двух аргументов
            char *sub_str1 = malloc(size_bas+1);
            char *sub_str2 = malloc(size_bas+1);
            if (sub_str1 == NULL)
            {
                printf("Ошибка выделения памяти.");
                return -1;
            }
            if (sub_str2 == NULL)
            {
                printf("Ошибка выделения памяти.");
                return -1;
            }
            // размеры аргументов
            int size1, size2;
            // получаем аргументы и их размеры
            size1 = current_str(stack, sub_str1, ind-1);
            size2 = current_str(stack, sub_str2, ind-size1-1);
            // сдвигаем индекс стека
            ind = ind - size1 - size2 ;


            // записываем выражение
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

    // копируем готовое выражение из стека в массив
    for (int i=0; i<ind; i++)
    {
        bazis3[i] = stack[i];
    }
    bazis3[ind] = '\0';
    free(stack);
    return 0;
}


void get_rez(int *mas, int **table, int rows, int columns)
{
    // сохраняем в массив столбец результата выражения
    for (int i=0; i<rows; i++)
    {
        mas[i] = table[i][columns];
    }
}

void triangel_Zegalkin(int *mas, int size, int *coeff, int ind)
{
    // треугольник Жегалкина

    coeff[ind] = mas[0]; // первый элемент текущей строки становится коэффициентом
    
    // если остался один элемент - треугольник построен
    if (size == 1)
        return;

    // перебираем соседние пары
    for (int i=0; i<size-1; i++)
    {
        // записываем XOR соседей на место левого элемента
        mas[i] = mas[i] ^ mas[i+1];
    }

    // рассматриваем следующую строку по тому же принципу
    triangel_Zegalkin(mas, size-1, coeff, ind+1);
}

void bazis_4(int *coeff, int size, char *bazis4, int *ind, char *vars, int cnt_vars)
{
    // строим полином Жегалкина

    // перебираем коэфиценты
    for(int i=0; i<size; i++)
    {
        // добавляем только слагаемые с коэффициентом 1
        if (coeff[i] == 1)
        {
            // если уже есть слагаемое, добавляем XOR перед следующим
            if ((*ind) > 0)
                bazis4[(*ind)++] = '^';

            // коэффициент с индексом 0 отвечает за константу 1
            if (i == 0)
                bazis4[(*ind)++] = '1';

            else
            {
                // проверяем, какие переменные входят в слагаемое
                for (int j=0; j<cnt_vars; j++)
                {
                    // маска с единицей в разряде переменной j
                    int bit = 1 << (cnt_vars -1 -j);

                    // если этот разряд номера коэффициента равен 1
                    if (i & bit)
                    {
                        // если перед этим записали переменную
                        // то соединим её со текушей
                        if ((*ind) > 0 && isalpha(bazis4[(*ind)-1]))
                            bazis4[(*ind)++] = '&';

                        // записываем переменную
                        bazis4[(*ind)++] = vars[j];
                    }
                }
            }
        }
    }
    bazis4[*ind] = '\0';
}