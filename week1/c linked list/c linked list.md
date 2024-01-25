兩種版本的 remove_list_node  
3 exceptional cases, we need to take care of:  
list is empty  
delete the head node  
node is not in the list  
以下的討論不涵蓋 list is empty 和 node is not in the list 的狀況
```c
void remove_list_node(List *list, Node *target)
{
    Node *prev = NULL;
    Node *current = list->head;
    // Walk the list
    while (current != target) {
        prev = current;
        current = current->next;
    }
    // Remove the target by updating the head or the previous node.
    if (!prev)
        list->head = target->next;
    else
        prev->next = target->next;
}
```
這是一般常見的 remove_list_node
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/6e6cb375-8932-4598-b3d9-0fa9d8cc5a54)
```c
void remove_list_node(List *list, Node *target)
{
    // The "indirect" pointer points to the *address*
    // of the thing we'll update.
    Node **indirect = &list->head;
    // Walk the list, looking for the thing that 
    // points to the node we want to remove.
    while (*indirect != target)
        indirect = &(*indirect)->next;
    *indirect = target->next;
}
```
Linus Torvalds 的想法則是拿一個指標指向「節點裡頭指向下個節點 的指標」，以「要更新的位址」為思考點來操作。
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/4d336e7a-3bae-41bd-9f97-9b764fa9182c)
  
-------------------------------------------------------------  
  
針對鏈結串列的新增節點的操作，考慮以下程式碼:
```c
void append(int value, list_entry_t **head)
{
    list_entry_t *direct = *head;
    list_entry_t *prev = NULL;

    list_entry_t *new = malloc(1 * sizeof(list_entry_t));
    new->value = value, new->next = NULL;

    while (direct) {
        prev = direct;           
        direct = direct->next;
    }

    if (prev)
        prev->next = new;
    else
        *head = new;
}
```
函式的參數列表中, 之所以用 list_entry_t **head, 而非 list_entry_t *head, 是因為原本傳入的 head 可能會被變更,  
但 C 語言在參數傳遞時永遠都是傳遞數值 (副本), 於是若接受 list_entry_t *head 做為參數, 那就要提供 append 函式的傳回值, 也就是說, 該函式原型宣告變為:
```c
list_entry_t *append(int value, list_entry_t *head);
```
如此就不優雅且顯得累贅。運用上述 indirect pointer 的技巧,我們可重寫 append 函式如下:
```c
void append_indirect(int value, list_entry_t **head)
{
    list_entry_t **indirect = head;

    list_entry_t *new = malloc(1 * sizeof(list_entry_t));
    new->value = value, new->next = NULL;

    while (*indirect)
        indirect = &((*indirect)->next);

    *indirect = new;
}
```
  
-------------------------------------------------------------  

案例探討: LeetCode 21. Merge Two Sorted Lists  
LeetCode 21. Merge Two Sorted Lists 簡述:  
```
Merge two sorted linked lists and return it as a sorted list. The list should be made by splicing together the nodes of the first two lists.
(給定二個已排序的 linked list, 傳回合併過後的 linked list)
```
直觀的做法是, 提供一個暫時節點, 依序將內含值較小的節點串上, 最後回傳暫時節點指向的次個節點:
```c
struct ListNode *mergeTwoLists(struct ListNode *L1, struct ListNode *L2) {
    struct ListNode *head = malloc(sizeof(struct ListNode));
    struct ListNode *ptr = head;
    while (L1 && L2) {
        if (L1->val < L2->val) {
            ptr->next = L1;
            L1 = L1->next;
        } else {
            ptr->next = L2;
            L2 = L2->next;
        }
        ptr = ptr->next;
    }
    ptr->next = L1 ? L1 : L2;
    return head->next;
}
```
倘若我們想避免配置暫時節點的空間 (即上方程式碼中的 malloc), 該怎麼做？運用上述 indirect pointer 的技巧:
```c
struct ListNode *mergeTwoLists(struct ListNode *L1,
                               struct ListNode *L2) { 
    struct ListNode *head;
    struct ListNode **ptr = &head;
    for (; L1 && L2; ptr = &(*ptr)->next) {
        if (L1->val < L2->val) {
            *ptr = L1;
            L1 = L1->next;
        } else {
            *ptr = L2;
            L2 = L2->next;
        }
    }
    *ptr = (struct ListNode *)((uintptr_t) L1 | (uintptr_t) L2);
    return head;
}
```
以下可以不用看, 很不直觀。  
觀察使用 indirect pointer 版本的程式碼, 其中 if-else 的程式碼都是將 ptr 指向下一個要接上的節點,  
之後將節點更新到下一個,不過要為 L1 跟 L2 分開寫同樣的程式碼, 該如何簡化？可以再使用一個指標的指標來指向 L1 或 L2。
```c
struct ListNode *mergeTwoLists(struct ListNode *L1, struct ListNode *L2) {
    struct ListNode *head = NULL, **ptr = &head, **node;

    for (node = NULL; L1 && L2; *node = (*node)->next) {
        node = (L1->val < L2->val) ? &L1: &L2;
        *ptr = *node;
        ptr = &(*ptr)->next;
    }
    *ptr = (struct ListNode *)((uintptr_t) L1 | (uintptr_t) L2);
    return head;
}
```
注意 node = L1->val < L2->val? &L1: &L2 不能寫成 node = &(L1->val < L2->val? L1: L2), 依據 C99 規格書 6.5.15 的註腳:
```
A conditional expression does not yield an lvalue
```
因此無法使用 & (address of) 去取得 L1->val < L2->val? L1: L2 的地址, 只能分開取得 L1 和 L2 的地址。
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/6797cc65-c37e-4bc3-af0b-7b0232b2dc14)
  
-------------------------------------------------------------  
  
LeetCode 23. Merge k Sorted Lists 則將 LeetCode 21. Merge Two Sorted Lists 指定的 2 個 linked list 擴充為 k 個的合併,  
其本質就是將分割好的 sorted lists 合併成一條，示意如下:  
![image](https://github.com/OuO333333/jserv-linux-kernel-internals-study/assets/37506309/69dbf149-caea-4638-bc2b-6872046ffcbc)  
顯然在 merge 階段可延用上述 mergeTwoLists 函式, 至於 list 合併的方向就是演算法勝出的關鍵, 可能的思路如下:  
1\. naive  
&nbsp;&nbsp;&nbsp;&nbsp;直觀地用第一條串列接上剩下的串列, 這樣會導致 lists[0] 愈來愈長, 立即會遇到的問題是：多數的情況下合併速度會愈來愈慢。
```c
struct ListNode *mergeKLists(struct ListNode **lists, int listsSize) {
    if (listsSize == 0) return NULL;
    for (int i = 1; i < listsSize; i++)
        lists[0] = mergeTwoLists(lists[0], lists[i]);
    return lists[0];
}
```
2\. 頭跟尾兩兩合併  
&nbsp;&nbsp;&nbsp;&nbsp;從固定第一條串列改成頭跟尾兩兩合併, 直到剩一條為止, 比起前一方法的每次都用愈來愈長的串列跟另一條串列合併,  
頭尾合併在多數的情況下兩條串列的長度比較平均, 合併會比較快。  
當合併完頭尾後, 偶數長度會少一半, 奇數長度則為 (listsSize + 1) / 2, 奇數更新的方式也可以用在偶數長度上。
```c
struct ListNode *mergeKLists(struct ListNode **lists, int listsSize) {
    if (listsSize == 0) return NULL;
    
    while (listsSize > 1) {
        for (int i = 0, j = listsSize - 1; i < j; i++, j--)
            lists[i] = mergeTwoLists(lists[i], lists[j]);
        listsSize = (listsSize + 1) / 2;
    }
    
    return lists[0];
}
```
3\. 分段合併  
參考 https://hackmd.io/@sysprog/c-linked-list#%E6%A1%88%E4%BE%8B%E6%8E%A2%E8%A8%8E-LeetCode-21-Merge-Two-Sorted-Lists  
4\. Divide and Conquer  
&nbsp;&nbsp;&nbsp;&nbsp;由於 lists 中的串列已排序, 可視為 sorted element, 直接進行 merge sort:
```c
struct ListNode *mergeKLists(struct ListNode **lists, int listsSize) {
    if (!listsSize)
        return NULL;
    if (listsSize <= 1)
        return *lists;

    int m = listsSize >> 1;
    struct ListNode *left = mergeKLists(lists, m);
    struct ListNode *right = mergeKLists(lists + m, listsSize - m);

    return mergeTwoLists(left, right);
}
```
  
-------------------------------------------------------------  

案例探討: Leetcode 2095. Delete the Middle Node of a Linked List  
這部份沒看了, 指標的指標好難用,  
只有用最簡單的方式寫出這題。  
https://github.com/OuO333333/leetcode/tree/master  
  
-------------------------------------------------------------  

案例探討: LeetCode 86. Partition List  
這部份沒看了, 指標的指標好難用,  
只有用最簡單的方式寫出這題。  
https://github.com/OuO333333/leetcode/blob/master/Partition%20List.cpp  
  
-------------------------------------------------------------  

  
