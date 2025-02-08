### **計算整數 `x` 的二進制表示中 `1` 的個數**

#### **代碼分析**
```cpp
#include <iostream>
using namespace std;

int main() {
    int x, c = 0;  // 修正變量名：cout -> c
    cin >> x;
    while (x > 0) {
        c++;         
        x = x & (x - 1);  // 每次消除最低位的 1
    }
    cout << c << endl;  // 輸出 1 的個數
    return 0;
}
