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

int create_listener(char *port) {
    struct addrinfo *res = NULL;
    int gai_err;
    struct addrinfo hint = {
        .ai_family = AF_INET6,
        .ai_socktype = SOCK_STREAM,
    };
    if ((gai_err = getaddrinfo(NULL, port, &hint, &res))) {
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
        int one = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
        if (bind(sock, ai->ai_addr, ai->ai_addrlen) < 0) {
            perror("bind");
            close(sock);
            sock = -1;
            continue;
        }
        if (listen(sock, SOMAXCONN) < 0) {
            perror("listen");
            close(sock);
            sock = -1;
            continue;
        }
        break;
    }
    freeaddrinfo(res);
    return sock;
}

void handler(int signal) {
    exit(0);
}

int main(int argc, char *argv[]) {
    signal(SIGCHLD, SIG_IGN);
    struct sigaction sigact = {
        .sa_handler = handler,
        .sa_flags = SA_RESTART,
    };
    sigaction(SIGTERM, &sigact, NULL);
    if (argc < 3) {
        perror("needs more args\n");
        return 1;
    }
    int sock = create_listener(argv[1]);
    if (sock < 0) {
        return 1;
    }
    while (1) {
        int connection = accept(sock, NULL, NULL);
        if (!fork()) {
            dup2(connection, STDIN_FILENO);
            dup2(connection, STDOUT_FILENO);
            close(connection);
            close(sock);
            execvp(argv[2], argv + 2);
            _exit(1);
        } else {
            close(connection);
        }
    }
    close(sock);
}