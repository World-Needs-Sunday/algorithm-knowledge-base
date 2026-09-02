---
id: ds-persistent-segtree-template
title: '可持久化线段树（单点修改区间求和 普通模板）'
category: 数据结构
subcategory: 可持久化线段树
tags: ["数据结构", "可持久化线段树", "历史版本", "区间求和"]
timeComplexity: '建树O(n)，单次修改O(log n)，单次区间查询O(log n)'
spaceComplexity: 'O(n+m·log n)'
codePath: '数据结构\可持久化线段树\单点修改，区间求和(第一次自己写的，不完善)\普通模板\Untitled1.cpp'
---

## 算法原理

本模板与P3919版本共享相同的可持久化核心思想——"只新建修改路径上的节点，其余节点与旧版本共享"。可持久化线段树的关键洞察在于：一次单点修改只影响从根到叶的 $O(\log n)$ 个节点，因此只需为这 $\log n$ 个节点创建新副本，其余子树指针直接复用旧版本。新版本与父版本共享 $O(n - \log n)$ 个节点，额外空间开销仅为 $O(\log n)$。

本模板与P3919版本的关键区别在于修改语义。P3919是"覆盖式赋值"（point assign）：叶节点执行 `nd[rt].val = k`，内部节点回溯时 `val = 左.val + 右.val` 重新从子节点求和。而本模板是"增量累加"（point add）：在修改路径上的每个节点（包括内部节点和叶节点）都执行 `nd[rt].val += k`。这一差异的本质原因是：在"区间求和"语义下，位置i增加k意味着所有范围包含i的节点（即整条根到叶路径上的每个节点）的区间和都应增加k。因此沿路径每个节点直接 += k 即可正确维护区间和，无需在回溯时从子节点重新求和。

从实现细节看，本模板的insert递归函数在复制旧节点后立即 `nd[rt].val += k`，然后判断是否为叶节点（若是则return，因为增量已经施加），否则递归进入包含i的那一侧子树创建新节点。注意本模板的insert末尾没有 `nd[rt].val = 左+右` 的回溯语句——因为增量k已经沿路径逐层施加，每个节点的val已经正确反映了"+k"后的区间和。相比之下，P3919的insert末尾必须有 `nd[rt].val = 左+右`，因为叶节点被覆盖为k（不是+k），内部节点的val必须从子节点重新计算才能反映覆盖后的正确和。

本模板的查询操作（myget）支持任意区间[ml, mr]的求和，采用标准线段树区间查询的三分支递归：完全不相交返回0、完全包含返回节点val、部分相交则递归左右子树求和。查询操作不创建新版本（与P3919不同），因此本模板的版本号仅在修改时递增。空间预分配使用经验上界 `nd.resize(n * 40)`，即假设最多n个初始节点加m次修改各 $\log n$ 个新节点，n*40是一个足够宽松的常数上界。

## 数据结构图示

```
增量累加示意 (位置 i 增加 k, 范围 [1,8]):

版本0 (旧):  [1,8]v -------- [1,4]v -------- [5,8]v -------- [3,4]v ...
                |                |                |
版本1 (新):  [1,8]*+k ------ [1,4]*+k ------ [5,8]v --------- [3,4]v ...
                                                               |
                                                   (含i的路径上每节点 +=k, 其余共享)

  * = 新建节点    v = 共享旧版本    +k = 该节点 val += k

路径细节 (位置 i=3, 增加 k, 范围 [1,8]):
  版本0:  root0 -> [1,8]v  -> [1,4]v  -> [3,4]v  -> [3,3]v   (含i=3的路径)
                          -> [1,2]v                      (不含i, 共享)
                 -> [5,8]v                               (不含i, 共享)

  版本1:  root1 -> [1,8]*+k -> [1,4]*+k -> [3,4]*+k -> [3,3]*+k (路径上全部新建并 +=k)
                             -> [1,2]v                        (共享版本0)
                  -> [5,8]v                                   (共享版本0)

区间查询示意 (在版本1查询 [2,5] 的和):
  [1,8]*  -> 部分相交, 递归左右
    [1,4]* -> 部分相交, 递归左右
      [1,2]v -> 完全包含 [2,5], 返回 val
      [3,4]* -> 完全包含 [2,5], 返回 val
    [5,8]v -> 部分相交, 递归左右
      [5,6]v -> 完全包含, 返回 val
      [7,8]v -> 不相交, 返回 0
  (全区间 [1,n] 查询时直接返回根 val, 小优化)
```

## 核心操作详解

**init(rt, l, r, A)**：递归建树。若l==r为叶节点赋值A[l]；否则分配左右孩子下标（`++nd_idx`），递归建子树，回溯 `val = 左.val + 右.val`。与P3919完全相同。

**init(n, m)（重载）**：顶层建树。`nd.resize(n * 40)` 预分配经验上界（比P3919的 `n*4 + m*ceil(log2(n))` 更宽松但更简单）。手动建根层：取mid，分配左右孩子递归建树，回溯求和。`++rt_idx` 前进到版本1。

**insert(rt, old, l, r, i, k)**：核心修改函数（递归版）。`nd[rt] = nd[old]` 复制旧节点。`nd[rt].val += k` 对当前节点施加增量。若l==r则return（叶节点，增量已施加）。否则取mid：若i在左半，分配新左孩子并递归（右子树共享）；若i在右半，分配新右孩子并递归（左子树共享）。注意末尾无需回溯求和——因为沿路径每层都已 += k。关键点：与P3919的 `nd[rt].val = k`（仅叶覆盖）+ `val = 左+右`（内部回溯）不同，本模板是 `nd[rt].val += k`（路径每层都加）。

**insert(old, n, i, k)（重载）**：顶层修改入口。`root[rt_idx] = root[old]` 复制旧根，`root[rt_idx].val += k` 施加增量。按i与mid关系分配新子节点并递归。`++rt_idx` 版本号递增。

**myget(rt, l, r, ml, mr)**：标准区间查询递归。三分支：`l > mr || r < ml`（不相交）返回0；`ml <= l && mr >= r`（完全包含）返回 `nd[rt].val`；否则取mid递归左+右之和。

**myget(ver, n, ml, mr)（重载）**：顶层查询入口。若 `ml==1 && mr==n`（全区间查询）直接返回 `root[ver].val`（小优化，避免递归）。否则手动分裂根层，递归左+右子树之和。

## 逐行代码解析

```cpp
#include<bits/stdc++.h>
using namespace std;
struct node
{
	long long val;       // 节点维护的区间和
	int son_l,son_r;     // 左右孩子下标, -1表示空
	node():val(0),son_l(-1),son_r(-1){}
};
int rt_idx = 0;   // 当前版本号
int nd_idx = 0;    // 节点分配计数器
vector<node> root; // 各版本的根节点
vector<node> nd;   // 节点存储池

// 递归建树: 在 nd[rt] 处建区间 [l,r]
void init(int rt,int l,int r,const vector<long long>& A)
{
	if(l == r) nd[rt].val = A[l]; // 叶节点赋值
	else
	{
		int mid = (l + r) >> 1;
		nd[rt].son_l = ++nd_idx;          // 分配左孩子
		init(nd[rt].son_l,l,mid,A);        // 递归建左子树
		nd[rt].son_r = ++nd_idx;          // 分配右孩子
		init(nd[rt].son_r,mid + 1,r,A);   // 递归建右子树
		nd[rt].val = nd[nd[rt].son_l].val + nd[nd[rt].son_r].val; // 回溯求和
	}
}
// 顶层建树: 读入数组, 预分配, 建版本0
void init(int n,int m)
{
	vector<long long> A(n + 1);
	root.resize(m + 1);
	nd.resize(n * 40);   // 经验上界, 比 P3919 的 n*4+m*log(n) 更宽松
	for(int i = 1;i <= n;i++) cin >> A[i];
	int l = 1,r = n;
	int mid = (l + r) >> 1;
	// 手动建根层
	root[rt_idx].son_l = ++nd_idx;
	init(root[rt_idx].son_l,l,mid,A);
	root[rt_idx].son_r = ++nd_idx;
	init(root[rt_idx].son_r,mid+1,r,A);
	root[rt_idx].val = nd[root[rt_idx].son_l].val + nd[root[rt_idx].son_r].val;
	++rt_idx; // 版本0建好, rt_idx -> 1
}
// 递归修改: 基于旧节点 old, 在新节点 rt 处创建增量路径
void insert(int rt,int old,int l,int r,int i,long long k)
{
	nd[rt] = nd[old];  // 关键: 先整体复制旧节点 (val + 两个子指针)
	nd[rt].val += k;    // 增量累加: 当前节点区间包含i, 区间和增加k
	if(l == r) return;  // 叶节点: 增量已施加, 直接返回 (无需回溯)
	int mid = (l + r) >> 1;
	if(i <= mid)        // 修改在左半: 新建左孩子, 右孩子共享
	{
		nd[rt].son_l = ++nd_idx;
		insert(nd[rt].son_l,nd[old].son_l,l,mid,i,k);
		// nd[rt].son_r 从 old 复制, 共享旧版本右子树
	}
	else                // 修改在右半: 新建右孩子, 左孩子共享
	{
		nd[rt].son_r = ++nd_idx;
		insert(nd[rt].son_r,nd[old].son_r,mid + 1,r,i,k);
	}
	// 注意: 末尾无 nd[rt].val = 左+右, 因为 +=k 已沿路径逐层施加
}
// 顶层修改: 基于版本 old, 创建新版本
void insert(int old,int n,int i,long long k)
{
	int l = 1;int r = n;
	int mid = (l + r) >> 1;
	root[rt_idx] = root[old];  // 复制旧版本根
	root[rt_idx].val += k;     // 根节点也 += k
	if(i <= mid)               // 修改在左半
	{
		root[rt_idx].son_l = ++nd_idx;
		insert(root[rt_idx].son_l,root[old].son_l,l,mid,i,k);
	}
	else                       // 修改在右半
	{
		root[rt_idx].son_r = ++nd_idx;
		insert(root[rt_idx].son_r,root[old].son_r,mid + 1,r,i,k);
	}
	++rt_idx; // 版本号递增
}
// 递归区间查询: 在 nd[rt] 子树中查询 [ml,mr] 的和
long long myget(int rt,int l,int r,int ml,int mr)
{
	if(l > mr || r < ml) return 0;              // 完全不相交
	if(ml <= l && mr >= r) return nd[rt].val;   // 完全包含: 直接返回
	int mid = (l + r) >> 1;
	return myget(nd[rt].son_l,l,mid,ml,mr) + myget(nd[rt].son_r,mid + 1,r,ml,mr); // 部分相交: 递归求和
}
// 顶层区间查询: 在版本 ver 中查询 [ml,mr] 的和
long long myget(int ver,int n,int ml,int mr)
{
	if(ml == 1 && mr == n) return root[ver].val; // 全区间优化: 直接返回根值
	else
	{
		int l = 1,r = n;
		int mid = (l + r) >> 1;
		// 手动分裂根层, 递归左右子树求和
		return myget(root[ver].son_l,l,mid,ml,mr) + myget(root[ver].son_r,mid + 1,r,ml,mr);
	}
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	int N,M;
	cin >> N >> M;
	init(N,M); // 建版本0
	for(int i = 1;i <= M;i++)
	{
		int v,op;
		cin >> v >> op;        // v = 基于的版本号, op = 操作类型
		if(op == 1)            // 修改: 基于版本v, 位置idx增加k
		{
			int idx;
			long long k;
			cin >> idx >> k;
			insert(v,N,idx,k); // 创建新版本
		}
		else                  // 查询: 在版本v中查询区间[x,y]的和
		{
			int x,y;
			cin >> x >> y;
			cout << myget(v,N,x,y) << '\n';
			// 注意: 查询不创建新版本 (与 P3919 不同)
		}
	}
	return 0;	
} 
```

## 复杂度分析

时间复杂度：建树 $O(n)$；单次修改 $O(\log n)$（沿修改路径递归 $\log n$ 层，每层 $O(1)$）；单次区间查询 $O(\log n)$（标准线段树区间查询最多访问 $O(\log n)$ 个节点）。m次操作总时间 $O(n + m \cdot \log n)$。

空间复杂度：$O(n + m \cdot \log n)$。初始建树约4n个节点，每次修改新增 $\log n$ 个节点。代码使用 `nd.resize(n * 40)` 作为经验上界，n*40对于n个初始节点加m次修改（m次各 $\log n$ 个新节点）在多数竞赛数据范围内足够。

## 适用场景

本模板适用于需要历史版本的区间求和场景，包括：历史区间和查询（在任意版本上查询某区间的和）、可持久化数组（将单点查询视为区间[i,i]的特例）、增量式版本管理（每次修改是"增加k"而非"覆盖为k"，适合累加统计）。与P3919的"覆盖式"不同，本模板的"增量式"更适合需要逐步累加的统计场景，如按时间线累加贡献的历史和查询。

## 常见陷阱与注意事项

- **增量式 vs 覆盖式的混淆**：本模板的 `nd[rt].val += k` 是增量累加，每次修改是"位置i增加k"。若题目要求"位置i改为k"（覆盖语义），则必须使用P3919的 `nd[rt].val = k` + 回溯求和方式，否则结果错误。
- **insert末尾无回溯求和**：本模板的递归insert末尾没有 `nd[rt].val = 左+右`，因为增量k沿路径逐层施加后每个节点已正确。若误加此行不会出错（因为左+右也等于原值+k），但浪费一次计算。若误删 `nd[rt].val += k` 则区间和维护错误。
- **空间预分配 n*40**：这是一个经验值而非严格上界。当n较小但m很大时（如n=10, m=10^6），n*40=400可能不足以容纳 $4n + m \cdot \log(n)$ 个节点，导致越界。建议根据实际数据范围精确计算或使用更大的上界。
- **全区间查询优化**：`myget` 重载中 `ml==1 && mr==n` 时直接返回 `root[ver].val`，这一优化依赖根节点val维护了整个区间的和。若修改逻辑有误导致根val不正确，则全区间查询也会出错。
- **查询不创建新版本**：与P3919不同，本模板的查询操作不执行 `root[rt_idx++] = root[v]`。版本号仅在修改时递增。若题目要求查询也产生版本号则需补充此行。

## 对比与扩展

- **增量式（本模板）vs 覆盖式（P3919）**：增量式 `val += k` 沿路径每层累加，末尾无回溯；覆盖式 `val = k` 仅叶赋值，内部回溯 `val = 左+右`。增量式适合"加"语义，覆盖式适合"赋值"语义。两者共享相同的可持久化框架（复制旧节点+共享未修改子树）。
- **可持久化线段树 vs 可持久化BIT**：线段树支持任意区间查询（区间和、区间最值等），更通用；BIT仅支持前缀和类操作，但常数更小、代码更短，适合仅需前缀和的场景。
- **扩展：主席树（Chairman Tree）**：将离散化后的权值线段树持久化，前缀[1..i]作为版本i，通过版本i与版本l-1的差分在线段树上二分，可在 $O(\log n)$ 内查询区间 $[l,r]$ 的第k小值。
- **扩展：可持久化并查集**：利用可持久化数组记录fa数组的历史版本，支持回退到历史版本的并查集操作。
- **扩展：可持久化分块**：将分块思想持久化，适用于修改和查询复杂度可以放宽到 $O(\sqrt{n})$ 的场景。
