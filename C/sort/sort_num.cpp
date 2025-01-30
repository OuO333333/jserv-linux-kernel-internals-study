#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// 主函數實現排序
void sortArray(vector<int> &arr) {
    vector<int> odd, even;

    // 將奇數和偶數分開
    for (int num : arr) {
        if (num % 2 == 0) {
            even.push_back(num);
        } else {
            odd.push_back(num);
        }
    }

    // 對奇數進行升序排列
    sort(odd.begin(), odd.end());

    // 對偶數進行降序排列
    sort(even.begin(), even.end(), greater<int>());

    // 合併奇數和偶數
    arr.clear();
    arr.insert(arr.end(), odd.begin(), odd.end());
    arr.insert(arr.end(), even.begin(), even.end());
}

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Enter the elements: ";
    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }

    // 排序
    sortArray(arr);

    // 輸出結果
    cout << "Sorted array: ";
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}

