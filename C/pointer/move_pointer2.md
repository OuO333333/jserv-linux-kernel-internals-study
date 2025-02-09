## 程式解釋與輸出分析

### **程式碼**
```c
#include <stdio.h>

int main() {
    int a[] = {1, 2, 3, 4, 5, 6, 7, 9};
    int *ptr = (int*)(&a + 1);

    printf("%d\n", &a);       // 陣列 a 的起始地址
    printf("%d\n", &a + 1);   // 陣列 a 末尾之後的地址
    printf("%d\n", ptr);      // ptr 指向 &a + 1 的地址
    printf("%d\n", *(ptr));   // 試圖存取 ptr 所指向的值（未定義行為）
    printf("%d\n", ptr - 1);  // ptr-1 會指向陣列的最後一個元素
    printf("%d\n", *(ptr - 1)); // 存取陣列的最後一個元素
    printf("%d\n", ptr - 2);  // ptr-2 指向倒數第二個元素
    printf("%d\n", *(ptr - 2)); // 存取倒數第二個元素
    printf("%d\n", *a);       // 陣列首元素
    printf("%d\n", *a + 7);   // 陣列首元素 + 7
    printf("%d\n", *(a + 7)); // 陣列索引 7（即 `a[7]`）
}
