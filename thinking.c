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

int main(void) {
    errno = 0;
    int res = dup2(-1, STDIN_FILENO);
    if (res == -1) {
        printf("11111111");
    }
    printf("11111111");
    printf("%s", strerror(errno));
    printf("22222222");
//    execlp("ls", "ls", NULL);
//    _exit(1);
    return 0;
}
