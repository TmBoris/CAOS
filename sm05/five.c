#include "poliz.h"
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DynArray {
    size_t size;
    size_t capacity;
    int *ptr;
} DynArray;

int append(struct DynArray *arr, int value) {
    if (arr->size + 1 > arr->capacity) {
        size_t newcap = (arr->capacity + 1);
        int *tmp = reallocarray(arr->ptr, newcap, sizeof(*tmp));
        if (!tmp) {
            // Handle allocation error
            return -PE_OUT_OF_MEMORY;
        }
        arr->ptr = tmp;
        arr->capacity = newcap;
    }
    arr->ptr[arr->size++] = value;
    return PE_OK;
}

int pop(struct DynArray *arr) {
    int ans = arr->ptr[arr->size - 1];
    arr->ptr = reallocarray(arr->ptr, arr->size - 1, sizeof(*arr->ptr));
    arr->size--;
    arr->capacity--;
    return ans;
}

typedef struct PolizState {
    struct DynArray *arr;
    int err;
} PolizState;

int put_on_stack(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    int ans = append(state->arr, iextra);
    if (ans != 0) {
        state->err = -ans;
    }
    return ans;
}

int plus(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    if (state->arr->size < 2) {
        state->err = PE_STACK_UNDERFLOW;
        return -PE_STACK_UNDERFLOW;
    }
    int a = pop(state->arr);
    int b = pop(state->arr);
    int c;
    if (__builtin_add_overflow(a, b, &c)) {
        state->err = PE_INT_OVERFLOW;
        return -PE_INT_OVERFLOW;
    }
    int ans = append(state->arr, a + b);
    if (ans != 0) {
        state->err = -ans;
    }
    return ans;
}

int minus(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    if (state->arr->size < 2) {
        state->err = PE_STACK_UNDERFLOW;
        return -PE_STACK_UNDERFLOW;
    }
    int a = pop(state->arr);
    int b = pop(state->arr);
    int c;
    if ((b == 0 && a == INT_MIN) || __builtin_add_overflow(-a, b, &c)) {
        state->err = PE_INT_OVERFLOW;
        return -PE_INT_OVERFLOW;
    }
    int ans = append(state->arr, b - a);
    if (ans != 0) {
        state->err = -ans;
    }
    return ans;
}

int mul(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    if (state->arr->size < 2) {
        state->err = PE_STACK_UNDERFLOW;
        return -PE_STACK_UNDERFLOW;
    }
    int a = pop(state->arr);
    int b = pop(state->arr);
    int c;
    if (__builtin_mul_overflow(a, b, &c)) {
        state->err = PE_INT_OVERFLOW;
        return -PE_INT_OVERFLOW;
    }
    int ans = append(state->arr, a * b);
    if (ans != 0) {
        state->err = -ans;
    }
    return ans;
}

int dev(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    if (state->arr->size < 2) {
        state->err = PE_STACK_UNDERFLOW;
        return -PE_STACK_UNDERFLOW;
    }
    int64_t a = pop(state->arr);
    int64_t b = pop(state->arr);
    if (b == INT_MIN && a == -1) {
        state->err = PE_INT_OVERFLOW;
        return -PE_INT_OVERFLOW;
    }
    if (a == 0) {
        state->err = PE_DIVISION_BY_ZERO;
        return -PE_DIVISION_BY_ZERO;
    }
    int64_t tmp = b / a;
    if (tmp * a > b && a > 0) {
        tmp--;
    } else if (tmp * a > b) {
        tmp++;
    }
    int ans = append(state->arr, tmp);
    if (ans != 0) {
        state->err = -ans;
    }
    return ans;
}

int ost(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    if (state->arr->size < 2) {
        state->err = PE_STACK_UNDERFLOW;
        return -PE_STACK_UNDERFLOW;
    }
    int64_t a = pop(state->arr);
    int64_t b = pop(state->arr);
    if (a == 0) {
        state->err = PE_DIVISION_BY_ZERO;
        return -PE_DIVISION_BY_ZERO;
    }
    int64_t tmp = b / a;
    if (tmp * a > b && a > 0) {
        tmp--;
    } else if (tmp * a > b) {
        tmp++;
    }
    int ans = append(state->arr, b - (tmp * a));

    if (ans != 0) {
        state->err = -ans;
    }
    return ans;
}

int grid(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    if (state->arr->size < 1) {
        state->err = PE_STACK_UNDERFLOW;
        return -PE_STACK_UNDERFLOW;
    }
    int a = pop(state->arr);
    if (a == INT_MIN) {
        state->err = PE_INT_OVERFLOW;
        return -PE_INT_OVERFLOW;
    }
    int ans = append(state->arr, -a);
    if (ans != 0) {
        state->err = -ans;
    }
    return ans;
}

int rd(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    int a;
    int res = scanf("%d", &a);
    if (res != 1) {
        state->err = PE_READ_FAILED;
        return -PE_READ_FAILED;
    }
    int ans = append(state->arr, a);
    if (ans != 0) {
        state->err = -ans;
    }
    return ans;
}

int wrt(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    if (state->arr->size < 1) {
        state->err = PE_STACK_UNDERFLOW;
        return -PE_STACK_UNDERFLOW;
    }
    int a = pop(state->arr);
    printf("%d", a);
    return PE_OK;
}

int enter(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    printf("\n");
    return PE_OK;
}

int del_top(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    if (state->arr->size < 1) {
        state->err = PE_STACK_UNDERFLOW;
        return -PE_STACK_UNDERFLOW;
    }
    pop(state->arr);
    return PE_OK;
}

int dupl(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    if (state->arr->size < iextra + 1 || iextra < 0) {
        state->err = PE_INVALID_INDEX;
        return -PE_INVALID_INDEX;
    }
    int ans =
        append(state->arr, state->arr->ptr[state->arr->size - iextra - 1]);
    if (ans != 0) {
        state->err = -ans;
    }
    return ans;
}

int swp(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    if (iextra == 0) {
        return PE_OK;
    }
    if (state->arr->size < iextra + 1 || iextra < 0) {
        state->err = PE_INVALID_INDEX;
        return -PE_INVALID_INDEX;
    }
    int a = state->arr->ptr[state->arr->size - iextra - 1];
    state->arr->ptr[state->arr->size - iextra - 1] =
        state->arr->ptr[state->arr->size - 1];
    state->arr->ptr[state->arr->size - 1] = a;
    return PE_OK;
}

int get_number(const char *str, size_t *i, bool *flag) {
    int ans;
    int scanf_correct = sscanf(str + *i, "%d", &ans);
    if (scanf_correct != 1) {
        *flag = false;
        return 0;
    }
    *flag = true;
    if (str[*i] == '+' || str[*i] == '-') {
        ++(*i);
    }
    while (isdigit(str[*i])) {
        ++(*i);
    }
    return ans;
}

int push(struct PolizItem **items, size_t *size, size_t *cap,
         struct PolizItem item) {
    if (*size >= *cap) {
        size_t new_capacity = (*cap) + 1;
        struct PolizItem *tmp = realloc(*items, new_capacity * sizeof(*tmp));
        if (!tmp) {
            tmp = realloc(*items, 0);
            exit(0);
        }
        *items = tmp;
        *cap = new_capacity;
    }
    (*items)[(*size)++] = item;
    return PE_OK;
}

struct PolizItem *poliz_compile(const char *str) {
    struct PolizItem *items = realloc(NULL, sizeof(*items));
    if (items == NULL) {
        exit(1);
    }
    size_t size = 0;
    size_t capacity = 1;
    for (size_t j = 0; str[j] != '\0';) {
        struct PolizItem item;
        if (str[j] == ' ') {
            ++j;
            continue;
        } else if (str[j] == 'd') {
            ++j;
            bool done;
            int res = get_number(str, &j, &done);
            if (!done) {
                res = 0;
            }
            item.handler = dupl;
            item.iextra = res;
        } else if (str[j] == 's') {
            ++j;
            bool done;
            int res = get_number(str, &j, &done);
            if (!done) {
                res = 1;
            }
            item.handler = swp;
            item.iextra = res;
        } else {
            int res;
            bool suc;
            res = get_number(str, &j, &suc);
            if (suc) {
                item.handler = put_on_stack;
                item.iextra = res;
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
                }
            }
        }
        push(&items, &size, &capacity, item);
        j++;
    }
    struct PolizItem tmp;
    tmp.handler = NULL;
    push(&items, &size, &capacity, tmp);
    return items;
}

struct PolizState *poliz_new_state() {
    struct PolizState *ps = NULL;
    ps = realloc(ps, sizeof(*ps));
    ps->err = 0;
    ps->arr = NULL;
    ps->arr = realloc(ps->arr, sizeof(*ps->arr));
    ps->arr->capacity = 0;
    ps->arr->ptr = NULL;
    ps->arr->size = 0;
    return ps;
}

void poliz_free_state(struct PolizState *state) {
    while (state->arr->size > 0) {
        pop(state->arr);
    }
    int *tmp = realloc(state->arr, 0);
    int a = 0;
    if (!tmp) {
        a++;
    }
    tmp = realloc(state, 0);
    if (!tmp) {
        a++;
    }
}

int poliz_last_error(struct PolizState *state) {
    return state->err;
}

// int main(void) {
//     const char *str = "-2147483642 10 % w n";
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