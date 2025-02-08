#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef int (*MathMethod)(int, int); 

int Mul(int a, int b) { return a*b; }
int Divide(int a, int b) { return a/b; }
int Minus(int a, int b) { return a-b; }
int Add(int a, int b) { return a+b; } 

// 這是一個 函數指針陣列，它的每個元素都是一個函數指針，指向對應的數學運算函數。
// 這樣我們可以透過索引 (calcArray[i]) 來選擇要執行的運算函數，而不需要 if-else 或 switch-case！
// & 可省略
// Mul 代表函數的地址，等價於 &Mul，所以 & 省略不影響程式運行。
int (*calcArray[])(int, int) =   // Array of function pointer，存放會使用到的function名稱
{ 
    Mul,     // 這邊亦可以打&Mul，&可省略
    Divide, 
    Minus, 
    Add 
};
char* operation[] =   //做為判斷的自訂條件，與argv比對用
{
    "x",
    "/",
    "-",
    "+"
};

// argc：命令列參數的個數。
// argv[0] = "./program" (程式名稱)
// argv[1] = "8" (第一個數字)
// argv[2] = "x" (運算符)
// argv[3] = "7" (第二個數字)
int main(int argc, char** argv)
{
    int size = sizeof(calcArray) / (sizeof(int(*)));    // 使用此方法可以避免每次增加功能時修改長度
    int i = 0;

    if (argc > 1) {   // 確保有輸入參數
        for (i = 0; i < size; i++) {
            if (strcmp(argv[2], operation[i]) == 0) { // 比對使用者輸入的運算符
                int a = atoi(argv[1]), b = atoi(argv[3]); // 將字串轉換為整數
                printf("%d %s %d = %d\n", a, argv[2], b, calcArray[i](a, b)); 
                break; // 找到對應運算後立即跳出迴圈
            }
        }
    }
}
