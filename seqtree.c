/* ============================================================
 * glimmer_3_seq_tree.c
 * 来源：3.md 的「Step 1. 使用顺序表完成二叉树操作」
 * 说明：顺序存储二叉树（数组），下标 1 为根，左孩子 2i，右孩子 2i+1。
 *       main 里验证两棵树：三层满二叉树 + 一棵缺节点的树。
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>         // bool ,布尔，表示 "真" 和 "假" 两种状态。

#define MAX_TREE_SIZE 100    // 宏定义

/*
 * 顺序存储二叉树结点
 * - data：结点数据
 * - used：当前位置是否有结点
 */
typedef struct {
    int data;
    bool used;
} SeqTreeNode;               // Seq = Sequential ，意思是顺序的、序列的。

/*
 * 顺序存储二叉树
 * - nodes：结点数组
 * - size：当前实际用到的最大下标
 */
typedef struct {
    SeqTreeNode nodes[MAX_TREE_SIZE];   // 定义了一个数组，名叫 nodes，里面有 100 个格子
    int size;
} SeqBiTree;

/* 1）初始化二叉树：把每个位置标记为空，size 置 0 */
void init_tree(SeqBiTree *tree)
{
    for (int i = 0; i < MAX_TREE_SIZE; i++)
    {
        tree->nodes[i].used = false;    // 每个位置都标记为"没有节点"
        tree->nodes[i].data = 0;
    }
    tree->size = 0;                     // 树刚被初始化后，无有效节点，数组还没用到任何下标
}

/* 2）创建根节点：固定放在下标 1 */
bool set_root(SeqBiTree *tree, int value)
{
    tree->nodes[1].data = value;
    tree->nodes[1].used = true;
    tree->size = 1;                     // 目前用到的最大下标是 1
    return true;
}

/* 3）创建左孩子：父节点下标 parent_node，左孩子下标 2*parent_node */
bool set_left_child(SeqBiTree *tree, int parent_node, int value)
{
    int child = 2 * parent_node;        // child 为左孩子下标

    // 先确认父节点的合法性
    if (parent_node < 1 || parent_node >= MAX_TREE_SIZE)
    {
        return false;                   // 父节点下标本身就不合法
    }
    if (tree->nodes[parent_node].used == false)
    {
        return false;                   // 父节点不存在
    }
    if (child >= MAX_TREE_SIZE)
    {
        return false;                   // 孩子下标越界，数组放不下了
    }

    // 确认父节点合法以及孩子下标未越界后，才能挂左孩子
    tree->nodes[child].data = value;
    tree->nodes[child].used = true;

    if (child > tree->size)
    {
        tree->size = child;             // 更新"用到的最大下标"
    }
    return true;
}

/* 4）创建右孩子：父节点下标 parent_node，右孩子下标 2*parent_node+1 */
bool set_right_child(SeqBiTree *tree, int parent_node, int value)
{
    int child = 2 * parent_node + 1;    // child 为右孩子下标

    if (parent_node < 1 || parent_node >= MAX_TREE_SIZE)
    {
        return false;
    }
    if (tree->nodes[parent_node].used == false)
    {
        return false;
    }
    if (child >= MAX_TREE_SIZE)
    {
        return false;
    }

    tree->nodes[child].data = value;
    tree->nodes[child].used = true;

    if (child > tree->size)
    {
        tree->size = child;
    }
    return true;
}

/* 5）层序遍历：按下标从小到大打印，空位打印 -1，每打完一层换行 */
void level_order(SeqBiTree *tree)
{
    int levelStart = 1;   // 本层第一个节点的下标
    int levelEnd = 1;     // 本层最后一个节点的下标

    while (levelStart <= tree->size)
    {
        int i;
        for (i = levelStart; i <= levelEnd && i <= tree->size; i++)
        {
            if (tree->nodes[i].used)
            {
                printf("%d ", tree->nodes[i].data);
            }
            else
            {
                printf("-1 ");          // 这个位置是空的，按要求打印 -1
            }
        }
        printf("\n");                   // 本层打完，换行

        levelStart = levelEnd + 1;
        levelEnd = levelEnd * 2 + 1;    // 1 -> 3 -> 7 -> 15 ...
    }
}

int main()
{
    /* 第一棵树：三层的满二叉树
             1
            / \
           2   3
          / \ / \
         4  5 6  7
    */
    SeqBiTree tree;
    init_tree(&tree);

    set_root(&tree, 1);
    set_left_child(&tree, 1, 2);
    set_right_child(&tree, 1, 3);
    set_left_child(&tree, 2, 4);
    set_right_child(&tree, 2, 5);
    set_left_child(&tree, 3, 6);
    set_right_child(&tree, 3, 7);

    printf("满二叉树：\n");
    level_order(&tree);
    printf("\n");

    /* 第二棵树：缺节点的树（没建 6，直接建了 7）
             1
            / \
           2   3
          / \   \
         4  5    7
    */
    SeqBiTree tree2;
    init_tree(&tree2);

    set_root(&tree2, 1);
    set_left_child(&tree2, 1, 2);
    set_right_child(&tree2, 1, 3);
    set_left_child(&tree2, 2, 4);
    set_right_child(&tree2, 2, 5);
    set_right_child(&tree2, 3, 7);      // 注意：没有建 6

    printf("有缺口的树：\n");
    level_order(&tree2);

    return 0;
}
