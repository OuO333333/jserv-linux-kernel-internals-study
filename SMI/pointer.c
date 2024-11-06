#include <stdio.h>
#include <stdint.h>

int main() {
    // 定義一個指標，指向指定的地址0x112233
    uint32_t *ptr = (uint32_t *)0x112233;

    // 使用指標操作將值0x445566寫入該地址
    // *ptr = 0x445566;

    // 印出指標本身的地址和它指向的值
    printf("指標 ptr 的地址：%p\n", (void *)&ptr);  // 指標的地址
    printf("ptr 所指向的地址：%p\n", (void *)ptr); // ptr指向的記憶體地址
    printf("ptr 所指向地址的值：0x%X\n", *ptr);    // ptr指向地址的值

    return 0;
}
