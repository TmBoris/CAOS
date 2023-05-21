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
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("needs 2 args\n");
        exit(1);
    }
    FILE *file = fopen("tmp.c", "w+");
    fprintf(file, "void foo() {\n");
    fprintf(file, "%s;\n", argv[1]);
    fprintf(file, "}\n");
    fprintf(file, "int main() {\n");
    fprintf(file, "foo();\n");
    fprintf(file, "}\n");
    fclose(file);
    pid_t pid;
    if (!(pid = fork())) {
        int fd = open("/dev/null", O_WRONLY);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        close(fd);
        execlp("gcc", "gcc", "-x", "c", "-c", "-o", "/dev/null", "tmp.c", NULL);
        _exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 1) {
            printf("Invalid\n");
        } else {
            printf("Valid\n");
        }
    }
    return 0;
}