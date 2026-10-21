#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "logic.h"
#include "normal_forms.h"
#include "dummy_variables.h"
#include "bazis.h"


int main(int argc, char *argv[])
{
    SetConsoleOutputCP(CP_UTF8); // для вывода кириллицы

     // если аргументов не два: имя программы и путь к файлу
    if (argc != 2)
    {
        printf("Использование: %s <путь_к_файлу>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    // если открыть файл не получилось
    if (file == NULL)
    {
        printf("Не удалось открыть файл: %s\n", argv[1]);
        perror("fopen");  // выводим системное описание ошибки с подписью fopen
        return 1;
    }

    // нужно узнать сколько символов в файле 
    // пытаемся перейти в конец файла, если не получилось
    // то обрабатываем ошибку
    if (fseek(file, 0, SEEK_END) != 0)
    {
        perror("Не удалось перейти в конец файла.");
        fclose(file);
        return 1;
    }
    // узнаем количество символов (байт)
    long size_file = ftell(file);

    // если ftell не смог определить позицию
    if (size_file == -1L)
    {
        perror("Не удалось определить позицию в файле.");
        fclose(file);
        return 1;
    }

    // пытаемся вернуться в начало файла
    if (fseek(file, 0, SEEK_SET) != 0)
    {
        perror("Не удалось перейти в начало файла");
        fclose(file);
        return 1;
    }

    // выделяем память для чтения строки
    char *string = malloc(size_file+1);

    // не удалось выделить память для строки
    if (string == NULL )
    {
        perror("Не удалось выделить память для чтения строки из файла.");
        fclose(file);
        return 1;
    }

    // пытаемся прочитать строку из файла в string
    if (!read_string(file, string, size_file + 1))
    {
        printf("Не удалось считать строку\n");
        fclose(file);
        return 1;
    }

    // выделяем память сразу для двух строк
    // tokens — формула в обратной польской записи
    // sub_string — формула без пробельных символов
    char *tokens= malloc(size_file+1), *sub_string = malloc(size_file+1);

    // проверяем выделение памяти
    if (tokens == NULL )
    {
        perror("Не удалось выделить память для обработки токенов.");
        return 1;
    }
    if (sub_string == NULL )
    {
        perror("Не удалось выделить память для дополнительной строки.");
        return 1;
    }

    // переводим буквы исходной строки в нижний регистр
    to_lower(string);

    // копируем формулу из string в sub_string без пробельных символов
    // сохраняем ее длину
    int len_sstr = remove_spaces(string,sub_string); 
    free(string);

    // создаём массив для имён переменных
    char variables[VAR_COUNT]; 

    // находим разные переменные в формуле и записываем их в variables
    // количество разных переменных сохраняем в count_vars
    int count_vars=count_variables(sub_string, variables, len_sstr);

    // сортируем найденные переменные по алфавиту
    qsort(variables, count_vars, sizeof(char), compare);

    // перевод в опз
    if (to_rpn(sub_string, tokens, size_file))
    {
        return 1;
    }
    free(sub_string);

    int vals[VAR_COUNT]; // массив значений переменных для текущего набора
    int combinations = 1 << count_vars; // считаем число наборов значений
    const int rows = combinations, cols = count_vars+1; // кол-во строк и столбцов соответственно

    // выделяем массив указателей на строки таблицы истинности
    int **table = malloc(rows * sizeof(int *));
    
    if (table == NULL )
    {
        perror("Не удалось выделить память для таблицы истинности.");
        return 1;
    }

    for (int i = 0; i < rows; i++)
    {
        table[i] = malloc(cols * sizeof(int));

        if (table[i] == NULL )
        {
            perror("Не удалось выделить память для таблицы истинности.");
            return 1;
        }
    }

    // выводим имена найденных переменных
    for (int i=0; i<count_vars; i++)
    {
        printf("%c ", variables[i]);
    } 
    printf("F\n");
    
    // перебираем все возможные наборы значений переменных
    for (int i = 0; i < combinations; i++)
    {
        // получаем набор с номером i и записываем значения в vals
        generate(vals, count_vars, i, variables);

        // перебираем переменные текущего набора
        for (int j = 0; j < count_vars; j++)
        {
            // находим значение переменной по её букве
            int value = vals[variables[j] - 'a'];
            table[i][j] = value;
            printf("%d ", value);
        }

        // вычисляем формулу в ОПЗ для текущего набора
        int res = result(tokens, vals, size_file);
        if (res == -1)
        {
            return 1;
        }
        table[i][count_vars] = res;
        printf("%d\n", res);
    }

    // максимальный размер одной скобки
    int block_len = 3*count_vars + 2;
    // проверяем переполение
    // rows * block_len + 1 ≤ INT_MAX =>
    if (rows > (INT_MAX-1)/block_len)
    {
        printf("Таблица слишком большая для построения нормальных форм.");
        return 1;
    }
    char *scnf = malloc(rows * block_len + 1);
    char *sdnf = malloc(rows * block_len + 1);
    
    if (scnf == NULL || sdnf == NULL)
    {
        perror("Не удалось выделить память для СКНФ/СДНФ.");
        return 1;
    }
    // строим СКНФ 
    build_scnf(table, combinations, count_vars, variables, scnf, count_vars);
     // строим СДНФ 
    build_sdnf(table, combinations, count_vars, variables, sdnf, count_vars);

    printf("СКНФ:\n");
    print_scnf(scnf);
    printf("СДНФ:\n");
    print_sdnf(sdnf);

    // создаём массив для имён фиктивных переменных
    char dumm_vars[VAR_COUNT];

    // ищем фиктивные переменные
    dum_vars(table, combinations, count_vars, dumm_vars, count_vars, variables);
    print_dum_vars(dumm_vars);

    // объявляем указатель на строку под базис
    char *basis;
    printf("Запись через базис {&, |, !}:\n");
    if (sdnf[0] != '\0')
    {
        // если сднф не пустая выводим ее
        // и она будет строкой для базса
        print_sdnf(sdnf);
        basis = sdnf;
        
    }
    else
    {
        // если сднф пустая выводим скнф
        // и она будет строкой для базса
        print_scnf(scnf);
        basis = scnf;
    }

    // отдельно обрабатываем формулу без переменных
    if (count_vars == 0)
    {
        printf("Запись через базис стрелки Пирса {-}: \n");
        printf("Записи через базис нет.\n");
        printf("Запись через базис штриха Шеффера {#}: \n");
        printf("Записи через базис нет.\n");
        printf("Запись через полином Жегалкина {^, &, 1}: \n");
        printf("%c\n", basis[0]);
        fclose(file);
        return 0;
    }

    // проверяем, можно ли преобразовать исходную формулу напрямую
    if (check_operators(tokens))
    {
        int len_bas_toks = len_bas(tokens);
        if (len_bas_toks == -1)
        {
            printf("Не удалось корректно посчитать длину базиса");
            return 1;
        }
        char *basis2 = malloc(len_bas_toks+1);
        if (basis2 == NULL)
        {
            perror("Ошибка выделения памяти: для второго базиса.");
            return 1;
        }
        // преобразуем исходную ОПЗ в выражение через стрелку Пирса
        if (bazis_2(tokens, basis2) == -1)
        {
            return 1;
        }
        printf("Запись через базис стрелки Пирса {-}: \n");
        print_bazis(basis2);
        free(basis2);

        char *basis3 = malloc(len_bas_toks+1);
        if (basis3 == NULL)
        {
            perror("Ошибка выделения памяти: для третьего базиса.");
            return 1;
        }
        // преобразуем исходную ОПЗ в выражение через штрих Шеффера
        if (bazis_3(tokens, basis3) == -1)
        {
            return 1;
        };
        printf("Запись через базис штриха Шеффера {#}: \n");
        print_bazis(basis3);
        free(basis3);
    }
    else 
    {
        // если исходная формула не подходит для прямого преобразования
        // берём выбранную выше СДНФ или СКНФ
        char *rpn_basis = malloc(len(basis)+1);
        if (rpn_basis == NULL)
        {
            perror("Ошибка выделения памяти: для опз базиса.");
            return 1;
        }
        // переводим выбранную СДНФ или СКНФ в ОПЗ
        if (to_rpn(basis, rpn_basis, len(basis)))
        {
            return 1;
        }

        int len_rpn_bas = len_bas(rpn_basis);
        if (len_rpn_bas == -1)
        {
            printf("Не удалось корректно посчитать длину базиса");
            return 1;
        }

        char *basis2 = malloc(len_rpn_bas+1);
        if (basis2 == NULL)
        {
            perror("Ошибка выделения памяти: для второго базиса.");
            return 1;
        }
        // преобразуем ОПЗ нормальной формы в запись через стрелку Пирса
        if (bazis_2(rpn_basis, basis2) == -1)
        {
            return 1;
        }
        printf("Запись через базис стрелки Пирса {-}: \n");
        print_bazis(basis2);
        free(basis2);

        char *basis3 = malloc(len_rpn_bas+1);
        if (basis3 == NULL)
        {
            perror("Ошибка выделения памяти: для третьего базиса.");
            return 1;
        }
        // преобразуем ОПЗ нормальной формы в запись через штрих Шеффера
        if (bazis_3(rpn_basis, basis3))
        {
            return 1;
        }
        printf("Запись через базис штриха Шеффера {#}: \n");
        print_bazis(basis3);
        free(basis3);
        free(rpn_basis);

    }

    free(tokens);
    free(scnf);
    free(sdnf);

    // выделяем массив для последнего столбца таблицы истинности
    int *rez_f = malloc(combinations * sizeof(int));
    if (rez_f == NULL)
    {
        perror("Ошибка выделения памяти: для столбца результатов.");
        return 1;
    }
    // копируем результаты из последнего столбца таблицы в rez_f
    get_rez(rez_f, table, combinations, count_vars);
    int *coeff = malloc(combinations * sizeof(int));
    if (coeff == NULL)
    {
        perror("Ошибка выделения памяти: для коэффицентов.");
        return 1;
    }

    // вычисляем коэффициенты методом треугольника
    triangel_Zegalkin(rez_f, combinations, coeff, 0);

    // индекс позиции, куда дальше записывать символ полинома
    int ind = 0;

    // проверяем переполение
    // count_vars * combinations + 2 <= INT_MAX =>
    if (combinations > (INT_MAX - 2) / count_vars)
    {

        printf("Переполнение для полинома Жегалкина.");
        return 1;

    }
    // выделяем память для строки полинома Жегалкина
    char *basis4 = malloc(count_vars * combinations + 2);

    if (basis4 == NULL)
    {
        perror("Ошибка выделения памяти: для полинома Жегалкина.");
        return 1;
    }
    // собираем строку полинома по коэффициентам
    bazis_4(coeff, combinations, basis4, &ind, variables, count_vars);
    printf("Запись через полином Жегалкина {^, &, 1}: \n");

    // если получилась непустая строка
    if (basis4[0] != '\0')
        // выводим
        print_bazis(basis4);
    else
        // все коэффициенты нулевые, поэтому функция равна 0
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
