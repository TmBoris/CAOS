#define _GNU_SOURCE
#include "math.h"
#include "stdint.h"
#include <ctype.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signalfd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void foo(int process_number, int reader, int n, int sfd) {
    //    printf("%d %d %d %d\n", process_number, process_number,
    //    process_number,
    //           process_number);
    pid_t other_process_pid;
    ssize_t s;
    size_t i = 0;
    struct signalfd_siginfo fdsi;
    for (;;) {
        s = read(sfd, &fdsi, sizeof(struct signalfd_siginfo));
        //        printf("%d %d %d %d\n", process_number, process_number,
        //        process_number,
        //               process_number);
        if (s != sizeof(struct signalfd_siginfo)) {
            perror("read");
            exit(1);
        }
        if (fdsi.ssi_signo == SIGUSR1) {
            if (i == 0) {
                //                printf("SIGUSR2 started\n");
                //                fflush(stdout);
                if (read(reader, &other_process_pid, sizeof(pid_t)) == -1) {
                    perror("read\n");
                    exit(1);
                }
                //                printf("SIGUSR2 finished\n");
                //                fflush(stdout);
                ++i;
            } else {
                //                printf("SIGUSR1 started\n");
                //                fflush(stdout);
                int num;
                if (read(reader, &num, sizeof(num)) == -1) {
                    perror("scanf\n");
                    exit(1);
                }
                if (num > n) {
                    exit(0);
                }
                printf("%d %d\n", process_number, num);
                fflush(stdout);
                if (num == n) {
                    kill(other_process_pid, SIGUSR1);
                    exit(0);
                }
            }
            //            printf("my process pid:%d\nother process pid:%d\n",
            //            getpid(), other_process_pid); printf("process %d
            //            killed other process\n", process_number);
            //            fflush(stdout);
            kill(other_process_pid, SIGUSR1);
            //            printf("killing succeed\n");
            //            fflush(stdout);
        } else {
            perror("Read unexpected signal\n");
            exit(1);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("needs two args\n");
        exit(1);
    }
    signal(SIGPIPE, SIG_IGN);
    int n = strtol(argv[1], NULL, 10);

    int pipds[2];
    if (pipe(pipds) == -1) {
        perror("pipe failed\n");
        exit(1);
    }

    pid_t pids[2];
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);

    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1) {
        perror("sigprocmask");
        exit(1);
    }

    int sfd = signalfd(-1, &mask, 0);
    if (sfd == -1) {
        perror("signalfd");
        exit(1);
    }

    if (!(pids[0] = fork())) {
        foo(1, pipds[0], n, sfd);
    }
    if (!(pids[1] = fork())) {
        foo(2, pipds[0], n, sfd);
    }

    // giving value of the pid of other process
    //    printf("pids[1]:%d\npids[0]:%d\n", pids[1], pids[0]);
    //    fflush(stdout);

    if (write(pipds[1], &pids[1], sizeof(pid_t)) == -1) {
        perror("write\n");
        exit(1);
    }
    if (write(pipds[1], &pids[0], sizeof(pid_t)) == -1) {
        perror("write\n");
        exit(1);
    }

    for (int i = 1; i <= n + 1; ++i) {
        if (write(pipds[1], &i, sizeof(int)) == -1) {
            perror("write\n");
            exit(1);
        }
        //        printf("%d number printed to pipe\n", i);
        //        fflush(stdout);
    }

    kill(pids[0], SIGUSR1);
    wait(NULL);
    wait(NULL);
    close(pipds[0]);
    close(pipds[1]);
    return 0;
}