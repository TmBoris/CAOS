#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void please_compute_and_print_some_strange_values_from_some_other_strange_values(
    const char *val1, const char *val2) {
    int32_t first = strtol(val1, NULL, 13);
    int32_t second = strtol(val2, NULL, 13);
    int32_t c;
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

// int main(void) {
//     please_compute_and_print_some_strange_values_from_some_other_strange_values(
//         "25b47736b", "25b47736c");
//     return 0;
// }