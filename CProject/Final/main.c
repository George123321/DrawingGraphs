#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include "StackString.h"
#include "StackDouble.h"

#define BUFFER_SIZE 255
#define NUMBER_OF_BINARY 5
#define NUMBER_OF_UNAR 7
#define NUMBER_OF_STACK_CONTAINS 6
typedef struct StackString STACK_S;

char binary_operations[NUMBER_OF_BINARY][2] = {"+", "-", "*", "/", "^"};
char unar_operations[NUMBER_OF_UNAR][6] = {"sqrt", "sin", "cos", "tan", "ln", "log10", "exp"};
char stack_contain[NUMBER_OF_STACK_CONTAINS][6] =
        {"(", "+", "-", "*", "/", "^"};
int priority_stack[NUMBER_OF_STACK_CONTAINS] = {1, 2, 2, 3, 3, 4};

char input[2 * BUFFER_SIZE];
char str[2 * BUFFER_SIZE];
int type_arr[2 * BUFFER_SIZE];
char buffer[BUFFER_SIZE][BUFFER_SIZE];
char postfix[BUFFER_SIZE][BUFFER_SIZE];
int length_buffer = 0;
int length = 0;

int is_good(char *elem) {
    for (int i = 0; i < NUMBER_OF_BINARY; i++) {
        if (strcmp(elem, binary_operations[i]) == 0) {
            return 1;
        }
    }
    for (int i = 0; i < NUMBER_OF_UNAR; i++) {
        if (strcmp(elem, unar_operations[i]) == 0) {
            return 1;
        }
    }
    if (strcmp(elem, "x") == 0 || strcmp(elem, "(") == 0 || strcmp(elem, ")") == 0) {
        return 1;
    }
    return 0;
}

int is_in_one_char_elems(char *elem) {
    for (int i = 0; i < NUMBER_OF_BINARY; i++) {
        if (strcmp(elem, binary_operations[i]) == 0 || strcmp(elem, "(") == 0 || strcmp(elem, ")") == 0) {
            return 1;
        }
    }
    return 0;
}

int is_binary_operation(char *elem) {
    for (int i = 0; i < NUMBER_OF_BINARY; i++) {
        if (strcmp(elem, binary_operations[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_unar_operation(char *elem) {
    for (int i = 0; i < NUMBER_OF_UNAR; i++) {
        if (strcmp(elem, unar_operations[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void read_line() {
    char c;
    input[0] = '(';
    int i = 1;
    while ((c = getchar()) != '\n') {
        input[i] = c;
        i++;
    }
    input[i] = ')';
    // проверим на унарный + -
    int number_of_unar_min_and_plus = 0;
    for (i = 0; i < strlen(input) - 1; i++) {
        if (input[i] == '(' && (input[i + 1] == '+' || input[i + 1] == '-')) {
            str[i + number_of_unar_min_and_plus] = input[i];
            str[i + 1 + number_of_unar_min_and_plus] = '0';
            str[i + 2 + number_of_unar_min_and_plus] = input[i + 1];
            i++;
            number_of_unar_min_and_plus++;
        } else {
            str[i + number_of_unar_min_and_plus] = input[i];
        }
    }
    str[strlen(str)] = ')';

    for (i = 0; i < strlen(str); i++) {
        char elem[] = {str[i], '\0'};
        if (str[i] == ' ') {
            type_arr[i] = 0;
        } else if (is_in_one_char_elems(elem)) {
            type_arr[i] = 1;
        } else {
            type_arr[i] = 2;
        }
    }

    int word_number = 0;
    for (i = 0; i < strlen(str); i++) {
        if (type_arr[i] == 1) {
            buffer[word_number][0] = str[i];
            buffer[word_number][1] = '\0';
            word_number++;
        } else if (type_arr[i] == 2) {
            int word_len = 0;
            while (type_arr[i + word_len] == 2) {
                buffer[word_number][word_len] = str[i + word_len];
                word_len++;
            }
            buffer[word_number][word_len] = '\0';
            word_number++;
            i = i + word_len - 1;
        }
    }
    length_buffer = word_number;
}

int word_is_number(char *word) {
    int number_of_points = 0;
    // проверим первый символ
    if (word[0] == '.') {
        number_of_points++;
    }
    if (!(isdigit(word[0]) || word[0] == '.')) {
        return 0;
    } else {
        if (strcmp(word, ".") == 0) { // на самом деле, костыль
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

int check_line() {
    int number_right_bracket = 0;
    int number_left_bracket = 0;
    for (int i = 0; i < length_buffer; i++) {
        if (!(word_is_number(buffer[i]) || is_good(buffer[i]))) {
            printf("Unknown expression: %s; program terminated\n", buffer[i]);
            exit(0);
        }
        if (strcmp(buffer[i], "(") == 0) {
            number_left_bracket++;
        }
        if (strcmp(buffer[i], ")") == 0) {
            number_right_bracket++;
        }
    }
    if (number_right_bracket - number_left_bracket != 0) {
        printf("Incorrect number of brackets.\n");
        exit(0);
    }
    return 1;
}

int index_in_array(const char *elem) {
    int index = 0;
    while (index < NUMBER_OF_STACK_CONTAINS && strcmp(elem, stack_contain[index]) != 0) {
        index++;
    }
    if (index < NUMBER_OF_STACK_CONTAINS) {
        return index;
    } else {
        printf("There is no such elem\n");
        return -1;
    }
}

void to_postfix() {
    STACK_S opStack;
    stack_s_init(&opStack);
    int elem_number = 0;
    for (int i = 0; i < length_buffer; i++) {
        if (strcmp(buffer[i], "(") == 0) {
            stack_s_push(&opStack, buffer[i]);
        } else if (word_is_number(buffer[i]) || strcmp(buffer[i], "x") == 0) {
            strncpy(postfix[elem_number], buffer[i], strlen(buffer[i]));
            elem_number++;
        } else if (is_binary_operation(buffer[i])) {
            if (is_empty_s(&opStack)) {
                stack_s_push(&opStack, buffer[i]);
            } else if (priority_stack[index_in_array(opStack.head->s)] >=
                       priority_stack[index_in_array(buffer[i])]) {
                while (!(is_empty_s(&opStack) || strcmp(opStack.head->s, "(") == 0) &&
                       priority_stack[index_in_array(opStack.head->s)] >= priority_stack[index_in_array(buffer[i])]) {
                    char *to_write = stack_s_pop(&opStack);
                    strncpy(postfix[elem_number], to_write, strlen(to_write));
                    elem_number++;
                }
                stack_s_push(&opStack, buffer[i]);
            } else {
                stack_s_push(&opStack, buffer[i]);
            }
        } else if (is_unar_operation(buffer[i])) {
            stack_s_push(&opStack, buffer[i]);
        }
        if (strcmp(buffer[i], ")") == 0) {
            while (strcmp(opStack.head->s, "(") != 0) {
                char *to_stack = stack_s_pop(&opStack);
                strncpy(postfix[elem_number], to_stack, strlen(to_stack));
                elem_number++;
            }
            stack_s_pop(&opStack);
            if (!is_empty_s(&opStack) && is_unar_operation(opStack.head->s)) {
                char *to_stack = stack_s_pop(&opStack);
                strncpy(postfix[elem_number], to_stack, strlen(to_stack));
                elem_number++;
            }
        }
    }
    length = elem_number;
}

void doMath(char *elem, struct StackDouble *st) {
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
            stack_d_push(st, NAN);
            return;
        }
        double x = stack_d_pop(st); // DONE: если достали NAN (пустой стек)
        double y = stack_d_pop(st);

        switch (type_of_binary_operation) {
            case 0:
                stack_d_push(st, y + x);
                break;
            case 1:
                stack_d_push(st, y - x);
                break;
            case 2:
                stack_d_push(st, y * x);
                break;
            case 3:
                if (x == 0) {
                    stack_d_push(st, NAN);
                    printf("Division by zero\n");
                    return;
                }
                stack_d_push(st, y / x);
                break;
            case 4:
                if (y <= 0) {
                    if (x != round(x)) { // если x не целый DONE: как проверить, что double целый
                        stack_d_push(st, NAN);
                        printf("Pow Error\n");
                        return;
                    }
                }
                stack_d_push(st, pow(y, x));
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
                stack_d_push(st, NAN);
                return;
            }
            double x = stack_d_pop(st);
            switch (type_of_unar_operation) {
                case 0:
                    if (x < 0) {
                        stack_d_push(st, NAN);
                        printf("Pow Error\n");
                        return;
                    }
                    stack_d_push(st, pow(x, 0.5));
                    break;
                case 1:
                    stack_d_push(st, sin(x));
                    break;
                case 2:
                    stack_d_push(st, cos(x));
                    break;
                case 3:
                    if (cos(x) == 0) {
                        stack_d_push(st, NAN);
                        printf("Tan Error\n");
                        return;
                    }
                    stack_d_push(st, tan(x));
                    break;
                case 4:
                    if (x <= 0) {
                        stack_d_push(st, NAN);
                        printf("LOG Error\n");
                        return;
                    }
                    stack_d_push(st, log(x));
                    break;
                case 5:
                    if (x <= 0) {
                        stack_d_push(st, NAN);
                        printf("LOG Error\n");
                        return;
                    }
                    stack_d_push(st, log10(x));
                    break;
                case 6:
                    stack_d_push(st, exp(x));
                    break;
                default:
                    break;
            }
        }
    }
    if (type_of_unar_operation == -1 && type_of_binary_operation == -1) {
        printf("There is no such operation: \"%s\".\n", elem);
        stack_d_push(st, NAN);
        exit(0);
    }
}

void add_to_stack(char *elem, struct StackDouble *st, double x) {
    if (strcmp(elem, "x") == 0) {
        stack_d_push(st, x);
    } else if (word_is_number(elem)) {
        double d = strtod(elem, NULL); // DONE: 10sqrt - исправить проверку на число. Жесткая проверка на число, или x
        stack_d_push(st, d);
    } else {
        doMath(elem, st);
    }
}

double calculate(double x) {
    struct StackDouble s;
    stack_d_init(&s);

    for (int elem = 0; elem < length; elem++) {
        add_to_stack(postfix[elem], &s, x);
        if (s.head->x != s.head->x) {
            break;
        }
    }
    double to_return = stack_d_pop(&s);
    if (!is_empty_d(&s)) {
        printf("Something wrong.\n");
        exit(0);
    }
    return to_return;
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

double *func_array(double *x, int num) {
    double *y = malloc((num + 1) * sizeof(double));
    if (y == NULL) {
        printf("Malloc error in function \"func_array\"\n"); // DONE: написать название функции, из которой программа вылетела
        exit(0);
    }

    for (int i = 0; i <= num; i++) {
        y[i] = calculate(x[i]);
    }
    return y;
}

void write_ans_file(double *x, double *y, int num) {
    FILE *f = fopen("C:/Users/George/Desktop/git_projects/DrawingGraphs/CProject/Final/Output/data.csv", "w");
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


int main() {
    read_line();
    check_line();
    to_postfix();

    int num = 1000;

    double *x = linspace(-15, 15, num);
    double *y = func_array(x, num);

    write_ans_file(x, y, num);

    free(x);
    free(y);

    return 0;
}