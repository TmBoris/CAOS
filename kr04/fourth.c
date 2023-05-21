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
    signal(SIGCHLD, SIG_IGN);
    if (argc != 4) {
        perror("needs 4 arguments\n");
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
    double x0 = strtod(argv[1], NULL);
    size_t n = strtol(argv[2], NULL, 10);
    double dx = strtod(argv[3], NULL);
    pid_t pids[4];
    if ((pids[0] = !fork())) {
        if ((pids[2] = !fork())) {
            for (size_t i = 0; i <= n; ++i) {
                double now = sin(x0 + i * dx);
                if (write(pipds1[1], &now, sizeof(double)) == -1) {
                    perror("error during writing to pipds1\n");
                    exit(1);
                }
            }
            double sum = 0;
            for (size_t i = 0; i <= n; ++i) {
                double now;
                if (read(pipds2[0], &now, sizeof(double)) == -1) {
                    perror("error while reading in first grandson\n");
                    exit(1);
                }
                sum += now * now;
            }
            printf("1 %.10g\n", sum);
            fflush(stdout);
            exit(0);
        } else {
            //        waitpid(pids[2], NULL, 0);
        }
    } else {
        if ((pids[1] = !fork())) {
            if ((pids[3] = !fork())) {
                double sum = 0;
                for (size_t i = 0; i <= n; ++i) {
                    double now;
                    if (read(pipds1[0], &now, sizeof(double)) == -1) {
                        perror("error while reading in first grandson\n");
                        exit(1);
                    }
                    sum += fabs(now);
                }
                printf("2 %.10g\n", sum);
                fflush(stdout);
                for (size_t i = 0; i <= n; ++i) {
                    double now = cos(x0 + i * dx);
                    if (write(pipds2[1], &now, sizeof(double)) == -1) {
                        perror("error during writing to pipds2\n");
                        exit(1);
                    }
                }
                exit(0);
            } else {
                //        waitpid(pids[3], NULL, 0);
            }
        } else {
            waitpid(pids[0], NULL, 0);
            waitpid(pids[1], NULL, 0);

            printf("0 0\n");
            fflush(stdout);
        }
    }

    close(pipds1[0]);
    close(pipds1[1]);
    close(pipds2[0]);
    close(pipds2[1]);
    return 0;
}