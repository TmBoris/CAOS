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

int create_connection(char *node, char *service) {
    struct addrinfo *res = NULL;
    int gai_err;
    struct addrinfo hint = {
        .ai_family = AF_UNSPEC,  // можно и AF_INET, и AF_INET6
        .ai_socktype = SOCK_STREAM,  // но мы хотим поток (соединение)
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

void handler(int signal) {
    _exit(0);
}

int main(int argc, char *argv[]) {

    struct sigaction sigact = {
        .sa_handler = handler,
        .sa_flags = SA_RESTART,
    };
    sigaction(SIGPIPE, &sigact, NULL);

    if (argc != 4) {
        fprintf(stderr, "Usage: %s NODE SERVICE KEY\n", argv[0]);
        return 1;
    }
    int sock = create_connection(argv[1], argv[2]);
    if (sock < 0) {
        return 0;
    }
    int helper1 = dup(sock);
    int helper2 = dup(sock);
    FILE *writer = fdopen(sock, "w");
    if (fprintf(writer, "%s\n", argv[3]) <= 0) {
        exit(0);
    }
    fflush(writer);
    size_t k;
    FILE *reader = fdopen(helper1, "r");
    if (fscanf(reader, "%ld", &k) != 1) {
        exit(0);
    }
    fclose(reader);
    for (size_t i = 0; i <= k; ++i) {
        if (fprintf(writer, "%ld\n", i) <= 0) {
            exit(0);
        }
        fflush(writer);
    }
    uint64_t ans;
    reader = fdopen(helper2, "r");
    if (fscanf(reader, "%ld", &ans) != 1) {
        exit(0);
    }
    fclose(reader);
    printf("%ld\n", ans);
    fflush(stdout);
    fclose(writer);
}