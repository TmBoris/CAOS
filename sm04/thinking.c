#include <ctype.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
//    int a;
//    int res = scanf("%d", &a);
//    printf("%d\n", res);
//    printf("%d", a);

//    int num;
//    char term;
//    if(scanf("%d%c", &num, &term) != 2 || term != '\n')
//        printf("failure\n");
//    else
//        printf("valid integer followed by enter key\n");
//    printf("%d", num);

    const char *str = "13467876556767";
    int a = atoi(str);
    printf("%d", a);
    return 0;
}
