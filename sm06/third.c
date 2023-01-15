#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    A = 0b10000000,
    B = 7,
    C = 0b11111,
    D = 0x400,
    E = 0x4FF,
    F = 0b11100000,
    G = 0b11111,
    H = 0b11110000,
    I = 0b11111000,
    J = 0b11110,
    K = 14
};

int main(void) {
    int c;
    int kir_coun = 0;
    int all_coun = 0;
    int cur_kir_symb = 0;
    bool flag = false;
    while ((c = getchar()) != EOF) {
        //        printf("%c", c);
        if (((A & c) >> B) == 0) {  // 0*******
            all_coun++;
            flag = false;
        } else if ((((3 << 6) & c) >> 6) == 2 && flag) {  // 10******
            cur_kir_symb += (C & c);
            if (D <= cur_kir_symb && cur_kir_symb <= E) {
                kir_coun++;
            }
            cur_kir_symb = 0;
        } else if ((F & c) >> 5 == 6) {  // 110*****
            all_coun++;
            cur_kir_symb += (G & c) << 6;
            flag = true;
        } else if ((H & c) >> 4 == K) {  // 1110****
            all_coun++;
            flag = false;
        } else if ((I & c) >> 3 == J) {  // 11110***
            all_coun++;
            flag = false;
        }
    }
    //    printf("1");
    printf("%d %d\n", all_coun, kir_coun);
    return 0;
}