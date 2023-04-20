#define _GNU_SOURCE
#include "math.h"
#include "stdint.h"
#include <arpa/inet.h>
#include <ctype.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <netdb.h>
#include <setjmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

enum { MAX_SIZE = 1000 };

int main(int argc, char *argv[]) {
    char host[MAX_SIZE];
    char service[MAX_SIZE];
    while (scanf("%s %s", host, service) == 2) {
        struct addrinfo *res = NULL;
        struct addrinfo hint = {
            .ai_family = AF_INET,  // можно и AF_INET, и AF_INET6
        };
        int gai_err;
        if ((gai_err = getaddrinfo(host, service, &hint, &res))) {
            printf("%s\n", gai_strerror(gai_err));
            continue;
        }
        int64_t min = INT64_MAX;
        struct in_addr min_addr;
        uint16_t port = 0;
        for (struct addrinfo *ai = res; ai; ai = ai->ai_next) {
            struct sockaddr_in *inet;
            inet = (struct sockaddr_in *)ai->ai_addr;
            if (ntohl(inet->sin_addr.s_addr) < min) {
                min = ntohl(inet->sin_addr.s_addr);
                min_addr = inet->sin_addr;
                port = ntohs(inet->sin_port);
            }
        }
        printf("%s:%hu\n", inet_ntoa(min_addr), port);
        freeaddrinfo(res);
    }
}