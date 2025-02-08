typedef int (*MathMethod)(int, int); 

int Mul(int a, int b) { return a * b; }
int Divide(int a, int b) { return a / b; }
int Minus(int a, int b) { return a - b; }
int Add(int a, int b) { return a + b; } 

int Calc(int x, int y, MathMethod Opt) {  // 改為 int 而非 int*
    return Opt(x, y);
} 
 
#include <stdio.h>

int main() {
    int a = 8, b = 7;
    printf("a x b = %d\n", Calc(a, b, Mul)); 
    printf("a / b = %d\n", Calc(a, b, Divide));
    printf("a + b = %d\n", Calc(a, b, Add));   // 修正：加法對應 Add
    printf("a - b = %d\n", Calc(a, b, Minus)); // 修正：減法對應 Minus
    return 0;
}

