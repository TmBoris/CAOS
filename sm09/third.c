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
    if (argc != 4) {
        perror("needs 3 args\n");
        exit(1);
    }

    // creating snake
    int rows = strtol(argv[2], NULL, 10);
    int cols = strtol(argv[3], NULL, 10);
    int fd = open(argv[1], O_TRUNC | O_CREAT | O_RDWR, 0600);
    struct stat buf;
    stat(argv[1], &buf);

    void *ptr = mmap(NULL, rows * cols * sizeof(int), PROT_WRITE | PROT_READ,
                     MAP_SHARED, fd, 0);

    int res = ftruncate(fd, cols * rows * sizeof(int));
    if (res < 0) {
        perror("can't truncate");
    }

    int left = -1;
    int ceil = -1;
    int right = cols;
    int floor = rows;
    int i = -1;
    int j = 0;
    int num = 1;
    while (true) {
        if (++i >= right) {
            break;
        }
        for (; i < right; ++i) {
            ((int *)ptr)[j * cols + i] = num++;
        }
        ceil++;
        i--;
        if (++j == floor) {
            break;
        }
        for (; j < floor; ++j) {
            ((int *)ptr)[j * cols + i] = num++;
        }
        right--;
        j--;
        if (--i == left) {
            break;
        }
        for (; i > left; --i) {
            ((int *)ptr)[j * cols + i] = num++;
        }
        floor--;
        i++;
        if (--j == ceil) {
            break;
        }
        for (; j > ceil; --j) {
            ((int *)ptr)[j * cols + i] = num++;
        }
        left++;
        j++;
    }
    munmap(ptr, buf.st_size);
    close(fd);
    return 0;
}