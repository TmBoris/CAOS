#include "math.h"
#include "stdint.h"
#include <ctype.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef double (*Func)(double);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("neels 2 args");
        exit(1);
    }
    void *h = dlopen("libm.so.6", RTLD_LAZY);
    Func func = dlsym(h, argv[1]);
    double num;
    int cur;
    while ((cur = scanf("%lf", &num)) == 1) {
        printf("%.10g\n", func(num));
    }
    dlclose(h);
    return 0;
}