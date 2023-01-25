#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint-gcc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int to_13_system(const char *str, const int *table) {
    const char *tmp = str;
    --str;
    while (*tmp != '\0') {
        ++tmp;
    }
    int ans = 0;
    size_t i = 0;
    while (tmp != str) {
        if (*tmp == 'a') {
            ans += 10 * (int)(table[i]);
        } else if (*tmp == 'b') {
            ans += 11 * (int)(table[i]);
        } else if (*tmp == 'c') {
            ans += 12 * (int)(table[i]);
        } else {
            ans += (int)*tmp * (int)(table[i]);
        }
        --tmp;
        ++i;
    }
    return ans;
}

void please_compute_and_print_some_strange_values_from_some_other_strange_values(
    const char *val1, const char *val2) {
    int table[9];
    int j = 1;
    for (size_t i = 0; i < 9; ++i) {
        table[i] = j;
        j *= 13;
    }
    int first = to_13_system(val1, table);
    int second = to_13_system(val2, table);
    int c;
    if (__builtin_add_overflow(first, second, &c)) {
        printf("1\n");
    } else {
        printf("0\n");
    }
    if ((first == INT_MIN && second == -1) || (second == 0)) {
        printf("1\n");
    } else {
        printf("0\n");
    }
}

 int main(void) {
     please_compute_and_print_some_strange_values_from_some_other_strange_values(
         "25b47736b", "25b47736c");
     return 0;
 }