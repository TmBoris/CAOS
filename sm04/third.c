#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

int mystrcmp(const char* str1, const char* str2) {
    const unsigned char* s1 = (unsigned char *)str1;
    const unsigned char* s2 = (unsigned char *)str2;
    while (true) {
        if (*s1 == '\0' && *s2 == '\0') {
            return 0;
        } else if (*s1 == '\0') {
            return -1;
        } else if (*s2 == '\0'){
            return 1;
        }
        if (*s1 < *s2) {
            return -1;
        } else if (*s1 > *s2) {
            return 1;
        } else {
            s1 += 1;
            s2 += 1;
        }
    }
}

int main(void) {
    int a = INT32_MAX;
    int b = INT32_MIN;
    printf("%d %d\n", a, b);
    return 0;
}




