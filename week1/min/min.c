#include <stdio.h>
#include <stdint.h>
int32_t min(int32_t a, int32_t b) {
    int32_t diff = (a - b);
    return b + (diff & (diff >> 31));
}
int main(void) {
    printf("min(1000, 2000) = %d\n", min(1000, 2000));
    printf("min(INT32_MAX, INT32_MIN) = %d\n", min(INT32_MAX, INT32_MIN));
    return 0;
}
