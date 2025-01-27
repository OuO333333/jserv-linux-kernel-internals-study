#include <iostream>
#include <vector>
using namespace std;

vector<int> num = {38, 27, 43, 3, 9, 82, 10};
vector<int> tmp(num.size());

void merge(int left, int right) {
    int mid = (left + right) / 2;
    // i 為左邊起始點, j 為右邊起始點, k 是臨時陣列 tmp 的索引，用於存放合併後的結果
    int i = left, j = mid + 1, k = 0;

    while (i <= mid || j <= right) {
        // 如果只剩右邊
        if (i > mid)
            tmp[k++] = num[j++];
        // 如果只剩左邊
        else if (j > right)
            tmp[k++] = num[i++];
        // 如果左邊較小
        else if (num[i] < num[j])
            tmp[k++] = num[i++];
        // 如果右邊較小
        else
            tmp[k++] = num[j++];
    }
    
    // 將 tmp 陣列中合併的結果覆蓋到原數列 num 中
    for (i = left, k = 0; i <= right; i++, k++)
        num[i] = tmp[k];
}

void mergeSort(int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        // 遞迴地處理左半部分
        mergeSort(left, mid);
        // 遞迴地處理右半部分
        mergeSort(mid + 1, right);
        // 合併左右兩部分
        merge(left, right);
    }
}

int main() {
    mergeSort(0, num.size() - 1);

    for (int n : num)
        cout << n << " ";
    cout << endl;

    return 0;
}

