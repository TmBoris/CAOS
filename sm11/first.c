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
        pid_t spid = fork();
        if (spid == 0) {
            printf("3 ");
        } else {
            int wstat2;
            pause();
            wait(&wstat2);
            printf("2 ");
        }
    } else {
        int wstat;
        wait(&wstat);
        printf("1\n");
    }
    return 0;
}