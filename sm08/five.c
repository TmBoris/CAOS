#include "dirent.h"
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
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void my_readdir_func(int deep, const char *path, const char *cut_path,
                     unsigned z) {
    if (deep > 3) {
        return;
    }
    DIR *dir = opendir(path);
    struct dirent *entry;
    if (!dir) {
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        char *full_tmp = malloc(strlen(path) + strlen(entry->d_name) + 2);
        memcpy(full_tmp, path, strlen(path));
        memcpy(full_tmp + strlen(path), "/", 1);
        memcpy(full_tmp + strlen(path) + 1, entry->d_name,
               strlen(entry->d_name));
        full_tmp[strlen(path) + 1 + strlen(entry->d_name)] = '\0';
        struct stat statbuf;
        int res = stat(full_tmp, &statbuf);
        if (res) {
            perror("error getting stat");
            exit(1);
        }
        if (deep == 0) {
            if ((!((strcmp(entry->d_name, ".") == 0 ||
                    (strcmp(entry->d_name, "..") == 0)))) &&
                entry->d_type == DT_DIR) {
                my_readdir_func(deep + 1, full_tmp, entry->d_name, z);
            }

            if ((!((strcmp(entry->d_name, ".") == 0 ||
                    (strcmp(entry->d_name, "..") == 0)))) &&
                entry->d_type == DT_REG && access(full_tmp, R_OK) == 0 &&
                statbuf.st_size <= z) {
                printf("%s\n", entry->d_name);
            }

        } else {
            char *tmp = malloc(strlen(cut_path) + strlen(entry->d_name) + 2);
            memcpy(tmp, cut_path, strlen(cut_path));
            memcpy(tmp + strlen(cut_path), "/", 1);
            memcpy(tmp + strlen(cut_path) + 1, entry->d_name,
                   strlen(entry->d_name));
            tmp[strlen(cut_path) + 1 + strlen(entry->d_name)] = '\0';
            if ((!((strcmp(entry->d_name, ".") == 0 ||
                    (strcmp(entry->d_name, "..") == 0)))) &&
                entry->d_type == DT_DIR) {
                my_readdir_func(deep + 1, full_tmp, tmp, z);
            }
            if ((!((strcmp(entry->d_name, ".") == 0 ||
                    (strcmp(entry->d_name, "..") == 0)))) &&
                entry->d_type == DT_REG && access(full_tmp, R_OK) == 0 &&
                statbuf.st_size <= z) {
                printf("%s\n", tmp);
            }
            free(tmp);
        }
        free(full_tmp);
    }
    closedir(dir);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        exit(-1);
    }
    my_readdir_func(0, argv[1], "", strtol(argv[2], NULL, 10));
}