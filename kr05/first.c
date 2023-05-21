#define _GNU_SOURCE
#include "math.h"
#include "stdint.h"
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

int create_connection(char *node, char *service) {
    struct addrinfo *res = NULL;
    int gai_err;
    struct addrinfo hint = {
        .ai_socktype = SOCK_DGRAM,  // но мы хотим поток (соединение)
    };
    if ((gai_err = getaddrinfo(node, service, &hint, &res))) {
        fprintf(stderr, "gai error: %s\n", gai_strerror(gai_err));
        return -1;
    }
    int sock = -1;
    for (struct addrinfo *ai = res; ai; ai = ai->ai_next) {
        sock = socket(ai->ai_family, ai->ai_socktype, 0);
        if (sock < 0) {
            perror("socket");
            continue;
        }
        if (connect(sock, ai->ai_addr, ai->ai_addrlen) < 0) {
            perror("connect");
            close(sock);
            sock = -1;
            continue;
        }
        break;
    }
    freeaddrinfo(res);
    return sock;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s NODE SERVICE\n", argv[0]);
        return 1;
    }
    while (1) {
        int sock = create_connection(argv[1], argv[2]);
        if (sock < 0) {
            return 1;
        }
        char *buf = "qwef";
        if (send(sock, buf, 3, 0) == -1) {
            return 1;
        }
        char info[100];
        if (recv(sock, info, 100, 0) == -1) {
            return 1;
        }
        printf("%s\n", info);
        if (strtol(info, NULL, 10) == 0) {
            break;
        }
        snprintf(argv[2], 100, "%s", info);
        close(sock);

    }
}