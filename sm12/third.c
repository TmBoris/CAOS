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
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("where is my maximum!?\n");
        exit(1);
    }
    int pipds1[2];
    int res = pipe2(pipds1, O_CLOEXEC);
    if (res != 0) {
        perror("pipe2 failed\n");
        exit(1);
    }
    int pipds2[2];
    res = pipe2(pipds2, O_CLOEXEC);
    if (res != 0) {
        perror("pipe2 failed\n");
        exit(1);
    }
    int max = strtol(argv[1], NULL, 10);
    pid_t first;
    pid_t second;
    if (!(first = fork())) {
        close(pipds2[1]);
        close(pipds1[0]);

        FILE *reader = fdopen(pipds2[0], "r");
        FILE *writer = fdopen(pipds1[1], "w");

        while (1) {
            int num;
            if (fscanf(reader, "%d", &num) <= 0 || num == max) {
                fclose(reader);
                fclose(writer);
                return 0;
            }
            printf("%d %d\n", 1, num++);
            fflush(stdout);
            fprintf(writer, "%d\n", num);
            fflush(writer);
        }
    }
    if (!(second = fork())) {
        close(pipds2[0]);
        close(pipds1[1]);

        FILE *reader = fdopen(pipds1[0], "r");
        FILE *writer = fdopen(pipds2[1], "w");

        while (1) {
            int num;
            if (fscanf(reader, "%d", &num) <= 0 || num == max) {
                fclose(reader);
                fclose(writer);
                return 0;
            }
            printf("%d %d\n", 2, num++);
            fflush(stdout);
            fprintf(writer, "%d\n", num);
            fflush(writer);
        }
    }
    FILE *file = fdopen(pipds2[1], "w");
    fprintf(file, "%d\n", 1);
    fflush(file);
    fclose(file);
    close(pipds1[0]);
    close(pipds1[1]);
    close(pipds2[0]);
    close(pipds2[1]);

    int st1, st2;
    waitpid(first, &st1, 0);
    waitpid(second, &st2, 0);
    printf("Done\n");
    fflush(stdout);

    return 0;
}