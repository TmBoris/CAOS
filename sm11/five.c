#include "math.h"
#include "stdint.h"
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

enum { MINSIZEOFBUF = 10, SIGNALCONST = 1024 };

struct Argv {
    char *buf;
};

size_t max(size_t one, size_t two) {
    if (one < two) {
        return two;
    }
    return one;
}

int strip_spaces(char *buf) {
    char *prev = buf;
    while (isspace(*buf) && *buf != '\0') {
        ++buf;
    }
    if (*buf == '\0') {
        *prev = '\0';
        return 0;
    }
    int counter = 0;
    *(prev) = *(buf++);
    while (*buf != '\0') {
        if (isspace(*prev) && isspace(*buf)) {
            ++buf;
        } else if (isspace(*buf)) {
            *(++prev) = ' ';
            ++buf;
            ++counter;
        } else {
            *(++prev) = *(buf++);
        }
    }
    if (isspace(*prev)) {
        *prev = '\0';
    } else {
        *(++prev) = '\0';
    }
    return counter;
}

int mysystem(const char *cmd) {
    char *argstr = calloc(strlen(cmd) + 1, sizeof(*argstr));
    int size = snprintf(argstr, (strlen(cmd) + 1) * sizeof(*argstr), "%s", cmd);
    if (size > (strlen(cmd) + 1) * sizeof(*argstr)) {
        perror("snprintf failed");
        exit(1);
    }
    int spaces = strip_spaces(argstr);
    if (strlen(argstr) == 0) {
        return -1;
    }
    struct Argv *argv = calloc(spaces + 2, sizeof(*argv));
    for (size_t i = 0; i < spaces + 2; ++i) {
        argv[i].buf = NULL;
    }
    size_t i = 0;
    size_t j = 0;
    size_t cur_size = 0;
    size_t buf_size = 0;
    while (argstr[i] != '\0') {
        if (isspace(argstr[i])) {
            if (cur_size + 1 >= buf_size) {
                char *tmp = realloc(argv[j].buf, cur_size + 1);
                if (!tmp) {
                    perror("can't realloc space");
                    exit(1);
                }
                argv[j].buf = tmp;
            }
            argv[j].buf[cur_size] = '\0';
            cur_size = 0;
            buf_size = 0;
            j++;
        } else {
            if (cur_size + 1 >= buf_size) {
                char *tmp =
                    realloc(argv[j].buf, max(MINSIZEOFBUF, cur_size * 2));
                if (!tmp) {
                    perror("can't realloc space");
                    exit(1);
                }
                argv[j].buf = tmp;
                buf_size = max(MINSIZEOFBUF, cur_size * 2);
            }
            argv[j].buf[cur_size] = argstr[i];
            cur_size++;
        }
        ++i;
    }
    if (cur_size + 1 >= buf_size) {
        char *tmp = realloc(argv[j].buf, cur_size + 1);
        if (!tmp) {
            perror("can't realloc space");
            exit(1);
        }
        argv[j].buf = tmp;
    }
    argv[j].buf[cur_size] = '\0';
    free(argstr);
    pid_t pid = fork();
    if (pid < 0) {
        return -1;
    } else if (pid == 0) {
        int res = execvp(argv[0].buf, (char(**))argv);
        if (res == -1) {
            perror("execvp failed");
            _exit(1);
        }
    } else {
        int status;
        waitpid(pid, &status, 0);
        for (size_t t = 0; t < spaces + 2; ++t) {
            free(argv[t].buf);
        }
        free(argv);
        if (WIFSIGNALED(status)) {
            return WTERMSIG(status) + SIGNALCONST;
        } else {
            return WEXITSTATUS(status);
        }
    }
    return 0;
}

// int main(void) {
//     mysystem("ls .");
//     return 0;
// }