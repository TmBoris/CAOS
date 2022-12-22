#include <stdio.h>
#include <stdint.h>

int main(void) {
    int64_t num;
    int res = scanf("%llx", &num);
    while (res > 0) {
        int64_t zero = 0;
        int64_t first = 1;
        if (num > zero) {
            int64_t i = 0;
            while (num >= first) {
                int64_t tmp = first;
                first = zero + first;
                zero = tmp;
                i += 1;
                if (first < zero) {
                    break;
                }
            }
            i -= 1;
            printf("%llx ", i);
        } else {
            int64_t tmp = -1;
            printf("%llx ", tmp);
        }
        res = scanf("%llx", &num);
    }
    printf("\n");
    return 0;
}