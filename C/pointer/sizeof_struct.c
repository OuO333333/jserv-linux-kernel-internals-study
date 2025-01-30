#include <stdio.h>

struct MyType {
    char a;
    int b;
    double c;
};

int main() {
    // 使用指標地址運算來計算 struct MyType 的大小
    struct MyType *ptr = (struct MyType *)0;  // 將指標初始化為地址 0
    struct MyType *nextPtr = ptr + 1;        // 指向下一個結構的地址
    int size = (char *)nextPtr - (char *)ptr; // 計算兩個指針之間的差值
    printf("Size of MyType: %d bytes\n", size);
    return 0;
}

