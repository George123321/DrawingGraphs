#ifndef DV_HW_STACK_S
#define DV_HW_STACK_S

#include <math.h>

struct NodeString {
    char *s;
    struct NodeString *next;
};

struct StackString {
    struct NodeString *head;
    int size;
};

void stack_s_init(struct StackString *st) {
    st->head = NULL;
    st->size = 0;
}

void stack_s_push(struct StackString *st, const char *data) {
    st->size += 1;

    struct NodeString *p = malloc(sizeof(*p));
    p->s = (char *)data;
    p->next = NULL;
    if (st->head == NULL) {
        st->head = p;
    }
    else {
        p->next = st->head;
        st->head = p;
    }
}

int is_empty_s(struct StackString *st) {
    return (st->size == 0);
}
 char *stack_s_pop(struct StackString *st) {
    if (is_empty_s(st)) { // TODO: exit_code
        return NULL;
    }
    else {
        st->size -= 1;
        char *to_return = st->head->s;
        struct NodeString *to_del = st->head;
        st->head = st->head->next;
        to_del->next = NULL;
        free(to_del);
        return to_return;
    }
}

void stack_s_print(struct StackString *st) {
    struct NodeString *p = st->head;
    printf("\n");
    while (p) {
        printf("%s\n", p->s);
        p = p->next;
    }
}

#endif //DV_HW_STACK_S
