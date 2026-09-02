---
id: ds-splay
title: 'Splay树（伸展树）'
category: 数据结构
subcategory: 平衡树
tags: ["数据结构", "平衡树", "Splay", "伸展树", "自平衡BST"]
timeComplexity: '均摊O(log n)'
spaceComplexity: 'O(n)'
codePath: '数据结构\平衡树\splay树\基础模板(可能有问题，空节点为0，半抄写版)\Untitled1.cpp'
---

## 算法原理

Splay树是一种自平衡二叉搜索树（BST），其核心思想是"局部性"（access locality）：最近被访问的节点很可能在不久后再次被访问。因此Splay在每次查找、插入、删除操作之后，都会通过"伸展"操作（splay）将刚刚操作的节点旋转到根的位置。BST性质要求左子树所有值小于根、右子树所有值大于根；splay操作在保持BST性质的前提下，通过一系列旋转把目标节点提升到根，同时缩短沿途路径。由于频繁访问的节点被"拉"到靠近根的位置，后续访问的成本就会降低。

单纯使用单旋（rot）将节点逐层上移会导致严重的退化问题。考虑一条向右倾斜的链状BST，若每次都从最深处节点单旋到根，树的结构几乎不变（仅相当于把链整体翻转），访问复杂度退化为 $O(n)$。Sleator和Tarjan在原始论文中证明：当目标节点x与其父y、祖父z呈"同向"（zig-zig，即x和y同为左孩子或同为右孩子）时，如果直接单旋x，树的形状几乎不变；正确的做法是"先旋父y再旋x"（从上往下旋），这样能将路径长度大致减半。而当x与y呈"异向"（zig-zag，即x是左孩子而y是右孩子，或反之）时，需要"先旋x再旋x"（即标准的双旋，类似于AVL树中的LR/RL旋转）。

具体而言，splay函数中的双旋判定为 $$(\text{tree}[y].\text{son}[0] == x) \oplus (\text{tree}[z].\text{son}[0] == y)$$：当异或结果为真（x和y处于不同方向，即zig-zag）时先调用 `rot(x)`；当异或结果为假（x和y处于同方向，即zig-zig）时先调用 `rot(y)`。无论哪种情况，最后都会再执行一次 `rot(x)`。这种"先旋父再旋自己"的同向双旋策略是防止链退化的关键，也是Splay与朴素单旋BST的根本区别。

Splay的均摊 $O(\log n)$ 复杂度基于势能函数（potential function）分析。定义势能 $\Phi(T) = \sum_u \log_2(\text{size}(u))$，其中 size(u) 为节点u的子树大小（含自身），$\log_2(\text{size}(u))$ 称为节点u的"秩"（rank）。Splay定理（Access Lemma）证明：将节点x伸展到根的均摊代价至多为 $3(r'(x) - r(x)) + 1$，其中r(x)是伸展前x的秩，r'(x)是伸展后的秩（此时x为根，$r'(x) = r(T)$即整棵树的秩）。由于 $r'(x) = r(T)$ 是固定上界，一串操作中秩的变化求和后大量项相互抵消（telescoping），总均摊代价为 $O(n \cdot \log n + m \cdot \log n)$，即每次操作均摊 $O(\log n)$。此外，哨兵节点（-INF和+INF）在初始化时插入，保证前驱/后继查找不会越界：任何元素的前驱至少是-INF，后继至少是+INF，从而 `get_pre` 和 `get_suc` 始终能返回有效的节点下标。

## 数据结构图示

```
单旋 rot(x) 示意 (此处展示右旋, 即 x 是 y 的左孩子 k=0):

  旋转前 (右旋):            旋转后:
        y                      x
       / \                    / \
      x   C      -->         A   y
     / \                        / \
    A   B                      B   C

  k = (tree[y].son[1] == x) = 0  (x 是左孩子)
  tree[y].son[0] = tree[x].son[1]   (B 变为 y 的左孩子)
  tree[x].son[1] = y                (y 变为 x 的右孩子)
  tree[z].son[...] = x             (x 接替 y 在 z 中的位置)


双旋策略 (splay):

  zig-zig (同向, x和y都是左孩子):
    旋转前:        先旋y:         再旋x:
        z             z              x
       /             /                \
      y             x                  z
       \             \                /
        x             y              y
                       \            /
                        (子树)    (子树)

  zig-zag (异向, x是左孩子, y是右孩子):
    旋转前:        先旋x:         再旋x:
        z             z              x
         \             \            / \
          y             x          z   y
         /               \
        x                 y

  判定: (tree[y].son[0]==x) ^ (tree[z].son[0]==y)
    结果为 true  -> 异向 (zig-zag) -> 先 rot(x)
    结果为 false -> 同向 (zig-zig) -> 先 rot(y)
```

## 核心操作详解

**upsz(x)**：更新节点x的子树大小。$\text{sz} = \text{sz}_{\text{左子树}} + \text{sz}_{\text{右子树}} + \text{cnt}$。其中cnt是该值的出现次数（允许重复元素）。注意tree[0]代表空节点，其sz必须为0，这样空子树贡献为0。

**rot(x)**：单旋，将x上移一层、y下移一层。通过 $k = (\text{tree}[y].\text{son}[1] == x)$ 判断x是y的哪个孩子：k=1表示x是右孩子（需左旋），k=0表示x是左孩子（需右旋）。$k \oplus 1$ 则给出另一个方向。x的内侧子树（靠近y的那棵）被移交给给y，y成为x的孩子，x接替y在z中的位置。旋转后调用 upsz(y)、upsz(x) 更新大小。

**splay(x, k)**：将x旋转到k的下方（k=0时到根）。循环条件 $\text{tree}[x].\text{fa} \neq k$。若有祖父z且z≠k，则用异或判定同向/异向决定先旋谁。最后必定执行一次 rot(x)。若k==0则更新全局root。

**find(v)**：从root出发沿BST性质向下走，$\text{tree}[x].\text{son}[v > \text{tree}[x].\text{val}]$ 利用比较结果直接作为方向索引（0=左，1=右）。走到值为v的节点或走到底为止，然后splay到根。find之后root要么是v本身，要么是离v最近的节点。

**get_pre(v) / get_suc(v)**：前驱/后继。先find(v)把v或最近节点旋到根。若root的值已经小于v则root即前驱；否则进入左子树一直往右走到底（左子树中的最大值）。后继对称。哨兵-INF保证最小元素也有前驱。

**del(v)**：经典删除。先取pre=get_pre(v)、suc=get_suc(v)，然后 `splay(pre,0); splay(suc,pre)`。此时pre为根、suc为pre的右孩子。由于 $\text{pre} < v < \text{suc}$ 且v唯一，v必定是suc的左孩子（$\text{tree}[\text{suc}].\text{son}[0]$）。若 $\text{cnt} > 1$ 则减少计数，否则直接将suc的左孩子置空。最后splay保持局部性。

**getrk(v)**：排名查询。find(v)后，若 $\text{root.val} < v$（v不在树中，root是最大的<v的节点），则排名 $= \text{sz}_{\text{左子树}} + \text{root.cnt}$（所有 $\leq$ root 的元素个数=v之前元素个数）；若 $\text{root.val} \geq v$（v在树中），则排名 $= \text{sz}_{\text{左子树}}$（严格小于v的元素个数）。

**getval(k)**：第k小查询。`k+=1` 跳过-INF哨兵。沿树下行：若 $\text{sz}_{\text{左子树}} + \text{cnt} < k$，说明答案在右子树，k减去 $\text{sz}_{\text{左子树}} + \text{cnt}$ 后向右走；若 $\text{sz}_{\text{左子树}} \geq k$，说明答案在左子树，向左走；否则当前节点即为答案。最后splay到根。

**insert(v)**：从root向下搜索v。若找到则cnt++；否则新建节点，挂到p的对应方向。无论哪种情况最后都splay(x,0)把新插入或已存在节点旋到根。

## 逐行代码解析

```cpp
#include<bits/stdc++.h>
using namespace std;
struct sytree
{
	int fa;        // 父节点下标
	int son[2];    // son[0]=左孩子, son[1]=右孩子
	int val;       // 节点存储的值
	int sz;        // 子树大小 (含自身, 含重复计数)
	int cnt;       // 该值的出现次数
	sytree(int a)  // 构造函数
	{
		son[0] = 0; son[1] = 0;
		val = a;
		cnt = 1;
		fa = 0;
		sz = 1;
	}
};
vector<sytree> tree;  // tree[0] 是哨兵空节点, 必须保证 sz=0, cnt=0
int root = 0;         // 全局根节点下标, 0表示空树

inline void upsz(int x)//修改大小 
{
	// 子树大小 = 左子树 + 右子树 + 自身重复次数
	// tree[0].sz 必须为0, 否则空子树会贡献错误值
	tree[x].sz = tree[tree[x].son[0]].sz + tree[tree[x].son[1]].sz + tree[x].cnt;
}
void rot(int x) //旋转: 将x上移一层, y下移
{
	int y = tree[x].fa;         // y 是 x 的父节点
	int z = tree[y].fa;         // z 是 y 的父节点 (x 的祖父)
	int k = tree[y].son[1] == x; // k=1: x是右孩子(左旋); k=0: x是左孩子(右旋)
	// x 的内侧子树移交给 y
	tree[y].son[k] = tree[x].son[k ^ 1];   // y的k方向孩子 = x的(k^1)方向孩子
	tree[tree[y].son[k]].fa = y;          // 更新该子树的父指针
	// y 下移成为 x 的孩子
	tree[x].son[k ^ 1] = y;               // x 的 (k^1) 方向孩子 = y
	tree[y].fa = x;                        // y 的父 = x
	// x 接替 y 在 z 中的位置
	tree[z].son[tree[z].son[1] == y] = x; // z 中原来指向 y 的位置改为指向 x
	tree[x].fa = z;                        // x 的父 = z
	upsz(y), upsz(x);                     // 先更新下层 y, 再更新上层 x
}
void splay(int x, int k)//将x移动到k下面 (k=0 表示移到根)
{
	while (tree[x].fa != k)
	{
		int y = tree[x].fa;
		int z = tree[y].fa;
		if (z != k) // 存在祖父, 可做双旋判定
		{
			// 异或为真: x和y方向不同 (zig-zag) -> 先旋 x
			// 异或为假: x和y方向相同 (zig-zig) -> 先旋 y
			(tree[y].son[0] == x) ^ (tree[z].son[0] == y) ? rot(x) : rot(y);
		}
		rot(x); // 最后必定再旋一次 x
	}
	if (k == 0) root = x; // 若目标是根则更新全局根
}
void find(int v)//找到v (或最近节点), 并将其 splay 到根
{
	int x = root;
	// v > tree[x].val 为真(1)走右, 为假(0)走左; 同时检查不等于v且子节点存在
	while (tree[x].son[v > tree[x].val] && tree[x].val != v)
		x = tree[x].son[v > tree[x].val];
	splay(x, 0);
}
int get_pre(int v) //找到v的前驱 (小于v的最大值)
{
	find(v);            // v 或最近节点旋到根
	int x = root;
	if (tree[x].val < v) return x; // 若 root 已经 < v, root 即前驱
	x = tree[x].son[0];           // 否则进左子树
	while (tree[x].son[1]) x = tree[x].son[1]; // 左子树中一直往右走到底
	return x;
}
int get_suc(int v)//找到v的后继 (大于v的最小值)
{
	find(v);
	int x = root;
	if (tree[x].val > v) return x; // 若 root 已经 > v, root 即后继
	x = tree[x].son[1];           // 否则进右子树
	while (tree[x].son[0]) x = tree[x].son[0]; // 右子树中一直往左走到底
	return x;
}
void del(int v)//删除v
{
	int pre = get_pre(v);  // 前驱 (因哨兵存在必定有值)
	int suc = get_suc(v);  // 后继
	splay(pre, 0); splay(suc, pre);
	// pre 为根, suc 为 pre 的右孩子
	// 由于 pre < v < suc 且 v 唯一, v 一定是 suc 的左孩子
	int tmp = tree[suc].son[0];
	if (tree[tmp].cnt > 1)  // 重复元素: 仅减少计数
	{
		--tree[tmp].cnt;
		splay(tmp, 0);
	}
	else                    // 唯一元素: 直接摘除
	{
		tree[suc].son[0] = 0;
		splay(suc, 0);
	}
}
int getrk(int v)//找到v的排名 (小于v的元素个数+1)
{
	find(v);
	// 若 root.val < v (v不在树中): 排名 = 左子树sz + root.cnt (所有<=root的个数)
	// 若 root.val >= v (v在树中):  排名 = 左子树sz (严格<v的个数)
	return tree[root].val < v ? tree[tree[root].son[0]].sz + tree[root].cnt : tree[tree[root].son[0]].sz;
}
int getval(int k)//找到第k小的数
{
	int x = root;
	k += 1; // +1 跳过 -INF 哨兵 (哨兵占第1名)
	while (true)
	{
		int t = tree[x].son[0]; // 左子树
		if (tree[t].sz + tree[x].cnt < k) // 答案在右子树
		{
			k -= tree[t].sz + tree[x].cnt; // 减去左子树和自身的元素数
			x = tree[x].son[1];
		}
		else
		{
			if (tree[t].sz >= k) x = tree[x].son[0]; // 答案在左子树
			else break; // 左子树sz < k <= 左子树sz+cnt, 当前节点即答案
		}
	}
	splay(x, 0);
	return tree[x].val;
}
void insert(int v)//添加元素
{
	int x = root, p = 0;
	while (x && tree[x].val != v) // 沿BST向下搜索v
	{
		p = x;
		x = tree[x].son[v > tree[x].val];
	}
	if (x) tree[x].cnt++; // 已存在: 增加计数
	else
	{
		x = tree.size();  // 新节点下标 = 当前vector大小
		tree[p].son[v > tree[p].val] = x; // 挂到父节点p的对应方向
		tree.emplace_back(v);             // 在vector末尾构造新节点
		tree[x].fa = p;                   // 设置父指针
	}
	splay(x, 0); // 旋到根, 保证局部性
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	tree.emplace_back(0);              // tree[0] = 空哨兵节点
	tree[0].sz = 0, tree[0].cnt = 0;   // 空节点大小和计数必须为0
	insert(-0x3f3f3f3f);               // 左哨兵 -INF
	insert(0x3f3f3f3f);                // 右哨兵 +INF
	int n;
	cin >> n;
	for (int i = 1; i <= n; i++)
	{
		int op, x;
		cin >> op >> x;
		switch (op)
		{
		case 1: insert(x); break;                 // 插入
		case 2: del(x); break;                     // 删除
		case 3: cout << getrk(x) << '\n'; break;   // 查排名
		case 4: cout << getval(x) << '\n'; break;  // 查第k小
		case 5: cout << tree[get_pre(x)].val << '\n'; break; // 前驱
		case 6: cout << tree[get_suc(x)].val << '\n'; break; // 后继
		}
	}
	return 0;
}
```

## 复杂度分析

Splay树每次操作的均摊时间复杂度为 $O(\log n)$。这一结论基于Sleator-Tarjan的势能分析：定义势能函数 $\Phi(T) = \sum_u \log_2(\text{size}(u))$，其中size(u)是节点u的子树大小。Access Lemma证明，将节点x伸展到根的实际代价加上势能变化不超过 $3 \cdot \log_2(\text{size}(T)) + 1 = O(\log n)$。在一串m次操作中，初始势能为0、最终势能不超过 $n \cdot \log_2(n)$，因此总实际代价 $\leq$ 总均摊代价 + 初始势能 $= O((m+n) \cdot \log n)$，每次操作均摊 $O(\log n)$。

空间复杂度为 $O(n)$，每个节点存储fa、son[2]、val、sz、cnt共5个整型字段。注意vector动态增长，tree[0]占用一个空节点位。

## 适用场景

Splay树适用于需要动态维护有序序列并支持排名查询、前驱后继查找、插入删除的场景。由于其独特的"最近访问优先"特性，Splay在访问局部性强的数据上表现优于严格平衡树。更重要的是，Splay是Link-Cut Tree（LCT）的基础数据结构——LCT利用Splay维护实链剖分，支持动态森林上的链查询、连通性判断、换根等操作。此外，Splay天然支持split/merge操作（通过splay到边界后断开/连接），适用于需要序列分割合并的场景。

## 常见陷阱与注意事项

- **必须初始化哨兵节点**：`insert(-0x3f3f3f3f)` 和 `insert(0x3f3f3f3f)` 必须在main开头执行，否则 `get_pre(最小元素)` 或 `get_suc(最大元素)` 会因找不到前驱/后继而出错。
- **tree[0]空节点的sz和cnt必须为0**：代码注释标注"可能有问题"正是因为此点。upsz函数会访问 `tree[tree[x].son[0]].sz`，当son[0]=0（空）时取到tree[0].sz，若不为0则子树大小计算错误。main中 `tree[0].sz = 0, tree[0].cnt = 0` 显式修正了这一点。
- **rot后必须调用upsz**：旋转改变了子树结构，若忘记更新sz则后续的排名查询和第k小查询全部出错。代码中 `upsz(y), upsz(x)` 先更新下层y再更新上层x，顺序不能反。
- **getval中的k+=1**：由于插入了-INF哨兵，最小的真实元素排名为2（第1名是-INF），因此查询第k小实际要找第k+1名。
- **del中v必须是suc的左孩子**：这一前提依赖于"v在树中唯一存在且pre<v<suc"的BST性质。若树中不存在v，get_pre和get_suc仍会返回最近的节点，但del的逻辑前提不成立，可能导致误删。

## 对比与扩展

- **Splay vs AVL**：AVL严格保持平衡因子 $\leq 1$，查询性能稳定 $O(\log n)$ 但旋转频繁、常数大；Splay不严格平衡但代码简洁、均摊 $O(\log n)$，且自带局部性优化。
- **Splay vs Treap**：Treap通过随机堆优先级保证平衡，期望 $O(\log n)$，实现简单且支持可持久化；Splay无法高效可持久化（旋转会破坏版本结构），但均摊分析更紧。
- **Splay vs 红黑树**：红黑树是工业级平衡树（STL的map/set底层），保证 $O(\log n)$ 最坏复杂度但实现极其复杂；Splay实现简单，适合竞赛使用。
- **扩展：LCT（Link-Cut Tree）**：基于Splay维护动态树，支持link/cut/makeroot/链查询等操作，是Splay最重要的应用延伸。
- **扩展：序列Splay**：将Splay按序列下标组织，利用splay到根+子树拼接实现区间反转、区间移动等操作。
