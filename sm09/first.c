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
    ZERO = 0,
    ONE = 1,
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9,
    TEN = 10,
    ELEVEN = 11,
    SUID = 0b100000000000,
    SGID = 0b010000000000,
    STICKY = 0b001000000000
};

const char *perms_to_str(char *buf, size_t size, int perms) {
    if (size == ZERO) {
        return buf;
    }
    char tmp_buf[TEN];
    tmp_buf[NINE] = '\0';
    int others = (perms % EIGHT);
    int group = (perms / EIGHT) % EIGHT;
    int owner = (perms / (EIGHT * EIGHT)) % EIGHT;
    // others
    if (((others & TWO) >> ONE) == ONE) {
        tmp_buf[SEVEN] = 'w';
    } else {
        tmp_buf[SEVEN] = '-';
    }
    if ((others & ONE) == ONE) {
        if (tmp_buf[SEVEN] == 'w' && ((perms & STICKY) >> NINE) == 1) {
            tmp_buf[EIGHT] = 't';
        } else {
            tmp_buf[EIGHT] = 'x';
        }
    } else {
        tmp_buf[EIGHT] = '-';
    }
    if (((others & FOUR) >> TWO) == ONE) {
        tmp_buf[SIX] = 'r';
    } else {
        tmp_buf[SIX] = '-';
    }

    // group
    if ((group & ONE) == ONE) {
        tmp_buf[FIVE] = 'x';
    } else {
        tmp_buf[FIVE] = '-';
    }
    if ((tmp_buf[EIGHT] == 'x' || tmp_buf[EIGHT] == 't') &&
        ((perms & SGID) >> TEN) == 1) {
        tmp_buf[FIVE] = 's';
    }
    if (((group & TWO) >> ONE) == ONE) {
        tmp_buf[FOUR] = 'w';
    } else {
        tmp_buf[FOUR] = '-';
    }
    if (((group & FOUR) >> TWO) == ONE) {
        tmp_buf[THREE] = 'r';
    } else {
        tmp_buf[THREE] = '-';
    }

    // owner
    if ((owner & ONE) == ONE) {
        tmp_buf[TWO] = 'x';
    } else {
        tmp_buf[TWO] = '-';
    }
    if ((tmp_buf[FIVE] == 'x' || tmp_buf[FIVE] == 's' ||
         tmp_buf[EIGHT] == 'x' || tmp_buf[EIGHT] == 't') &&
        ((perms & SUID) >> ELEVEN) == 1) {
        tmp_buf[TWO] = 's';
    }
    if (((owner & TWO) >> ONE) == ONE) {
        tmp_buf[ONE] = 'w';
    } else {
        tmp_buf[ONE] = '-';
    }
    if (((owner & FOUR) >> TWO) == ONE) {
        tmp_buf[ZERO] = 'r';
    } else {
        tmp_buf[ZERO] = '-';
    }
    size_t trans = size - ONE;
    if (strlen(tmp_buf) < trans) {
        trans = strlen(tmp_buf);
    }

    memcpy(buf, tmp_buf, trans);
    buf[trans] = '\0';
    return buf;
}

// int main() {
//     char buf[16];
//     for (int i = 510; i < 07777; ++i) {
//         printf("%d %s\n", i,  perms_to_str(buf, sizeof(buf), i));
//     }
//     return 0;
// }