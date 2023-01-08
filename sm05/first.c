#include <stdio.h>
#include <stdlib.h>

enum { A = 1103515245, C = 12345, M = (1 << 31) };

struct RandomGenerations;

typedef struct RandomGenerator {
    uint curr;
    struct RandomGenerations *ops;
    struct RandomGenerator *next;
    struct RandomGenerator *prev;
    // x2 = (A * x1 + C) % M
} RandomGenerator;

void food(struct RandomGenerator *gen) {
    if (gen->next == NULL) {
        free(gen->ops);
        free(gen);
        return;
    }
    gen = gen->next;
    free(gen->prev);
    food(gen);
}

uint foon(struct RandomGenerator *gen) {
    while (gen->next) {
        gen = gen->next;
    }
    gen->next = calloc(1, sizeof(*gen));
    gen->next->prev = gen;
    gen->next->ops = gen->ops;
    gen->next->next = NULL;
    gen->next->curr = (A * gen->curr + C) % M;
    gen = gen->next;
    return gen->curr;
}

typedef struct RandomGenerations {
    void (*destroy)(RandomGenerator *gen);

    uint (*next)(RandomGenerator *gen);
} RandomGenerations;

RandomGenerator *random_create(uint seed) {
    RandomGenerator *ans = calloc(1, sizeof(*ans));
    ans->ops = calloc(1, sizeof(*(ans->ops)));
    ans->ops->destroy = food;
    ans->ops->next = foon;
    ans->next = NULL;
    ans->prev = NULL;
    ans->curr = seed;
    return ans;
}

// int main(void) {
//     RandomGenerator *rr = random_create(1234);
//     printf("%d\n", rr->ops->next(rr));
//
//     for (int j = 0; j < 100; ++j) {
//         printf("%d\n", rr->ops->next(rr));
//     }
//     rr->ops->destroy(rr);
//     return 0;
// }