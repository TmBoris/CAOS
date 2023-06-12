#include <limits.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum { NTHREADS = 100, NUMSFORONETHREAD = 1000 };

struct Item {
    struct Item *_Atomic next;
    long long value;
};

struct Item tmp = {NULL, -1};
struct Item *_Atomic pre_head = &tmp;

void add(struct Item *new_node, struct Item *last_added) {
    struct Item *prev = last_added;
    while (prev->next && prev->next->value < new_node->value) {
        prev = prev->next;
    }
    struct Item *old_val = prev->next;
    while (1) {
        if (old_val && old_val->value < new_node->value) {
            prev = old_val;
            old_val = prev->next;
            continue;
        }
        atomic_store(&new_node->next, old_val);
        int changed =
            atomic_compare_exchange_strong(&prev->next, &old_val, new_node);
        if (changed) {
            sched_yield();
            break;
        } else {
            if (old_val->value < new_node->value) {
                prev = old_val;
                old_val = prev->next;
            }
        }
    }
}

void *thread_func(void *ptr) {
    int num = (intptr_t)ptr;
    struct Item *last_added = pre_head;
    for (int i = num * NUMSFORONETHREAD;
         i < num * NUMSFORONETHREAD + NUMSFORONETHREAD; ++i) {
        struct Item *cur = calloc(1, sizeof(*cur));
        cur->value = i;
        cur->next = NULL;
        add(cur, last_added);
        last_added = cur;
    }

    pthread_exit(0);
}

int main(int argc, char *argv[]) {

    pthread_t pthreads[NTHREADS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);

    for (int i = 0; i < NTHREADS; ++i) {
        pthread_create(&pthreads[i], &attr, *thread_func, (void *)(intptr_t)i);
    }

    for (int i = 0; i < NTHREADS; ++i) {
        pthread_join(pthreads[i], NULL);
    }

    // bool same = true;

    struct Item *cur = pre_head->next;
    // int i = 0;
    while (cur) {
        // if (i != cur->value) {
        //     same = false;
        // }
        // if (cur->value == i) {
        //     printf("%lld == %d\n", cur->value, i++);
        // } else {
        //     printf("%lld != %d f;lsjfladjfkadjfadkfj\n", cur->value, i++);
        // }
        printf("%lld\n", cur->value);
        cur = cur->next;
    }

    // printf("%d\n", same);

    return 0;
}