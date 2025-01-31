#include <stdio.h>
#include <stdlib.h>

// 定義 BST 節點
struct Node {
    int value;
    struct Node* left;
    struct Node* right;
};

// 創建新節點
struct Node* createNode(int value) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->value = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// 插入節點
struct Node* insert(struct Node* root, int value) {
    if (root == NULL) return createNode(value);  // 若樹為空，創建根節點

    if (value < root->value)
        root->left = insert(root->left, value);  // 插入左子樹
    else if (value > root->value)
        root->right = insert(root->right, value); // 插入右子樹

    return root;
}

// 搜尋節點
struct Node* search(struct Node* root, int value) {
    if (root == NULL || root->value == value)
        return root; // 找到目標或樹為空

    if (value < root->value)
        return search(root->left, value); // 在左子樹搜尋
    else
        return search(root->right, value); // 在右子樹搜尋
}

// 找到最小值節點（用於刪除）
struct Node* findMin(struct Node* root) {
    while (root->left != NULL) 
        root = root->left;
    return root;
}

// 刪除節點
struct Node* deleteNode(struct Node* root, int value) {
    if (root == NULL) return root; // 空樹直接返回

    if (value < root->value)
        root->left = deleteNode(root->left, value); // 刪除左子樹
    else if (value > root->value)
        root->right = deleteNode(root->right, value); // 刪除右子樹
    else {
        // 節點有 0 或 1 個子節點
        if (root->left == NULL) {
            struct Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            struct Node* temp = root->left;
            free(root);
            return temp;
        }

        // 節點有 2 個子節點，找到右子樹的最小節點
        struct Node* temp = findMin(root->right);
        root->value = temp->value;  // 取代當前節點
        root->right = deleteNode(root->right, temp->value); // 刪除重複節點
    }
    return root;
}

// 中序遍歷（遞增輸出）
void inorderTraversal(struct Node* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("%d ", root->value);
        inorderTraversal(root->right);
    }
}

// 主函數
int main() {
    struct Node* root = NULL;
    root = insert(root, 10);
    root = insert(root, 5);
    root = insert(root, 15);
    root = insert(root, 2);
    root = insert(root, 7);
    root = insert(root, 12);
    root = insert(root, 18);

    printf("中序遍歷: ");
    inorderTraversal(root);
    printf("\n");

    // 搜尋
    int key = 7;
    if (search(root, key))
        printf("找到節點 %d\n", key);
    else
        printf("未找到節點 %d\n", key);

    // 刪除節點
    root = deleteNode(root, 10);
    printf("刪除 10 後，中序遍歷: ");
    inorderTraversal(root);
    printf("\n");

    return 0;
}

