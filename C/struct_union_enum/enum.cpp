#include <iostream>
using namespace std;

// 定義一個枚舉類型
enum Color {
    RED,    // 默認值為 0
    GREEN,  // 默認值為 1
    BLUE    // 默認值為 2
};

int main() {
    Color c = GREEN; // 設置枚舉變量

    if (c == GREEN) {
        cout << "The color is green." << endl;
    }

    // 輸出枚舉成員的值
    cout << "RED: " << RED << endl;
    cout << "GREEN: " << GREEN << endl;
    cout << "BLUE: " << BLUE << endl;

    return 0;
}

