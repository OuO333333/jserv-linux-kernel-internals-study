#include <stdio.h>
char *func(){
    char p[] = "hello world";
    return p;
}
int main(){
    printf("%p\n", func());  // 使用 %p 來印出指標的地址
}
