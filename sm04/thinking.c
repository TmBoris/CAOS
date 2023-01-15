#include <ctype.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct A {
    int a;
    long long b;
} A;

int main(void) {
    A *a = NULL;
    a = realloc(a, sizeof(*a));
    if (a == NULL) {
        printf("%d", -5 / -3);
    }
    realloc(a, 0);
    printf("%d", -5 / -3);

    return 0;
}

