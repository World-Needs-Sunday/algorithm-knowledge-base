---
id: ds-segtree-basic
title: '线段树基础模板（区间加法+区间求和 P3372）'
category: 数据结构
subcategory: 线段树
tags: ["数据结构", "线段树", "区间修改", "区间求和", "lazy标记", "P3372"]
timeComplexity: '建树O(n)，单次O(log n)'
spaceComplexity: 'O(4n)'
codePath: '数据结构\普通线段树\基础模板\Untitled1.cpp'
---

## 算法原理

线段树（Segment Tree）是一种基于分治思想的二叉树数据结构，用于维护区间上的可合并信息。它将整个区间 [1, n] 递归地二分为两个子区间，直到每个叶子节点对应一个长度为 1 的单元区间。树的根节点（通常编号为 1）维护整个区间 [1, n] 的聚合信息（本模板中为区间和）；对于任意节点 rt，其左子节点编号为 `rt<<1`（即 2*rt），维护区间 [l, mid]；右子节点编号为 `(rt<<1)+1`（即 2*rt+1），维护区间 [mid+1, r]，其中 `mid=(l+r)/2`。由于每次二分区间长度减半，树的高度为 $O(\log n)$，这是线段树能够高效支持区间操作的根本原因。

如果每次区间修改都把所有叶子节点更新一遍，复杂度将退化到 $O(n)$。Lazy Propagation（延迟标记）是解决这一问题的关键技巧：当一个修改操作完全覆盖某节点 rt 所代表的区间时，我们只更新该节点的 tree 值并在 lazy 数组上"记账"，不再继续向下递归到叶子。`lazy[rt]` 表示"rt 的子树中所有节点都还需要加上 lazy[rt]，但目前尚未真正下传"。只有当后续操作需要访问 rt 的子节点时，才通过 `my_down` 函数把 lazy 标记下传给两个孩子，并把自身的 lazy 清零。这样每次修改至多影响 $O(\log n)$ 个节点，从而保证单次操作 $O(\log n)$。

线段树的四个核心操作紧密配合：build（建树）自底向上初始化每个节点的区间和；pushdown（my_down）在访问子节点前把父节点的延迟标记下传，保证子节点 tree 值的正确性；update（my_add）在修改时先 pushdown，若完全覆盖则打标记返回，否则递归两个孩子并在返回时 pushup（用孩子值更新自身）；query（my_get）同样先 pushdown，再根据区间相交情况决定返回、合并或递归。pushdown 必须在 update 和 query 递归前都调用，否则子节点的 tree 值会因为没有应用父节点的历史修改而出错。

本模板的建树部分有一个特殊处理：叶子节点的初始值被赋为它的下标 l（即 `tree[rt]=l`），而非从输入读取。这意味着初始时位置 i 的值为 i，对应一类"初始序列为 1,2,3,...,n"的题目变体。若要改成读入任意数组 a[]，只需把 `tree[rt]=l` 改为 `tree[rt]=a[l]` 即可。

## 数据结构图示

```
                 [1,8] rt=1  sum=36
                /              \
        [1,4] rt=2            [5,8] rt=3
        /        \             /        \
   [1,2] rt=4  [3,4] rt=5  [5,6] rt=6  [7,8] rt=7
    /    \      /    \       /    \       /    \
 [1]rt=8 [2]rt=9 [3]rt=10 [4]rt=11 [5]rt=12 [6]rt=13 [7]rt=14 [8]rt=15
```

Lazy 标记下传示例：对 [1,8] 全区间加 k 后，根节点 rt=1 被完全覆盖，直接执行 $\text{tree}[1] \leftarrow \text{tree}[1] + 8k$ 和 $\text{lazy}[1] \leftarrow \text{lazy}[1] + k$，子节点不变。下次查询 [1,4] 时调用 `my_down(1,1,8)`，把 lazy[1] 下传：

```
   my_down(1,1,8):  mid=4
     tree[2] += lazy[1]*(4-1+1) = 4k   lazy[2] += k
     tree[3] += lazy[1]*(8-4)   = 4k   lazy[3] += k
     lazy[1] = 0
```

之后 rt=2、rt=3 的 tree 已正确，可继续向下递归。

## 核心操作详解

### init(rt, l, r) / init(n)

- **作用**：建树 / 分配数组并启动建树。
- **参数**：rt 当前节点编号，l,r 当前区间，n 总规模。
- **算法步骤**：若 `l==r` 为叶子，赋初值（本模板赋下标 l）；否则取 `mid=(l+r)/2`，递归建左子 [l,mid] 和右子 [mid+1,r]，回溯时 `tree[rt]=tree[左]+tree[右]`。`init(n)` 负责 lazy 开 4n+1 初始化为 0，tree 开 4n+1，然后从 `init(1,1,n)` 开始。
- **关键细节**：数组必须开 4n 而非 2n。原因是当 n 不是 2 的幂时，线段树最后一层会有大量"空缺"，按 `rt<<1` 编号会导致最大节点编号接近 $4n$。开 $4n+1$ 才能保证不越界。

### my_down(i, l, r)

- **作用**：将节点 i 的 lazy 标记下传给左右子节点。
- **参数**：i 节点编号，l,r 区间。
- **算法步骤**：若 `lazy[i]==0` 或 `l==r`（叶子无需下传）直接返回；`mid=(l+r)/2`；左子区间长度 $mid-l+1$，对左子执行 $\text{tree}[i \ll 1] \leftarrow \text{tree}[i \ll 1] + \text{lazy}[i] \times (mid-l+1)$ 和 $\text{lazy}[i \ll 1] \leftarrow \text{lazy}[i \ll 1] + \text{lazy}[i]$；右子区间长度 $r-mid$，同理；最后 `lazy[i]=0`。
- **关键细节**：tree 的增量必须是"lazy × 区间长度"，因为 lazy 表示"每个元素都要加 lazy"，区间和的增量就是 $\text{lazy} \times \text{len}$。lazy 是累加的（+=）而不是覆盖。

### my_get(rt, l, r, nl, nr)

- **作用**：查询区间 [nl,nr] 的和。
- **参数**：rt,l,r 当前节点，nl,nr 查询区间。
- **算法步骤**：先 `my_down` 保证子节点正确；若 [l,r] 与 [nl,nr] 不相交返回 0；若 [l,r] 被 [nl,nr] 完全包含返回 `tree[rt]`；否则取 mid 递归左右子并相加。
- **关键细节**：进入函数第一行就 my_down，因为后续可能要读子节点的 tree，必须先把父标记下传。

### my_add(rt, l, r, nl, nr, k)

- **作用**：区间 [nl,nr] 每个元素加 k。
- **算法步骤**：先 `my_down`；不相交返回；若完全包含，执行以下更新后返回（打标记不下传）：

  $$\text{tree}[rt] \leftarrow \text{tree}[rt] + (r-l+1) \times k$$

  $$\text{lazy}[rt] \leftarrow \text{lazy}[rt] + k$$

  否则递归左右子，回溯 `tree[rt]=tree[左]+tree[右]`（pushup）。
- **关键细节**：完全包含时只更新自己并打标记，子节点保持旧值——这就是 lazy 的核心；回溯的 pushup 保证父节点区间和正确。

## 逐行代码解析

```cpp
#include<bits/stdc++.h>
/*代紅3372*/ 
using namespace std;
vector<long long> tree;
vector<long long> lazy;
```

使用全局 vector 存储线段树和 lazy 数组，元素类型 `long long` 防止区间和溢出。

```cpp
void init(int rt,int l,int r)
{
	if(l == r)
	{
		tree[rt] = l;          // 叶子：初值=下标（题目变体）
		return;
	}
	int mid = (l + r) >> 1;
	init(rt << 1 , l , mid);              // 建左子 [l,mid]
	init((rt << 1) + 1, mid + 1,r);       // 建右子 [mid+1,r]
	tree[rt] = tree[rt << 1] + tree[(rt << 1) + 1]; // pushup：区间和=左+右
}
void init(int n)
{
	lazy.resize(4 * n + 1 , 0);   // lazy 初始化为 0（加法单位元）
	tree.resize(n * 4 + 1);       // tree 大小 4n+1
	init(1,1,n);                  // 从根开始建树
}
```

`tree[rt]=l` 是本模板的特点：叶子初值取下标。`resize(4*n+1,0)` 把 lazy 全置 0，避免垃圾值被当成下传标记。

```cpp
void my_down(int i ,int l,int r)
{
	if(lazy[i] == 0 || l == r) return;   // 无标记或叶子，无需下传
	int mid = (l + r) >> 1;
	lazy[i << 1] += lazy[i];                              // 左子标记累加
	tree[i << 1] += (long long)(mid - l + 1) * lazy[i];   // 左子和 += lazy*左子长度
	lazy[(i << 1) + 1] += lazy[i];                        // 右子标记累加
	tree[(i << 1) + 1] += (long long)(r - mid) * lazy[i]; // 右子和 += lazy*右子长度
	lazy[i] = 0;                       // 清空自身标记
}
```

注意 `(long long)` 强转避免 int 溢出：`lazy[i]` 与长度相乘可能超过 int 范围。左子长度为 `mid-l+1`，右子长度为 `r-mid`，二者之和恰为 `r-l+1`。

```cpp
long long my_get(int rt,int l,int r,const int& nl,const int& nr)
{
	my_down(rt,l,r);                       // 关键：先下传标记
	if(l > nr || r < nl) return 0;        // 不相交
	else if(nl <= l && nr >= r) return tree[rt];  // 完全包含
	else 
	{
		int mid = (l + r) >> 1;
		return my_get(rt << 1 , l , mid , nl , nr)            // 左子
		     + my_get((rt << 1) + 1, mid + 1,r,nl,nr);        // 右子
	}
}
```

三段式判断：不相交返回 0、完全包含返回 tree、部分相交递归两边求和。`const int&` 传参避免拷贝。

```cpp
void my_add(int rt,int l,int r,const int& nl,const int& nr,const long long& k)
{
	my_down(rt,l,r);                       // 先下传
	if(l > nr || r < nl) return;          // 不相交
	if(nl <= l && nr >= r)                 // 完全包含：打标记
	{
		tree[rt] += (long long)(r - l + 1) * k;   // 区间和 += k*长度
		lazy[rt] += k;                            // 标记累加
		return;
	}
	int mid = (l + r) >> 1;
	my_add(rt << 1,l,mid,nl,nr,k);                  // 递归左子
	my_add((rt << 1) + 1,mid + 1,r,nl,nr,k);        // 递归右子
	tree[rt] = tree[rt << 1] + tree[(rt << 1) + 1]; // pushup
}
```

完全包含分支里 `return` 前只更新 tree 和 lazy，不递归子节点，这正是 lazy 的体现；若误把子节点也更新会退化到 $O(n)$。

```cpp
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);   // 快读
	int n,m;
	cin >> n >> m;
	init(n);
	for(int i = 0;i < m;i++)
	{
		int op;
		cin >> op;
		if(op == 1)                       // 区间加
		{
			int x,y;
			long long k; 
			cin >> x >> y >> k;
			my_add(1,1,n,x,y,k);
		}
		else                              // 区间求和
		{
			int x,y;
			cin >> x >> y;
			cout << my_get(1,1,n,x,y) << '\n';
		}
	}
	return 0;
} 
```

主程序读入 n,m，建树后处理 m 次操作：op=1 调 my_add，否则调 my_get 输出。

## 复杂度分析

- **建树 init**：每个节点访问且仅访问一次，共约 $2n-1$ 个节点，复杂度 $O(n)$。
- **修改 my_add / 查询 my_get**：递归路径沿树高 $O(\log n)$ 下降。由于区间操作在每一层至多访问 4 个节点（左右各延伸的边界节点 + 完全包含的内部节点），总访问节点数 $O(\log n)$。单次操作 $O(\log n)$。
- **空间**：tree、lazy 各开 $4n+1$。当 n 非 2 的幂时，按 `rt<<1` 编号会使最大下标接近 $4n$，故需 $4n$ 而非 $2n$。空间 $O(4n) = O(n)$。
- **总复杂度**：m 次操作 $O((n + m) \log n)$。

## 适用场景

适用于"区间加法修改 + 区间求和查询"的静态序列问题，如洛谷 P3372。它是所有 lazy 线段树的入门模板，掌握后可推广到区间最值、区间赋值、双标记（乘法线段树）等变体。当操作只有单点修改 + 区间查询时，树状数组（BIT）代码更短、常数更小，是更优选择；一旦涉及区间修改，lazy 线段树则是标准解法。

## 常见陷阱与注意事项

1. **数组大小必须开 4n 而非 2n**：n 非 2 的幂时编号会接近 $4n$，开 $2n$ 会导致越界 RE。
2. **my_down 必须在 update 和 query 递归子节点之前调用**，否则子节点 tree 值未应用父标记，结果错误。
3. **lazy 数组必须初始化为 0**（加法单位元），否则会把垃圾值当下传标记。
4. **tree 增量是 lazy×区间长度，不是 lazy 本身**：$\text{tree}[\text{child}] \leftarrow \text{tree}[\text{child}] + \text{lazy} \times \text{len}$，漏乘长度是常见错误。
5. **tree 和 k 必须用 long long**：$n=10^5$、$k=10^4$ 时区间和可达 $10^9$，多次累加会爆 int。
6. **完全包含分支不要误递归子节点**：本模板在该分支只更新 tree 和 lazy 后 return，若误更新子节点会退化到 $O(n)$。

## 对比与扩展

- **与树状数组（BIT）对比**：BIT 只支持可差分的操作（前缀和可逆），区间加+区间查需两棵 BIT，代码极短但灵活性差；线段树支持任意可合并信息（最值、GCD、历史最值等），适用面广得多。
- **与平衡树/Splay 对比**：线段树下标固定，不支持动态插入删除；若需动态维护有序序列，应使用平衡树或 FHQ Treap。
- **扩展方向**：(1) 区间赋值（覆盖型 lazy，需额外标记区分"未赋值"）；(2) 双标记乘法线段树（见 ds-segtree-mul）；(3) 离散化线段树处理大值域（见 ds-segtree-discretize）；(4) 可持久化线段树（主席树）支持历史版本查询；(5) 李超树维护线段/函数最值；(6) 线段树合并解决树上权值统计。
