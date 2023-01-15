#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        //        printf("%s\ni = %d\n", argv[i], i);
        errno = 0;
        long zero = 0;
        int8_t a;
        int16_t b;
        int32_t c;
        char *new;
        long num = strtol(argv[i], &new, 0);
        if (errno != 0 || (num == 0 && strcmp(argv[i], "0") != 0) ||
            *new != '\0') {
            printf("-1\n");
        } else {
            if (!__builtin_add_overflow(num, zero, &a)) {
                printf("1\n");
            } else if (!__builtin_add_overflow(num, zero, &b)) {
                printf("2\n");
            } else if (!__builtin_add_overflow(num, zero, &c)) {
                printf("4\n");
            }
        }
    }
}