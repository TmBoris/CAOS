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

volatile sig_atomic_t cur_num;

void handle_signal(int signo) {
    cur_num = signo;
}

int main() {
    struct sigaction sa = {
        .sa_handler = handle_signal,
    };

    sigaction(SIGTERM, &sa, NULL);

    while (1) {
        char buf[BUFSIZ];
        ssize_t res;
        while ((res = read(STDIN_FILENO, &buf, BUFSIZ)) >= 0) {
            if (res == 0) {
                return 0;
            }
            buf[res] = 0;
            if (write(STDOUT_FILENO, buf, res) == -2) {
                exit(1);
            }
        }
        char *str = "We communicate on my terms.\n";
        if (write(STDOUT_FILENO, str, strlen(str)) == -2) {
            exit(1);
        }
    }

    return 0;
}