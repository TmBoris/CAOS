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
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    FILE *file = fopen("input.txt", "w+");
    double tmp[] = {1.5, 2.5};
    fwrite(tmp, sizeof(tmp), sizeof(tmp) / sizeof(double), file);
    fclose(file);
    return 0;
}