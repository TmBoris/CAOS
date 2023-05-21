#define _XOPEN_SOURCE
#define _GNU_SOURCE
#include "math.h"
#include "stdint.h"
#include <ctype.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("USAGE: name of file with time_points\n");
        exit(1);
    }
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("no such file\n");
        exit(1);
    }
    char *buf = NULL;
    size_t i = 0;
    size_t line_size;
    time_t prev;
    while ((getline(&buf, &line_size, file)) != -1) {
        strip_spaces(buf);
        struct tm tm;
        strptime(buf, "%Y/%m/%d %H:%M:%S", &tm);
        tm.tm_isdst = -1;
        time_t now = mktime(&tm);
        if (i++ != 0) {
            printf("%ld\n", now - prev);
        }
        prev = now;
    }
    free(buf);
    return 0;
}