## 程式解釋與輸出分析

### **程式碼**
```c
#include <stdio.h>

int main() {
    char s[] = "0113256";
    char *p = s;
    printf("%c", *p++);   // 輸出 '0'，p 指向下一個字元
    printf("%c", *(p++)); // 輸出 '1'，p 再次向後移動
    printf("%c", (*p)++); // 輸出 '1'，但 '1' 會被修改為 '2'
    printf("%c", *++p);   // p 先前進一格到 '3'，然後輸出 '3'
    printf("%c", *(++p)); // p 再前進一格到 '2'，然後輸出 '2'
    printf("%c", ++*p);   // 先將 '2' 加 1 變成 '3'，然後輸出 '3'
    printf("%c", ++(*p)); // '3' 再加 1 變成 '4'，然後輸出 '4'
    
    printf("\n%s", s); // 輸出修改後的字串 0123456
    return 0;
}
