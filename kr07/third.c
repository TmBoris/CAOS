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
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

enum { NSECINMSEC = 1000, NSECINSEC = 1000000000 };

void handler(int signal) {
    exit(0);
}

int main() {
    struct sigaction sigact = {
        .sa_handler = handler,
        .sa_flags = SA_RESTART,
    };
    sigaction(SIGALRM, &sigact, NULL);
    struct timespec time;

    if ((scanf("%ld %ld", &time.tv_sec, &time.tv_nsec)) != 2) {
        perror("scanf failed\n");
        exit(1);
    }

    struct timespec now;
    timespec_get(&now, TIME_UTC);

    if (time.tv_sec < now.tv_sec ||
        (time.tv_sec == now.tv_sec && time.tv_nsec < now.tv_nsec)) {
        exit(0);
    }

    struct itimerval st;
    st.it_value.tv_sec = time.tv_sec - now.tv_sec;
    if (time.tv_nsec >= now.tv_nsec) {
        st.it_value.tv_usec = (time.tv_nsec - now.tv_nsec) / NSECINMSEC;
    } else {
        st.it_value.tv_usec =
            (NSECINSEC + time.tv_nsec - now.tv_nsec) / NSECINMSEC;
        --st.it_value.tv_sec;
    }
    st.it_interval.tv_sec = 0;
    st.it_interval.tv_usec = 0;

    if (setitimer(ITIMER_REAL, &st, NULL) == -1) {
        exit(0);
    }

    pause();

    return 0;
}