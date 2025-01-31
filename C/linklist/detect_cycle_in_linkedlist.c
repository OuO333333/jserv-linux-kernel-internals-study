#include <stdio.h>
#include <stdlib.h>

// 定義鏈結串列節點結構
struct ListNode {
    int val;
    struct ListNode* next;
};

// **檢測鏈結串列是否有環（快慢指標法）**
int hasCycle(struct ListNode *head) {
    if (!head || !head->next) return 0; // 空鏈結串列或只有一個節點，必定無環

    struct ListNode *slow = head, *fast = head;

    while (fast && fast->next) {
        slow = slow->next;        // 慢指標走一步
        fast = fast->next->next;  // 快指標走兩步

        if (slow == fast) return 1; // 發現快慢指標相遇，代表有環
    }
    
    return 0; // 如果 fast 到達 NULL，代表沒有環
}

// **輔助函式：建立新節點**
struct ListNode* createNode(int val) {
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    newNode->val = val;
    newNode->next = NULL;
    return newNode;
}

// 檢查鏈結串列是否有環（Detect Cycle in a Linked List）
int main() {
    struct ListNode* head = createNode(3);
    head->next = createNode(2);
    head->next->next = createNode(0);
    head->next->next->next = createNode(-4);

    // 製造一個環，讓最後一個節點指向第二個節點（形成 2 → 0 → -4 → 2）
    head->next->next->next->next = head->next;

    if (hasCycle(head)) {
        printf("鏈結串列中有環\n");
    } else {
        printf("鏈結串列中無環\n");
    }

    return 0;
}

