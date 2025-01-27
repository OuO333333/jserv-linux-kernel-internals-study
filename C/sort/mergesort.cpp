#include <iostream>
#include <vector>
using namespace std;

vector<int> num = {38, 27, 43, 3, 9, 82, 10};
vector<int> tmp(num.size());

void merge(int left, int right) {
    int mid = (left + right) / 2;
    int i = left, j = mid + 1, k = 0;

    while (i <= mid || j <= right) {
        if (i > mid)
            tmp[k++] = num[j++];
        else if (j > right)
            tmp[k++] = num[i++];
        else if (num[i] < num[j])
            tmp[k++] = num[i++];
        else
            tmp[k++] = num[j++];
    }

    for (i = left, k = 0; i <= right; i++, k++)
        num[i] = tmp[k];
}

void mergeSort(int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSort(left, mid);
        mergeSort(mid + 1, right);
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

