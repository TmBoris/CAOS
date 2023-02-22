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
    if (argc != 2) {
        perror("There must be two argc");
        exit(1);
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("can't open the file");
        exit(1);
    }
    struct stat buf;
    stat(argv[1], &buf);
    int length = buf.st_size / sizeof(double);
    void *ptr = mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    size_t counter = 0;
    double sum = 0;
    for (size_t i = 0; i < length; ++i) {
        sum += ((double *)ptr)[i];
        ++counter;
    }
    if (counter == 0) {
        printf("%a\n", 0.0);
    }
    printf("%a\n", sum / (double)counter);
    munmap(ptr, buf.st_size);
    close(fd);
    return 0;
}