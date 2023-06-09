#include <stdatomic.h>
#include <stdint.h>

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
    while (!atomic_compare_exchange_strong(&m->lock, &zero, one)) {
        atomic_fetch_add(&m->waiters, 1);
        futex_wait(&m->lock, 1);
        atomic_fetch_sub(&m->waiters, 1);
        zero = 0;
    }
}
void caos_mutex_unlock(caos_mutex_t *m) {
    atomic_store(&m->lock, 0);
    if (m->waiters > 0) {
        futex_wake(&m->lock, 1);
    }
}