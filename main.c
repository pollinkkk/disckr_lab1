#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "logic.h"
#include "normal_forms.h"
#include "dummy_variables.h"
#include "bazis.h"


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


    char *scnf = malloc(rows * (3 * count_vars + 2)+1);
    char *sdnf = malloc(rows * (3 * count_vars + 2)+1);
    
    if (scnf == NULL || sdnf == NULL)
    {
        printf("Не удалось выделить память для СКНФ/СДНФ.");
        return 1;
    }

    build_scnf(table, combinations, count_vars, variables, scnf, count_vars);
    build_sdnf(table, combinations, count_vars, variables, sdnf, count_vars);

    printf("СКНФ:\n");
    print_scnf(scnf);
    printf("СДНФ:\n");
    print_sdnf(sdnf);

    char dumm_vars[VAR_COUNT];

    dum_vars(table, combinations, count_vars, dumm_vars, count_vars, variables);
    print_dum_vars(dumm_vars);

    char *basis;
    printf("Запись через базис {&, |, !}:\n");
    if (sdnf[0] != '\0')
    {
        print_sdnf(sdnf);
        basis = sdnf;
        
    }
    else
    {
        print_scnf(scnf);
        basis = scnf;
    }

    if (count_vars == 0)
    {
        printf("Запись через базис стрелки Пирса {-}: \n");
        printf("Записи через базис нет.\n");
        printf("Запись через базис штриха Шеффера {#}: \n");
        printf("Записи через базис нет.\n");
        printf("Запись через полином Жегалкина {^, &, 1}: \n");
        printf("%c\n", basis[0]);
        return 0;
    }
    if (check_operators(tokens))
    {
        char *basis2 = malloc(len_bas(tokens)+1);
        if (basis2 == NULL)
        {
            printf("Ошибка выделения памяти: для второго базиса.");
            return 1;
        }
        bazis_2(tokens, basis2);
        printf("Запись через базис стрелки Пирса {-}: \n");
        print_bazis(basis2);
        free(basis2);

        char *basis3 = malloc(len_bas(tokens)+1);
        if (basis3 == NULL)
        {
            printf("Ошибка выделения памяти: для третьего базиса.");
            return 1;
        }
        bazis_3(tokens, basis3);
        printf("Запись через базис штриха Шеффера {#}: \n");
        print_bazis(basis3);
        free(basis3);
    }
    else 
    {
        char *rpn_basis = malloc(len(basis)+1);
        if (rpn_basis == NULL)
        {
            printf("Ошибка выделения памяти: для опз базиса.");
            return 1;
        }
        to_rpn(basis, rpn_basis, len(basis));

        char *basis2 = malloc(len_bas(rpn_basis)+1);
        if (basis2 == NULL)
        {
            printf("Ошибка выделения памяти: для второго базиса.");
            return 1;
        }
        bazis_2(rpn_basis, basis2);
        printf("Запись через базис стрелки Пирса {-}: \n");
        print_bazis(basis2);
        free(basis2);

        char *basis3 = malloc(len_bas(rpn_basis)+1);
        if (basis2 == NULL)
        {
            printf("Ошибка выделения памяти: для третьего базиса.");
            return 1;
        }
        bazis_3(rpn_basis, basis3);
        printf("Запись через базис штриха Шеффера {#}: \n");
        print_bazis(basis3);
        free(basis3);
        free(rpn_basis);

    }

    free(tokens);
    free(scnf);
    free(sdnf);

    int *rez_f = malloc(combinations * sizeof(int));
    if (rez_f == NULL)
    {
        printf("Ошибка выделения памяти: для столбца результатов.");
        return 1;
    }
    get_rez(rez_f, table, combinations, count_vars);
    int *coeff = malloc(combinations * sizeof(int));
    if (coeff == NULL)
    {
        printf("Ошибка выделения памяти: для коэффицентов.");
        return 1;
    }

    triangel_Zegalkin(rez_f, combinations, coeff, 0);

    int ind = 0;
    char *basis4 = malloc(count_vars * (1 << count_vars) + 2);
    bazis_4(coeff, combinations, basis4, &ind, variables, count_vars);
    printf("Запись через полином Жегалкина {^, &, 1}: \n");
    if (basis4[0] != '\0')
        print_bazis(basis4);
    else
        printf("0\n");
    
    free(basis4);
    free(coeff);
    free(rez_f);

    for (int i = 0; i < rows; i++)
    {
        free(table[i]);
    }
    free(table);

    fclose(file);
    return 0;
}
