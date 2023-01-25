#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { ERROR = 13, MAX_SIZE = 10 };

typedef struct Stack {
    int arr[MAX_SIZE];
    jmp_buf buf[MAX_SIZE];
} Stack;

Stack stack;
int i = 0;
bool last = false;

jmp_buf *try(int exception) {
    if (!last) {
        ++i;
    }
    last = true;
    stack.arr[i] = exception;
    return &stack.buf[i++];
}

void endtry() {
    if (last) {
        --i;
    }
    if (i == 0) {
        exit(ERROR);
    }
    --i;
    last = false;
}

void throw(int exception) {
    if (last) {
        --i;
    }
    bool flag = false;
    while (i >= 0) {
        if (stack.arr[i] == exception) {
            flag = true;
            break;
        }
        --i;
    }
    if (!flag) {
        exit(ERROR);
    }
    stack.arr[i] = 0;
    last = false;
    longjmp(stack.buf[i], 1);
}

// int main(void) {
//     printf("%d\n", i);
//     if (!setjmp(*try(42))) {  // добавили обработчик
//         printf("%d\n", i);
//
//         puts("level 1");
//         if (!setjmp(*try(42))) {  // и ещё один
//             printf("%d\n", i);
//
//             puts("level 2");
//             if (!setjmp(*try(42))) {  // добавили обработчик
//                 printf("%d\n", i);
//
//                 puts("level 3");
//                 if (!setjmp(*try(1))) {  // и ещё один
//                     puts("level 4");  // выбросили исключение
//                     printf("%d\n", i);
//
//                     puts("this string is not displayed");
//                     endtry();
//                     printf("entry 4 %d\n", i);
//                 } else {
//                     puts("handle exception 1");
//                 }
//                 endtry();
//                 printf("entry 3 %d\n", i);
//             } else {
//                 puts("handle exception 4231478");
//             }
//             throw(42);  // выбросили исключение
//             puts("this string is not displayed");
//             endtry();
//         } else {
//             printf("handle exception 422 i = %d\n", i);
//         }
//         endtry();
//         if (!setjmp(*try(42))) {  // добавили обработчик
//             printf("%d\n", i);
//
//             puts("level 3");
//             if (!setjmp(*try(1))) {  // и ещё один
//                 puts("level 4");  // выбросили исключение
//                 printf("%d\n", i);
//
//                 puts("this string is not displayed");
//                 endtry();
//                 printf("entry 4 %d\n", i);
//             } else {
//                 puts("handle exception 1");
//             }
//             endtry();
//             printf("entry 3 %d\n", i);
//         } else {
//             puts("handle exception 4231478");
//         }
//     } else {
//         puts("handle exception 421");
//     }
//
//     return 0;
// }

// int main() {
//    if (!setjmp(*try(42))) { // добавили обработчик
//        puts("level 1");
//        if (!setjmp(*try(1))) { // и ещё один
//            puts("level 2");
//            throw(42); // выбросили исключение
//            puts("this string is not displayed");
//            endtry();
//        } else {
//            puts("handle exception 1");
//        }
//        endtry();
//    } else {
//        puts("handle exception 42");
//    }
//    if (!setjmp(*try(42))) { // добавили обработчик
//        puts("level 1");
//        if (!setjmp(*try(1))) { // и ещё один
//            puts("level 2");
//            throw(42); // выбросили исключение
//            puts("YOUR CODE IS NOT WORKING 1");
//            endtry();
//        } else {
//            puts("handle exception 1");
//        }
//        endtry();
//    } else {
//        puts("handle exception 42");
//    }
//    throw(1);
//    puts("YOUR CODE IS NOT WORKING 2");
//    return 0;
// }