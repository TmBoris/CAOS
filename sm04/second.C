#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int cmp(const void *a, const void *b) {
    return *(int64_t*)a - *(int64_t*)b;
}

int main(void) {
    int64_t num[1000];
    int64_t val;
    size_t j = 0;
    int64_t max = 0;
    while (scanf("%llx ", &val) > 0) {
        num[j] = val;
        j += 1;
        if (val > max) {
            max = val;
        }
    }
    int64_t zero = 0;
    int64_t first = 1;
    int64_t t = 0;
    for (size_t i = 0; i < j; i += 1) {
        if (num[i] > 0) {
            while (num[i] >= first) {
                int64_t tmp = first;
                first = zero + first;
                zero = tmp;
                t += 1;
            }
            t -= 1;
            int64_t tmp = first;
            first = zero;
            zero = tmp - first;
            printf("%llx ", t);
        } else {
            int64_t tmp = -1;
            printf("%llx ", tmp);
        }
    }
    printf("\n");
    return 0;
}