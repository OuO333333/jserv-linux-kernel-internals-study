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
