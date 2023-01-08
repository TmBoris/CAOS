#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct BSearchResult {
    size_t low;
    size_t high;
    int result;
};

struct BSearchResult
bsearch2(const void *key, const void *base, size_t nmemb, size_t size,
         int (*compar)(const void *p1, const void *p2, void *user),
         void *user) {
    struct BSearchResult ans = {0, 0, 0};
    if (nmemb == 0 || compar(key, base, user) < 0) {
        return ans;
    } else if (compar(key, base + (nmemb - 1) * size, user) > 0) {
        ans.high = nmemb;
        ans.low = nmemb;
        ans.result = 0;
        return ans;
    }
    bool was = false;

    size_t l = 0;
    size_t r = nmemb - 1;
    while (l != r) {
        size_t m = l / 2 + r / 2;
        if (compar(key, base + m * size, user) < 0) {
            r = m;
        } else if (compar(key, base + m * size, user) == 0) {
            was = true;
            r = m;
        } else {
            l = m + 1;
        }
    }
    ans.low = l;

    l = 0;
    r = nmemb - 1;
    while (l != r) {
        size_t m = l / 2 + r / 2 + 1;
        if (compar(key, base + m * size, user) < 0) {
            r = m - 1;
        } else {
            l = m;
        }
    }
    if (compar(key, base + l * size, user) <= 0) {
        ans.high = l + 1;
    } else {
        ans.high = l;
    }

    if (was) {
        ans.result = 1;
    } else {
        ans.result = 0;
    }
    return ans;
}

// int cmp(const int *a, const int *b, int *user) {
//     if (*a > *b) {
//         return 1;
//     } else if (*a == *b) {
//         return 0;
//     } else {
//         return -1;
//     }
// }
//
//  int main(void) {
//      int arr[11] = {1, 2, 3, 4, 5, 7, 8, 8, 8, 10, 10};
//      int key = 11;
//      size_t nmemb = 11;
//      size_t size = 4;
//      int user = 0;
//
//      struct BSearchResult ans = bsearch2(&key, arr, nmemb, size, (int
//      (*)(const void *, const void *, void *)) &cmp, &user); printf("%d ",
//      ans.result); printf("%zu ", ans.low); printf("%zu ", ans.high);
//
//      // попробовать заменить на l/2 + r/2 и тестить тестить тестить
//
//  }