#include <stdio.h>

// 声明 xorSwap 函数
void xorSwap(int *x, int *y);

int main() {
    int a = 5;
    int b = 10;

    printf("交换前：a = %d, b = %d\n", a, b);

    // 调用 xorSwap 函数交换 a 和 b 的值
    xorSwap(&a, &b);

    printf("交换后：a = %d, b = %d\n", a, b);

    return 0;
}

// 实现 xorSwap 函数
void xorSwap(int *x, int *y) {
    *x ^= *y;
    *y ^= *x;
    *x ^= *y;
}

