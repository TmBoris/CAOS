#include <limits.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <unistd.h>

int *arr;
pthread_t *threads;
int *event_fd;
int num_of_threads;

void *thread_func(void *ptr) {
    int num = (intptr_t)ptr;
    uint64_t one;
    while (1) {
        if (read(event_fd[num], &one, sizeof(one)) >= 0) {
            int value;
            if (scanf("%d", &value) != 1) {
                for (int i = 0; i < num_of_threads; ++i) {
                    pthread_cancel(threads[i]);
                }
                return NULL;
            }
            printf("%d %d\n", num, value);
            fflush(stdout);
            long next_thread = value % num_of_threads;
            if (next_thread < 0) {
                next_thread += num_of_threads;
            }
            one = 1;
            if (write(event_fd[next_thread], &one, sizeof(one)) < 0) {
                pthread_exit(0);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("needs 2 args\n");
        exit(1);
    }

    num_of_threads = strtol(argv[1], NULL, 10);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);

    event_fd = calloc(num_of_threads, sizeof(*event_fd));
    threads = calloc(num_of_threads, sizeof(*threads));

    for (int i = 0; i < num_of_threads; ++i) {
        event_fd[i] = eventfd(0, 0);
        pthread_create(&threads[i], &attr, thread_func, (void *)(intptr_t)i);
    }
    uint64_t one = 1;
    if (write(event_fd[0], &one, sizeof(one)) < 0) {
        perror("error while whiting at the beggining\n");
        exit(1);
    }
    for (int i = 0; i < num_of_threads; ++i) {
        pthread_join(threads[i], NULL);
        close(event_fd[i]);
    }
    free(event_fd);
    free(threads);
}