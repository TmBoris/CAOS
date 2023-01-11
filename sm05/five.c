#include "poliz.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct List {
    int value;
    struct List *next;
    struct List *prev;
} List;

int append(struct List **last, int value) {
    List *tmp = calloc(1, sizeof(*tmp));
    if (!tmp) {
        // Handle allocation error
        return -PE_OUT_OF_MEMORY;
    }
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
        (*last) = (*last)->prev;
        free((*last)->next);
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
    append(&(state->last), a + b);
    return PE_OK;
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
    append(&(state->last), b - a);
    return PE_OK;
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
    append(&(state->last), b * a);
    return PE_OK;
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
    append(&(state->last), b / a);
    return PE_OK;
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
    append(&(state->last), b % a);
    return PE_OK;
}

int grid(PolizState *state, int iextra) {
    if (state->err) {
        return state->err;
    }
    if (state->last == NULL) {
        return -PE_STACK_UNDERFLOW;
    }
    int a = pop(&(state->last));
    append(&(state->last), -a);
    return PE_OK;
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
    append(&(state->last), a);
    return PE_OK;
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

int del(PolizState *state, int iextra) {
    if (state->err) {
        return state->err;
    }
    struct List *tmp = state->last;
    size_t i = 0;
    while (i < iextra) {
        if (!tmp || tmp->prev == NULL) {
            return -PE_INVALID_INDEX;
        }
        tmp = tmp->prev;
        ++i;
    }
    if (tmp->prev == NULL) {
        tmp->next->prev = NULL;
        free(tmp);
    } else {
        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
        free(tmp);
    }
    return PE_OK;
}

int swp(PolizState *state, int iextra) {
    if (state->err) {
        return state->err;
    }
    struct List *tmp = state->last;
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
    // запишу мысли и пойду вникать в матан
    // в общем нужно будет сделать свич кейс с присваиванием разных функций
    // эти фукнции собственно и нужно будет написать
    // в стейте просто будут храниться данные, то есть в массиве, который я уже
    // положил в стейт на нем и будет реализован стек. Отсается в этой функции
    // написать свич кейс(в нем видимо иногда нужно будет что-то считывать и
    // класть в стек, а нет. Тут считывать ничего не надо, надо просто присвоить
    // нужные функции куда надо а уже когда будем циклами пробегаться будет
    // происходить магия, в результате которой будет много движухи со стеком и
    // функциями пока что не очень понимаю зачем нам всегда передавать экстра в
    // хендлер, но думаю для единообразия экстра это чиселка, которая
    // используется в d и s
    size_t spaces = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ') {
            spaces++;
        }
    }
    struct PolizItem *items = calloc(spaces + 2, sizeof(*items));
    for (size_t i = 0, j = 0; i < spaces + 1; ++i, ++j) {
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
                item.handler = del;
                item.iextra = atoi(&str[j] + 1);
                break;
            case 's':
                item.handler = swp;
                item.iextra = atoi(&str[j] + 1);
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
    }
    struct PolizItem tmp;
    tmp.handler = NULL;
    *(items + spaces + 1) = tmp;
    return items;
}

struct PolizState *poliz_new_state() {
    struct PolizState *ps = calloc(1, sizeof(*ps));
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

 int main(void) {
     // нужно в poliz_compile замутить более качественную проверку на числа
     // потому что они могут начинаться с + или -
     // еще я точно уверен, что не до конца проставил все ошибки, нужно будет
     // этот моментик тоже повнимательнее просмотреть
     // все остальное вроде должно работать
     // ну или дебаггенр ждет тебя

     // все что было до этого поправил, нашел косяк в последних двух хендлерах
     // нужно будет переписать циклы, потому что сейчас я с начала, а не с
     // конца
     // стек раскручиваю

     const char *str = "d";
     struct PolizItem *items = poliz_compile(str);
     struct PolizState *state = poliz_new_state();
     for (int i = 0; items[i].handler != NULL; ++i) {
         int err = items[i].handler(state, items[i].iextra);
         if (err < 0) {
             fprintf(stderr, "error: %d\n", -err);
             break;
         } else if (err > 0) {
             abort();
         }
     }
     poliz_free_state(state);
     free(items);
 }