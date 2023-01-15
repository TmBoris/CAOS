#include <stdio.h>
#include <stdlib.h>

int main() {
    /* The four registers do not need to be initialized as the processor will
     * write over it. */
    int a, b, c, d;

    {
        __asm__("cpuid"
                : "=a"(a), "=b"(b), "=c"(c),
                  "=d"(d)   // The output variables. EAX -> a and vice versa.
                : "0"(1));  // Put the infotype into EAX.
        //        printf ("EAX: %b\nEBX: %x\nECX: %x\nEDX: %x\n",a, b, c, d);
    }
    int model;
    if (((a & 3840) >> 8) == 6 || ((a & 3840) >> 8) == 15) {
        model = (((15 << 4) & a) >> 4) + (((15 << 16) & a) >> 12);
    } else {
        model = ((15 << 4) & a) >> 4;
    }
    int family;
    if (((a & 3840) >> 8) == 15) {
        family = (((255 << 20) & a) >> 20) + ((a & 3840) >> 8);
    } else {
        family = ((a & 3840) >> 8);
    }
    printf("family=%d model=%d ecx=0x%x edx=0x%x\n", family, model, c, d);

    return 0;
}