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

volatile sig_atomic_t first = 0;
volatile sig_atomic_t second = 0;
volatile sig_atomic_t last_signal = 0;

void change_mode(int signal) {
    last_signal = signal;
}

int main(void) {

    struct sigaction sa = {
        .sa_handler = change_mode,
        .sa_flags = SA_RESTART,
    };

    sigset_t possible_signals, oldmask;

    sigemptyset(&possible_signals);
    sigaddset(&possible_signals, SIGUSR1 | SIGUSR2 | SIGTERM);
    sigprocmask(SIG_BLOCK, &possible_signals, &oldmask);

    sigaction(SIGUSR2, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    printf("%d\n", getpid());
    fflush(stdout);
    while (1) {
        sigsuspend(&oldmask);
        if (last_signal == SIGUSR1) {
            printf("%d\n%d\n", first, second);
            fflush(stdout);
            first++;
        } else if (last_signal == SIGUSR2) {
            second++;
        } else {
            exit(0);
        }
    }
}