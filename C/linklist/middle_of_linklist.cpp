#include <iostream>
using namespace std;

// 定義單鏈表結構
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(NULL) {}  // 構造函數
};

// 找到中間節點的函數
ListNode* findMiddle(ListNode* head) {
    if (!head) return NULL;  // 空鏈表處理

    ListNode* slow = head;  
    ListNode* fast = head;  

    // 快指針走兩步，慢指針走一步
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;  // 返回中間節點
}

// 找到鏈結串列的中間節點（Find the Middle Node of a Linked List）
int main() {
    // 創建單鏈表 1 -> 2 -> 3 -> 4 -> 5
    ListNode* head = new ListNode(1);
    head->next = new ListNode(2);
    head->next->next = new ListNode(3);
    head->next->next->next = new ListNode(4);
    head->next->next->next->next = new ListNode(5);

    ListNode* mid = findMiddle(head);
    if (mid) {
        cout << "中間節點值：" << mid->val << endl;  // 輸出 3
    }

    return 0;
}

