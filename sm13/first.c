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

volatile int counter = 0;

void print_next_num(int signo) {
    if (counter == 4) {
        exit(0);
    }
    printf("%d\n", counter++);
    fflush(stdout);
}

int main(void) {
    struct sigaction sigact = {
        .sa_handler = print_next_num,
        .sa_flags = SA_RESTART,
    };
    sigaction(SIGRTMIN, &sigact, NULL);
    pid_t pid = getpid();
    printf("%d\n", pid);
    fflush(stdout);
    while (1) {
        pause();
    }
}