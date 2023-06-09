#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdatomic.h>

enum { MAX = 100 };

pthread_t pthreads[MAX];


void *thread_func(void *ptr) {
    int num = (intptr_t)ptr;
    int res;
    if (scanf("%d", &res) != 1) {
        pthread_exit(0);
    }
    pthread_create(&pthreads[num + 1], NULL, *thread_func, (void *)(intptr_t)(num + 1));
    
    pthread_join(pthreads[num + 1], NULL);
    
    printf("%d\n", res);
    pthread_exit(0);
}

int main(int argc, char *argv[]) {

    pthread_create(&pthreads[0], NULL, *thread_func, (void *)(intptr_t)0);
    

    pthread_join(pthreads[0], NULL);

    return 0;
}