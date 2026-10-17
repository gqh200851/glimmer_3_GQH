## md 的框架和部分表述有 AI 协助;

## 代码全部在本地编译运行过；

## 涉及到底层（栈、栈帧）的部分我尽量尝试用自己的理解进行了描述；

##                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       顺序存储和链式存储各对应一个代码文件，同时md 里也按题目顺序把代码都贴了。

------

# C-EASY-3：二叉树与递归

## Step 1. 什么是树

### 1. 什么是树？什么是二叉树？该怎么定义一棵二叉树？

**树的定义：** 树是 n 个节点组成的有限集合，用来表示"一对多"的层次关系。

- n = 0 时是空树；
- 非空时有且仅有一个没有父节点的节点，叫**根节点**；
- 除了根节点，每个节点有且只有一个父节点；
- 每个节点可以有 0 个或多个子节点。

| 术语 | 意思 |
|---|---|
| 根节点 | 树最顶上的那个节点，没有父节点 |
| 叶子节点 | 没有子节点的节点 |
| 父节点 / 子节点 | 直接相连的上下两层节点，上为父、下为子 |
| 兄弟节点 | 拥有同一个父节点的节点 |
| 层 | 根是第 1 层，根的孩子是第 2 层，以此类推 |
| 深度（高度） | 树的最大层数 |
| 度 | 一个节点有几个子节点，就叫它的度是几 |

**二叉树：** 每个节点最多有两个子节点的树，而且这两个子节点是有顺序的——左边的叫**左孩子**（左子树），右边的叫**右孩子**（右子树），左右不能互换。哪怕一个节点只有一个孩子，也要说清楚它是左孩子还是右孩子。

二叉树有一个很自然的**递归定义**：

> 一棵二叉树，要么是空树；要么由一个根节点、一棵左子树、一棵右子树组成，而左子树和右子树本身又各自是一棵二叉树。

这个定义先放在这里，后面写递归遍历的时候会发现，代码几乎就是照着这个定义翻译出来的。

注：顺便认识两种"长得很整齐"的二叉树（题目里提到了完全二叉树）

- **满二叉树**：每一层的节点都长满了。第 k 层最多有 2^(k-1) 个节点，深度为 h 的满二叉树总共有 2^h - 1 个节点。
- **完全二叉树**：除了最后一层之外，其他层全都长满；最后一层的节点从左到右紧挨着排，中间不许有缺口。

```
满二叉树：            完全二叉树：
       1                  1
      / \                / \
     2   3              2   3
    / \ / \            / \
   4  5 6  7          4   5
```

完全二叉树的节点在数组里能排得严丝合缝，这正好是下面顺序存储的基础。

---

### 2. 二叉树在 C 语言中怎么存储？有什么特点？

我了解到了两种方式。

**方式一：顺序存储（数组）**

把节点按层放进数组里，**为了方便**，让根节点占下标 1（下标 0 空着不用）：

下标：  0          1                2   3        4   5   6   7
数据： [空]       1                2   3        4   5   6   7
                   根(第1层)       第2层          第3层

下标之间的关系（这是这一章最常用的公式）：

- 下标为 i 的节点，左孩子下标是 **2i**；
- 右孩子下标是 **2i + 1**；
- 反过来，父节点下标是 **i / 2**（整数除法，自动舍去小数）。

我按层数验证了一下这个规律：第 1 层占 1；第 2 层占 2~3；第 3 层占 4~7。假设第 k 层占的是 2^(k-1) ~ 2^k-1，那么这一层每个节点的孩子都排在 2i 和 2i+1 的位置，正好把 2^k ~ 2^(k+1)-1 排满，严丝合缝地接上第 k+1 层。所以公式对每一层都成立（题目说的数学归纳法，我目前是这样理解的）。

**方式二：链式存储**

像上一章的链表那样，每个节点里放两个指针，分别指向左孩子和右孩子。这是 Step 2 的内容。

两种方式的对比：

| 对比项 | 顺序存储（数组） | 链式存储（指针） |
|---|---|---|
| 找孩子/父亲 | 下标直接算，知道下标就知道` O(1)`(时间复杂度) | 只能顺着指针一层一层往下走`O(h)` |
| 空间 | 空位置也要占格子，树越歪越浪费 | 有几个节点就申请几个节点 |
| 适合的树 | 完全二叉树这种排得满的 | 空节点多、长得歪的树 |
| 额外开销 | 几乎没有 | 每个节点要多存两个指针 |

**二叉树的特点小结（我自己的理解）：**

1. 每个节点最多两个分叉，左右有严格顺序；
2. 它是**非线性**结构，不能像链表那样"一条路走到黑"；
3. 它的结构本身是递归的（子树还是树），所以后面的操作几乎都可以用递归写。

---

### 使用顺序表完成二叉树操作

题目给出的结构体：

```c
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
} SeqTreeNode;               // Seq =  Sequential ，意思是顺序的、序列的。


/*
 * 顺序存储二叉树
 * - nodes：结点数组
 * - size：数组最大容量
 */
typedef struct {                         
    SeqTreeNode nodes[MAX_TREE_SIZE];   // 定义了一个数组，名叫 nodes，里面有 100 个格子，每个格子都是一个 SeqTreeNode 类型(包含 data 和 used) 
    int size;                     
} SeqBiTree;
```

**为什么节点里除了 data 还要有一个 used？** 因为数组里会有"空位置"，光靠 data 没法区分这个位置是真的没节点，还是恰好存了个 0。所以用 bool 类型的 used 打标记：true 表示这个位置有节点，false 表示空。

**关于 size，我不是很确定我理解对没有：** 结构体注释写的是"size：数组最大容量"，而容量是 `MAX_TREE_SIZE` 这个宏定死的，不会变。我最后把 size 理解成"**当前实际用到的最大下标**"，或者说"即时(有效)的数组最大容量"。如：一开始是 0，放了根节点变成 1，放到第 3 层的节点就变成 7。后面层序遍历要打印到哪里为止，就靠它判断。

如果把size理解成固定的数组最大容量，已经有MAX_TREE_SIZE了，这样子size显得有点多余。



#### 1）初始化函数 init_tree

由题知：需要把每个节点都标记清空， 并将size 置 0。

因为要在函数里修改外面的树，形参照例写成指针（上一章 swap 的教训：不传地址就改不到外面的东西）。

```c
void init_tree(SeqBiTree *tree)
{
    for (int i = 0; i < MAX_TREE_SIZE; i++)
    {
        tree->nodes[i].used = false;	// 每个位置都标记为"没有节点"
        tree->nodes[i].data = 0;   		
    }
    tree->size = 0;                     // 树刚被初始化后，无有效节点，数组还没用到任何下标
}
```



#### 2）创建根节点 set_root

根节点固定放在下标 1：

```c
bool set_root(SeqBiTree *tree, int value)
{
    tree->nodes[1].data = value;
    tree->nodes[1].used = true;
    tree->size = 1;                    // 目前用到的最大下标是 1
    return true;
}
```

**为什么根放 1 而不是 0？** 除了下标与数据位置一一对应外，如果根在 0，那按照前面根据数学归纳法的结论，它的左孩子是 2×0 = 0，与根自己重复。所以下标 0 直接空出来不用。



#### 3）创建左孩子 set_left_child

父节点下标是 parent_node，左孩子下标就是 2×parent_node。

挂孩子之前要先检查两件事：父节点得真的存在（不能朝棍木里挂孩子）；孩子下标不能超出数组范围。

**我自己写第一遍的时候搞忘要检查父节点是否合法了。**后面补上了。

```c
bool set_left_child(SeqBiTree *tree, int parent_node, int value)
{
    int left = 2 * parent_node;      // left 为左孩子下标
	
    // 先确认父节点的合法性
    if (parent_node < 1 || parent_node >= MAX_TREE_SIZE)
    {
        return false;                 // 父节点下标本身就不合法
    }
    if (tree->nodes[parent_node].used == false)
    {
        return false;                 // 父节点不存在
    }
    if (left >= MAX_TREE_SIZE)
    {
        return false;                 // 孩子下标越界，数组放不下了
    }
	
    // 	确认父节点合法以及孩子下标未越界后，才能挂左孩子
    tree->nodes[left].data = value;
    tree->nodes[left].used = true;

    if (left > tree->size)
    {
        tree->size = left;            // 更新"用到的最大下标"
    }
    return true;
}
```



#### 4）创建右孩子 set_right_child

和左孩子几乎一样，只是下标变成 2×parent_node + 1：

```c
bool set_right_child(SeqBiTree *tree, int parent_node, int value)
{
    int Rchild = 2 * parent_node + 1;  // 右孩子下标

    if (parent_node < 1 || parent_node >= MAX_TREE_SIZE)
    {
        return false;
    }
    if (tree->nodes[parent_node].used == false)
    {
        return false;
    }
    if (Rchild >= MAX_TREE_SIZE)
    {
        return false;
    }

    tree->nodes[Rchild].data = value;
    tree->nodes[Rchild].used = true;

    if (Rchild > tree->size)
    {
        tree->size = Rchild;
    }
    return true;
}
```



#### 5）层序遍历 level_order

要求：按数组下标从小到大打印，空位打印 -1，**每打完一层换一次行**。

直接从 1 打印到 size 不难，难点在"到哪换一行"。我把每层的最后一个下标列出来找规律：

第 1 层：1 个节点，结束于 1
第 2 层：2 个节点，结束于 3
第 3 层：4 个节点，结束于 7
第 4 层：8 个节点，结束于 15

这是按照一开始的想法的一版。因为不知道怎么判断换行，所以另外定义了专门判断是否需要换行的函数。

```c
// 这个是专门判断是否需要换行的函数
bool end_need_huanhang(int i)
{
    i = i + 1;
    while (i % 2 == 0)
    {
        i = i / 2;
    }
    if (i == 1)
    {
        return true;               
    }
    else
    {
        return false;
    }        
}
/*其实可以改成这样：
bool end_need_huanhang(int i)
{
    i = i + 1;
    while (i % 2 == 0)
    {
        i = i / 2;
    }
    return i == 1;
}*/


// 这是主体
void level_order(SeqBiTree *tree)
{
    
    int i = 1;
    while (i <= tree->size && i <= MAX_TREE_SIZE)  	// 保险起见
    {
        if (tree->nodes[i].used == true)
        {
            printf("%d ", tree->nodes[i].data);
        }
        else
        {
            printf("-1 ");
        }
        
        if (end_need_huanhang(i) == true)  // 其实可以也可以改成这样：if (end_need_huanhang(i))
        {
            printf("\n");
        }        
    i++;
    }    
}
```

**然后发现完全可以优化换行的判断。** !?笨笨!?



注意到规律：下一层的结尾 = 上一层结尾 × 2 + 1（1 → 3 → 7 → 15 …）。

可以用两个变量分别记"本层开头"和"本层结尾"，一层一层打：

```c
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
                printf("-1 ");        // 这个位置是空的，按要求打印 -1
            }
        }
        printf("\n");                 // 本层打完，换行

        levelStart = levelEnd + 1;
        levelEnd = levelEnd * 2 + 1;  // 1 -> 3 -> 7 -> 15 ...
    }
}
```

for 循环里的 `i <= levelEnd && i <= tree->size` 是为了处理最后一层没长满的情况：只打印到 size（数组实际用到的最大下标） 就停，不能把后续没初始化的内容也打出来。



#### 6）主函数建一棵三层二叉树并验证

```c
int main()
{
    SeqBiTree tree;
    init_tree(&tree);

    /* 建一棵三层的二叉树：
             1
            / \
           2   3
          / \ / \
         4  5 6  7
    */
    set_root(&tree, 1);
    
    set_left_child(&tree, 1, 2);
    set_right_child(&tree, 1, 3);
    
    set_left_child(&tree, 2, 4);
    set_right_child(&tree, 2, 5);
    
    set_left_child(&tree, 3, 6);
    set_right_child(&tree, 3, 7);

    printf("满二叉树：\n");
    level_order(&tree);

    return 0;
}
```

运行结果：

```
满二叉树：
1
2 3
4 5 6 7
```



和预期一致。我又故意建了一棵缺节点的树（没建 6，直接建了 7），验证空位会不会打印 -1：

```
             1
            / \
           2   3
          / \   \
         4  5    7
```

```c
int main()
{
	SeqBiTree tree2;
	init_tree(&tree2);

	set_root(&tree2, 1);

	set_left_child(&tree2, 1, 2);
	set_right_child(&tree2, 1, 3);

	set_left_child(&tree2, 2, 4);
	set_right_child(&tree2, 2, 5);

	set_right_child(&tree2, 3, 7);   

	printf("有缺口的树：\n");
	level_order(&tree2); 
}
```

运行结果：

```
有缺口的树：
1
2 3
4 5 -1 7
```

下标 6 是空的，确实打印了 -1。



#### 关于顺序存储的空间浪费：

顺序存储是"用空间复杂度换时间复杂度"，只适合接近于完全二叉树这种"排得满"的树，因为空节点很多的二叉树会浪费大量的空间。

------

## Step 2. 链二叉树

### 1. 链二叉树的结构体和创建节点

链式存储的思路和上一章的链表一模一样，只是链表节点只有一个 next，而二叉树节点有两个"下一层"的指针：

```c
#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int data;               // 节点存储的数据
    struct TreeNode *left;  // 指向左子树的指针
    struct TreeNode *right; // 指向右子树的指针
} TreeNode;
```

回忆链表：先 malloc 一个新节点，再让上一个节点的 next 指向它，链条就接上了。树同理，只不过要分别接 left 和 right。

**create_node：创建一个数据为 value、左右指针都为 NULL 的新节点。**

```c
TreeNode* create_node(int value)
{
    TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode)); // 申请一块刚好能装下 TreeNode 的内存，并把 malloc 返回的那个void * 类型的 "通用指针" 强制转换成 TreeNode 类型的，然后把这块内存的地址存到一个叫 node 的指针变量里
    node->data = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}
```

### 2. 在主函数里手动创建一棵树

建这棵树：

```
        1
       / \
      2   3
     / \ / \
    4  5 6  7
```

我的做法是人工先把 7 个节点一个一个全部申请出来，再用指针把它们连起来：

```c
int main()
{
    TreeNode *n1 = create_node(1);
    TreeNode *n2 = create_node(2);
    TreeNode *n3 = create_node(3);
    TreeNode *n4 = create_node(4);
    TreeNode *n5 = create_node(5);
    TreeNode *n6 = create_node(6);
    TreeNode *n7 = create_node(7);

    TreeNode *root = n1;          // 本来可以直接把这步省略的,但为了方便记住根节点，选择另赋 root = n1 来记住树的入口n1是root(根节点)。
             
    n1->left = n2, n1->right = n3; 
    n2->left = n4, n2->right = n5;
    n3->left = n6, n3->right = n7;
    
    return 0;
}
```

树和链表一样，必须有一个 root 指针记住根节点——根是整棵树唯一的入口，一旦 root 丢了，整棵树就都找不到了（和链表丢了 head 一个道理）。

### 3. 思考：为什么简单的 for 循环打印不了链二叉树？

 这个有点反直觉，因为上一章遍历链表就是一个 while 循环：

```c
Node *p = head;
while (p != NULL) { printf(...); p = p->next; }
```

为什么树不行？我的理解是：

1. **链表是一条线，树是分叉的。** 链表每个节点只有一个出口 next，一个游标 p 从头走到尾就能遍历所有节点，而树每个节点有两个出口 left 和 right，用相同的(基于for的)游走无法遍历。
2. **节点只会"向下指"，回不去。** 结构体里没有指向父节点的指针。游标从 1 走到左孩子 2，再走到 4，把左边走完以后，它没法回到 1 去走右边的 3——没有指针指回去。
3. **要记的"回头路"可能有很多条。** 每遇到一个分叉点，都得把"这个节点的右子树还没看"记下来。单靠一个游标变量记不住这么多东西，得有个地方把它们存起来。

**说白了就是 for/while 循环适合"路线是一条直线、状态简单"的重复。而树的遍历路线是会"绕回来"的，需要一个能帮我们记路的东西。**

这个东西有两个版本：

- **递归**：把"记路"交给系统，系统用函数调用栈帮你记；（栈，stack，是一种**"后进先出"**的数据结构。）
- **自己造一个栈**：拿数据结构手动记，也就是这一章最后面的题目。

------

## Step 3. 递归

### 1. 递归是什么

递归就是一个函数在执行过程中直接或间接地调用自己。

```c
void f() {
    f(); // 自己调用自己
}
```

但这样会无限调用下去，程序直接崩溃。所以一个能正常工作的递归必须有两个部分：

1. **递归条件**：什么时候继续调用自己；
2. **结束条件（出口）**：什么时候停止调用、开始返回。

对树的遍历来说，结束条件为：**节点为 NULL（空树）时，什么都不用做，直接返回。** 因为叶子节点的左右指针都是 NULL，递归走到 NULL 自然就停了。

### 2. 前序、中序、后序遍历

经过学习后得知，三种遍历的区别只在于"**根节点在什么时候被访问**"（打印数据）：

- **前序遍历**：根 → 左子树 → 右子树
- **中序遍历**：左子树 → 根 → 右子树
- **后序遍历**：左子树 → 右子树 → 根

```c
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
```

在主函数里调用：

```c
printf("前序遍历：");
preorder(root);
printf("\n");

printf("中序遍历：");
inorder(root);
printf("\n");

printf("后序遍历：");
postorder(root);
printf("\n");
```

运行结果：

```
前序遍历：1 2 4 5 3 6 7
中序遍历：4 2 5 1 6 3 7
后序遍历：4 5 2 6 7 3 1
```

**我在学习完栈的相关知识之后，尝试重新用栈去理解上面的递归版本的遍历：（以前序为例）**

**前序遍历的函数调用栈（系统自动管理的栈） ：**

![](otherthings\stack.png)



### 3. 为什么只改 printf 的位置，就能实现三种遍历？

因为三次递归是这三个动作的三种排列：

[处理根]  [递归左子树]  [递归右子树]

**以前序遍历为例：**

前面就正常从root开始，顺着最左支遍历到叶子；

关键在最后：当 `preorder()` 执行完返回时，程序会**闪现回到当初调用它的那一行**，也就是` preorder(root->left)` 调用结束的位置，即“返回上一个父节点”，然后接着执行下一行 `preorder(root->right)`

打印顺序就是：**1 → 2 → 4 → 5 → 3 → 6 → 7**，和运行结果一致。

中序、后序无非是把"打印"这个动作挪到了左调用之后、或者两个调用之后。比如中序遍历走到节点 2 时，是先把 4 这棵左子树全部走完并打印了 4，回到 2 才打印 2，所以左半部分输出是 `4 2 5`。

树的结构是递归的，先前的子节点也是之后的父节点，与用来遍历的函数的递归结构相仿。

### 4. depth 函数：统计树的深度

题目给的函数签名：

```c
int depth(TreeNode *root, int current_depth, int max_depth);
```

- `current_depth`：当前这个节点在第几层。从根开始数，根是第 1 层，所以主函数调用时传 1；
- `max_depth`：目前为止找到的最大层数，一开始什么都没找到，传 0。

思路：每走到一个节点，就看看它所在的层数有没有刷新纪录，然后继续往左右孩子走（往下走一层，current_depth 加 1）；走到 NULL 就说明这条路到头了，把当前的纪录返回。

其实跟前面的遍历差不多，只是把printf换成了两个depth参数的传递

```c
int depth(TreeNode *root, int current_depth, int max_depth)
{
    if (root == NULL)
    {
        return max_depth;          // 空树：纪录不变，返回
    }

    if (current_depth > max_depth)
    {
        max_depth = current_depth; // 当前节点刷新了最大层数
    }

    max_depth = depth(root->left, current_depth + 1, max_depth);
    max_depth = depth(root->right, current_depth + 1, max_depth);

    return max_depth;
}
```

**必须用 `max_depth = depth(...)` 把返回值接回来**，纪录才能一层层带回来。

#### 为什么后面两个 int 不用指针变量？

上一章的结论是"想在函数里改外面的变量，就得传指针"。但这两个 int 都不满足需要指针的情况：

- **current_depth 不需要被共享，反而需要"各层各的"。** 传值时，每次递归都会在自己的栈帧里有一份独立的副本，父节点传 `current_depth + 1` 给孩子，自己的值一点不变。这正好就是我们想要的：第 2 层的节点永远认为自己在第 2 层，不会被孩子那层的计算污染。
- **max_depth 虽然要更新，但它有返回值这条通道。** 每一层算完都通过 return 把最新纪录交回去，父层接住就行，不需要用指针直接改外面的变量。

指针真正的用武之地是"函数内部要直接改外面的变量、又没法靠返回值传递"的时候（比如 swap 要同时改两个变量）。这里一个参数需要独立、一个参数能靠返回值，所以用普通 int 就够了。

#### 另一种写法：

我在询问AI与上面这个深度统计相关的问题的时候发现另外一种方法，连两个辅助参数都不用，让函数直接返回"这棵树的深度"就行：

```c
/* 返回以 root 为根的这棵树的深度 */
int depth2(TreeNode *root)
{
    if (root == NULL)
    {
        return 0;                  // 空树深度为 0
    }

    int leftDepth = depth2(root->left);
    int rightDepth = depth2(root->right);

    if (leftDepth >= rightDepth)
    {
        return leftDepth + 1;      // 左右子树谁更深，就听谁的，再加上自己这一层
    }
    else
    {
        return rightDepth + 1;
    }
}
```

思路：空树深度是 0；非空树的深度 = 左右子树中较深的那个深度 + 1（自己这层）。

虽然这样可以少传入两个参数，但是这样思路对于我来说我感觉要更加难想、更加难理解一点 。

**递归两种不同的风格**：题目给的 depth 是"带着答案往下走"，depth2 是"等下面的结果返回后，在回来的路上把答案拼出来"。



### 5. 我对递归和循环的感悟

写完整章，我觉得它们既像又不像：

- **像的地方：** 都在重复执行一段逻辑，都必须有停下来的条件（递归的结束条件 ≈ 循环条件不成立的时候）。没有出口的递归和死循环一样，都会让程序卡死。
- **不像的地方：** 循环是"自己手动维护一个状态（比如游标 p），一步步往前推"；递归是"把一个缩小后的问题交给另一个自己，等它给我结果"。
- **递归的代码更贴近问题的定义。** 二叉树的定义本身就是递归的（左右子树还是树），所以遍历代码像是对定义直接翻译
- **循环更省、更可控，但处理分叉时要自己准备"缓存记路工具"。** 这一章最后的栈模拟就是例子——没有递归帮忙，就得自己造一个栈，把系统替我做的事重做一遍。

我在学习完栈的相关知识后。带着栈的知识复盘了前面的基于递归的前序遍历，觉得可以这样总结：**递归 = 函数调用 + 系统自动管理的一个栈。**  



### 6. 底层：栈和栈帧

递归在底层靠**函数调用栈**实现。先认识栈：栈是一种"后进先出"（LIFO）的数据结构，可以想象成往一个桶里放盘子，最后放进去的盘子最先被拿出来。只能在栈顶放入（压栈/push）和取出（出栈/pop）。

每调用一次函数，系统都会在栈上创建一份新的**函数调用记录**，叫**栈帧**。栈帧里一般保存：

1. 这次调用的参数；
2. 这次调用的局部变量；
3. 返回地址（函数执行完从哪一行继续往下跑）；
4. 一些寄存器的旧值。

题目里用阶乘举了例子，`factorial(5)` 的过程是这样的：

```
递推（不断入栈）：          回归（不断出栈）：
  factorial(5)               factorial(1) 返回 1
  factorial(4)               factorial(2) = 2*1，返回 2
  factorial(3)               factorial(3) = 3*2，返回 6
  factorial(2)               factorial(4) = 4*6，返回 24
  factorial(1)  ←栈顶        factorial(5) = 5*24，返回 120
```

递归不是"把函数代码复制了很多份"，而是**同一份代码被调用了很多次**，每次调用都有自己独立的栈帧，所以参数和局部变量互不干扰。比如题目里的：

```c
void test(int n) {
    int x = n;
    printf("%d\n", x);
    if (n > 1) {
        test(n - 1);
    }
}
```

调用 `test(3)` 时，`test(3)` 里的 x 是 3，`test(2)` 里的 x 是 2，`test(1)` 里的 x 是 1——这是三个不同栈帧里的三个不同变量，只是同名。这也解释了前面遍历里，每一层的 root、current_depth 为什么都不会互相打架。

**为什么递归太深会崩溃？** 栈空间是有限的，每递归一层就压一个栈帧，层数太多，栈帧把栈空间占满，就会**栈溢出**（stack overflow）。开头那个没有结束条件的 `f()` 就是这么死的。

### 7. 用栈模拟前序遍历（不用递归）

题目给了栈的定义和四个基本操作，我先把它们读懂并加上了简单注释：

```c
typedef struct Stack {
    TreeNode **arr;  // 存放节点指针的数组
    int top;         // 栈顶元素的下标，-1 表示空栈
    int capacity;    // 栈的容量
} Stack;
```

这里的 `TreeNode **arr` 我盯着看了一会，用上一章的指针知识拆开它：栈里要存的是"节点指针"，类型是 `TreeNode*`；而 arr 是 malloc 出来的数组，数组名指向它的第一个元素，第一个元素的类型是 `TreeNode*`，所以 arr 就成了"指向 TreeNode* 的指针"，即 `TreeNode**`。

**注：**

1 **我当时看了好久才理解arr是动态数组  。     : (**

 **动态数组（指针 + malloc）：大小运行时定，想多大就多大。指向一块 malloc 出来的连续内存，用 [下标]的方式去访问它。**



2 `Stack *stack = malloc(sizeof(Stack));`**看到这一步的第一时间我很困惑，明明还没有计算出 Stack 里 arr 的空间，怎么就能直接 malloc Stack 的空间呢，怎么知道此时应该 malloc 多大的空间呢？后面我才发现虽然这个 arr 参与构成了一个动态数组，但是arr指向的其实是这个动态数组的起点（或者说第一个元素的位置）。所谓“ Stack 里 arr ”其实应该是“ Stack 里 *arr ”，Stack 里装的 *arr 只是一个指针（8 字节），并不和真正的数组一样，空间需要包含整个数组里所有元素。**

 **所以此处malloc 8+4+4 = 16 字节即可。**

```c
/* 创建一个容量为 capacity 的栈 */
Stack *createStack(int capacity) {
    Stack *stack = malloc(sizeof(Stack));  // 关于这一步见上：
    stack->arr = malloc(sizeof(TreeNode *) * capacity); // 申请动态数组的空间
    stack->top = -1;     // 空栈，栈顶下标为 -1
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

/* 出栈：取出并返回栈顶节点。空栈返回 NULL */
TreeNode *pop(Stack *stack) {
    if (isEmpty(stack)) {
        return NULL;
    }
    return stack->arr[stack->top--];   // 先取出栈顶，top 然后减 1
}
```

#### 补全 preorderTraversal

前序的顺序是"根 → 左 → 右"：

1. 先把根节点压栈；
2. 弹出一个节点的同时就立刻打印它（一见面就访问，符合前序）；
3. 把它的**右孩子先压栈，左孩子后压栈**。因为栈是后进先出，后压进去的左孩子会先被弹出来，顺序才是"先左后右"。
4. 重复，直到栈空。

```c
void preorderTraversal(TreeNode *root)
{
    if (root == NULL)
    {
        return;
    }

    Stack *stack = createStack(100);   // 用 stack 接住这新创建的容量为 100 的栈的地址

    push(stack, root);

    while (!isEmpty(stack))
    {
        TreeNode *node = pop(stack);   // 弹出栈顶节点，赋给 node（接下来要访问的那个节点）
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
```

我第一次写的时候顺手先压了左孩子、再压右孩子，结果打印出来变成 `1 3 7 6 2 5 4`，右子树跑到前面去了。卡了一下才反应过来"后进先出"这个顺序，必须反着压。

**注意事项：**

1. 边界检查传入的根节点是否为空的；
2. 孩子反着压
3. 申请的内存记得还

手动模拟前几步的过程（栈写成"栈底 → 栈顶"）：

| 步骤 | 弹出 | 打印 | 压栈 | 栈里还剩 |
|---|---|---|---|---|
| 1 | — | — | 压入 1 | 1 |
| 2 | 1 | 1 | 先压 3，再压 2 | 3 → 2 |
| 3 | 2 | 2 | 先压 5，再压 4 | 3 → 5 → 4 |
| 4 | 4 | 4 | 无孩子 | 3 → 5 |
| 5 | 5 | 5 | 无孩子 | 3 |
| 6 | 3 | 3 | 先压 7，再压 6 | 7 → 6 |
| 7 | 6 | 6 | 无孩子 | 7 |
| 8 | 7 | 7 | 无孩子 | 空 |

主函数里调用：

```c
printf("栈模拟前序：");
preorderTraversal(root);
printf("\n");
```

完整运行结果：

```
前序遍历：1 2 4 5 3 6 7
中序遍历：4 2 5 1 6 3 7
后序遍历：4 5 2 6 7 3 1
树的深度：3
栈模拟前序：1 2 4 5 3 6 7
```

写完这个模拟我最大的体悟：前面问"为什么 for 循环打印不了树"，是因为**光有一个游标不够，还得有个东西帮你记回头路**。递归时，系统的函数调用栈在背后替你记；非递归时，就得自己造一个栈、手动把要回来访问的节点一个个压进去。

而递归写起来短，不是因为它没有栈，而是那个栈不用我自己写。

------

## 学习小结

这一章概念不多，但递归绕了我挺久，几个最关键的点：

1. **二叉树每个节点最多两个孩子，左右有顺序**；树本身是递归定义的（子树还是树）；
2. **顺序存储靠下标公式**：父 i、左 2i、右 2i+1、父亲 i/2。找得快，但空节点浪费空间，只适合排得满的完全二叉树；
3. **链式存储用 left / right 两个指针**，省空间但只能一层层往下找，根节点 root 是唯一入口；
4. **递归必须有递归条件和结束条件**，树遍历的结束条件就是节点为 NULL；
5. **前中后序只差 printf 的位置**：根在前、在中、在后；递归返回时会自动回到当初调用的那一行继续执行；
6. **递归的底层是函数调用栈**：每层调用一个栈帧，参数和局部变量各自独立；递归太深会栈溢出；
7. 用栈手动模拟遍历之后，"递归为什么能遍历树"就彻底不玄了——**系统不过是替我维护了一个栈而已**。
