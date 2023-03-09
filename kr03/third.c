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
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct Node {
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

void reead(struct Node *ptr, int32_t offset) {
    struct Node node = ptr[offset];
    if (node.right_idx != 0) {
        reead(ptr, node.right_idx);
    }
    printf("%d ", node.key);
    if (node.left_idx != 0) {
        reead(ptr, node.left_idx);
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
    struct stat buf;
    stat(argv[1], &buf);

    struct Node *ptr = mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
    reead(ptr, 0);
    printf("\n");
    close(fd);
    return 0;
}