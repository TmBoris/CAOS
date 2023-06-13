#include <limits.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

enum { KNUMOFARGS = 5 };

struct Info {
    int iters;
    double *first_ind;
    double first_sum;
    double *second_ind;
    double second_sum;
    pthread_mutex_t *first_mutex;
    pthread_mutex_t *second_mutex;
    bool order;
};

void *worker(void *arg) {
    struct Info *info = (struct Info *)arg;
    if (!info->order) {
        pthread_mutex_lock(info->first_mutex);
        pthread_mutex_lock(info->second_mutex);
        for (int i = 0; i < info->iters; ++i) {
            *info->first_ind += info->first_sum;
            *info->second_ind += info->second_sum;
        }
        pthread_mutex_unlock(info->second_mutex);
        pthread_mutex_unlock(info->first_mutex);
    } else {
        pthread_mutex_lock(info->second_mutex);
        pthread_mutex_lock(info->first_mutex);
        for (int i = 0; i < info->iters; ++i) {
            *info->first_ind += info->first_sum;
            *info->second_ind += info->second_sum;
        }
        pthread_mutex_unlock(info->first_mutex);
        pthread_mutex_unlock(info->second_mutex);
    }
    return NULL;
}

int main() {
    int acc_count;
    int thr_count;

    if (scanf("%d%d", &acc_count, &thr_count) != 2) {
        perror("need at least 2 args\n");
        exit(1);
    }

    double *accs = calloc(acc_count, sizeof(*accs));
    pthread_mutex_t *marr = calloc(acc_count, sizeof(*marr));

    for (int i = 0; i < acc_count; ++i) {
        accs[i] = 0;
        pthread_mutex_t tmp = PTHREAD_MUTEX_INITIALIZER;
        marr[i] = tmp;
    }
    pthread_t *threads = calloc(thr_count, sizeof(*threads));
    pthread_attr_t attr;
    pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);

    struct Info *info = calloc(thr_count, sizeof(*info));
    for (int i = 0; i < thr_count; ++i) {
        int first;
        int second;
        if (scanf("%u %u %lf %u %lf", &info[i].iters, &first,
                  &info[i].first_sum, &second,
                  &info[i].second_sum) != KNUMOFARGS) {
            perror("5 args in each string!\n");
            exit(1);
        }
        info[i].order = (second > first);
        info[i].first_mutex = &marr[first];
        info[i].second_mutex = &marr[second];
        info[i].first_ind = &accs[first];
        info[i].second_ind = &accs[second];
        pthread_create(&threads[i], &attr, worker, &info[i]);
    }
    for (int i = 0; i < thr_count; ++i) {
        pthread_join(threads[i], NULL);
    }
    for (int i = 0; i < acc_count; ++i) {
        printf("%.10g\n", accs[i]);
    }
}