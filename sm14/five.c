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

enum { BUFSIZE = 1025, IPADDRSIZE = 100 };

int create_listener(char *service) {
    struct addrinfo *res = NULL;
    int gai_err;
    struct addrinfo hint = {
        .ai_family = AF_INET6,
        .ai_socktype = SOCK_DGRAM,
    };
    if ((gai_err = getaddrinfo(NULL, service, &hint, &res))) {
        fprintf(stderr, "gai error: %s\n", gai_strerror(gai_err));
        return -1;
    }
    int sock = -1;
    for (struct addrinfo *ai = res; ai; ai = ai->ai_next) {
        sock = socket(ai->ai_family, ai->ai_socktype, 0);
        if (sock < 0) {
            continue;
        }
        int one = 1;
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
        if (bind(sock, ai->ai_addr, ai->ai_addrlen) == 0) {
            break;
        }
    }
    freeaddrinfo(res);
    return sock;
}

void handler(int signal) {
    exit(0);
}

int main(int argc, char *argv[]) {
    struct sockaddr_in6 peer_addr;
    socklen_t peer_addr_len;
    struct sigaction sigact = {
        .sa_handler = handler,
        .sa_flags = SA_RESTART,
    };
    sigaction(SIGTERM, &sigact, NULL);
    if (argc != 2) {
        perror("input format: UDP_PORT\n");
        return 1;
    }
    int sock = create_listener(argv[1]);
    if (sock < 0) {
        return 1;
    }
    while (1) {
        char buf[BUFSIZE];
        peer_addr_len = sizeof(peer_addr);
        ssize_t nread = recvfrom(sock, buf, BUFSIZE, 0,
                                 (struct sockaddr *)&peer_addr, &peer_addr_len);
        if (nread == -1) {
            continue; /* Ignore failed request */
        }
        buf[nread] = '\0';
        struct in6_addr ia = peer_addr.sin6_addr;
        char ip_addr[IPADDRSIZE];
        inet_ntop(AF_INET6, &ia, ip_addr, IPADDRSIZE);
        printf("[%s:%d] %s\n", ip_addr, ntohs(peer_addr.sin6_port), buf);
        fflush(stdout);
    }
    close(sock);
}