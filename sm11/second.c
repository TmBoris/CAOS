#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

void reader(pid_t main_pd) {
    int num;
    int res = scanf("%d", &num);
    if (res < 1) {
        return;
    }
    pid_t pid = fork();
    if (pid < 0) {
        printf("-1\n");
        exit(3);
    } else if (pid == 0) {
        reader(main_pd);
    } else {
        int wstat;
        wait(&wstat);
        pid_t curr_pd = getpid();
        if (WIFEXITED(wstat) && WEXITSTATUS(wstat) == 0) {
            printf("%d\n", num);
            exit(0);
        } else if (WIFEXITED(wstat) && WEXITSTATUS(wstat) == 3 &&
                   main_pd == curr_pd) {
            exit(0);
        } else {
            exit(3);
        }
    }
}

int main() {
    pid_t main_pd = getpid();
    reader(main_pd);
    return 0;
}
