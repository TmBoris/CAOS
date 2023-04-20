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


int main() {
    int fd[2];
    pipe(fd);

    FILE *reader = fdopen(fd[0], "r");
    FILE *writer = fdopen(fd[0], "w");
    fclose(reader);
    FILE *reader2 = fdopen(fd[1], "r");
    fprintf(writer, "%d\n", 10000);
//    int ans;
//    fscanf(reader2, "%d", &ans);
//    printf("%d\n", ans);
//    fclose(writer);
//    fclose(reader2);
    return 0;
}