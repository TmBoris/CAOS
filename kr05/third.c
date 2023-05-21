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
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <netdb.h>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }
    char *str = "tcp";
    struct servent *sv = getservbyname(argv[1], str);
    if (!sv) {
        printf("Non serviam.\n");
    } else {
        printf("%d\n", ntohs(sv->s_port));
    }
    return 0;
}