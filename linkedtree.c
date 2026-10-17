/* ============================================================
 * glimmer_3_linked_tree.c
 * 来源：3.md 的「Step 2. 链二叉树」与「Step 3. 递归」
 * 说明：链式存储二叉树。包含 create_node、前中后序遍历、
 *       depth 深度统计，以及用栈模拟的非递归前序遍历。
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int data;               // 节点存储的数据
    struct TreeNode *left;  // 指向左子树的指针
    struct TreeNode *right; // 指向右子树的指针
} TreeNode;

/* 创建新节点：数据为 value，左右指针都指向 NULL */
TreeNode* create_node(int value)
{
    TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
    node->data = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* 前序遍历：根 -> 左 -> 右 */
void preorder(TreeNode *root)
{
    if (root != NULL)
    {
        printf("%d ", root->data);   // 根在最前面
        preorder(root->left);
        preorder(root->right);
    }
}

/* 中序遍历：左 -> 根 -> 右 */
void inorder(TreeNode *root)
{
    if (root != NULL)
    {
        inorder(root->left);
        printf("%d ", root->data);   // 根夹在中间
        inorder(root->right);
    }
}

/* 后序遍历：左 -> 右 -> 根 */
void postorder(TreeNode *root)
{
    if (root != NULL)
    {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->data);   // 根在最后面
    }
}

/* 统计树的深度：current_depth 是当前节点在第几层，max_depth 是最大层数纪录 */
int depth(TreeNode *root, int current_depth, int max_depth)
{
    if (root == NULL)
    {
        return max_depth;            // 空树：纪录不变，返回
    }

    if (current_depth > max_depth)
    {
        max_depth = current_depth;   // 当前节点刷新了最大层数
    }

    max_depth = depth(root->left, current_depth + 1, max_depth);
    max_depth = depth(root->right, current_depth + 1, max_depth);

    return max_depth;
}

/* ---------- 用栈模拟前序遍历（非递归） ---------- */

typedef struct Stack {
    TreeNode **arr;  // 存放节点指针的数组
    int top;         // 栈顶元素的下标，-1 表示空栈
    int capacity;    // 栈的容量
} Stack;

/* 创建一个容量为 capacity 的栈 */
Stack *createStack(int capacity) {
    Stack *stack = malloc(sizeof(Stack));
    stack->arr = malloc(sizeof(TreeNode *) * capacity); // 申请动态数组的空间
    stack->top = -1;                                    // 空栈，栈顶下标为 -1
    stack->capacity = capacity;
    return stack;
}

/* 判断栈是否为空：top 为 -1 就是空 */
int isEmpty(Stack *stack) {
    return stack->top == -1;
}

/* 压栈：把节点放到栈顶。栈满了就直接返回（不报错的简单处理） */
void push(Stack *stack, TreeNode *node) {
    if (stack->top == stack->capacity - 1) {
        return;
    }
    stack->arr[++stack->top] = node;   // top 先加 1，再放入节点
}

/* 出栈：返回被弹出的栈顶节点。空栈返回 NULL */
TreeNode *pop(Stack *stack) {
    if (isEmpty(stack)) {
        return NULL;
    }
    return stack->arr[stack->top--];   // 先取出栈顶，top 再减 1
}

/* 非递归前序遍历：先把根压栈；弹出一个节点就打印；右孩子先压、左孩子后压 */
void preorderTraversal(TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }

    Stack *stack = createStack(100);
    push(stack, root);

    while (!isEmpty(stack))
    {
        TreeNode *node = pop(stack);   // 弹出栈顶节点，赋给 node
        printf("%d ", node->data);     // 访问它

        if (node->right != NULL)
        {
            push(stack, node->right);  // 右孩子先压
        }
        if (node->left != NULL)
        {
            push(stack, node->left);   // 左孩子后压（后压的先弹出来）
        }
    }

    free(stack->arr);                  // 申请的内存记得还
    free(stack);
}

int main()
{
    /* 建这棵树：
             1
            / \
           2   3
          / \ / \
         4  5 6  7
    */
    TreeNode *n1 = create_node(1);
    TreeNode *n2 = create_node(2);
    TreeNode *n3 = create_node(3);
    TreeNode *n4 = create_node(4);
    TreeNode *n5 = create_node(5);
    TreeNode *n6 = create_node(6);
    TreeNode *n7 = create_node(7);

    TreeNode *root = n1;   // root 记住树的入口（根节点）

    n1->left = n2;
    n1->right = n3;
    n2->left = n4;
    n2->right = n5;
    n3->left = n6;
    n3->right = n7;

    printf("前序遍历：");
    preorder(root);
    printf("\n");

    printf("中序遍历：");
    inorder(root);
    printf("\n");

    printf("后序遍历：");
    postorder(root);
    printf("\n");

    printf("树的深度：%d\n", depth(root, 1, 0));

    printf("栈模拟前序：");
    preorderTraversal(root);
    printf("\n");

    return 0;
}
