#include <stdio.h>

// 以 32 位格式打印二進制
void printBinary(unsigned int n) {
    for (int i = 31; i >= 0; i--) { // 32-bit integer
        printf("%d", (n >> i) & 1); // 逐位打印
        if (i % 4 == 0) printf(" "); // 每 4 位加一個空格，便於閱讀
    }
    printf("\n");
}

unsigned int reverseBits(unsigned int n) {
    unsigned int result = 0;
    for (int i = 0; i < 32; i++) {
        result = (result << 1) | (n & 1);
        n >>= 1;
    }
    return result;
}

int main() {
    unsigned int n = 0b00000010100101000001111010011100; // 示例輸入
    printf("Original : ");
    printBinary(n);

    unsigned int reversed = reverseBits(n);
    printf("Reversed : ");
    printBinary(reversed);

    return 0;
}

