#define _GNU_SOURCE
 
#include <stdio.h>
#include <linux/futex.h>
#include <sys/syscall.h> 
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
 
void futex_wait(void *addr, int val) {
    // atomically: block on addr if (*addr == val)
    syscall(SYS_futex, addr, FUTEX_WAIT, val, NULL, NULL, 0);
}
 
void futex_wake(void *addr, int num) {
    // wake up to num threads blocked on addr
    syscall(SYS_futex, addr, FUTEX_WAKE, num, NULL, NULL, 0);
}
 
 
typedef struct t {
    _Atomic int lock;  // 1 - locked, 0 - unlocked
    _Atomic uint32_t waiters;
} caos_mutex_t;
void caos_mutex_init(caos_mutex_t *m) {
    atomic_store(&m->lock, 0);
    atomic_store(&m->waiters, 0);
}
void caos_mutex_lock(caos_mutex_t *m) {
    int zero = 0;
    int one = 1;
    while (atomic_compare_exchange_strong(&m->lock, &zero, one) != 0) {
        atomic_fetch_add(&m->waiters, 1);
        futex_wait(&m->lock, 1);
        atomic_fetch_sub(&m->waiters, 1);
    }
}
void caos_mutex_unlock(caos_mutex_t *m) {
    atomic_store(&m->lock, 0);
    if (m->waiters > 0) {
        futex_wake(&m->lock, 1);
    }
}

void *thread(void *ptr)
{
    caos_mutex_t *mtx = (caos_mutex_t*)ptr;
    caos_mutex_lock(mtx);
    printf("LOCKED\n");
    printf("%d\n", gettid());
    sleep(4);
    printf("UNLOCKED\n");
    printf("%d\n", gettid());
    caos_mutex_unlock(mtx);
    return 0;
}
 
int main(int argc, char **argv)
{
    caos_mutex_t mtx;
    caos_mutex_init(&mtx);
    pthread_t thread1;
    pthread_t thread2;
    pthread_t thread3;
    
    pthread_create(&thread1, NULL, *thread, (void *) &mtx);
    pthread_create(&thread2, NULL, *thread, (void *) &mtx);
    pthread_create(&thread3, NULL, *thread, (void *) &mtx);
    
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    pthread_join(thread3,NULL);
    return 0;
}