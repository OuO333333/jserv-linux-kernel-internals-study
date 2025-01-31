#include <stdio.h>
#include <stdlib.h>

// 定義鏈結串列的結構
struct ListNode {
    int val;
    struct ListNode* next;
};

// 合併兩個已排序的鏈結串列（非遞迴）
struct ListNode* mergeTwoLists(struct ListNode* l1, struct ListNode* l2) {
    struct ListNode dummy;   // 建立 dummy 節點
    struct ListNode* tail = &dummy; // 指向 dummy 節點
    dummy.next = NULL;

    while (l1 && l2) {  // 當兩個鏈結串列都還有節點時
        if (l1->val < l2->val) {
            tail->next = l1;
            l1 = l1->next;
        } else {
            tail->next = l2;
            l2 = l2->next;
        }
        tail = tail->next; // 更新 tail 指標
    }

    // 若有一個鏈結串列還有剩餘節點，直接接到 tail 後面
    if (l1) tail->next = l1;
    if (l2) tail->next = l2;

    return dummy.next;  // 返回合併後的鏈結串列的頭
}

// 輔助函式：建立新節點
struct ListNode* createNode(int val) {
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    newNode->val = val;
    newNode->next = NULL;
    return newNode;
}

// 輔助函式：打印鏈結串列
void printList(struct ListNode* head) {
    while (head) {
        printf("%d -> ", head->val);
        head = head->next;
    }
    printf("NULL\n");
}

// 合併兩個已排序的鏈結串列（Merge Two Sorted Linked Lists）
int main() {
    struct ListNode* l1 = createNode(1);
    l1->next = createNode(3);
    l1->next->next = createNode(5);

    struct ListNode* l2 = createNode(2);
    l2->next = createNode(4);
    l2->next->next = createNode(6);

    printf("List 1: ");
    printList(l1);
    printf("List 2: ");
    printList(l2);

    struct ListNode* mergedList = mergeTwoLists(l1, l2);
    printf("Merged List: ");
    printList(mergedList);

    return 0;
}

