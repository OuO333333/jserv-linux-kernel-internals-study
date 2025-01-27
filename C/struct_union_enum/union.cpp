#include <iostream>
using namespace std;

// 定義一個聯合體
union Data {
    int i;
    float f;
    char c;
};

int main() {
    Data d;

    d.i = 42;   // 設置整數值
    cout << "i: " << d.i << endl;

    d.f = 3.14; // 設置浮點數值
    cout << "f: " << d.f << endl;

    d.c = 'A';  // 設置字符
    cout << "c: " << d.c << endl;

    // 注意：d.i 和 d.f 的值會被覆蓋
    cout << "i (after c): " << d.i << endl;

    return 0;
}

