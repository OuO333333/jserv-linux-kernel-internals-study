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
