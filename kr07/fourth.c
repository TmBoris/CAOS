#include <limits.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Info {
    int chank_size;
    int chank_number;
    void *state;
    const void *begin;
    size_t size;
    size_t n;
    const void *init_state;
    void (*binary_op)(void *state, const void *element_or_state);
};

void *thread_func(void *ptr) {
    struct Info *arg = (struct Info *)ptr;

    for (size_t i = arg->chank_size * arg->chank_number;
         i < arg->chank_size * (arg->chank_number + 1) && i < arg->n; ++i) {
        arg->binary_op(arg->state, arg->begin + (i * arg->size));
    }

    pthread_exit(0);
}

void std_accumulate(void *result, const void *begin, size_t size, size_t n,
                    const void *init_state,
                    void (*binary_op)(void *state,
                                      const void *element_or_state)) {
    struct Info arg[4];

    pthread_t pthreads[4];
    void *state = calloc(4, size);
    memcpy(result, init_state, size);

    for (size_t i = 0; i < 4; ++i) {
        memcpy(state + (i * size), init_state, size);
        struct Info tmp = {n / 4 + 1, i, state + (i * size), begin,
                           size,      n, init_state,         binary_op};
        arg[i] = tmp;
        pthread_create(&pthreads[i], NULL, *thread_func, (void *)&arg[i]);
    }

    for (size_t i = 0; i < 4; ++i) {
        pthread_join(pthreads[i], NULL);
        binary_op(result, arg[i].state);
    }

    free(state);
}

void summator(void *state, const void *element) {
    *(int *)state += *(const int *)element;
}

int main(int argc, char *argv[]) {
    int result = 10;
    int init_val = 5;
    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std_accumulate((void *)(&result), a, sizeof(int), 0, &init_val, summator);
    printf("%d\n", result);
    return 0;
}