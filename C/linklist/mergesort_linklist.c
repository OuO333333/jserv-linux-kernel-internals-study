#include <stdio.h>
#include <stdlib.h>

// 定義鏈結串列節點結構
struct ListNode {
    int val;
    struct ListNode* next;
};

// **使用快慢指標找到中間節點**
struct ListNode* findMiddle(struct ListNode* head) {
    struct ListNode* slow = head;
    struct ListNode* fast = head->next; // fast 先移動一格，確保偶數長度時 slow 為左半部最後一個
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow; // 返回中間節點
}

// **合併兩個已排序的鏈結串列（使用非遞迴版本）**
struct ListNode* mergeTwoLists(struct ListNode* l1, struct ListNode* l2) {
    struct ListNode dummy;
    struct ListNode* tail = &dummy;
    dummy.next = NULL;

    while (l1 && l2) {
        if (l1->val < l2->val) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next;
    }
    if (l1) tail->next = l1;
    if (l2) tail->next = l2;

    return dummy.next;
}

// **Merge Sort 排序鏈結串列**
struct ListNode* mergeSort(struct ListNode* head) {
    if (!head || !head->next) return head; // 只有一個節點時，直接返回

    struct ListNode* mid = findMiddle(head); // 找到中間節點
    struct ListNode* rightHead = mid->next;
    mid->next = NULL; // 切斷鏈結串列，分成左右兩部分

    // 遞迴對左右部分進行 Merge Sort
    struct ListNode* left = mergeSort(head);
    struct ListNode* right = mergeSort(rightHead);

    // 合併排序後的左右部分
    return mergeTwoLists(left, right);
}

// **輔助函式：建立新節點**
struct ListNode* createNode(int val) {
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    newNode->val = val;
    newNode->next = NULL;
    return newNode;
}

// **輔助函式：打印鏈結串列**
void printList(struct ListNode* head) {
    while (head) {
        printf("%d -> ", head->val);
        head = head->next;
    }
    printf("NULL\n");
}

// **測試程式**
int main() {
    struct ListNode* head = createNode(4);
    head->next = createNode(2);
    head->next->next = createNode(1);
    head->next->next->next = createNode(3);
    head->next->next->next->next = createNode(5);

    printf("Original List: ");
    printList(head);

    head = mergeSort(head);

    printf("Sorted List: ");
    printList(head);

    return 0;
}

