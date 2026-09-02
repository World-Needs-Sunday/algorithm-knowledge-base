---
id: ds-persistent-segtree-3919
title: '可持久化线段树（单点修改单点查询 P3919）'
category: 数据结构
subcategory: 可持久化线段树
tags: ["数据结构", "可持久化线段树", "历史版本", "单点修改", "P3919"]
timeComplexity: '建树O(n)，单次修改O(log n)，单次查询O(log n)'
spaceComplexity: 'O(n+m·log n)'
codePath: '数据结构\可持久化线段树\单点修改，区间求和(第一次自己写的，不完善)\3919(在这个模板基础上的)\Untitled1.cpp'
---

## 算法原理

可持久化线段树（Persistent Segment Tree）的核心思想是"只新建修改路径上的节点，其余节点与旧版本共享"。普通线段树的单点修改会沿根到叶走一条 $O(\log n)$ 长度的路径，沿途修改 $O(\log n)$ 个节点；可持久化线段树在这条路径上为每个节点创建一个新副本，而路径之外的的所有节点直接复用旧版本中的指针。因此每次修改只花费 $O(\log n)$ 的时间和 $O(\log n)$ 的空间，就能得到一个"完整的新版本"——新版本的根指向新创建的路径节点，而那些未修改的子树指针仍指向旧版本的节点。

从版本共享的角度看，新版本与父版本之间共享了 $O(n - \log n)$ 个节点（即整棵树除去修改路径上的 $\log n$ 个节点后，其余全部共享）。这意味着创建一个新版本的额外空间开销仅为 $O(\log n)$ 个节点，而非 $O(n)$。m次修改后总节点数为 $O(n + m \cdot \log n)$。这种"版本间共享不可变子树"的设计是函数式数据结构（Functional Data Structure）的典型范式：节点一旦创建便不可变，修改操作返回新的结构而保留旧结构。

本模板（P3919版本）处理的是"单点修改、单点查询"的场景，即每次修改将位置i的值覆盖为k（overwrite语义）。这与下方"普通模板"的增量累加（nd[rt].val += k）有本质区别：P3919是覆盖式赋值，叶节点直接 `nd[rt].val = k`，路径上每个内部节点的val需要重新计算为左右子之和；而普通模板是增量更新，路径上每个节点都累加k。此外，P3919的查询操作（op==2）虽然不修改数据，但会执行 `root[rt_idx++] = root[v]`，即在版本链中创建一个与版本v完全相同的新版本号——这是因为P3919的题面要求每个操作（无论修改还是查询）都产生一个递增的版本号，查询操作复制的版本保证版本号连续。

空间复杂度为 $O(n + m \cdot \log n)$：初始建树使用 $O(n)$ 个节点（线段树共约2n个节点，此模板手动建根层故约4n），每次修改新增 $O(\log n)$ 个节点（路径长度），m次修改共 $m \cdot \log n$ 个。代码中 `nd.resize(n * 4 + m * ceil(log2(n)))` 正是按此上界预分配。

## 数据结构图示

```
节点共享示意 (修改位置 i=2, 覆盖值 k):

版本0 (旧):    [1,4] -------- [3,4] -------- [5,8] -------- ...
                 |                |
版本1 (新):    [1,4]*-------- [3,4]*-------- [5,8] -------- ...
                                                              |
                                                  (其余子树指针共享旧版本)

  * = 新建节点    无标记 = 直接复用旧版本指针

路径细节 (修改 i=2, 范围 [1,8]):
  版本0:  root0 -> [1,8] -> [1,4] -> [1,2] -> [2,2]      (修改路径)
                           -> [3,4]                       (共享)
                  -> [5,8]                                (共享)

  版本1:  root1 -> [1,8]* -> [1,4]* -> [1,2]* -> [2,2]*  (全部新建)
                              -> [3,4]                    (共享版本0)
                   -> [5,8]                               (共享版本0)

版本链 (P3919 特有: 查询也产生版本号):
  版本0: 初始数组
  版本1: 修改操作 insert(0, n, i, k)       -> root[1]
  版本2: 查询操作 root[2] = root[查询版本]  -> root[2] (复制)
  版本3: 修改操作 insert(版本号, n, i, k)  -> root[3]
```

## 核心操作详解

**init(rt, l, r, A)**：递归建树。若l==r为叶节点，直接赋值 `nd[rt].val = A[l]`。否则取mid，先 `++nd_idx` 分配左孩子下标并递归建左子树，再 `++nd_idx` 分配右孩子下标并递归建右子树，最后 `nd[rt].val = 左.val + 右.val` 回溯求和。注意son_l和son_r初始化为-1，建树后才被赋值为有效下标。

**init(n, m)（重载）**：顶层的初始化入口。读取数组A[1..n]，`root.resize(m+1)` 为最多m+1个版本预留根节点空间，`nd.resize(n*4 + m*ceil(log2(n)))` 按空间上界预分配所有节点。然后手动建根层：取mid=(1+n)/2，分别递归init左子树和右子树，最后 `root[0].val = 左.val + 右.val`。`++rt_idx` 使版本指针前进到1（版本0已建好）。

**insert(rt, old, l, r, i, k)**：核心修改函数（递归版）。第一步 `nd[rt] = nd[old]` 将旧节点整体复制到新节点（包括val和两个子指针）。若l==r（叶节点），执行覆盖赋值 `nd[rt].val = k` 并返回。否则取mid：若i在左半，则 `++nd_idx` 分配新左孩子，递归 `insert(新左, 旧左, l, mid, i, k)`，右指针保留旧版本的（共享）；若i在右半则对称处理。最后 `nd[rt].val = 左.val + 右.val` 回溯更新。关键点：只有修改路径上的节点被新建，另一侧的子指针直接从old复制过来，指向旧版本的节点。

**insert(old, n, i, k)（重载）**：顶层修改入口。`root[rt_idx] = root[old]` 复制旧版本根节点。然后根据i与mid的关系，分配新子节点并递归。最后更新根val并 `++rt_idx`。注意顶层init手动建了根层（root[0]有son_l和son_r），所以insert的重载也需要手动处理根层的分裂，而非直接调用递归insert。

**myget(rt, l, r, i)**：递归单点查询。若i不在[l,r]范围内返回0；若l==r返回叶节点值；否则取mid，按i所在半区递归左或右子树。

**myget(ver, n, i)（重载）**：顶层查询入口。在版本ver的根上，按i与mid的关系递归左或右子树。

## 逐行代码解析

```cpp
#include<bits/stdc++.h>
using namespace std;
/*代紅3919*/
struct node
{
	long long val;       // 节点维护的值 (区间和或单点值)
	int son_l, son_r;    // 左右孩子下标, 初始化为-1表示空
	node() :val(0), son_l(-1), son_r(-1) {}
};
int rt_idx = 0;   // 当前版本号 (root数组的写入位置)
int nd_idx = 0;    // 节点分配计数器 (nd数组的写入位置)
vector<node> root; // root[v] = 版本v的根节点 (存储根层信息)
vector<node> nd;   // 所有线段树节点的存储池

// 递归建树: 在nd[rt]处建区间[l,r]的子树
void init(int rt, int l, int r, const vector<long long>& A)
{
	if (l == r) nd[rt].val = A[l]; // 叶节点: 直接赋值
	else
	{
		int mid = (l + r) >> 1;
		nd[rt].son_l = ++nd_idx;           // 分配左孩子下标
		init(nd[rt].son_l, l, mid, A);      // 递归建左子树
		nd[rt].son_r = ++nd_idx;           // 分配右孩子下标
		init(nd[rt].son_r, mid + 1, r, A);  // 递归建右子树
		nd[rt].val = nd[nd[rt].son_l].val + nd[nd[rt].son_r].val; // 回溯求和
	}
}
// 顶层建树: 读入数组, 预分配空间, 建版本0
void init(int n, int m)
{
	vector<long long> A(n + 1);
	root.resize(m + 1);                              // 预留 m+1 个版本
	nd.resize(n * 4 + m * ceil(log2(n)));            // 预分配节点池 (空间上界)
	for (int i = 1; i <= n; i++) cin >> A[i];
	int l = 1, r = n;
	int mid = (l + r) >> 1;
	// 手动建根层: root[0] 是一个特殊的根节点, 分别建左右子树
	root[rt_idx].son_l = ++nd_idx;
	init(root[rt_idx].son_l, l, mid, A);
	root[rt_idx].son_r = ++nd_idx;
	init(root[rt_idx].son_r, mid + 1, r, A);
	root[rt_idx].val = nd[root[rt_idx].son_l].val + nd[root[rt_idx].son_r].val;
	++rt_idx; // rt_idx 从 0 变为 1, 版本0已建好
}
// 递归修改: 基于旧节点old, 在新节点rt处创建修改路径
void insert(int rt, int old, int l, int r, int i, long long k)
{
	nd[rt] = nd[old]; // 关键: 先整体复制旧节点 (val + 两个子指针)
	if (l == r)       // 叶节点: 覆盖赋值
	{
		nd[rt].val = k; // P3919 是覆盖语义, 不是 += 
		return;
	}
	int mid = (l + r) >> 1;
	if (i <= mid)    // 修改在左半: 新建左孩子, 右孩子共享旧版本
	{
		nd[rt].son_l = ++nd_idx;                    // 分配新左孩子
		insert(nd[rt].son_l, nd[old].son_l, l, mid, i, k); // 递归左
		// nd[rt].son_r 保持从 old 复制来的值 (共享旧版本右子树)
	}
	else            // 修改在右半: 新建右孩子, 左孩子共享旧版本
	{
		nd[rt].son_r = ++nd_idx;
		insert(nd[rt].son_r, nd[old].son_r, mid + 1, r, i, k);
	}
	nd[rt].val = nd[nd[rt].son_l].val + nd[nd[rt].son_r].val; // 回溯求和
}
// 顶层修改: 基于版本old, 创建新版本
void insert(int old, int n, int i, long long k)
{
	int l = 1; int r = n;
	int mid = (l + r) >> 1;
	root[rt_idx] = root[old]; // 复制旧版本根节点
	if (i <= mid)  // 修改在左半
	{
		root[rt_idx].son_l = ++nd_idx;                       // 新建根的左孩子
		insert(root[rt_idx].son_l, root[old].son_l, l, mid, i, k); // 递归
		// root[rt_idx].son_r 共享旧版本
	}
	else          // 修改在右半
	{
		root[rt_idx].son_r = ++nd_idx;
		insert(root[rt_idx].son_r, root[old].son_r, mid + 1, r, i, k);
	}
	root[rt_idx].val = nd[root[rt_idx].son_l].val + nd[root[rt_idx].son_r].val; // 回溯
	++rt_idx; // 版本号+1
}
// 递归单点查询: 在nd[rt]子树中查询位置i的值
long long myget(int rt, int l, int r, int i)
{
	if (i < l || i > r) return 0;   // 不在范围内
	if (l == r) return nd[rt].val;  // 叶节点: 返回值
	int mid = (l + r) >> 1;
	if (i <= mid) return myget(nd[rt].son_l, l, mid, i);      // 在左半
	else return myget(nd[rt].son_r, mid + 1, r, i);           // 在右半
}
// 顶层单点查询: 在版本ver中查询位置i的值
long long myget(int ver, int n, int i)
{
	int l = 1, r = n;
	int mid = (l + r) >> 1;
	if (i <= mid) return myget(root[ver].son_l, l, mid, i);  // 手动处理根层分裂
	else return myget(root[ver].son_r, mid + 1, r, i);
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int N, M;
	cin >> N >> M;
	init(N, M); // 建版本0
	for (int i = 1; i <= M; i++)
	{
		int v, op;
		cin >> v >> op;        // v = 基于的版本号, op = 操作类型
		if (op == 1)          // 修改操作: 基于版本v修改位置idx为k
		{
			int idx;
			long long k;
			cin >> idx >> k;
			insert(v, N, idx, k); // 创建新版本
		}
		else                  // 查询操作: 查版本v的位置x的值
		{
			int x;
			cin >> x;
			cout << myget(v, N, x) << '\n';
			// P3919 特有: 查询也要产生新版本号
			// root[rt_idx++] = root[v] 复制版本v作为新版本
			root[rt_idx++] = root[v];
		}
	}
	return 0;
}
```

## 复杂度分析

时间复杂度：建树 $O(n)$（递归建n个叶节点和n-1个内部节点）；单次修改 $O(\log n)$（沿修改路径递归 $\log n$ 层）；单次查询 $O(\log n)$（沿查询路径递归 $\log n$ 层）。m次操作总时间 $O(n + m \cdot \log n)$。

空间复杂度：$O(n + m \cdot \log n)$。初始建树约使用4n个节点（含根层手动分配），每次修改新增 $\log n$ 个节点（修改路径长度），m次修改共 $m \cdot \log n$ 个。代码中 `nd.resize(n * 4 + m * ceil(log2(n)))` 按此上界预分配。

## 适用场景

可持久化线段树适用于需要访问历史版本的数据场景，包括：版本回溯（P3919——查询任意历史版本的单点值）、区间第k小（主席树/Chairman Tree——将每个前缀作为一个版本，利用版本差分查询区间第k小）、可持久化数组（函数式数组，支持在任意历史版本上修改和查询）、历史区间求和等。P3919本版是"单点修改单点查询"的最基础形态，是理解可持久化线段树的入门模板。

## 常见陷阱与注意事项

- **空间预分配必须足够大**：`nd.resize(n * 4 + m * ceil(log2(n)))` 是空间上界。若m次操作中修改次数较多，空间不足会导致越界。注意ceil(log2(n))在n=1时为0，可能导致空间不足，实际使用时常取n*40作为保险上界。
- **查询操作也会创建新版本**：P3919题面要求每个操作（含查询）都产生递增版本号，因此 `root[rt_idx++] = root[v]` 不可省略。若省略则版本号不连续，后续基于版本号的修改/查询会错位。
- **nd[rt] = nd[old] 的顺序**：必须先复制旧节点再判断是否为叶节点。若先判断叶节点直接赋值则跳过了复制，导致内部节点的子指针未正确共享。
- **根层手动分裂**：此模板的init和insert的重载版本都手动处理了根层的mid分裂（因为root[0]是一个特殊的"超级根"节点），这与标准模板直接从根递归不同，容易在移植时遗漏。
- **son_l/son_r初始化为-1**：空孩子用-1表示，递归查询时若碰到-1会导致越界。建树时所有内部节点的子指针都被赋值，叶节点的子指针保持-1（不使用），myget在叶节点处直接返回不会访问子指针。

## 对比与扩展

- **P3919（覆盖式）vs 普通模板（增量式）**：P3919的叶节点执行 `nd[rt].val = k`（覆盖），适合"将位置i改为k"的语义；普通模板执行 `nd[rt].val += k`（增量累加），适合"将位置i增加k"的语义。两者的路径回溯也不同：P3919回溯时 `val = 左+右`（因为叶被覆盖），普通模板路径上每个节点都 += k（因为增量沿路径传播）。
- **可持久化线段树 vs 可持久化树状数组（BIT）**：线段树更通用，支持任意区间查询和单点修改的历史版本；BIT仅适合前缀和类操作，但常数更小、代码更短。
- **扩展：主席树（Chairman Tree）**：基于可持久化线段树，将数组前缀[1..i]作为第i个版本，利用版本i和版本i-1的差分在 $O(\log n)$ 内查询区间 $[l,r]$ 的第k小值。
- **扩展：可持久化Trie**：将同样的"只新建路径节点"思想应用于Trie，支持历史版本上的异或最大值查询等。
- **扩展：可持久化平衡树**：利用Treap（非旋转式）实现可持久化平衡树，支持历史版本上的插入/删除/排名查询。
