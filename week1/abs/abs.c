#include <stdio.h>
#include <stdint.h>
int32_t imp1(int32_t x) {
    int32_t mask = x >> 31;
    printf("mask = %d, x + mask = %d\n", mask, x + mask);
    return (x + mask) ^ mask;
}
int32_t imp2(int32_t x) {
    int32_t mask = (x >> 31);
    printf("mask = %d, x ^ mask = %d\n", mask, x ^ mask);
    return (x ^ mask) - mask;
}
int main(void) {
    printf("imp1:\n");
    printf("abs(INT32_MIN) = %d\n", imp1(INT32_MIN));
    printf("abs(INT32_MIN + 1) = %d\n", imp1(INT32_MIN + 1));

    printf("\nimp2:\n");
    printf("abs(INT32_MIN) = %d\n", imp2(INT32_MIN));
    printf("abs(INT32_MIN + 1) = %d\n", imp2(INT32_MIN + 1));
    return 0;
}
