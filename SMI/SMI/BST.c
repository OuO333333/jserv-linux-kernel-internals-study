#include <stdio.h>
#include <stdlib.h>

// 定義節點結構
typedef struct Node {
    int data;
    struct Node *left;
    struct Node *right;
} Node;

// 創建新節點
Node* createNode(int data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// 插入節點
Node* insert(Node* root, int data) {
    if (root == NULL) {
        return createNode(data);
    }
    if (data < root->data) {
        root->left = insert(root->left, data);
    } else if (data > root->data) {
        root->right = insert(root->right, data);
    }
    return root;
}

// 找到最小值的節點（用於刪除操作）
Node* findMin(Node* root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

// 刪除節點
Node* delete(Node* root, int data) {
    if (root == NULL) {
        return root;
    }
    if (data < root->data) {
        root->left = delete(root->left, data);
    } else if (data > root->data) {
        root->right = delete(root->right, data);
    } else {
        // 節點有 0 或 1 個子節點
        if (root->left == NULL) {
            Node *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node *temp = root->left;
            free(root);
            return temp;
        }

        // 節點有 2 個子節點，找到右子樹中的最小值
        Node *temp = findMin(root->right);
        root->data = temp->data;
        root->right = delete(root->right, temp->data);
    }
    return root;
}

// 前序遍歷（Pre-order Traversal）
void preOrder(Node* root) {
    if (root != NULL) {
        printf("%d ", root->data);
        preOrder(root->left);
        preOrder(root->right);
    }
}

// 中序遍歷（In-order Traversal）
void inOrder(Node* root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("%d ", root->data);
        inOrder(root->right);
    }
}

// 後序遍歷（Post-order Traversal）
void postOrder(Node* root) {
    if (root != NULL) {
        postOrder(root->left);
        postOrder(root->right);
        printf("%d ", root->data);
    }
}

// 主函數
int main() {
    Node *root = NULL;

    // 插入節點
    root = insert(root, 50);
    root = insert(root, 30);
    root = insert(root, 70);
    root = insert(root, 20);
    root = insert(root, 40);
    root = insert(root, 60);
    root = insert(root, 80);

    printf("中序遍歷 (In-order Traversal): ");
    inOrder(root);
    printf("\n");

    printf("刪除節點 20\n");
    root = delete(root, 20);
    printf("中序遍歷: ");
    inOrder(root);
    printf("\n");

    printf("刪除節點 30\n");
    root = delete(root, 30);
    printf("中序遍歷: ");
    inOrder(root);
    printf("\n");

    printf("刪除節點 50\n");
    root = delete(root, 50);
    printf("中序遍歷: ");
    inOrder(root);
    printf("\n");

    printf("前序遍歷 (Pre-order Traversal): ");
    preOrder(root);
    printf("\n");

    printf("後序遍歷 (Post-order Traversal): ");
    postOrder(root);
    printf("\n");

    return 0;
}
