#include <stdio.h>  // 引入標頭檔 提供標準輸入輸出函式
#include <stdlib.h> // 引入標頭檔 提供記憶體管理函式

// 定義一個函式 alloc 負責動態分配 n 個 int 大小的記憶體 並回傳指向該記憶體的 int 指標
int *alloc(int n) { 
    return (int *)malloc(n * sizeof(int)); // 使用 malloc 分配 n 個 int 的記憶體 並轉型為 int*
}

// 定義一個函式 modify 接受一個指向 int 指標的指標 (int **p) 修改其指向的陣列的第二個元素
void modify(int **p) { 
    (*p)[1] = 42; // *p 取得 int* 即 arr 再存取 arr[1] 並將其值設為 42
}

int main() {
    int **pp, *arr; // 宣告兩個變數：
                    // arr 是 int* 用來存放 alloc(3) 分配的陣列起始位置
                    // pp 是 int** 指向 arr 即指標的指標

    arr = alloc(3); // 呼叫 alloc(3) 分配 3 個 int 的記憶體 並讓 arr 指向該記憶體

    pp = &arr; // pp 取得 arr 的位址 讓 pp 成為 arr 的指標的指標 (`int**`)

    modify(pp); // 呼叫 modify(pp) 傳入 arr 的指標的指標
                // 在 modify 內 (*p)[1] = 42; 等效於 arr[1] = 42;

    printf("%d\n", arr[1]); // 輸出 `arr[1]`，結果應該是 42

    free(arr); // 釋放 `malloc` 分配的記憶體，防止記憶體洩漏

    return 0; // 結束 `main` 函式，回傳 0 表示正常結束
}
