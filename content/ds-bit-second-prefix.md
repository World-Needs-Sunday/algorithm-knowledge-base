---
id: ds-bit-second-prefix
title: '二阶前缀和（树状数组维护）'
category: 数据结构
subcategory: 前缀和与差分
subSubcategory: '一维'
tags: [前缀和, 树状数组, 二阶前缀和, BIT, 单点修改]
timeComplexity: 'O(log n)'
spaceComplexity: 'O(n)'
level: B
verified: '2026-10-28'
codePath: '数据结构\前缀和与差分\2阶前缀和\Untitled1.cpp'
---

## 算法原理## 核心公式 / 状态定义与转移方程## 数据结构图示## 核心操作详解

两棵树状数组各维护一个一维序列，结构与普通 BIT 完全相同；区别只在「节点里存的是什么」：

```text
        B1（存 a[j]）                B2（存 j·a[j]）
     ┌──────────────┐           ┌──────────────┐
     │ idx=1: a[1]  │           │ idx=1: 1·a[1] │
     │ idx=2: a[1..2]累加│        │ idx=2: ∑j·a[j] │
     │ idx=3: a[3]  │           │ idx=3: 3·a[3] │
     │ idx=4: a[1..4]累加│        │ idx=4: ∑j·a[j] │
     │  ...         │           │  ...         │
     └──────────────┘           └──────────────┘
            ↓ 单点修改都走 lowbit 向上跳、前缀查询都 lowbit 向下跳
     查询时组合：S2[x] = (x+1)·B1.get(x) − B2.get(x)
```

两棵树的下标结构完全一致，同一个 `idx` 的修改会**同步**作用到两棵树上（差值分别为 $k$ 和 $k\cdot idx$）。查询时不需要遍历序列，只要两次 `get` 再套公式，这就是把「两遍前缀和」压缩成 $O(\log n)$ 的关键。



**一句话结论**：二阶前缀和 $S_2[x]=\sum_{i=1}^{x}\sum_{j=1}^{i}a[j]$ 无法用一棵 BIT 直接维护（系数 $(x-j+1)$ 依赖查询时的 $x$），但把它拆成 $(x+1)\sum a[j]-\sum j\cdot a[j]$ 后，就变成了两个与 $x$ 无关的量，可以用两棵树状数组分别维护，单次修改、查询都只要 $O(\log n)$。

**解决的问题**（洛谷 P4868 Preprefix sum，源自你的笔记标题）：维护序列 $a[1..n]$，支持两种操作：

1. `Modify idx x`：把 $a[idx]$ **改为** $x$（注意是"改为"，不是"加上"）；
2. `Query idx`：查询二阶前缀和 $S_2[idx]=\sum_{i=1}^{idx}\sum_{j=1}^{i}a[j]$。

用普通做法回答一次 Query 要先算一阶前缀和再求一次前缀和，$O(n)$ 或 $O(n\log n)$，查询多了就撑不住。

**核心推导怎么来的**：把二阶前缀和看成「每个 $a[j]$ 被哪些外层 $i$ 数到」——固定 $j$，内层要包含 $a[j]$ 必须 $j\le i$，而外层 $i\in[1,x]$，所以 $a[j]$ 共被数 $x-j+1$ 次。交换求和顺序：

$$S_2[x]=\sum_{j=1}^{x}(x-j+1)\,a[j]$$

系数 $x-j+1$ 同时含「查询变量 $x$」和「位置变量 $j$」，直接维护会崩：同一个 $a[j]$ 对不同查询 $x$ 需要不同系数，而修改时根本不知道将来会查哪个 $x$。把系数拆成 $(x+1)-j$ 再分离：

$$S_2[x]=(x+1)\sum_{j=1}^{x}a[j]-\sum_{j=1}^{x}j\cdot a[j]$$

查询时 $x$ 固定，$(x+1)$ 可提出求和号。于是只需维护两个与 $x$ 无关的前缀量：$\sum a[j]$（B1）与 $\sum j\cdot a[j]$（B2），查询时用 $(x+1)$ 组合。

**为什么这样做是对的**：B1、B2 各自是标准的单点修改 + 前缀查询 BIT；上述等式是恒等变形（仅交换求和顺序 + 拆分系数），没有任何近似，所以用公式组合出来的值与定义逐项求和完全一致。修改时按差值 $k=x-old$ 同步更新两棵树，保证任意时刻 $B_1.get(i)=\sum_{j\le i}a[j]$、$B_2.get(i)=\sum_{j\le i}j\cdot a[j]$ 两个不变量成立，查询公式随之始终正确。



**状态定义**：

- $B_1$：树状数组，维护 $a[j]$；
- $B_2$：树状数组，维护 $j\cdot a[j]$。

**核心公式**（查询）：

$$S_2[x]=(x+1)\cdot B_1.get(x)-B_2.get(x)$$

其中 $B_1.get(x)=\sum_{j=1}^{x}a[j]$、$B_2.get(x)=\sum_{j=1}^{x}j\cdot a[j]$。

**修改转移**（把 $a[idx]$ 改为 $x$）：

1. 求旧值：$old=B_1.get(idx)-B_1.get(idx-1)$；
2. 求差值：$k=x-old$；
3. 同步更新：$B_1.add(idx,k)$、$B_2.add(idx,k\cdot idx)$。

若题目是「加上 $x$」而不是「改为」，则直接 $k=x$，省去求 old 的步骤。

**手算示例**：$a=[1,2,3]$（下标 1 起）。

- 初始化后：$B_1.get=[1,3,6]$，$B_2.get=[1,5,14]$。
- `Query 3`：$S_2[3]=4\times 6-14=10$，与逐项 $1+(1+2)+(1+2+3)=10$ 一致。
- `Modify 2 5`：$old=3-1=2$，$k=5-2=3$；$B_1.add(2,3)$、$B_2.add(2,6)$，此后 $B_1.get(3)=9$、$B_2.get(3)=20$。
- `Query 3`：$4\times 9-20=16$，与 $1+(1+5)+(1+5+3)=16$ 一致；`Query 1` 得 1、`Query 2` 得 $3\times 6-11=7$。

以上四条指令已由本代码实测输出 `10 / 16 / 1 / 7`。

**公式速查卡**（三件事背熟即可上考场）：

| 操作 | 动作 |
|---|---|
| 初始化 | $B_1.add(i,\ a[i])$、$B_2.add(i,\ i\cdot a[i])$ |
| 改为 $a[p]=v$ | $k=v-old$（$old$ 用两次 get 差出），$B_1.add(p,k)$、$B_2.add(p,p\cdot k)$ |
| 查询 $S_2[x]$ | $(x+1)\cdot B_1.get(x)-B_2.get(x)$ |



### 单点修改（两棵树同步）

`add` 沿 $idx\to idx+lowbit(idx)$ 向上跳，把差值累加到所有覆盖该点的节点。两棵树的跳转路径完全相同，只是累加的值相差一个 $idx$ 倍——所以实现里就是两次 `add`，一次加 $k$、一次加 $k\cdot idx$。

### 前缀查询（两棵树各查一次）

`get` 沿 $idx\to idx-lowbit(idx)$ 向下跳，累加覆盖段。查询回答 $S_2[x]$ 需要两次 `get`（分别拿 $\sum a[j]$ 和 $\sum j\cdot a[j]$），再做一次乘法和一次减法——共 $O(\log n)$。

### 求旧值（"改为"操作才有）

`old = get(idx) - get(idx-1)` 用两次差分式查询取出单点值，是「改为」类操作的必要前置；「加上」类操作不经过这一步。

## 逐行代码解析

代码拆成 2 个模块，按顺序拼接即完整程序。

### 模块 1 lowbit 与树状数组结构体

```cpp
#include<bits/stdc++.h>
using namespace std;
inline int lowbit(int x)
{
	return x & -x;
}

struct treearr
{
	vector<long long> arr;
	int n;
	treearr() {}
	treearr(int k)
	{
		n = k;
		arr.assign(n + 1,0);
	}
	
	void add(int idx,long long k)
	{
		while(idx <= n)
		{
			arr[idx] += k;
			idx += lowbit(idx);
		}
	}
	
	long long get(int idx)
	{
		long long ans = 0;
		while(idx)
		{
			ans += arr[idx];
			idx -= lowbit(idx);
		}
		return ans;
	}
};

```

- `lowbit(x) = x & -x`：取出 $x$ 最低位的 1，BIT 跳转（向上 `+= lowbit`、向下 `-= lowbit`）全靠它。
- `add(idx, k)`：单点修改——沿 `idx → idx + lowbit(idx)` 向上更新直到越界；存的是「差分贡献」，一个节点的修改只影响覆盖它的祖先节点。
- `get(idx)`：前缀和查询——沿 `idx → idx − lowbit(idx)` 向下累加直到 0；`get(0)` 自然返回 0，正好是求旧值时 `get(idx-1)` 在 `idx=1` 的安全兜底。
- 节点值用 `long long`：$B_2$ 存的是 $j\cdot a[j]$，两者乘积很容易超 int。

### 模块 2 主流程：初始化、修改与查询

```cpp
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	int N,M;
	cin >> N >> M;
	treearr S1(N),S2(N);
	for(int i = 1;i <= N;i++)
	{
		long long x;
		cin >> x;
		S1.add(i,x);
		S2.add(i,x * i);
	}
	
	for(int i = 1;i <= M;i++)
	{
		string str;
		cin >> str;
		if(str == "Modify")
		{
			int idx;
			long long x;
			cin >> idx >> x;
			long long k = x - S1.get(idx) + S1.get(idx - 1);
			S1.add(idx,k);
			S2.add(idx,k * idx);
		}
		else if(str == "Query")
		{
			int idx;
			cin >> idx;
			cout << (long long)(idx + 1) * S1.get(idx) - S2.get(idx) << '\n';
		}
	}
	return 0;
}
```

- 初始化就是按定义逐位置入：`S1.add(i, x)` 与 `S2.add(i, x * i)`，$O(n\log n)$。
- `Modify`：`k = x - S1.get(idx) + S1.get(idx - 1)` 即 $k=x-old$（注意是「改为」所以要求差值）；随后两棵树分别加 $k$ 与 $k\cdot idx$，**必须同步**，漏一棵查询公式就不成立。
- `Query`：直接套公式 `(idx+1) * S1.get(idx) - S2.get(idx)`；先乘再减，注意 `(idx+1)` 乘 `get` 的结果可能超 int，所以显式 `(long long)` 转换。

## 复杂度分析

- **单次 Modify**：求 old 两次 `get`（$O(\log n)$）+ 两次 `add`（$O(\log n)$），总 $O(\log n)$。
- **单次 Query**：两次 `get` + 常数次算术，$O(\log n)$。
- **初始化**：$n$ 次 `add`，$O(n\log n)$（可用「每个点只在 BIT 上建一条链并一次累加」的线性建树优化，但模板场景 $O(n\log n)$ 已足够）。
- **空间**：两棵 BIT 各 $n+1$ 个 `long long`，$O(n)$。
- **对比**：暴力每次查询两遍前缀和 $O(n)$（或 BIT 套 BIT $O(n\log n)$）；双 BIT 把查询压到 $O(\log n)$，查询次数越多收益越大。

## 适用场景

- **单点修改 + 二阶前缀和查询**：如 P4868 Preprefix sum 这类「改一个值、问带权前缀」的题，本模板直接套。
- **高阶推广**：$k$ 阶前缀和用 $k$ 棵 BIT 维护 $j^t\cdot a[j]$（$t=0,\dots,k-1$），查询时按二项式系数组合；思路一致，公式更长。
- **区间加版本**：若操作变成「区间加 + 二阶前缀和查询」（如洛谷 P4514 上帝造题的七分钟），先把差分引入、问题转化为对差分数组的二阶前缀和，本质同构但细节更多。
- **不要用**：只有静态查询（无修改）时直接用前缀和数组 $O(n)$ 预处理 + $O(1)$ 查询，BIT 反而多余；只有一阶前缀查询时一棵 BIT 就够，别套两棵。

## 常见陷阱与注意事项

1. **「改为」与「加上」混淆**：改成"改为"要先求 $old$ 再算差值 $k=x-old$；"加上"直接 $k=x$。把"改为"当"加上"会直接加爆或减爆。
2. **两棵 BIT 不同步**：`Modify` 必须同时 `S1.add(idx,k)` 与 `S2.add(idx,k*idx)`，只更一棵查询公式立刻失真。
3. **公式系数记混**：是 $(x+1)\cdot\Sigma a[j]-\Sigma j\cdot a[j]$，不是 $(x-1)$ 也不是 $(x+1)\cdot\Sigma j\cdot a[j]$；记混一个符号都全错。
4. **下标从 1 开始**：BIT 下标不能为 0；题目 1 起编号正好，若输入 0 起要先平移。
5. **溢出**：$j\cdot a[j]$ 与 $(x+1)\cdot get(x)$ 都可能超 int，BIT 数组和答案全用 `long long`。
6. **get(idx-1) 越界**：idx=1 时 `get(0)` 是安全的（while(0) 直接不进入循环返回 0），但如果自己手写边界判断反而容易引入错误。
7. **初始化别漏 B2**：建树时 `S2.add(i, x*i)` 的 `*i` 忘了乘，B2 就退化成 B1，查询公式必然错。
8. **k 阶扩展 ≠ 无脑开树**：三阶及以上每棵树维护 $j^t a[j]$，组合系数要用组合数恒等式一步步展开核对，不能凭感觉堆树。

## 对比与扩展

**与普通前缀和、普通 BIT 对比**：

| 维护对象 | 查询内容 | 修改代价 | 查询代价 |
|---|---|---|---|
| 静态前缀和数组 | 一阶前缀 | 不支持 | O(1) |
| 一棵 BIT | 一阶前缀 | O(log n) | O(log n) |
| 两棵 BIT（本模板） | 二阶前缀 | O(log n) | O(log n) |
| 暴力两遍前缀 | 二阶前缀 | O(1) 改原数组 | O(n) |

**与「BIT 套 BIT」对比**：直接在 BIT 节点里再存前缀和是 $O(n\log n)$ 查询且码量大；拆两棵树的思路本质是把「带位置权」的部分独立维护，交换求和序这个技巧与「区间查加权和」类问题同源。

**扩展方向**：

- **三阶前缀和**：需要三棵 BIT。办法是把 $(x-j+1)$ 再展开成关于 $x$ 的二次多项式，按 $x$ 的幂次分组（$x^2$、$x$、常数三项各配一棵 BIT 维护带权前缀 $\sum j^t\cdot a[j]$，$t=0,1,2$），公式比二阶长一截，思路完全一致——「系数含查询变量 $x$ 的部分提出来，与 $x$ 无关的加权前缀交给 BIT」。
- **一般 k 阶前缀和**：$k$ 棵 BIT，第 $t$ 棵（$t=0,1,\dots,k-1$）维护 $j^t\cdot a[j]$，查询时用对应的组合数/多项式系数把各棵的 `get` 合并起来。系数每升一阶要重新展开核对一遍，不能凭感觉堆树。
- **区间加版本（P4514 上帝造题的七分钟）**：操作变成「区间加 + 二阶前缀和查询」时，先对原数组引入差分，把问题转化为「对差分数组求二阶前缀和」，然后套本模板的两棵树——本质相同，只是多绕了一层差分、细节更多。
- **二维推广**：二阶二维前缀和可类比多树维护 $i\cdot a[i][j]$、$j\cdot a[i][j]$、$ij\cdot a[i][j]$ 等权组合，树状数组换成二维版本即可。