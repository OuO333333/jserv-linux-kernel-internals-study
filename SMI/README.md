1. volatile 關鍵字
volatile關鍵字告訴編譯器，這個變數的值可能會隨時改變，無法預測。
這樣的變數在程式執行中有可能被硬體中斷、其他執行緒、或外部因素更改。
使用volatile時，編譯器會避免對該變數進行特定的優化，確保每次存取該變數時都能得到最新的值。
```c
#include <stdio.h>

volatile int flag = 0; // 告訴編譯器這個變數可能被其他程序更改

void interrupt_handler() {
    flag = 1; // 中斷程序修改flag
}

int main() {
    while (flag == 0) {
        // 主程式持續等待flag變為1
    }
    printf("中斷發生，flag 已被設為1\n");
    return 0;
}
```
