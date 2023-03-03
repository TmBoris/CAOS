#include <read_file.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum { BUFFER_SIZE = 4096 };

 struct FileContent {
     ssize_t size;
     char *data;
 };

int max(int f, int s) {
    if (f < s) {
        return s;
    }
    return f;
}

struct FileContent read_file(int fd) {
    struct FileContent f = {.size = 1, .data = NULL};
    char buf[BUFFER_SIZE];
    ssize_t res;
    size_t cur_size = 0;
    while ((res = read(fd, buf, sizeof(buf))) > 0) {
        if (cur_size + res > f.size) {
            char *tmp = realloc(f.data, max(f.size * 2, BUFFER_SIZE));
            if (!tmp) {
                free(f.data);
                f.data = NULL;
                f.size = -1;
                return f;
            }
            f.data = tmp;
            f.size = max(f.size * 2, BUFFER_SIZE);
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
    if (cur_size + 1 > f.size) {
        char *tmp = realloc(f.data, f.size + 1);
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

 int main(void) {
     FILE *file = fopen("input.txt", "r");
     struct FileContent f = read_file(fileno(file));
     printf("%d\n", f.size);
     return 0;
 }