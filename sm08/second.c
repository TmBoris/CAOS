#include "math.h"
#include "stdint.h"
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <read_file.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum { BUFFER_SIZE = 4096 };

// struct FileContent {
//     ssize_t size;
//     char *data;
// };

size_t max(size_t one, size_t two) {
    if (one < two) {
        return two;
    }
    return one;
}

struct FileContent read_file(int fd) {
    struct FileContent f = {.size = 0, .data = NULL};
    char buf[BUFFER_SIZE];
    ssize_t res;
    size_t cur_size = 0;
    while ((res = read(fd, buf, sizeof(buf))) > 0) {
        if (cur_size + res >= f.size) {
            char *tmp = realloc(f.data, max(f.size * 2, f.size + res));
            if (!tmp) {
                free(f.data);
                f.data = NULL;
                f.size = -1;
                return f;
            }
            f.data = tmp;
            f.size = max(f.size * 2, f.size + res);
        }
        memcpy(f.data + cur_size, buf, res);
        cur_size += res;
    }
    if (res == -1) {
        free(f.data);
        f.data = NULL;
        f.size = -1;
        return f;
    }
    if (cur_size + 1 != f.size) {
        char *tmp = realloc(f.data, cur_size + 1);
        if (!tmp) {
            free(f.data);
            f.data = NULL;
            f.size = -1;
            return f;
        }
        f.data = tmp;
    }
    f.data[cur_size] = '\0';
    f.size = cur_size;
    return f;
}

// int main(void) {
//     FILE *file = fopen("input.txt", "r");
//     struct FileContent f = read_file(fileno(file));
//     printf("%d\n", f.size);
//     return 0;
// }