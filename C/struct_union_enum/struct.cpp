#include <iostream>
using namespace std;

// 定義一個結構體
struct Person {
    string name;  // 姓名
    int age;      // 年齡
    float height; // 身高
};

int main() {
    Person p1; // 定義結構變量

    // 設置成員值
    p1.name = "Alice";
    p1.age = 25;
    p1.height = 170.5;

    // 輸出成員值
    cout << "Name: " << p1.name << endl;
    cout << "Age: " << p1.age << endl;
    cout << "Height: " << p1.height << endl;

    return 0;
}

