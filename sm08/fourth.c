#include "math.h"
#include "stdint.h"
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct Node {
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

void reead(int fd, int32_t offset) {
    struct Node node[1];
    ssize_t res = pread(fd, node, sizeof(*node), offset * sizeof(*node));
    if (res < 0) {
        perror("error while reading from file");
        exit(1);
    }
    if (node->right_idx != 0) {
        reead(fd, node->right_idx);
    }
    printf("%d ", node->key);
    if (node->left_idx != 0) {
        reead(fd, node->left_idx);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("needs more args");
        exit(1);
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("error opening file");
        exit(1);
    }
    reead(fd, 0);
    printf("\n");
    close(fd);
    return 0;
}