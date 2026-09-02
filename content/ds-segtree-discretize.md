---
id: ds-segtree-discretize
title: '离散化+线段树（大值域少查询）'
category: 数据结构
subcategory: 线段树
tags: ["数据结构", "线段树", "离散化", "大值域"]
timeComplexity: '预处理O(m log m)，单次O(log m)'
spaceComplexity: 'O(m)'
codePath: '数据结构\普通线段树\离散化+线段树(大值域，少查询)\Untitled1.cpp'
---

## 算法原理

当值域极大（如 $n=10^9$）但实际操作次数 m 很小时，直接对 [1,n] 建线段树需要 $O(n)$ 空间，显然不可行。离散化是标准应对手段，但朴素离散化（点离散化）只保留查询端点本身，会丢失端点之间连续区间的信息。例如查询 [1, 10^9] 的和，若只离散化出 {1, 10^9} 两个点，中间 $10^9-2$ 个值就被忽略了。本模板采用"区间离散化"（又称"段离散化"）：不仅保留每个关键点作为一个长度为 1 的段，还在相邻关键点 dis[i] 与 dis[j] 之间若有间隔（`dis[i]+1 < dis[j]`）时，额外创建一个段表示连续区间 [dis[i]+1, dis[j]-1]。这样整个 [1,n] 被划分成若干"操作粒度一致"的段，段数至多 $2m+2$，可控。

线段树建立在这些"段"之上：树节点 rt 不再代表下标区间 [l,r]，而是代表"段区间"——第 l 段到第 r 段。每个段 i 有真实的值域范围 [myleft[i], myright[i]]，存储在两个并行数组中。树节点 rt 的区间和增量（lazy 下传时）必须用真实值域长度 $\text{myright}[mid] - \text{myleft}[l] + 1$ 计算，而非段数 $mid-l+1$，因为不同段代表的真实长度不同（点段长度 1，间隙段长度可能上亿）。这是与基础模板唯一本质区别。

本模板还有一个关键优化：初始时位置 i 的值为 i（即序列为 1,2,...,n）。线段树只维护"操作带来的增量"（op=1 的区间加 k 的累计和），初始的等差数列和在查询时用等差求和公式 $\text{getsum}(l,r) = \frac{(l+r) \times (r-l+1)}{2}$ 现场计算。这样建树时无需初始化 tree（全 0 即可），查询结果 = $\text{getval}(\text{增量}) + \text{getsum}(\text{初始})$。这与基础模板中"叶子 `tree[rt]=l`"对应同一类问题，但离散化版本通过把初始和剥离出来，避免了在大值域上初始化。

四个操作的配合：init 读取所有操作，收集端点去重，构建 myleft/myright 段表并分配线段树数组；down 用真实段长度下传 lazy；insert（区间加）和 getval（查询增量）的边界判断都用 `myleft[l]/myright[r]` 而非 l/r；查询时主程序额外加上 getsum。由于段边界均由操作端点构成，任何 [ml,mr] 查询/修改都能精确分解为若干完整段，递归时大多命中"完全包含"或"不相交"分支。

## 数据结构图示

设 n=10，操作端点为 {1,10,3,7}。排序去重 `dis={1,3,7,10}`。构建段：

```
段1: [1,1]    (点)
段2: [2,2]    (1与3之间的间隙，因为 1+1<3)
段3: [3,3]    (点)
段4: [4,6]    (3与7之间的间隙)
段5: [7,7]    (点)
段6: [8,9]    (7与10之间的间隙)
段7: [10,10]  (点)

myleft  = [_, 1, 2, 3, 4, 7, 8, 10]   (下标0为占位)
myright = [_, 1, 2, 3, 6, 7, 9, 10]
```

共 7 段，线段树建在 [1,7] 上（段编号），但每段真实长度不同：

```
              [段1..段7] rt=1
             /              \
      [段1..段4] rt=2      [段5..段7] rt=3
       /        \           /        \
   [段1,段2]  [段3,段4]  [段5,段6]  [段7,段7]
   rt=4       rt=5       rt=6       rt=7
    /    \      /    \       /    \
 段1    段2  段3    段4    段5    段6     段7
 真实    真实  真实   真实   真实   真实    真实
 [1,1]  [2,2] [3,3] [4,6]  [7,7] [8,9]   [10,10]
 长度1  长度1 长度1 长度3  长度1 长度2    长度1
```

down 下传示例：rt=2（段1..段4，真实范围 [1,6]，mid=段2 即真实 [2,2]）的 lazy=k 下传到 rt=4（段1,段2，真实 [1,2]）：

```
tree[4] += k * (myright[mid] - myleft[l] + 1)
         = k * (myright[2] - myleft[1] + 1) = k*(2-1+1) = 2k
         （而非 k*(mid-l+1) = k*(2-1+1) = 2k  本例恰好相同）

但对 rt=5（段3,段4，真实 [3,6]）：
tree[5] += k * (myright[r] - myleft[mid+1] + 1)
         = k * (myright[4] - myleft[3] + 1) = k*(6-3+1) = 4k
         （若误用 r-mid = 4-2 = 2，只得 2k，漏掉段4的真实长度3）
```

可见用段数代替真实长度会在间隙段上严重出错。

## 核心操作详解

### getsum(l, r)

- **作用**：计算初始等差数列 [l,r] 的和 = $\frac{(l+r)(r-l+1)}{2}$。
- **参数**：l,r 真实值域端点。
- **算法步骤**：直接套等差求和公式 $(\text{首项}+\text{末项}) \times \text{项数} / 2$。
- **关键细节**：用 `unsigned long long` 防止 $10^9$ 级别相乘溢出。这是"剥离初始值"的关键，使线段树只需维护增量，建树时 tree 全 0 即可。

### getleft(i) / getright(i)

- **作用**：二分查找值为 i 的段编号（getleft 找 `myleft` 中位置，getright 找 `myright` 中位置）。
- **参数**：i 真实值域坐标。
- **算法步骤**：`lower_bound` 在 myleft/myright 上二分，返回段编号。
- **关键细节**：本模板主流程未直接调用（insert/getval 直接用值域边界比较），作为通用工具保留，便于扩展。

### init(n, m)

- **作用**：读入所有操作，构建离散段表，分配线段树。
- **参数**：n 值域上界，m 操作数。
- **算法步骤**：(1) 收集端点 `dis={1,n}` 并加入每个操作的 l,r；(2) `sort`+`unique` 去重；(3) 遍历 dis，对每个点加一个长度 1 段 `[dis[i],dis[i]]`，若与下一点有间隙（`dis[i]+1 < dis[j]`）则加一个间隙段 `[dis[i]+1, dis[j]-1]`；(4) 按 cnt 分配 tree、lazy 为 `4*cnt+1`。
- **关键细节**：`myleft/myright[0]=(0,0)` 占位使段从下标 1 开始。cnt 即段数 sz，返回给主程序。

### down(i, l, r)

- **作用**：下传 lazy，用真实段长度。
- **参数**：i 节点，l,r 段区间。
- **算法步骤**：`lazy[i]==0` 或 `l==r` 返回；`mid=(l+r)/2`；左子真实长度 = $\text{myright}[mid] - \text{myleft}[l] + 1$，对左子执行 $\text{tree}[i \ll 1] \leftarrow \text{tree}[i \ll 1] + \text{lazy}[i] \times \text{左长度}$ 和 $\text{lazy}[i \ll 1] \leftarrow \text{lazy}[i \ll 1] + \text{lazy}[i]$；右子真实长度 = $\text{myright}[r] - \text{myleft}[mid+1] + 1$，同理；`lazy[i]=0`。
- **关键细节**：这是与基础模板唯一本质区别——长度用 $\text{myright}/\text{myleft}$ 而非下标差。左子覆盖段 [l,mid]，真实范围 $[\text{myleft}[l],\; \text{myright}[mid]]$；右子覆盖段 [mid+1,r]，真实范围 $[\text{myleft}[mid+1],\; \text{myright}[r]]$。

### insert(rt, l, r, ml, mr, k)

- **作用**：对真实值域 [ml,mr] 区间加 k。
- **参数**：rt,l,r 段节点，ml,mr 真实值域边界，k 增量。
- **算法步骤**：先 `down`；若 `myleft[l] > mr` 或 `myright[r] < ml`（真实值域不相交）返回；若 `ml<=myleft[l]` 且 `mr>=myright[r]`（真实值域完全包含）则执行以下更新后返回：

  $$\text{tree}[rt] \leftarrow \text{tree}[rt] + k \times (\text{myright}[r] - \text{myleft}[l] + 1)$$

  $$\text{lazy}[rt] \leftarrow \text{lazy}[rt] + k$$

  否则递归左右子并 pushup。
- **关键细节**：边界判断全部基于真实值域 myleft/myright，而非段编号 l/r。

### getval(rt, l, r, ml, mr)

- **作用**：查询真实值域 [ml,mr] 的增量和。
- **算法步骤**：先 `down`；不相交返回 0；完全包含返回 `tree[rt]`；否则递归左右子相加。
- **关键细节**：只返回增量，主程序需额外加 `getsum(ml,mr)` 得到完整答案。

## 逐行代码解析

```cpp
#include<bits/stdc++.h>
/*代紅P13825*/
using namespace std;
struct T
{
	int op, l, r, k;
};
inline unsigned long long getsum(int l, int r)
{
	return ((unsigned long long)l + r) * ((unsigned long long)r - l + 1) / 2;
}
vector<unsigned long long> tree;
vector<unsigned long long> lazy;
vector<T> A;
vector<int> myleft;     // 段 i 的真实左端点
vector<int> myright;    // 段 i 的真实右端点
```

`getsum` 用等差求和公式算初始和；`unsigned long long` 应对 10^9 级值域相乘（可达 10^18）。`myleft/myright` 是段表的核心：第 i 段代表真实值域 [myleft[i], myright[i]]。

```cpp
inline int getleft(int i)
{
	return lower_bound(myleft.begin(), myleft.end(), i) - myleft.begin();
}
inline int getright(int i)
{
	return lower_bound(myright.begin(), myright.end(), i) - myleft.begin();
}
```

二分工具函数，返回值对应的段编号。本模板主流程未直接使用，保留以备扩展。

```cpp
int init(int n, int m)
{
	vector<int> dis;
	A.resize(m + 1);
	dis.emplace_back(1);      // 收集 1 和 n 两个端点
	dis.emplace_back(n);
	for (int i = 1; i <= m; i++)
	{
		cin >> A[i].op;
		if (A[i].op == 1) { cin >> A[i].l >> A[i].r >> A[i].k; }
		else { cin >> A[i].l >> A[i].r; }
		dis.emplace_back(A[i].l);
		dis.emplace_back(A[i].r);     // 收集每个操作的 l,r
	}
	sort(dis.begin(), dis.end());
	dis.erase(unique(dis.begin(), dis.end()), dis.end());   // 去重
```

先读入全部操作存入 A（离线），同时把 1、n 及所有操作端点收集到 dis。`sort`+`unique` 是标准离散化去重流程。

```cpp
	int cnt = 0;
	myleft.emplace_back(0);     // 下标 0 占位
	myright.emplace_back(0);
	for (int i = 0; i < dis.size(); i++)
	{
		int j = i + 1;
		myleft.emplace_back(dis[i]);      // 点段 [dis[i],dis[i]]
		myright.emplace_back(dis[i]);
		if (j < dis.size() && dis[i] + 1 < dis[j])   // 存在间隙
		{
			myleft.emplace_back(dis[i] + 1);         // 间隙段 [dis[i]+1, dis[j]-1]
			myright.emplace_back(dis[j] - 1);
			++cnt;
		}
		++cnt;
	}
	tree.resize(cnt * 4 + 1);
	lazy.resize(cnt * 4 + 1);
	return cnt;    // 返回段数
}
```

这是"区间离散化"的核心。对每个离散点建一个点段；若它与下一个离散点之间有间隙（`dis[i]+1 < dis[j]`），就额外建一个间隙段覆盖整个连续空白。这样所有真实值域都被段覆盖，且段数与操作数同阶。`myleft/myright[0]=(0,0)` 占位使段编号从 1 起，与线段树节点编号对齐。

```cpp
void down(int i, int l, int r)
{
	if (lazy[i] == 0 || l == r) return;
	int mid = (l + r) >> 1;
	// 左子真实长度 = myright[mid] - myleft[l] + 1
	tree[i << 1] += lazy[i] * (myright[mid] - myleft[l] + 1);
	lazy[i << 1] += lazy[i];
	// 右子真实长度 = myright[r] - myleft[mid + 1] + 1
	tree[(i << 1) | 1] += lazy[i] * (myright[r] - myleft[mid + 1] + 1);
	lazy[(i << 1) | 1] += lazy[i];
	lazy[i] = 0;
}
```

注意长度计算与基础模板的区别：这里用真实值域端点之差 `myright[mid]-myleft[l]+1`，而不是段数 `mid-l+1`。左子代表段 [l,mid]，其真实范围是 [myleft[l], myright[mid]]；右子代表段 [mid+1,r]，真实范围是 [myleft[mid+1], myright[r]]。

```cpp
void insert(int rt, int l, int r, int ml, int mr, int k)
{
	down(rt, l, r);
	if (myleft[l] > mr || myright[r] < ml) return;    // 真实值域不相交
	if (ml <= myleft[l] && mr >= myright[r])          // 真实值域完全包含
	{
		tree[rt] += (unsigned long long)k * (myright[r] - myleft[l] + 1);
		lazy[rt] += k;
		return;
	}
	int mid = (l + r) >> 1;
	insert(rt << 1, l, mid, ml, mr, k);
	insert((rt << 1) | 1, mid + 1, r, ml, mr, k);
	tree[rt] = tree[rt << 1] + tree[(rt << 1) | 1];   // pushup
}
unsigned long long getval(int rt, int l, int r, int ml, int mr)
{
	down(rt, l, r);
	if (myleft[l] > mr || myright[r] < ml) return 0;
	if (ml <= myleft[l] && mr >= myright[r]) return tree[rt];
	int mid = (l + r) >> 1;
	return getval(rt << 1, l, mid, ml, mr) + getval((rt << 1) | 1, mid + 1, r, ml, mr);
}
```

insert 与 getval 的结构同基础模板，但所有边界比较都换成真实值域：`myleft[l] > mr` 判断段 l 的左端是否超过查询右界。完全包含时增量也是 `k * (myright[r]-myleft[l]+1)` 用真实长度。

```cpp
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n, m;
	cin >> n >> m;
	int sz = init(n, m);
	for (int i = 1; i <= m; i++)
	{
		if (A[i].op == 1) insert(1, 1, sz, A[i].l, A[i].r, A[i].k);
		else cout << getval(1, 1, sz, A[i].l, A[i].r) + getsum(A[i].l, A[i].r) << '\n';
		// 查询 = 增量 + 初始等差和
	}
	return 0;
}
```

主程序先 `init` 得到段数 sz，再回放操作：op=1 调 insert，op=2 输出 `getval(增量) + getsum(初始等差和)`。`getsum` 现场计算初始值之和，避免在大值域上建初始树。

## 复杂度分析

- **预处理 init**：收集 $2m+2$ 个端点，排序去重 $O(m \log m)$；构建段表 $O(m)$；段数 cnt 至多 $2 \times (2m+2) \approx 4m$。
- **线段树空间** $O(4 \cdot \text{cnt}) = O(m)$。
- **insert / getval** 单次 $O(\log \text{cnt}) = O(\log m)$：树高 $O(\log m)$，每层至多 4 个节点。
- **总复杂度**：m 次操作 $O(m \log m)$，远优于对 [1,n] 直接建树的 $O(n)$。
- **对比**：当 $n=10^9$、$m=10^5$ 时，空间从 $4 \times 10^9$（不可行）降至 $4 \times 10^5$（可行），时间从 $O(n)$ 降至 $O(m \log m)$。

## 适用场景

适用于值域极大（$10^9$ 级别）但操作次数较少（$10^5$ 级别）的离线区间修改/查询问题。它是"离线 + 离散化"思想的典型应用：所有操作预先读入，根据端点构建紧凑的段表，再套用标准 lazy 线段树。若问题需要在线（操作不预先知道）或值域与操作数同阶，则无需离散化，直接用普通线段树即可。动态开点线段树（按需建节点）是处理大值域在线问题的另一种方案，但常数和空间通常不如离线离散化。

## 常见陷阱与注意事项

1. **必须用"区间离散化"而非"点离散化"**：只保留端点会丢失间隙区间的值，导致查询结果偏小。间隙段是本模板的灵魂。
2. **down / insert / getval 中的区间长度必须用 $\text{myright}-\text{myleft}+1$**，不能写成下标差 $mid-l+1$，否则间隙段的真实长度被忽略，结果严重错误。
3. **边界判断也要用真实值域**：`myleft[l] > mr` 而非 `l > mr`，因为段编号与真实值域不是一一线性映射。
4. **tree、lazy、getsum 要用 unsigned long long / long long**：间隙段长度可达 $10^9$，乘以 k 后可能达 $10^{18}$，超出 int 甚至需要警惕 long long 上限。
5. **myleft/myright[0] 占位使段从 1 开始**，下标与线段树节点编号对齐；忘记占位会导致段编号错位。
6. **查询结果要加 getsum（初始等差和）**：本模板只维护增量，漏加 getsum 会只返回增量部分。若初始序列不是等差数列，需相应替换 getsum。
7. **端点收集务必加入 1 和 n**：保证整个值域被段覆盖，否则边界段外的查询出错。

## 对比与扩展

- **与普通线段树对比**：离散化版用"段"代替"点"，下标含义从"位置"变为"段编号"，长度计算随之改变；但本质仍是同一棵 lazy 线段树，down/insert/getval 的骨架完全一致。
- **与动态开点线段树对比**：动态开点适合在线、随机访问的大值域，按需创建节点，空间 $O(q \log n)$；离线离散化空间 $O(m)$ 更紧凑，但需要预先知道所有操作。两者各有适用场景。
- **与树状数组+离散化对比**：BIT 也可离散化，但同样只能处理可差分操作；线段树可处理最值、双标记等更复杂情形。
- **扩展方向**：(1) 结合双标记（乘法+加法）处理大值域的乘加问题；(2) 二维离散化 + 二维线段树；(3) 离线询问的莫队算法可作为另一种大值域替代方案；(4) 将"段"思想用于扫描线、矩形面积并等问题。
