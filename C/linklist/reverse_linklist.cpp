#include <iostream>
using namespace std;

// 定義單鏈表節點結構
struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(NULL) {}  // 構造函數
};

// 反轉單鏈表的函數
ListNode* reverseList(ListNode* head) {
    ListNode* prev = NULL;  
    ListNode* curr = head;  

    while (curr) {
        ListNode* next = curr->next;  // 保存下一個節點
        curr->next = prev;            // 反轉指針
        prev = curr;                   // 前移 prev
        curr = next;                   // 前移 curr
    }

    return prev;  // 返回新的頭節點
}

// 打印鏈表
void printList(ListNode* head) {
    while (head) {
        cout << head->val << " -> ";
        head = head->next;
    }
    cout << "NULL" << endl;
}

// 測試函數
int main() {
    // 建立鏈表 1 -> 2 -> 3 -> 4 -> 5
    ListNode* head = new ListNode(1);
    head->next = new ListNode(2);
    head->next->next = new ListNode(3);
    head->next->next->next = new ListNode(4);
    head->next->next->next->next = new ListNode(5);

    cout << "原始鏈表：" << endl;
    printList(head);

    // 反轉鏈表
    head = reverseList(head);

    cout << "反轉後的鏈表：" << endl;
    printList(head);

    return 0;
}

