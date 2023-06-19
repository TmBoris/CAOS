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
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

enum { SUNPATHSIZE = 108 };

int make_named_socket(const char *filename) {
    struct sockaddr_un name;
    int sock;
    size_t size;
    sock = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    name.sun_family = AF_FILE;
    snprintf(name.sun_path, SUNPATHSIZE, "%s", filename);
    printf("%s\n", name.sun_path);
    size = (offsetof(struct sockaddr_un, sun_path) + strlen(name.sun_path) + 1);
    if (connect(sock, (struct sockaddr *)&name, size) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    return sock;
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s MSG COUNT SOCKETS...\n", argv[0]);
        return 1;
    }
    int *sockets = calloc(argc - 3, sizeof(int));
    for (size_t i = 3; i < argc; ++i) {
        sockets[i - 3] = make_named_socket(argv[i]);
    }
    int n = strlen(argv[1]);
    int goal = strtol(argv[2], NULL, 10);
    int *cnts = calloc(argc, sizeof(*cnts));
    while (1) {
        for (size_t i = 3; i < argc; ++i) {
            int res;
            if ((res = write(sockets[i - 3], argv[1] + (cnts[i] % n),
                             n - (cnts[i] % n))) == -1) {
                continue;
            }
            cnts[i] += res;
            if (cnts[i] == goal * n) {
                free(cnts);
                free(sockets);
                return 0;
            }
        }
    }
}