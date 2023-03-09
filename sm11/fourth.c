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
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

enum { BADEXITCODE = 7 };

int main(int argc, char *argv[]) {
    if (argc < 2) {
        perror("need more arguments");
        exit(1);
    }
    pid_t main_pd = getpid();
    char path[PATH_MAX];
    char *dir = getenv("XDG_RUNTIME_DIR");
    if (dir == NULL) {
        dir = getenv("TMPDIR");
    }
    if (dir == NULL) {
        dir = "/tmp";
    }
    int size = snprintf(path, PATH_MAX, "%s/%dtmp.py", dir, main_pd);
    if (size > PATH_MAX) {
        perror("directory name is too long");
        exit(1);
    }
    int fd = open(path, O_CREAT | O_TRUNC | O_RDWR, 0700);
    dprintf(fd, "%s\n", "#!/usr/bin/env python3\n");
    dprintf(fd, "%s\n", "import os\n");
    dprintf(fd, "%s\n", "import sys\n");
    dprintf(fd, "%s", "print(");
    dprintf(fd, "%s", argv[1]);
    for (size_t i = 2; i < argc; ++i) {
        dprintf(fd, " * %s", argv[i]);
    }
    dprintf(fd, "%s\n", ")");
    dprintf(fd, "%s\n", "os.remove(sys.argv[0])");
    close(fd);
    execlp(path, path, NULL);
    return BADEXITCODE;
}