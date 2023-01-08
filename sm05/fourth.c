#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int ans = 0;
    int c;
    while ((c = getchar()) != EOF) {
        if (isdigit(c)) {
            ans += c - '0';
        }
    }
    printf("%d\n", ans);
    return 0;
}