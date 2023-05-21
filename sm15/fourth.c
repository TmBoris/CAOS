// #define _GNU_SOURCE
#include "math.h"
#include "stdatomic.h"
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
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

enum { NUMOFITERS = 8, START = 100000 };

clock_t start;
volatile atomic_uint counter = 0;
volatile atomic_uint last_prime = 0;

bool isprime(uint32_t a) {
    int k = 2;
    while (k * k <= a) {
        if (a % k == 0) {
            return false;
        }
        ++k;
    }
    return true;
}

void print_num(int signo) {
    counter++;
    printf("%u\n", last_prime);
    fflush(stdout);
    if (counter == NUMOFITERS) {
        //        clock_t stop = clock();
        //        double elapsed = (double)(stop - start) / CLOCKS_PER_SEC;
        //        printf("\nTime elapsed: %.5f\n", elapsed);
        exit(0);
    }
}

int main(void) {
    //    start = clock();
    uint32_t low;
    uint32_t high;
    if (scanf("%u", &low) != 1) {
        perror("needs two numbers\n");
        exit(1);
    }
    if (scanf("%u", &high) != 1) {
        perror("needs two numbers\n");
        exit(1);
    }

    struct itimerval it_val;

    struct sigaction sigact = {
        .sa_handler = print_num,
        .sa_flags = SA_RESTART,
    };
    sigaction(SIGVTALRM, &sigact, NULL);
    it_val.it_value.tv_sec = 0;
    it_val.it_value.tv_usec = START;
    it_val.it_interval = it_val.it_value;
    if (setitimer(ITIMER_VIRTUAL, &it_val, NULL) == -1) {
        perror("error calling setitimer()");
        exit(1);
    }

    for (uint32_t i = low; i < high; ++i) {
        if (isprime(i)) {
            last_prime = i;
        }
    }
    if (counter != NUMOFITERS) {
        printf("%d\n", -1);
        fflush(stdout);
    }
    //    clock_t stop = clock();
    //    double elapsed = (double)(stop - start) / CLOCKS_PER_SEC;
    //    printf("\nTime elapsed: %.5f\n", elapsed);
    return 0;
}
