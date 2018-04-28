#ifndef DV_HW_STACK_H
#define DV_HW_STACK_H

#include <math.h>

struct NodeDouble {
    double x;
    struct NodeDouble *next;
};

struct StackDouble {
    struct NodeDouble *head;
    int size;
};

void stack_d_init(struct StackDouble *st) {
    st->head = NULL;
    st->size = 0;
}

void stack_d_push(struct StackDouble *st, const double data) {
    st->size += 1;

    struct NodeDouble *p = malloc(sizeof(struct NodeDouble*));
    p->x = data;
    p->next = NULL;
    if (st->head == NULL) {
        st->head = p;
    }
    else {
        p->next = st->head;
        st->head = p;
    }
}

int is_empty_d(struct StackDouble *st) {
    return (st->size == 0);
}

double stack_d_pop(struct StackDouble *st) {
    if (is_empty_d(st)) { // TODO: exit_code
        return NAN;
    }
    else {
        st->size -= 1;
        double to_return = st->head->x;
        struct NodeDouble *to_del = st->head;
        st->head = st->head->next;
        to_del->next = NULL;
        free(to_del);
        return to_return;
    }
}

void stack_d_print(struct StackDouble *st) {
    struct NodeDouble *p = st->head;
    printf("\n");
    while (p) {
        printf("%f\n", p->x);
        p = p->next;
    }
}

#endif //DV_HW_STACK_H
