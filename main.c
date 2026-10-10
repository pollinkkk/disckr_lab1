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
    char string[MAX_SIZE];

    if (!read_string(file, string, sizeof(string)))
    {
        printf("Не удалось считать строку\n");
        fclose(file);
        return 1;
    }

    char tokens[MAX_SIZE], sub_string[256];
    to_lower(string);
    int len_sstr = remove_spaces(string,sub_string);
    char variables[VAR_COUNT]; 
    int count_vars=count_variables(sub_string, variables, len_sstr);
    qsort(variables, count_vars, sizeof(char), compare);
    to_rpn(sub_string, tokens);
    int vals[MAX_SIZE];
    int combinations = 1 << count_vars;
    const int rows = combinations, cols = count_vars+1;
    int **table = malloc(rows * sizeof(int *));
    
    for (int i = 0; i < rows; i++)
    {
        table[i] = malloc(cols * sizeof(int));
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

        char res = result(tokens, vals);
        table[i][count_vars] = res - '0';
        printf("%c\n", res);
    }
    char scnf[MAX_SIZE_2];
    char sdnf[MAX_SIZE_2];

    build_scnf(table, combinations, count_vars, variables, scnf);
    build_sdnf(table, combinations, count_vars, variables, sdnf);

    print_scnf(scnf);
    print_sdnf(sdnf);

    write_to_file(table, combinations, count_vars);

    for (int i = 0; i < rows; i++)
    {
        free(table[i]);
    }
    free(table);

    fclose(file);
    return 0;
}
