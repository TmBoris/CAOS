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

enum { MAX_ARGS = 20, BUF_SIZE = 16, TENSYS = 10, NOSIGNAL = 0 };

volatile sig_atomic_t last_signal = 0;
volatile sig_atomic_t filenum = 0;

void handler(int signal) {
    last_signal = signal;
}

int main(int argc, char *argv[]) {
    if (argc > MAX_ARGS + 1) {
        perror("too much args\n");
        exit(1);
    }

    int64_t sums[MAX_ARGS] = {0};

    int files[argc - 1];
    for (size_t i = 0; i + 1 < argc; ++i) {
        files[i] = open(argv[i + 1], O_RDONLY);
    }

    struct sigaction sa = {
        .sa_handler = handler,
    };

    sigset_t possible_signals, oldmask;

    sigemptyset(&possible_signals);
    sigemptyset(&oldmask);
    sigaddset(&possible_signals, SIGTERM);
    sigaction(SIGTERM, &sa, NULL);
    for (int i = 0; i + 1 < argc; ++i) {
        sigaction(SIGRTMIN + i, &sa, NULL);
        sigaddset(&possible_signals, SIGRTMIN + i);
    }
    sigprocmask(SIG_BLOCK, &possible_signals, NULL);

    printf("%d\n", getpid());
    fflush(stdout);
    while (1) {
        if (last_signal == NOSIGNAL) {
            sigsuspend(&oldmask);
        } else if (last_signal == SIGTERM) {
            for (size_t i = 0; i + 1 < argc; ++i) {
                printf("%ld\n", sums[i]);
            }
            exit(0);
        } else if (SIGRTMIN <= last_signal ||
                   last_signal <= SIGRTMIN + argc - 2) {
            filenum = last_signal - SIGRTMIN;
            last_signal = NOSIGNAL;
            int64_t sum = 0;
            int res;
            char buf[BUF_SIZE];
            sigprocmask(SIG_UNBLOCK, &possible_signals, NULL);
            while ((res = read(files[filenum], buf, BUF_SIZE)) > 0) {
                int64_t cur = strtoll(buf, NULL, 10);
                sum += cur;
                if (last_signal != NOSIGNAL) {
                    break;
                }
            }
            sigprocmask(SIG_BLOCK, &possible_signals, NULL);
            sums[filenum] += sum;
            if (res == 0) {
                close(files[filenum]);
            }
        }
    }

    return 0;
}