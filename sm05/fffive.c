#include "poliz.h"
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct List {
    int value;
    struct List *next;
    struct List *prev;
} List;

int append(struct List **last, int value) {
    List *tmp = realloc(NULL, sizeof(*tmp));
    if (!tmp) {
        // Handle allocation error
        return -PE_OUT_OF_MEMORY;
    }
    tmp->prev = NULL;
    tmp->next = NULL;
    tmp->value = 0;
    if (!(*last)) {
        (*last) = tmp;
        tmp->value = value;
    } else {
        (*last)->next = tmp;
        (*last)->next->prev = (*last);
        (*last) = tmp;
        tmp->value = value;
    }
    return PE_OK;
}

int pop(struct List **last) {
    int ans = (*last)->value;
    if (!((*last)->prev)) {
        free((*last));
        *last = NULL;
    } else {
        List *tmp = (*last)->prev;
        free((*last));
        (*last) = tmp;
        (*last)->next = NULL;
    }
    return ans;
}

typedef struct PolizState {
    struct List *last;
    int err;
} PolizState;

int put_on_stack(PolizState *state, int iextra) {
    if (state->err) {
        return state->err;
    }
    return append(&(state->last), iextra);
}

int plus(PolizState *state, int iextra) {
    if (state->err) {
        return state->err;
    }
    if (state->last == NULL || state->last->prev == NULL) {
        return -PE_STACK_UNDERFLOW;
    }
    int a = pop(&(state->last));
    int b = pop(&(state->last));
    int c;
    if (__builtin_add_overflow(a, b, &c)) {
        return -PE_INT_OVERFLOW;
    }
    return append(&(state->last), a + b);
}

int minus(PolizState *state, int iextra) {
    if (state->err) {
        return state->err;
    }
    if (state->last == NULL || state->last->prev == NULL) {
        return -PE_STACK_UNDERFLOW;
    }
    int a = pop(&(state->last));
    int b = pop(&(state->last));
    int c;
    if (__builtin_add_overflow(-a, b, &c)) {
        return -PE_INT_OVERFLOW;
    }
    return append(&(state->last), b - a);
}

int mul(PolizState *state, int iextra) {
    if (state->err) {
        return state->err;
    }
    if (state->last == NULL || state->last->prev == NULL) {
        return -PE_STACK_UNDERFLOW;
    }
    int a = pop(&(state->last));
    int b = pop(&(state->last));
    int c;
    if (__builtin_mul_overflow(a, b, &c)) {
        return -PE_INT_OVERFLOW;
    }
    return append(&(state->last), b * a);
}

int dev(PolizState *state, int iextra) {
    if (state->err) {
        return state->err;
    }
    if (state->last == NULL || state->last->prev == NULL) {
        return -PE_STACK_UNDERFLOW;
    }
    int a = pop(&(state->last));
    int b = pop(&(state->last));
    if (a == 0) {
        return -PE_DIVISION_BY_ZERO;
    }
    if (MIN(a, b) == INT_MIN && MAX(a, b) == -1) {
        return -PE_INT_OVERFLOW;
    }
    int tmp = b / a;
    if (tmp * a > b && a > 0) {
        tmp--;
    } else if (tmp * a > b) {
        tmp++;
    }
    return append(&(state->last), tmp);
}

int ost(PolizState *state, int iextra) {
    if (state->err) {
        return state->err;
    }
    if (state->last == NULL || state->last->prev == NULL) {
        return -PE_STACK_UNDERFLOW;
    }
    int a = pop(&(state->last));
    int b = pop(&(state->last));
    if (a == 0) {
        return -PE_DIVISION_BY_ZERO;
    }
    if (MIN(a, b) == INT_MIN && MAX(a, b) == -1) {
        return -PE_INT_OVERFLOW;
    }
    int tmp = b / a;
    if (tmp * a > b && a > 0) {
        tmp--;
    } else if (tmp * a > b) {
        tmp++;
    }
    return append(&(state->last), b - (tmp * a));
}

int grid(PolizState *state, int iextra) {
    if (state->err) {
        return state->err;
    }
    if (state->last == NULL) {
        return -PE_STACK_UNDERFLOW;
    }
    int a = pop(&(state->last));
    return append(&(state->last), -a);
}

int rd(PolizState *state, int iextra) {
    if (state->err) {
        return state->err;
    }
    int a;
    int res = scanf("%d", &a);
    if (res != 1) {
        return -PE_READ_FAILED;
    }
    return append(&(state->last), a);
}

int wrt(PolizState *state, int iextra) {
    if (state->err) {
        return state->err;
    }
    if (state->last == NULL) {
        return -PE_STACK_UNDERFLOW;
    }
    int a = pop(&(state->last));
    printf("%d", a);
    return PE_OK;
}

int enter(PolizState *state, int iextra) {
    if (state->err) {
        return state->err;
    }
    printf("\n");
    return PE_OK;
}

int del_top(PolizState *state, int iextra) {
    if (state->err) {
        return state->err;
    }
    if (state->last == NULL) {
        return -PE_STACK_UNDERFLOW;
    }
    pop(&(state->last));
    return PE_OK;
}

int dupl(PolizState *state, int iextra) {
    if (state->err) {
        return state->err;
    }
    struct List *tmp = state->last;
    if (!tmp || iextra < 0) {
        return -PE_INVALID_INDEX;
    }
    size_t i = 0;
    while (i < iextra) {
        if (tmp->prev == NULL) {
            return -PE_INVALID_INDEX;
        }
        tmp = tmp->prev;
        ++i;
    }
    return append(&(state->last), tmp->value);
}

int swp(PolizState *state, int iextra) {
    if (state->err) {
        return state->err;
    }
    struct List *tmp = state->last;
    if (iextra == 0) {
        return PE_OK;
    } else if (!tmp || iextra < 0) {
        return -PE_INVALID_INDEX;
    }
    size_t i = 0;
    while (i < iextra) {
        if (tmp->prev == NULL) {
            return -PE_INVALID_INDEX;
        }
        tmp = tmp->prev;
        ++i;
    }
    int a = tmp->value;
    tmp->value = state->last->value;
    state->last->value = a;
    return PE_OK;
}

struct PolizItem *poliz_compile(const char *str) {
    size_t spaces = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ') {
            spaces++;
        }
    }
    struct PolizItem *items = realloc(NULL, (spaces + 2) * sizeof(*items));
    for (size_t i = 0, j = 0; i < spaces + 1; ++i) {
        struct PolizItem item;
        if (isdigit(str[j]) || (str[j] == '+' && isdigit(str[j + 1])) ||
            (str[j] == '-' && isdigit(str[j + 1]))) {
            item.handler = put_on_stack;
            item.iextra = atoi(&str[j]);
        } else {
            switch (str[j]) {
            case '+':
                item.handler = plus;
                break;
            case '-':
                item.handler = minus;
                break;
            case '*':
                item.handler = mul;
                break;
            case '/':
                item.handler = dev;
                break;
            case '%':
                item.handler = ost;
                break;
            case '#':
                item.handler = grid;
                break;
            case 'r':
                item.handler = rd;
                break;
            case 'w':
                item.handler = wrt;
                break;
            case 'n':
                item.handler = enter;
                break;
            case ';':
                item.handler = del_top;
                break;
            case 'd':
                item.handler = dupl;
                item.iextra = atoi(&str[j] + 1);
                break;
            case 's':
                item.handler = swp;
                if (str[j + 1] != EOF && str[j + 1] == '0' &&
                    !isdigit(str[j + 2])) {
                    item.iextra = 0;
                } else {
                    item.iextra = atoi(&str[j] + 1);
                    if (item.iextra == 0) {
                        item.iextra = 1;
                    }
                }
                break;
            }
        }
        *(items + i) = item;
        while (str[j] != ' ') {
            ++j;
            if (str[j] == '\0') {
                break;
            }
        }
        if (str[j] == '\0') {
            break;
        }
        while (str[j] == ' ') {
            ++j;
            if (str[j] == '\0') {
                break;
            }
        }
    }
    struct PolizItem tmp;
    tmp.handler = NULL;
    *(items + spaces + 1) = tmp;
    return items;
}

struct PolizState *poliz_new_state() {
    struct PolizState *ps = realloc(NULL, sizeof(*ps));
    ps->err = 0;
    ps->last = NULL;
    return ps;
}

void poliz_free_state(struct PolizState *state) {
    while (state->last != NULL && state->last->prev != NULL) {
        state->last = state->last->prev;
        pop(&(state->last->next));
    }
    free(state->last);
    free(state);
}

int poliz_last_error(struct PolizState *state) {
    return state->err;
}

// int main(void) {
//     const char *str = "-3 -5 % w n";
//     struct PolizItem *items = poliz_compile(str);
//     struct PolizState *state = poliz_new_state();
//     for (int i = 0; items[i].handler != NULL; ++i) {
//         int err = items[i].handler(state, items[i].iextra);
//         if (err < 0) {
//             fprintf(stderr, "error: %d\n", -err);
//             break;
//         } else if (err > 0) {
//             abort();
//         }
//     }
//     poliz_free_state(state);
//     free(items);
// }