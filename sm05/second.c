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
    size_t r = nmemb;
    while (l < r) {
        size_t m = l + (r - l) / 2;
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
    r = nmemb;
    while (l < r) {
        size_t m = l + (r - l) / 2;
        if (compar(key, base + m * size, user) < 0) {
            r = m;
        } else {
            l = m + 1;
        }
    }
    ans.high = l;

    if (was) {
        ans.result = 1;
    } else {
        ans.result = 0;
    }
    return ans;
}

// int cmp(const int *a, const int *b, int *user) {
//     return *a - *b;
// }
//
// int main(void) {
//     int arr[11] = {1, 1, 3, 5, 5, 5, 8, 8, 8, 10, 10};
//     int key = 8;
//     size_t nmemb = 11;
//     size_t size = 4;
//     int user = 0;
//
//     struct BSearchResult ans =
//         bsearch2(&key, arr, nmemb, size,
//                  (int (*)(const void *, const void *, void *)) & cmp, &user);
//     printf("%d ", ans.result);
//     printf("%zu ", ans.low);
//     printf("%zu ", ans.high);
//
//     // попробовать заменить на l/2 + r/2 и тестить тестить тестить
// }