#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint64_t cur_num = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

bool is_prime(uint64_t num) {
    if (num == 2) {
        return true;
    }
    if (num <= 1 || num % 2 == 0) {
        return false;
    }
    uint64_t a = 3;
    while (a * a <= num) {
        if (num % a == 0) {
            return false;
        }
        a += 2;
    }
    return true;
}

void *thread_func(void *ptr) {
    uint64_t base = (uint64_t)ptr;
    while (1) {
        if (is_prime(base)) {
            pthread_mutex_lock(&mutex);
            cur_num = base;
            pthread_cond_signal(&cond);
            pthread_mutex_unlock(&mutex);
        }
        ++base;
    }
    pthread_exit(0);
}

int main(int argc, char *argv[]) {

    uint64_t base;
    int count;
    if (scanf("%ld %d", &base, &count) != 2) {
        perror("i need 2 numbers\n");
        exit(1);
    }

    pthread_t thread;
    pthread_create(&thread, NULL, *thread_func, (void *)base);
    uint64_t *arr = calloc(count, sizeof(*arr));
    size_t max = count;

    while (count--) {
        pthread_mutex_lock(&mutex);
        while (!cur_num) {
            pthread_cond_wait(&cond, &mutex);
        }
        arr[count] = cur_num;
        cur_num = 0;
        pthread_mutex_unlock(&mutex);
    }

    for (size_t k = max; k > 0; --k) {
        printf("%ld\n", arr[k - 1]);
    }

    free(arr);

    exit(0);
}