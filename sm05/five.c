#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "poliz.h"

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
            return PE_OUT_OF_MEMORY;
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

int PutOnStack(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    return append(state->arr, iextra);
}

int Plus(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    if (state->arr->size < 2) {
        return PE_STACK_UNDERFLOW;
    }
    int a = pop(state->arr);
    int b = pop(state->arr);
    append(state->arr,a + b);
    return PE_OK;
}

int Minus(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    if (state->arr->size < 2) {
        return PE_STACK_UNDERFLOW;
    }
    int a = pop(state->arr);
    int b = pop(state->arr);
    append(state->arr,b - a);
    return PE_OK;
}

int Mul(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    if (state->arr->size < 2) {
        return PE_STACK_UNDERFLOW;
    }
    int a = pop(state->arr);
    int b = pop(state->arr);
    append(state->arr,b * a);
    return PE_OK;
}

int Der(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    if (state->arr->size < 2) {
        return PE_STACK_UNDERFLOW;
    }
    int a = pop(state->arr);
    int b = pop(state->arr);
    append(state->arr,b / a);
    return PE_OK;
}

int Ost(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    if (state->arr->size < 2) {
        return PE_STACK_UNDERFLOW;
    }
    int a = pop(state->arr);
    int b = pop(state->arr);
    append(state->arr,b % a);
    return PE_OK;
}

int Grid(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    if (state->arr->size < 1) {
        return PE_STACK_UNDERFLOW;
    }
    int a = pop(state->arr);
    append(state->arr, -a);
    return PE_OK;
}

int Read(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    int a;
    int res = scanf("%d", &a);
    if (res < 1) {
        return PE_READ_FAILED;
    }
    append(state->arr, a);
    return PE_OK;
}

int Write(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    if (state->arr->size < 1) {
        return PE_STACK_UNDERFLOW;
    }
    int a = pop(state->arr);
    printf("%d", a);
    return PE_OK;
}

int Enter(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    printf("\n");
    return PE_OK;
}

int DelTop(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    if (state->arr->size < 1) {
        return PE_STACK_UNDERFLOW;
    }
    pop(state->arr);
    return PE_OK;
}

int Del(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    if (state->arr->size < iextra + 1) {
        return PE_STACK_UNDERFLOW;
    }
    DynArray *tmp;
    for (size_t i = 0; i < iextra; ++i) {
        int a = pop(state->arr);
        append(tmp, a);
    }
    int litter = pop(state->arr);
    for (size_t i = 0; i < iextra; ++i) {
        int a = pop(tmp);
        append(state->arr, a);
    }
    return PE_OK;
}

int Swap(PolizState *state, int iextra) {
    if (state->err) {
        return -state->err;
    }
    if (state->arr->size < iextra + 1) {
        return PE_STACK_UNDERFLOW;
    }
    int first = pop(state->arr);
    DynArray *tmp;
    for (size_t i = 0; i < iextra; ++i) {
        int a = pop(state->arr);
        append(tmp, a);
    }
    int second = pop(state->arr);
    append(state->arr, first);
    for (size_t i = 0; i < iextra; ++i) {
        int a = pop(tmp);
        append(state->arr, a);
    }
    append(state->arr, second);
    return PE_OK;
}



struct PolizItem *poliz_compile(const char *str) {
    // запишу мысли и пойду вникать в матан
    // в общем нужно будет сделать свич кейс с присваиванием разных функций
    // эти фукнции собственно и нужно будет написать
    // в стейте просто будут храниться данные, то есть в массиве, который я уже положил в стейт
    // на нем и будет реализован стек. Отсается в этой функции написать свич кейс(в нем видимо иногда нужно будет что-то
    // считывать и класть в стек, а нет. Тут считывать ничего не надо, надо просто присвоить нужные функции куда надо
    // а уже когда будем циклами пробегаться будет происходить магия, в результате которой
    // будет много движухи со стеком и функциями
    // пока что не очень понимаю зачем нам всегда передавать экстра в хендлер, но думаю для единообразия
    // экстра это чиселка, которая используется в d и s
    size_t spaces = 0;
    for(size_t i = 0 ; str[i] != '\0' ; i++) {
        if (str[i] == ' ') {
            spaces++;
        }
    }
    struct PolizItem *items = calloc(spaces + 2, sizeof(*items));
    for (size_t i = 0, j = 0; i < spaces + 1; ++i, ++j) {
        struct PolizItem item;
        if (isdigit(*str)) {
            item.handler = PutOnStack;
            item.iextra = atoi(str);
        } else {
            switch (*str) {
            case '+':
                item.handler = Plus;
                break;
            case '-':
                item.handler = Minus;
                break;
            case '*':
                item.handler = Mul;
                break;
            case '/':
                item.handler = Der;
                break;
            case '%':
                item.handler = Ost;
                break;
            case '#':
                item.handler = Grid;
                break;
            case 'r':
                item.handler = Read;
                break;
            case 'w':
                item.handler = Write;
                break;
            case 'n':
                item.handler = Enter;
                break;
            case ';':
                item.handler = DelTop;
                break;
            case 'd':
                item.handler = Del;
                item.iextra = atoi(str + 1);
                break;
            case 's':
                item.handler = Swap;
                item.iextra = atoi(str + 1);
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
    ps->arr = calloc(1, sizeof(*ps->arr));
    return ps;
}

void poliz_free_state(struct PolizState *state) {
    while (state->arr->size > 0) {
        pop(state->arr);
    }
    free(state->arr);
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
    const char *str = "r";
    struct PolizItem *items = poliz_compile(str);
    struct PolizState *state = poliz_new_state();
    for (int i = 0; items[i].handler != NULL; ) {
        int err = items[i].handler(state, items[i].iextra);
        if (err < 0) {
            fprintf(stderr, "error: %d\n", -err);
            break;
        } else if (err > 0) {
            i = err;
        } else {
            ++i;
        }
    }
    poliz_free_state(state);
    free(items);
}