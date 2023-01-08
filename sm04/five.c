#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

int main(void) {
    int64_t a, b;
    int n;
    int res = scanf("%lld %lld %d", &a, &b, &n);
    if (res <= 2) {
        return 1;
    }
    for (int64_t i = a - 1; i != b; ++i) {
        for (int64_t j = a - 1; j != b; ++j) {
            if (i == a - 1 && j == a - 1) {
                printf("%*C", n, ' ');
            } else if (i == a - 1) {
                printf(" %*lld", n, j);
            } else if (j == a - 1) {
                printf("%*lld", n, i);
            } else {
                printf(" %*lld", n, i * j);
            }
        }
        printf("\n");
    }
    return 0;
}