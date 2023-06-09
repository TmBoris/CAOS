#include <stdio.h>
 
void spin_lock(volatile int* ptr);
void spin_unlock(volatile int* ptr);
 
int main() {
    volatile int t = 0;
    spin_lock(&t);
    printf("%d\n", t);
    // uncomment and find infinite loop
    // spin_lock(&t);
    spin_unlock(&t);
    return 0;
}