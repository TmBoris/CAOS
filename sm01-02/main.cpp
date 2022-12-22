#include "bits/stdc++.h"

int invoke(int16_t *arr, int size, int pivot);

int main() {
    int16_t* arr = new int16_t[10];
    for (size_t i = 0; i < 10; ++i) {
        arr[i] = i;
    }
    int ans = invoke(arr, 10, 5);

    return 0;
}
