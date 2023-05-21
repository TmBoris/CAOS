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
#include <time.h>
#include <unistd.h>

enum { SECINDAY = 86400 };

int main() {
    int32_t after_days;
    while ((scanf("%d", &after_days)) == 1) {
        time_t now = time(NULL);
        int32_t sec_in_day = SECINDAY;
        int32_t tmp;
        if (__builtin_smul_overflow(after_days, sec_in_day, &tmp)) {
            printf("OVERFLOW\n");
        } else if (__builtin_sadd_overflow(now, after_days * sec_in_day,
                                           &tmp)) {
            printf("OVERFLOW\n");
        } else {
            now += after_days * sec_in_day;
            struct tm *ans = localtime(&now);
            char buf[BUFSIZ];
            if (strftime(buf, BUFSIZ, "%Y-%m-%d", ans) == 0) {
                perror("strftime\n");
                exit(1);
            }
            printf("%s\n", buf);
        }
    }

    return 0;
}