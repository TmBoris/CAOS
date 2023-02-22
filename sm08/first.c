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

enum {
    TOO_BIG_WORD = 62,
    BAD_SYMBOL1 = 32,
    BAD_SYMBOL2 = 127,
    SMALL_BUF_SIZE = 100,

};

void word_pipeline(int64_t *int_res, double *d_res, bool *flaot_was,
                   char *word) {
    bool is_int = true;
    bool have_digit = false;
    if (isdigit(word[0])) {
        have_digit = true;
    }
    if (!(word[0] == '+' || word[0] == '-' || isdigit(word[0]))) {
        is_int = false;
    }
    for (size_t i = 1; i < strlen(word); ++i) {
        if (!isdigit(word[i])) {
            is_int = false;
        } else {
            have_digit = true;
        }
    }
    if (is_int && have_digit) {
        errno = 0;
        long res = strtol(word, NULL, 10);
        if (errno != ERANGE) {
            long zero = 0;
            int32_t check;
            if (!__builtin_add_overflow(res, zero, &check)) {
                if (res > *int_res) {
                    *int_res = res;
                }
            }
        }
    } else {
        if (!isnan(*d_res)) {
            errno = 0;
            char *end;
            double res = strtod(word, &end);
            if (!(errno == ERANGE || *end != '\0')) {
                *flaot_was = true;
                if (res < *d_res || isnan(res)) {
                    *d_res = res;
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int64_t int_res = INT64_MIN;
    double d_res = INFINITY;
    FILE *file;
    if (argc < 2) {
        file = stdin;
    } else if (argc == 2) {
        file = fopen(argv[1], "rb");
        if (!file) {
            fprintf(stderr, "%s", "File opening failed\n");
            return 1;
        }
    } else {
        fprintf(stderr, "%s", "Too much args\n");
        return 1;
    }
    int cur[1];
    char small_buf[SMALL_BUF_SIZE];
    int i = 0;
    bool float_was = false;
    while ((*cur = getc(file)) != EOF) {
        if (isspace(*cur)) {
            small_buf[i] = '\0';
            if (i > 0) {
                word_pipeline(&int_res, &d_res, &float_was, small_buf);
            }
            i = 0;
        } else if (i > TOO_BIG_WORD) {
            fprintf(stderr, "%s", "word is too long\n");
            return 1;
        } else if (*cur != '\t' && *cur != '\r' && *cur != '\n' &&
                   ((*cur >= 0 && *cur < BAD_SYMBOL1) || *cur == BAD_SYMBOL2)) {
            fprintf(stderr, "%s", "bad symbol\n");
            return 1;
        } else {
            small_buf[i++] = *cur;
        }
    }
    small_buf[i] = '\0';
    if (i > 0) {
        word_pipeline(&int_res, &d_res, &float_was, small_buf);
    }
    if (int_res == INT64_MIN || (d_res == INFINITY && float_was == false)) {
        fprintf(stderr, "%s", "no int or double\n");
        return 1;
    }
    printf("%d %.10g\n", (int)int_res, d_res);
    return 0;
}