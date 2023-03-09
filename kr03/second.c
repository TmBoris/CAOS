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
#include <unistd.h>

int separate(int fd_in, int fd_out_even, int fd_out_odd) {
    ssize_t res;
    int num;
    errno = 0;
    while ((res = read(fd_in, &num, sizeof(int))) > 0) {
        if (num % 2 == 0) {
            res = write(fd_out_even, &num, sizeof(int));
        } else {
            res = write(fd_out_odd, &num, sizeof(int));
        }
        if (errno != 0) {
            return -1;
        }
    }
    if (errno != 0) {
        return -1;
    }
    return 0;
}
