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
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    pid_t *pidtable = calloc(sizeof(*pidtable), argc);
    int pipds[2];
    int fd = -1;

    for (size_t i = 1; i < argc; ++i) {
        if (pipe2(pipds, O_CLOEXEC) != 0) {
            perror("pipe2 failed");
            exit(1);
        }

        pid_t pid = fork();
        if (pid < 0) {
            // kill all earlier proccesses
            int status;
            for (size_t j = 0; j < i; ++j) {
                if (pidtable[j] != 0) {
                    kill(pidtable[j], SIGKILL);
                    waitpid(pidtable[j], &status, 0);
                }
            }
            free(pidtable);
            exit(1);
        } else if (pid == 0) {
            // son
            close(pipds[0]);
            if (fd != STDIN_FILENO) {
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            if (i + 1 != argc) {
                dup2(pipds[1], STDOUT_FILENO);
                close(pipds[1]);
            }
            execlp(argv[i], argv[i], NULL);
            free(pidtable);
            _exit(1);
        } else {
            // parent
            close(fd);
            fd = pipds[0];
            close(pipds[1]);
            pidtable[i] = pid;
        }
    }

    for (size_t i = 0; i < argc; ++i) {
        int status;
        if (pidtable[i] != 0) {
            waitpid(pidtable[i], &status, 0);
        }
    }
    free(pidtable);
    return 0;
}