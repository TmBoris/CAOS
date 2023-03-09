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

int main(int argc, char *argv[]) {
    pid_t pid = fork();
    if (pid < 0) {
        _exit(1);
    } else if (!pid) {
        int fd = open(argv[2], O_RDONLY);
        if (fd < 0) {
            _exit(1);
        }
        dup2(fd, 0);
        close(fd);

        fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (fd < 0) {
            _exit(1);
        }
        dup2(fd, 1);
        close(fd);

        execlp(argv[1], argv[1], NULL);
        _exit(1);
    } else {
        int status;
        wait(&status);
    }
}