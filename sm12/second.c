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
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        perror("needs 3 arguments\n");
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
    int first_sum = 0;
    int second_sum = 0;
    if (!fork()) {
        FILE *file = fopen(argv[1], "r");
        int num;
        while (fscanf(file, "%d", &num) == 1) {
            first_sum += num;
        }
        //        printf("first sum: %d\n", first_sum);
        fclose(file);
        if (write(pipds1[1], &first_sum, sizeof(int)) == -1) {
            perror("error during writing to the file\n");
            exit(1);
        }
        exit(0);
    }
    if (!fork()) {
        FILE *file = fopen(argv[2], "r");
        int num;
        while (fscanf(file, "%d", &num) == 1) {
            second_sum += num;
        }
        //        printf("second sum: %d\n", second_sum);
        fclose(file);
        if (write(pipds2[1], &second_sum, sizeof(int)) == -1) {
            perror("error during writing to the file\n");
            exit(1);
        }
        exit(0);
    }
    wait(NULL);
    wait(NULL);

    int buf[2];
    if (read(pipds1[0], buf, sizeof(int)) == -1) {
        perror("problem with reading in the end\n");
        exit(1);
    }
    if (read(pipds2[0], buf + 1, sizeof(int)) == -1) {
        perror("problem with reading in the end\n");
        exit(1);
    }
    printf("%d %d %d\n", buf[1], buf[0], buf[0] + buf[1]);
    close(pipds1[0]);
    close(pipds1[1]);
    close(pipds2[0]);
    close(pipds2[1]);
    return 0;
}