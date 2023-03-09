#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    struct stat buf1;
    struct stat buf2;
    for (size_t i = 1; i < argc; ++i) {
        errno = 0;
        int res1 = stat(argv[i], &buf1);
        //        printf("%s %d\n ",strerror(errno), errno);
        //        errno = 0;
        int res2 = lstat(argv[i], &buf2);
        //        printf("%s\n",strerror(errno));
        //        printf("%s %d %d\n", argv[i], res1, res2);
        if (res1 != 0) {
            if (res2 == -1) {
                printf("%s %s\n", argv[i], "(missing)");
            } else if (errno == 2) {
                printf("%s %s\n", argv[i], "(broken symlink)");
            } else {
                printf("%s\n", argv[i]);
            }
        } else {
            printf("%s\n", argv[i]);
        }
    }
    return 0;
}