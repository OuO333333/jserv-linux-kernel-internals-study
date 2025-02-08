#include <stdio.h>
#include <stdlib.h>

// 定義單鏈表結構
typedef struct ListNode {
    int val;
    struct ListNode* next;
} ListNode;

// 創建新節點
ListNode* createNode(int x) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    if (!newNode) {
        printf("內存分配失敗\n");
        exit(1);
    }
    newNode->val = x;
    newNode->next = NULL;
    return newNode;
}

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

// 釋放鏈表內存
void freeList(ListNode* head) {
    ListNode* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

// 找到鏈結串列的中間節點（Find the Middle Node of a Linked List）
int main() {
    // 創建單鏈表 1 -> 2 -> 3 -> 4 -> 5
    ListNode* head = createNode(1);
    head->next = createNode(2);
    head->next->next = createNode(3);
    head->next->next->next = createNode(4);
    head->next->next->next->next = createNode(5);

    ListNode* mid = findMiddle(head);
    if (mid) {
        printf("中間節點值：%d\n", mid->val);  // 輸出 3
    }

    // 釋放內存
    freeList(head);

    return 0;
}
