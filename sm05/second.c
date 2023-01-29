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

    size_t left = 0;
    size_t right = nmemb;
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (compar(key, base + mid * size, user) < 0) {
            right = mid;
        } else if (compar(key, base + mid * size, user) == 0) {
            was = true;
            right = mid;
        } else {
            left = mid + 1;
        }
    }
    ans.low = left;

    left = 0;
    right = nmemb;
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (compar(key, base + mid * size, user) < 0) {
            right = mid;
        } else {
            left = mid + 1;
        }
    }
    ans.high = left;

    if (was) {
        ans.result = 1;
    } else {
        ans.result = 0;
    }
    return ans;
}