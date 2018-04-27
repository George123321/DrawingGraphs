#include <stdio.h>
#include <malloc.h>
#include "Stack.h"
#include <string.h>
#include <ctype.h>

#define inaccuracy 1e-8

void doMath(char *elem, struct Stack *st) {
    char binary_operations[5][2] = {"+", "-", "*", "/", "^"};
    char unar_operations[7][6] = {"sqrt", "sin", "cos", "tan", "ln", "log10", "exp"};

    // посмотрим, какая операция
    int type_of_binary_operation = -1;
    int type_of_unar_operation = -1;
    for (int i = 0; i < 5; i++) {
        if (strcmp(elem, binary_operations[i]) == 0) {
            type_of_binary_operation = i;
            break;
        }
    }
    if (type_of_binary_operation != -1) {
        if (st->size <= 1) {
            printf("The binary operator is applied to too few numbers (requires 2).\n");
            stack_push(st, NAN);
            return;
        }
        double x = stack_pop(st); // DONE: если достали NAN (пустой стек)
        double y = stack_pop(st);

        switch (type_of_binary_operation) {
            case 0:
                stack_push(st, y + x);
                break;
            case 1:
                stack_push(st, y - x);
                break;
            case 2:
                stack_push(st, y * x);
                break;
            case 3:
                if (x == 0) {
                    stack_push(st, NAN);
                    printf("Division by zero\n");
                    return;
                }
                stack_push(st, y / x);
                break;
            case 4:
                if (y <= 0) {
                    if (x != round(x)) { // если x не целый DONE: как проверить, что double целый
                        stack_push(st, NAN);
                        printf("Pow Error\n");
                        return;
                    }
                }
                stack_push(st, pow(y, x));
                break;
            default:
                break;
        }
    } else { // тогда ищем унарную операцию
        for (int i = 0; i < 7; i++) {
            if (strcmp(elem, unar_operations[i]) == 0) {
                type_of_unar_operation = i;
                break;
            }
        }
        if (type_of_unar_operation != -1) {
            if (st->size == 0) {
                printf("The unar operator is applied to too few numbers (requires 1).\n");
                stack_push(st, NAN);
                return;
            }
            double x = stack_pop(st);
            switch (type_of_unar_operation) {
                case 0:
                    if (x < 0) {
                        stack_push(st, NAN);
                        printf("Pow Error\n");
                        return;
                    }
                    stack_push(st, pow(x, 0.5));
                    break;
                case 1:
                    stack_push(st, sin(x));
                    break;
                case 2:
                    stack_push(st, cos(x));
                    break;
                case 3:
                    if (cos(x) == 0) {
                        stack_push(st, NAN);
                        printf("Tan Error\n");
                        return;
                    }
                    stack_push(st, tan(x));
                    break;
                case 4:
                    if (x <= 0) {
                        stack_push(st, NAN);
                        printf("LOG Error\n");
                        return;
                    }
                    stack_push(st, log(x));
                    break;
                case 5:
                    if (x <= 0) {
                        stack_push(st, NAN);
                        printf("LOG Error\n");
                        return;
                    }
                    stack_push(st, log10(x));
                    break;
                case 6:
                    stack_push(st, exp(x));
                    break;
                default:
                    break;
            }
        }
    }
    if (type_of_unar_operation == -1 && type_of_binary_operation == -1) {
        printf("There is no such operation: \"%s\".", elem);
        stack_push(st, NAN);
        exit(0);
    }
}

int word_is_number(char *word) {
    int number_of_points = 0;
    // проверим первый символ
    if (word[0] == '.') {
        number_of_points++;
    }
    if (!(isdigit(word[0]) || word[0] == '.' || word[0] == '+' || word[0] == '-')) {
        return 0;
    } else {
        if (strcmp(word, ".") == 0 || strcmp(word, "+.") == 0 || strcmp(word, "-.") == 0 || strcmp(word, "+") == 0 ||
            strcmp(word, "-") == 0) { // на самом деле, костыль
            return 0;
        }
        for (int i = 1; i < strlen(word); i++) {
            if (!(isdigit(word[i]) || word[i] == '.')) {
                return 0;
            } else {
                if (word[i] == '.') {
                    number_of_points++;
                }
            }
        }
        if (number_of_points > 1) {
            return 0;
        } else {
            return 1;
        }
    }
}

void add_to_stack(char *elem, struct Stack *st, double x) {
    if (strcmp(elem, "x") == 0) {
        stack_push(st, x);
    } else if (word_is_number(elem)) {
        double d = strtod(elem, NULL); // DONE: 10sqrt - исправить проверку на число. Жесткая проверка на число, или x
        stack_push(st, d);
    } else {
        doMath(elem, st);
    }
}

char **read_line(int *length) {
    char **words = (char **) calloc(1, sizeof(char *)); //исходный размер массива

    int i = 0;
    int j = 0;
    char c; //заготовка под чтение символов

    words[i] = (char *) calloc(1, sizeof(char)); //исходный размер слова

    printf("Enter the expression:\n");
    while ((c = getchar()) != '\n') //считываем посимвольно пока не наткнемся на конец строки
    {
        if (c != ' ') { //если это не пробел, то
            words[i] = (char *) realloc(words[i],
                                        (j + 1) * sizeof(char)); //увеличиваем размер текущего слова word[i] на j+1
            words[i][j] = c;//присваиваем текущему элементу слова значение считанного символа
            j++;//увеличиваем счетчик на 1
        } else {//иначе,
            words[i][j] = '\0';//присваиваем последнему элементу в слове символ конца слова
            j = 0;//обнуляем счетчик элементов в слове
            i++;//увеличиваем счетчик слов на 1
            words = (char **) realloc(words, (i + 1) * sizeof(char *)); //увеличиваем размер массива слов на 1
            words[i] = (char *) calloc(1, sizeof(char));
        }
    }
    words[i][j] = '\0';
    *length = i + 1;

    return words;
}

double calculate(int length, char **expression, double x) {
    struct Stack s;
    stack_init(&s);

    for (int elem = 0; elem < length; elem++) {
        add_to_stack(expression[elem], &s, x);
        if (s.head->x != s.head->x) {
            break;
        }
    }
    double to_return = stack_pop(&s);
    if (!is_empty(&s)) {
        printf("Something wrong.\n");
        exit(0);
    }
    return to_return;
}

double calculate_postfix_variable(int length, char **expression, double x) {
    /*
    char **expression_copy = (char **) malloc(length * sizeof(char *));
    if(expression_copy == NULL) {
        printf("Malloc error in function \"calculate_postfix_variable\"\n");
        exit(0);
    }
    for (int i = 0; i < length; i++) {
        expression_copy[i] = (char *) malloc(sizeof(expression[i]));
        if(expression_copy[i] == NULL) {
            printf("Malloc error in function \"calculate_postfix_variable\"\n");
            exit(0);
        }
        strcpy(expression_copy[i], expression[i]);
    }

    for (int i = 0; i < length; i++) {
        if (strcmp(expression_copy[i], "x") == 0) {
            sprintf(expression_copy[i], "%lf", x); // DONE: передавать x в стек сразу
        }
    }*/

    double ans = calculate(length, expression, x);
    /*
    for (int i = 0; i < length; i++) {
        char *ptr = expression_copy[i];
        free(ptr);
    }
    free(expression_copy);*/

    return ans;
}

double calculate_postfix(int length, char **expression) {
    double x = 0;
    for (int i = 0; i < length; i++) {
        if (strcmp(expression[i], "x") == 0) {
            printf("Variable found. Enter its value.\n");
            scanf("%lf", &x);
            return calculate_postfix_variable(length, expression, x);
        }
    }
    return calculate(length, expression, x);
}

double *linspace(double x_left, double x_right, int num) { // TODO: вводить количество точек
    double *x = malloc((num + 1) * sizeof(double));
    if (x == NULL) {
        printf("Malloc error in function \"inspace\"\n");
        exit(0);
    }

    for (int i = 0; i <= num; i++) {
        x[i] = x_left + ((x_right - x_left) / num) * i;
    }
    return x;
}

double *func_array(int length, char **func, double *x, int num) {
    double *y = malloc((num + 1) * sizeof(double));
    if (y == NULL) {
        printf("Malloc error in function \"func_array\"\n"); // DONE: написать название функции, из которой программа вылетела
        exit(0);
    }

    for (int i = 0; i <= num; i++) {
        y[i] = calculate_postfix_variable(length, func, x[i]);
    }
    return y;
}

void write_ans_file(double *x, double *y, int num) {
    FILE *f = fopen("C:/Users/George/Desktop/git_projects/C_plus_plus/DV_HW/1_c_ii/Output/data.csv", "w");
    if (f == NULL) {
        printf("Error opening file!\n");
    } else {
        fprintf(f, "x,y\n");
        for (int i = 0; i <= num; i++) {
            if (y[i] != y[i]) { // это значит, если NAN
                fprintf(f, "%lf,%s\n", x[i], "NaN");
            } else {
                fprintf(f, "%lf,%lf\n", x[i], y[i]);
            }
        }
    }
}

int equel(double x, double y) {
    return fabs(x - y) < inaccuracy;
}

int main() {
    int length = 0;
    char **expression = read_line(&length);

    int num = 1000;

    double *x = linspace(-15, 15, num);
    double *y = func_array(length, expression, x, num);

    write_ans_file(x, y, num);

    free(x);
    free(y);

    for (int i = 0; i < length; i++) {
        char *expr_ptr = expression[i];
        free(expr_ptr);
    }
    free(expression);
    //printf("%lf\n", calculate_postfix(length, expression));
    return 0;
}