#include <stdio.h>
void func(int *p) { *p = 2; }
int main() {
    int A = 1, C = 3;
    int *ptrA = &A;
    func(ptrA);
    printf("%d\n", *ptrA);
    return 0;
}

