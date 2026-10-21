#include <stdio.h>
#include "normal_forms.h"

void build_scnf(int **table, int rows, int columns, char *vars, char *scnf, int cnt_vars)
{
    // строит скнф

    // если выражение без переменных
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
    int current_ind = 0; // текущий свободный индекс в скнф

    // проходимся по строкам таблицы истинности
    for (int i=0; i<rows; i++)
    {
        // для скнф берем только строки где результат 0
        if (table[i][columns] == 0)
        {
            // если до этого была записана хотя бы одна скобка
            if (current_ind>0)
            {
                // объединяем скобки через &
                scnf[current_ind++] = '&';
                scnf[current_ind++] = '(';
            }
            else
                // иначе открываем первую скобку
                scnf[current_ind++] = '(';

            // перебираем значения переменных в текущей строке
            for (int j=0; j<columns; j++)
            {
                // если переменая == 0 записываем ее без отрицания
                if (table[i][j] == 0)
                    scnf[current_ind++] = vars[j];
                else {
                    // иначе записываем с отрицанием
                    scnf[current_ind++] = '!';
                    scnf[current_ind++] = vars[j];
                }
                // если после переменной есть еще одна
                // то объединяем переменные |
                if (j+1 < columns)
                    scnf[current_ind++] = '|';
            }
            // закрываем
            scnf[current_ind++] = ')';
        }
    }
    scnf[current_ind] = '\0';
}

void build_sdnf(int **table, int rows, int columns, char *vars, char *sdnf, int cnt_vars)
{
    // строим сднф

    // если переменных нет
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

    // идем по строкам таблицы
    for (int i=0; i<rows; i++)
    {
        // берем строки в которых результат 1
        if (table[i][columns] == 1)
        {

            // если до этого были скобки
            if (current_ind>0)
            {
                // объединяем их |
                sdnf[current_ind++] = '|';
                sdnf[current_ind++] = '(';
            }
            else
                // открывем первую скобку
                sdnf[current_ind++] = '(';

            // идем по значениям перемнных
            for (int j=0; j<columns; j++)
            {
                // если значение переменной == 1
                // то записываем без отрицания
                if (table[i][j] == 1)
                    sdnf[current_ind++] = vars[j];
                else {
                    // иначе записываем с отрицанием
                    sdnf[current_ind++] = '!';
                    sdnf[current_ind++] = vars[j];
                }
                // если дальше есть еще переменные, то объеднияем их &
                if (j+1 < columns)
                    sdnf[current_ind++] = '&';
            }
            // закрываем
            sdnf[current_ind++] = ')';
        }
    }
    sdnf[current_ind] = '\0';
}

void print_scnf(char *scnf)
{
    // печатаем скнф
    int i = 0;
    while (scnf[i] != '\0') 
    {
        printf("%c", scnf[i++]);
    }
    if (i==0)
        printf("Не удалось построить СКНФ: функция тождественно истинна.");
    printf("\n");
}

void print_sdnf(char *sdnf)
{
    // печатаем сднф
    int i = 0;
    while (sdnf[i] != '\0') 
    {
        printf("%c", sdnf[i++]);
    }
    if (i==0)
        printf("Не удалось построить СДНФ: функция тождественно ложна.");
    printf("\n");
}