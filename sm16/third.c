#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

enum { MAX = 10 };

pthread_t pthreads[MAX];

void *thread_func(void *ptr) {
    int num = (intptr_t)ptr;
    if (num > 0) {
        pthread_join(pthreads[num - 1], NULL);
    }

    printf("%ld\n", (intptr_t)ptr);
    pthread_exit(0);
}

int main(int argc, char *argv[]) {

    for (int i = 0; i < MAX; ++i) {
        pthread_create(&pthreads[i], NULL, *thread_func, (void *)(intptr_t)i);
    }

    pthread_join(pthreads[MAX - 1], NULL);

    return 0;
}