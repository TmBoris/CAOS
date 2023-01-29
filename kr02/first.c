#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void strip_spaces(char *buf) {
    char *prev = buf;
    while (isspace(*buf) && *buf != '\0') {
        ++buf;
    }
    if (*buf == '\0') {
        *prev = '\0';
        return;
    }
    *(prev) = *(buf++);
    while (*buf != '\0') {
        if (isspace(*prev) && isspace(*buf)) {
            ++buf;
        } else if (isspace(*buf)) {
            *(++prev) = ' ';
            ++buf;
        } else {
            *(++prev) = *(buf++);
        }
    }
    if (isspace(*prev)) {
        *prev = '\0';
    } else {
        *(++prev) = '\0';
    }
}

// int main(void) {
//     char str[] =  "     ";
//     strip_spaces(str);
//     printf("%s\n", str);
// }