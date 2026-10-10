#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "logic.h"
#include "normal_forms.h"


int main(int argc, char *argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    if (argc != 2)
    {
        printf("Использование: %s <путь_к_файлу>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL)
    {
        printf("Не удалось открыть файл: %s\n", argv[1]);
        perror("fopen");
        return 1;
    }

    // нужно узнать сколько символов в файле 
    // для этого ставим указатель в файле на конец
    fseek(file, 0, SEEK_END);
    // узнаем количество символов (байт)
    long size_file = ftell(file);
    fseek(file, 0, SEEK_SET);

    // выделяем память для чтения строки
    char *string = malloc(size_file+1);

    if (string == NULL )
    {
        printf("Не удалось выделить память для чтения строки из файла.");
        return 1;
    }
    if (!read_string(file, string, size_file + 1))
    {
        printf("Не удалось считать строку\n");
        fclose(file);
        return 1;
    }

    char *tokens= malloc(size_file+1), *sub_string = malloc(size_file+1);

    if (tokens == NULL )
    {
        printf("Не удалось выделить память для обработки токенов.");
        return 1;
    }
    if (sub_string == NULL )
    {
        printf("Не удалось выделить память для дополнительной строки.");
        return 1;
    }
    to_lower(string);

    int len_sstr = remove_spaces(string,sub_string); 
    free(string);
    char variables[VAR_COUNT]; 
    int count_vars=count_variables(sub_string, variables, len_sstr);

    qsort(variables, count_vars, sizeof(char), compare);
    to_rpn(sub_string, tokens, size_file);
    free(sub_string);

    int vals[VAR_COUNT];
    int combinations = 1 << count_vars;
    const int rows = combinations, cols = count_vars+1;

    int **table = malloc(rows * sizeof(int *));
    
    if (table == NULL )
    {
        printf("Не удалось выделить память для таблицы истинности.");
        return 1;
    }

    for (int i = 0; i < rows; i++)
    {
        table[i] = malloc(cols * sizeof(int));

        if (table[i] == NULL )
        {
            printf("Не удалось выделить память для таблицы истинности.");
            return 1;
        }
    }

    for (int i=0; i<count_vars;i++)
    {
        printf("%c ", variables[i]);
    } printf("F\n");
    
    for (int i = 0; i < combinations; i++)
    {
        generate(vals, count_vars, i, variables);

        for (int j = 0; j < count_vars; j++)
        {
            int value = vals[variables[j] - 'a'];
            table[i][j] = value;
            printf("%d ", value);
        }

        char res = result(tokens, vals, size_file);
        table[i][count_vars] = res - '0';
        printf("%c\n", res);
    }
    
    free(tokens);

    char *scnf = malloc(rows * (3 * count_vars + 2)+1);
    char *sdnf = malloc(rows * (3 * count_vars + 2)+1);
    
    if (scnf == NULL || sdnf == NULL)
    {
        printf("Не удалось выделить память для СКНФ/СДНФ.");
        return 1;
    }

    build_scnf(table, combinations, count_vars, variables, scnf);
    build_sdnf(table, combinations, count_vars, variables, sdnf);

    print_scnf(scnf);
    print_sdnf(sdnf);

    free(scnf);
    free(sdnf);

    for (int i = 0; i < rows; i++)
    {
        free(table[i]);
    }
    free(table);

    fclose(file);
    return 0;
}
