#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

void normalize_path(char *buf) {
    char *prev = buf;
    bool flag = false;
    while (*buf != '\0') {
        if (*buf == '/' && !flag) {
            flag = true;
            *prev = *buf;
            ++prev;
        } else if (*buf != '/') {
            flag = false;
            *prev = *buf;
            ++prev;
        }
        ++buf;
    }
    *prev = '\0';
}

int main(void) {
    char str[] = "////A/////h";
    normalize_path(str);
    printf("%s\n", str);
}