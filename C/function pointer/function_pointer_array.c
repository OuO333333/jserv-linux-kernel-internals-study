#include <stdio.h>

// 定義兩個函數
int square(int x) {
    return x * x;
}

int cube(int x) {
    return x * x * x;
}

int main() {
    int (*a[10])(int);  // 宣告一個函數指標陣列
    a[0] = square;
    a[1] = cube;
    
    printf("%d\n", a[0](5));   // 調用 square，輸出 25
    printf("%d\n", a[1](5));  // cube，輸出 125
    return 0;
}

