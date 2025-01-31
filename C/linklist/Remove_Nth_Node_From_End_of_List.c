#include <stdio.h>
#include <stdlib.h>

// 定義鏈結串列節點結構
struct ListNode {
    int val;
    struct ListNode* next;
};

// **刪除倒數第 N 個節點**
struct ListNode* removeNthFromEnd(struct ListNode* head, int n) {
    struct ListNode dummy;
    dummy.next = head;  // 設置一個虛擬頭節點，方便處理邊界情況
    struct ListNode *fast = &dummy, *slow = &dummy;

    // **Step 1：讓 fast 先走 N 步**
    for (int i = 0; i < n; i++) {
        if (fast->next) {
            fast = fast->next;
        }
    }

    // **Step 2：快慢指標同時移動，直到 fast 到達尾部**
    while (fast->next) {
        fast = fast->next;
        slow = slow->next;
    }

    // **Step 3：刪除 slow->next（即倒數第 N 個節點）**
    struct ListNode* temp = slow->next;
    slow->next = slow->next->next;
    free(temp);  // 釋放記憶體

    return dummy.next;  // 返回新的頭部（可能變動）
}

// **輔助函式：建立新節點**
struct ListNode* createNode(int val) {
    struct ListNode* newNode = (struct ListNode*)malloc(sizeof(struct ListNode));
    newNode->val = val;
    newNode->next = NULL;
    return newNode;
}

// **輔助函式：列印鏈結串列**
void printList(struct ListNode* head) {
    while (head) {
        printf("%d -> ", head->val);
        head = head->next;
    }
    printf("NULL\n");
}

// **測試程式**
int main() {
    struct ListNode* head = createNode(1);
    head->next = createNode(2);
    head->next->next = createNode(3);
    head->next->next->next = createNode(4);
    head->next->next->next->next = createNode(5);

    printf("原始鏈結串列: ");
    printList(head);

    int n = 2; // 刪除倒數第 2 個節點
    head = removeNthFromEnd(head, n);

    printf("刪除倒數第 %d 個節點後: ", n);
    printList(head);

    return 0;
}

