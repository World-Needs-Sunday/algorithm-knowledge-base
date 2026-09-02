---
id: ds-mo-team
title: '普通莫队'
category: 数据结构
subcategory: 莫队
tags: ["数据结构", "莫队", "分块", "离线查询", "区间查询"]
timeComplexity: 'O((n+m)·√n)'
spaceComplexity: 'O(n+m)'
codePath: '数据结构\普通莫队\Untitled1.cpp'
---

## 算法原理

莫队（Mo's Algorithm）是一种基于分块的离线区间查询算法。其核心策略是"离线重排+双指针增量"：不逐个回答查询，而是先将所有查询按照特定规则排序，然后用两个指针[l, r]表示当前已维护的区间，通过移动指针来增量地加入或移除元素，从而在两次查询之间复用已有的计算结果。每次指针移动只增减一个元素，若加入/删除单个元素的代价为 $O(1)$，则总复杂度取决于指针的总移动量。

分块大小s取 $\sqrt{n}$。将数轴[1, n]划分为若干长度为s的块（第i块覆盖 $[i \cdot s,\; (i+1) \cdot s - 1]$）。排序规则为：先按查询左端点l所在的块号（l/s）升序排列，同一块内再按右端点r排序。这样左指针在块内移动最多s步，跨块时最多2s步，m个查询共 $m \cdot s$ 次左移；右指针在同一块内单调移动最多n步，共 $(\sqrt{n})$ 个块（即 $n/s = \sqrt{n}$ 个块），总计 $n \cdot (n/s) = n \cdot \sqrt{n}$ 次右移。总移动量为 $O(m \cdot s + n \cdot n/s)$，取 $s = \sqrt{n}$ 时两者均为 $O(n \cdot \sqrt{n})$（当m与n同阶时），达到最优。

奇偶性优化（parity optimization）进一步减少右指针的移动距离。标准排序中同一块内右端点始终升序，导致右指针在块间切换时需要从块末尾回到块开头再前进。奇偶优化让奇数块内r按降序排列、偶数块内r按升序排列，这样右指针在块间切换时无需"折返跑"，直接延续上一块的终点继续移动。实测可减少约一半的右指针移动量。代码中 `if (al & 1) return this->r > b.r; else return this->r < b.r;` 即实现了此优化。

双指针的移动顺序至关重要。四个while循环的顺序为：先扩展（expand left, expand right）再收缩（shrink left, shrink right）。即先将l向左扩展、r向右扩展，再收缩l、收缩r。这一顺序保证了在收缩前区间已经包含了目标范围，避免出现 $l > r + 1$ 的非法状态。具体来说，若先收缩l（l++），则可能l越过r导致区间为空，此时my_pop会移除本不在区间内的元素；而先扩展确保了在收缩时被移除的元素确实在当前区间内。代码中 `s = sqrt(n) + 1` 的+1是为了避免n=0时除零错误。

## 数据结构图示

```
分块排序示意 (s = sqrt(n), 设 n=16, s=4):

块号:  |--- 块0 (l/s=0) ---|--- 块1 (l/s=1) ---|--- 块2 (l/s=2) ---|--- 块3 ---|
位置:  1  2  3  4            5  6  7  8          9 10 11 12         13 14 15 16

查询排序 (奇偶优化):
  块0 (偶数块): q1(r=3), q2(r=6), q3(r=8)     <- r 升序 -->
  块1 (奇数块): q5(r=12), q4(r=9)            <- r 降序 <--
  块2 (偶数块): q7(r=10), q8(r=15)           <- r 升序 -->
  块3 (奇数块): q10(r=16), q9(r=14)          <- r 降序 <--

双指针移动示意:
  当前 [l,r] = [3, 6], 目标 [l',r'] = [5, 9]

  步骤1: 扩展左  l: 3->2 (push [2])      [l=2, r=6]
  步骤2: 扩展右  r: 6->9 (push [7,8,9])  [l=2, r=9]
  步骤3: 收缩左  l: 2->5 (pop [2,3,4])  [l=5, r=9]  ✓ 到达目标
  (步骤4: 收缩右 r 不需移动, r已=9)

  关键: 先扩展后收缩, 确保收缩时元素一定在区间内
```

## 核心操作详解

**query结构体与排序**：`query` 包含 l, r, idx 三个字段。`operator<` 实现莫队排序：先取 `al = this->l / s` 和 `bl = b.l / s`，若块号不同则按块号升序；若同块则按奇偶性决定r的排序方向——`al & 1` 为真（奇数块）时r降序，为假（偶数块）时r升序。`idx` 用于将排好序的答案写回原始顺序。

**my_push(i)**：向当前区间加入位置i的元素A[i]。维护的是各元素出现次数的平方和 `cur`。操作模式为"先减旧贡献，修改计数，再加新贡献"：$\text{cur} \mathrel{-}= \text{cnt}[A[i]]^2$（移除旧的平方贡献），$\text{cnt}[A[i]]{+}{+}$（计数增加），$\text{cur} \mathrel{+}= \text{cnt}[A[i]]^2$（加上新的平方贡献）。这一模式适用于任何以计数为自变量的贡献函数。

**my_pop(i)**：从当前区间移除位置i的元素A[i]，是my_push的逆操作：$\text{cur} \mathrel{-}= \text{cnt}[A[i]]^2$，$\text{cnt}[A[i]]{-}{-}$，$\text{cur} \mathrel{+}= \text{cnt}[A[i]]^2$。同样遵循"减旧贡献、改计数、加新贡献"模式。

**主循环**：初始化 `l=1, r=0`（空区间）。对排序后的每个查询，执行四个while循环调整指针：`while (B[i].l < l) my_push(--l)`（向左扩展）、`while (B[i].r > r) my_push(++r)`（向右扩展）、`while (B[i].l > l) my_pop(l++)`（向左收缩）、`while (B[i].r < r) my_pop(r--)`（向右收缩）。调整完毕后 `ans[B[i].idx] = cur` 记录答案。注意四个循环的顺序是先扩展后收缩。

## 逐行代码解析

```cpp
#include<algorithm>
#include<iostream>
#include<vector>
#include<cmath>
/*代紅P2709*/
using namespace std;
int n, m, k, s;          // n=数组长度, m=查询数, k=颜色范围, s=块大小
long long cur = 0;        // 当前区间的答案 (各颜色出现次数的平方和)
vector<int> cnt;          // cnt[c] = 颜色c在当前区间中的出现次数
vector<int> A;            // A[1..n] = 输入数组 (颜色序列)
vector<long long> ans;    // ans[i] = 第i个查询的答案

struct query
{
	int l, r, idx;        // l=左端点, r=右端点, idx=原始查询编号
	bool operator< (query& b)
	{
		int al = this->l / s;  // 当前查询左端点所在块号
		int bl = b.l / s;      // 比较对象左端点所在块号
		if (al != bl) return al < bl;  // 不同块: 按块号升序
		else
		{
			// 同块: 奇偶优化
			// 奇数块 (al&1==1): r降序, 避免块间折返
			// 偶数块 (al&1==0): r升序
			if (al & 1) return this->r > b.r;  // 奇数块 r降序
			else return this->r < b.r;         // 偶数块 r升序
		}
	}
};
vector<query> B;          // 查询数组

void my_push(int i)       // 加入位置i的元素 A[i]
{
	// 模式: 减旧贡献 -> 改计数 -> 加新贡献
	cur -= (long long)cnt[A[i]] * cnt[A[i]];  // 减去旧的 cnt^2
	cnt[A[i]]++;                               // 计数+1
	cur += (long long)cnt[A[i]] * cnt[A[i]];  // 加上新的 cnt^2
}
void my_pop(int i)        // 移除位置i的元素 A[i]
{
	// my_push 的逆操作
	cur -= (long long)cnt[A[i]] * cnt[A[i]];  // 减去旧的 cnt^2
	cnt[A[i]]--;                               // 计数-1
	cur += (long long)cnt[A[i]] * cnt[A[i]];  // 加上新的 cnt^2
}

int main()
{
	cin >> n >> m >> k;
	s = sqrt(n) + 1;       // 块大小 = sqrt(n) + 1 (+1 避免除零)
	A.resize(n + 1, 0);
	B.resize(m + 1);
	ans.resize(m + 1, 0);
	cnt.resize(k + 1, 0);  // 颜色范围 [1, k]
	for (int i = 1;i <= n;i++) cin >> A[i];  // 读入颜色序列
	for (int i = 1;i <= m;i++)
	{
		cin >> B[i].l >> B[i].r;  // 读入查询区间
		B[i].idx = i;              // 记录原始编号 (排序后需还原)
	}
	sort(B.begin() + 1, B.end());  // 莫队排序 (跳过B[0]空位)

	int l = 1;int r = 0;  // 初始空区间 [1, 0]
	for (int i = 1;i <= m;i++)
	{
		// 四个while循环: 先扩展后收缩
		// 顺序关键: 先expand再shrink, 避免l > r+1的非法状态
		while (B[i].l < l) my_push(--l);  // 扩展左: l向左移, push新元素
		while (B[i].r > r) my_push(++r);  // 扩展右: r向右移, push新元素
		while (B[i].l > l) my_pop(l++);   // 收缩左: l向右移, pop旧元素
		while (B[i].r < r) my_pop(r--);   // 收缩右: r向左移, pop旧元素
		ans[B[i].idx] = cur;              // 记录答案到原始位置
	}
	for (int i = 1;i <= m;i++) cout << ans[i] << endl;  // 按原始顺序输出
	return 0;
}
```

## 复杂度分析

时间复杂度为 $O((n + m) \cdot \sqrt{n})$，具体分解如下：

左指针移动量：在同一块内，左指针最多移动s步（块大小）；跨块时最多移动2s步。m个查询的左指针总移动量为 $O(m \cdot s) = O(m \cdot \sqrt{n})$。

右指针移动量：在同一块内（同一块的所有查询），右指针单调移动（升序或降序），最多移动n步。共有 $n/s = \sqrt{n}$ 个块，因此右指针总移动量为 $O(n \cdot (n/s)) = O(n \cdot \sqrt{n})$。

取 $s = \sqrt{n}$ 时，总移动量为 $O(m \cdot \sqrt{n} + n \cdot \sqrt{n}) = O((m+n) \cdot \sqrt{n})$。每次移动的增量更新（my_push/my_pop）为 $O(1)$，因此总时间复杂度为 $O((n+m) \cdot \sqrt{n})$。排序为 $O(m \cdot \log m)$。

空间复杂度为 $O(n + m)$：cnt数组 $O(k)$，A数组 $O(n)$，查询和答案数组各 $O(m)$。

## 适用场景

莫队算法适用于离线区间查询场景，且区间答案可以 $O(1)$ 增量维护（即加入或删除单个元素时可以在 $O(1)$ 内更新答案）。典型应用包括：区间不同数个数（维护每种颜色的计数，答案为非零计数种类数）、区间众数（维护计数后取最大值）、区间元素出现次数的平方和（P2709小B的询问，本代码所解）、区间逆序对数等。莫队不适用于需要在线回答查询的场景，也不适用于加入/删除元素代价高于 $O(1)$ 的查询。

## 常见陷阱与注意事项

- **块大小 $s = \sqrt{n} + 1$**：`+1` 是为了在n=0时避免除零错误。若不写+1，当n=0时 `l / s` 会除以0导致未定义行为。s的取值直接影响复杂度，$s = \sqrt{n}$ 是理论最优分块。
- **排序必须使用奇偶性优化**：奇偶优化（奇数块r降序、偶数块r升序）可显著减少右指针的折返移动，实测可提速30%-50%。若不使用奇偶优化，右指针在块间切换时需从当前块末尾回到下一块开头再前进，浪费大量移动。
- **四个while循环的顺序**：必须先扩展（expand left, expand right）后收缩（shrink left, shrink right）。若先收缩，则可能 $l > r$ 导致区间为空，此时my_pop会移除不在区间内的元素，产生错误答案。例如若先执行 `while (B[i].l > l) my_pop(l++)`，当目标l远大于当前l时，l会越过r，后续的push会基于错误的cnt状态。
- **cur的维护模式**：$\text{cur} \mathrel{-}= \text{cnt}^2;\; \text{cnt}{+}{+};\; \text{cur} \mathrel{+}= \text{cnt}^2$ 这一"减旧贡献、改计数、加新贡献"的模式适用于任何以计数为自变量的贡献函数f(cnt)。若答案定义为 $\sum f(\text{cnt}_i)$，则push时 $\text{cur} \mathrel{-}= f(\text{cnt});\; \text{cnt}{+}{+};\; \text{cur} \mathrel{+}= f(\text{cnt})$，pop时对称。本代码中 $f(x) = x^2$。
- **答案的idx还原**：排序打乱了查询顺序，必须用 `B[i].idx` 将答案写回原始位置 `ans[B[i].idx]`，最后按1..m顺序输出。若忘记idx则答案顺序错误。
- **cnt数组的大小**：cnt的下标是颜色值A[i]而非位置i，因此 `cnt.resize(k+1)` 按颜色范围k分配。若颜色值可能为0或超过k则需离散化。

## 对比与扩展

- **普通莫队 vs 带修莫队（Mo's with updates）**：带修莫队在查询中引入修改操作，通过增加第三维时间戳，将块大小调整为 $n^{2/3}$，排序按(块号, r块号, 时间)三关键字排列。复杂度从 $O(n \cdot \sqrt{n})$ 升为 $O(n^{5/3})$。
- **普通莫队 vs 树上莫队（Mo's on trees）**：树上莫队利用欧拉序（DFS序）将树链查询转化为区间查询，再套用普通莫队。欧拉序的入栈+出栈特性使得一条链上的节点恰好在区间中出现一次（LCA除外）。
- **普通莫队 vs 三维莫队（3D Mo's）**：当查询有三个维度（如时间、左端点、右端点）时，使用三维分块，块大小取 $n^{2/3}$，复杂度为 $O(n^{5/3})$。带修莫队本质上是三维莫队的特例。
- **普通莫队 vs 回滚莫队（Rollback Mo's）**：当删除操作难以 $O(1)$ 维护（如并查集的删除），使用回滚莫队：只做扩展不做收缩，通过记录修改历史在块间回滚到初始状态。复杂度仍为 $O(n \cdot \sqrt{n} \cdot \alpha)$。
- **扩展：莫队二次离线**：当增量更新需要 $O(\log n)$ 时（如维护区间逆序对），利用莫队二次离线将复杂度从 $O(n \cdot \sqrt{n} \cdot \log n)$ 降为 $O(n \cdot \sqrt{n} + n \cdot \log n)$。
