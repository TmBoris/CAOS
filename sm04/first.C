#include <stdio.h>
#include <stdint.h>

int main(void) {
    unsigned a;
    unsigned b;
    int s = scanf("%u %u", &a, &b);
    if (s < 0) {
        return 1;
    }
    unsigned c = ((uint64_t)a + (uint64_t)b) / 2;
    printf("%u\n", c);
    return 0;
}