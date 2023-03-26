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

volatile sig_atomic_t cur_num = 1;

void minus(int signal) {
    cur_num = 1;
}

void square(int signal) {
    cur_num = 2;
}

int main(void) {
    struct sigaction sigact1 = {
        .sa_handler = minus,
        .sa_flags = SA_RESTART,
    };
    sigaction(SIGUSR1, &sigact1, NULL);
    struct sigaction sigact2 = {
        .sa_handler = square,
        .sa_flags = SA_RESTART,
    };
    sigaction(SIGUSR2, &sigact2, NULL);
    pid_t pid = getpid();
    printf("%d\n", pid);
    fflush(stdout);
    int64_t num;
    while ((scanf("%ld", &num)) == 1) {
        if (cur_num == 1) {
            printf("%ld\n", -num);
        } else {
            printf("%ld\n", num * num);
        }
        fflush(stdout);
    }
}