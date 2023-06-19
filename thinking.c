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
    char* a = "abacaba";
    char b[100];
    strcpy(b, a);
    char c[100];
    snprintf(c, 100, "%s", a);
    for (size_t i = 0; i < 7; ++i) {
        if (c[i] == *(b + i)) {
            printf("1\n");
        }
    }
    return 0;
}