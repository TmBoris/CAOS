#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint-gcc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void tinyconv(uint8_t bytes[], size_t size) {
    for (size_t i = 0; i < size; ++i) {
        int ans = 0;
        ans += (bytes[i] & 128) >> 7;
        ans += (bytes[i] & 64) >> 5;
        ans += (bytes[i] & 32) >> 3;
        ans += (bytes[i] & 16) >> 1;
        ans += (bytes[i] & 8) << 1;
        ans += (bytes[i] & 4) << 3;
        ans += (bytes[i] & 2) << 5;
        ans += (bytes[i] & 1) << 7;
        bytes[i] = ans;
    }
}

// int main(void) {
//
//     return 0;
// }