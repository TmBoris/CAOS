#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

void strip_spaces(char *buf) {
    char *prev = buf;
    while (*buf == ' ') {
        ++buf;
    }
    bool flag = false;
    while (*buf != '\0') {
        if (*buf == ' ' && *(buf + 1) == '\0') {
            if (*prev == ' ') {
                prev--;
            }
            break;
        } else {
            if (*buf == ' ' && !flag) {
                flag = true;
                *prev = *buf;
                ++prev;
            } else if (*buf != ' ') {
                flag = false;
                *prev = *buf;
                ++prev;
            }
            ++buf;
        }
    }
    *prev = '\0';

}

int main(void) {
    char str[] = "  d lkglf f    ";
    strip_spaces(str);
    printf("%s\n", str);
}