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
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signalfd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main() {

    int pipds[2];
    if (pipe(pipds) == -1) {
        perror("pipe failed\n");
        exit(1);
    }

    FILE *reader = fdopen(pipds[0], "r");
    FILE *writer = fdopen(pipds[1], "w");

    pid_t pid = fork();
    if (pid == 0) {
        sleep(1);
        for (size_t i = 0; i < 10; ++i) {
            int num;
            if (fscanf(reader, "%d", &num) != 1) {
                perror("scanf\n");
                exit(1);
            }
            printf("%d\n", num);
            fflush(stdout);
        }
    }
    for (size_t i = 0; i < 10; ++i) {
        fprintf(writer, "%d\n", i);
        fflush(writer);
    }

    wait(NULL);
}