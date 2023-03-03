#include "math.h"
#include "stdint.h"
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main() {
    pid_t pid = fork();
    if (pid == 0) {
        pid_t pid2 = fork();
        if (pid2 == 0) {
            exit(0);
            sleep(100);
        } else {
            exit(0);
            sleep(60);
        }
    } else {
        int wstat;
        wait(&wstat);
        if (WEXITSTATUS(wstat)) {
            printf("1\n");
        }
    }
}
