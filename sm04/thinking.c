#include <ctype.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

struct tmp;

typedef struct other {
    struct tmp *xxx;
} other;

typedef struct tmp {
    int q;
} tmp;

void Printer(const void* arr, size_t i) {
    printf("%d ", arr[i]);
}

int main(void) {
    return 0;
}

