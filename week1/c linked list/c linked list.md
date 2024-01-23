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
