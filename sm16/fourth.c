#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

enum { MAX = 1000000, THREE = 3, STO = 100 };

double arr[3];
pthread_t pthreads[THREE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_func(void *ptr) {
    int i = (intptr_t)ptr;
    for (size_t k = 0; k < MAX; ++k) {
        pthread_mutex_lock(&mutex);
        arr[i] += (i + 1) * STO;
        arr[(i + 1) % THREE] -= ((i + 1) * STO + 1);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(0);
}

int main(int argc, char *argv[]) {

    for (int i = 0; i < THREE; ++i) {
        pthread_create(&pthreads[i], NULL, *thread_func, (void *)(intptr_t)i);
    }

    for (int i = 0; i < THREE; ++i) {
        pthread_join(pthreads[i], NULL);
    }

    printf("%.10g\n%.10g\n%.10g\n", arr[0], arr[1], arr[2]);
    pthread_mutex_destroy(&mutex);

    return 0;
}