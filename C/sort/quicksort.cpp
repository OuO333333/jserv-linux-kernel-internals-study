#include <iostream>
#include <vector>
#include <cstdio>
#include <algorithm> // 為了使用 swap 函數

using namespace std;

vector<int> num = {38, 27, 43, 3, 9, 82, 10};

// 快速排序函數
void quickSort(vector<int> &num, int L, int R) {
    // 如果序列只有一個元素（L == R），直接返回
    if (L >= R)
        return;

    // 選擇最右邊的元素作為基準點（pivot）
    int pivot = num[R]; // pivot 為 num 的最後一個元素
    int pos = L; // pos 用於標記小於 pivot 的區域的右邊界

    // 將小於 pivot 的元素移動到左側
    for (int i = L; i < R; ++i) { // 遍歷 [L, R] 的所有元素
        if (num[i] < pivot) { // 如果 num[i] 小於 pivot
            swap(num[i], num[pos]); // 將 num[i] 與 num[pos] 交換
            pos++; // pos 向右移動一格
        }
    }

    // 將 pivot 放到左右區間的分界處（確保 pivot 左側小於它，右側大於等於它）
    swap(num[R], num[pos]);

    // 對左側子序列進行遞迴排序
    quickSort(num, L, pos - 1);

    // 對右側子序列進行遞迴排序
    quickSort(num, pos + 1, R);
}

int main() {

    quickSort(num, 0, num.size() - 1);

    // 輸出排序後的數列
    for (int i = 0; i < num.size(); ++i) {
        printf("%d ", num[i]); // 依次輸出每個數字
    }
    return 0; // 程式結束
}

