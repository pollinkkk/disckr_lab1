#include <stdio.h>
#include "dummy_variables.h"

void dum_vars(int **table, int rows, int columns, char *dumm_vars, int count_vars, char* vars)
{
    int ind = 0;
    // перебираем переменные
    for (int i=0; i<columns; i++)
    {
        // расстояния между строками в которых отличается 
        // только i-ая переменная 
        int step = 1 << (count_vars - 1 - i);
        int j=0; // перебираем строки с первой
        int flag = 1; // изначально считаем переемнную фиктивной

        // пока строка для сравнения не выходит за пределы таблицы
        while (j+step<rows)
        {
            // выбираем только строки где i-ая переменная = 0
            if ((j & step)  == 0) 
            {
                // сравниваем результаты функций на двух наборах
                // в одном переемнная 0, в другом 1
                // а остальные переменные одинаковы
                if (table[j][count_vars] != table[j+step][count_vars]) 
                {
                    // не фиктивна
                    flag = 0;
                    break;
                }
            }
            j++; // следующая строка
        }

        // если флаг для переменной так и не изменился - она фиктивна
        if (flag)
        {
            dumm_vars[ind++] = vars[i];
        }
    }
    dumm_vars[ind] = '\0';
}
void print_dum_vars(char* vars)
{
    // выводим фиктивные переменные

    // если строка пустая
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