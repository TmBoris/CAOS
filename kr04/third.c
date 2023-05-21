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
    size_t semicolon_pos = 0;
    for (size_t i = 1; i < argc; ++i) {
        if (strcmp(argv[i], ";") == 0) {
            semicolon_pos = i;
            break;
        }
    }
    pid_t pids[2];
    if (!(pids[0] = fork())) {
        execvp(argv[semicolon_pos + 1], argv + semicolon_pos + 1);
        _exit(1);
    }
    if (!(pids[1] = fork())) {
        argv[semicolon_pos] = NULL;
        execvp(argv[1], argv + 1);
        _exit(1);
    }
    pid_t pid = wait(NULL);
    if (pid == pids[0]) {
        kill(pids[1], SIGKILL);
    } else {
        kill(pids[0], SIGKILL);
    }

    return 0;
}