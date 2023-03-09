#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { BUFSIZE = 64 };

struct BufHolder {
    char buf[BUFSIZE];
};

typedef double (*Func)(double);

int main(int argc, char *argv[]) {
    if (argc < 4) {
        perror("needs more args\n");
        exit(1);
    }
    char *string_of_args = argv[3];
    void *h = dlopen(argv[1], RTLD_LAZY);
    size_t size = 0;
    struct BufHolder bh;
    for (int i = 1; i < strlen(string_of_args); ++i) {
        if (string_of_args[i] == 'i') {
            int inter = (int)strtol(argv[3 + i], NULL, 10);
            memcpy(bh.buf + size, &inter, sizeof(int));
            size += sizeof(int);
        } else if (argv[3][i] == 'd') {
            double d = strtod(argv[3 + i], NULL);
            memcpy(bh.buf + size, &d, sizeof(double));
            size += sizeof(double);
        } else {
            memcpy(bh.buf + size, &argv[i + 3], sizeof(char *));
            size += sizeof(char *);
        }
    }
    bh.buf[size] = '\0';
    if (string_of_args[0] == 'v') {
        void (*func)(struct BufHolder) = dlsym(h, argv[2]);
        func(bh);
    } else if (string_of_args[0] == 'i') {
        int (*func)(struct BufHolder) = dlsym(h, argv[2]);
        printf("%d\n", func(bh));
    } else if (string_of_args[0] == 'd') {
        double (*func)(struct BufHolder) = dlsym(h, argv[2]);
        printf("%.10g\n", func(bh));
    } else {
        char *(*func)(struct BufHolder) = dlsym(h, argv[2]);
        printf("%s\n", func(bh));
    }
    dlclose(h);
    return 0;
}