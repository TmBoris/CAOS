#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
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
    int arr[10];
    arr[5] = 5;
    for (size_t i = 0; i < 10; ++i) {

    }
    return 0;
}
