#include <stdio.h>
const char *func() {
    const char *p = "hello world";
    return p;
}
int main() {
    printf("%p\n", (void *)func());  // 使用 %p 來印出指標的地址
    return 0;
}
