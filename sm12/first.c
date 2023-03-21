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
        perror("needs 2 arguments");
        exit(1);
    }
    int pipds[2];
    int res = pipe2(pipds, O_CLOEXEC);
    if (res != 0) {
        perror("pipe2 failed");
        exit(1);
    }
    if (!fork()) {
        dup2(pipds[1], STDOUT_FILENO);
        execlp(argv[1], argv[1], NULL);
        _exit(1);
    }
    close(pipds[1]);
    if (!fork()) {
        dup2(pipds[0], STDIN_FILENO);
        execlp(argv[2], argv[2], NULL);
        _exit(1);
    }
    close(pipds[0]);
    wait(NULL);
    wait(NULL);
    return 0;
}
