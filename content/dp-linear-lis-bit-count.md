---
id: dp-linear-lis-bit-count
title: '最长上升子序列（树状数组优化·带计数）'
category: 动态规划
subcategory: 线性DP
tags: ["动态规划", "线性DP", "最长上升子序列", "LIS", "树状数组", "BIT", "离散化", "计数"]
timeComplexity: 'O(n log n)'
spaceComplexity: 'O(n)'
codePath: '动态规划\线性DP\最长上升子序列（树状数组优化带计数）\Untitled1.cpp'
---

## 算法原理

本算法在 $O(n \log n)$ 时间内同时求解最长上升子序列（LIS）的**长度**和**方案数**。朴素 $O(n^2)$ DP 中，状态 $dp[i]$ 表示以 $A[i]$ 结尾的 LIS 长度，转移为 $dp[i] = \max_{j < i, A[j] < A[i]} dp[j] + 1$。若同时计数，还需维护 $cnt[i]$ 表示以 $A[i]$ 结尾的 LIS 方案数。朴素做法对每个 $i$ 线性扫描所有 $j < i$，内层为 $O(n)$。

树状数组优化的核心观察：转移方程 $dp[i] = \max_{A[j] < A[i]} dp[j] + 1$ 中的约束条件是 $A[j] < A[i]$（值域约束），而非 $j < i$（下标约束）。下标约束 $j < i$ 由遍历顺序自然保证（从左到右处理），因此转移只需查询"所有已处理元素中，值小于 $A[i]$ 的那些元素的最大 $dp$ 值及对应方案数"。这恰好是一个**前缀最大值查询**，可以用树状数组在 $O(\log n)$ 内完成。

但由于 $A[i]$ 的值域可能很大或含负数，不能直接以值为下标建 BIT。算法首先对 $A$ 做离散化（排序 + 去重 + 二分映射），将值压缩到 $[1, m]$ 范围（$m$ 为不同值的个数）。然后以压缩后的值为 BIT 下标，每个 BIT 节点存储一个 $(len, cnt)$ 二元组：$len$ 为该值域前缀内的最大 LIS 长度，$cnt$ 为达到该长度的方案数。

BIT 的合并规则是本算法的精髓：查询前缀 $[1, v-1]$ 时，遍历覆盖该前缀的所有 BIT 节点，取 $len$ 最大者，若多个节点的 $len$ 相同则将它们的 $cnt$ 相加。更新时，将新元素的 $(len, cnt)$ 插入 BIT，沿途每个节点用相同的合并规则与已有值比较更新。这把朴素 DP 的 $O(n)$ 内层转移压缩为 $O(\log n)$ 的 BIT 查询 + 更新。

## 状态定义与转移方程

朴素 DP 状态定义：

$$dp[i] = \text{以 } A[i] \text{ 结尾的 LIS 长度}$$

$$cnt[i] = \text{以 } A[i] \text{ 结尾的 LIS 方案数}$$

朴素转移方程：

$$dp[i] = \max_{\substack{j < i \\ A[j] < A[i]}} \big(dp[j] + 1\big)$$

$$cnt[i] = \sum_{\substack{j < i,\; A[j] < A[i] \\ dp[j] + 1 = dp[i]}} cnt[j]$$

树状数组优化后的转移（离散化后 $rank(i)$ 为 $A[i]$ 的排名）：

$$dp[i] = \text{BIT.query}\big(rank(i) - 1\big).len + 1$$

$$cnt[i] = \begin{cases} \text{BIT.query}\big(rank(i) - 1\big).cnt & \text{if query 结果非零} \\ 1 & \text{if 无前驱（query 为零）} \end{cases}$$

BIT 节点合并规则（查询/更新时对 $(len_1, cnt_1)$ 与 $(len_2, cnt_2)$ 取并）：

$$\text{merge}\big((len_1, cnt_1),\; (len_2, cnt_2)\big) = \begin{cases} (len_1, cnt_1) & \text{if } len_1 > len_2 \\ (len_2, cnt_2) & \text{if } len_2 > len_1 \\ (len_1, cnt_1 + cnt_2) & \text{if } len_1 = len_2 \end{cases}$$

最终答案：

$$\text{ans\_len} = \text{BIT.query}(m).len, \quad \text{ans\_cnt} = \text{BIT.query}(m).cnt \pmod{10^9 + 7}$$

## 数据结构图示

```
离散化示例：
  原始 A = [3, 1, 4, 1, 5, 9, 2, 6]
  排序去重 tmp = [1, 2, 3, 4, 5, 6, 9]
  rank: A[1]=3→3, A[2]=1→1, A[3]=4→4, A[4]=1→1,
        A[5]=5→5, A[6]=9→7, A[7]=2→2, A[8]=6→6

BIT 结构（以 rank 为下标，存储 (len, cnt)）：

         BIT[1]  BIT[2]  BIT[3]  BIT[4]  BIT[5]  BIT[6]  BIT[7]
          (1,1)   (2,1)   (1,1)   (3,1)   (4,1)   (5,1)   (6,1)

  lowbit 覆盖关系（BIT 标准结构）：
  BIT[1] 覆盖 [1]       BIT[2] 覆盖 [1,2]
  BIT[3] 覆盖 [3]       BIT[4] 覆盖 [1,2,3,4]
  BIT[5] 覆盖 [5]       BIT[6] 覆盖 [5,6]
  BIT[7] 覆盖 [7]

  query(6) = merge(BIT[6], BIT[4])
            = merge((5,1), (3,1)) = (5,1)   ← len=5 取较大者
  query(3) = merge(BIT[3], BIT[2])
            = merge((1,1), (2,1)) = (2,1)   ← len=2 取较大者
```

## 核心操作详解

### lowbit(x)

- **作用**：返回 $x$ 的最低位 $1$ 对应的值，即 $x \mathbin{\&} (-x)$。
- **关键细节**：BIT 的基石操作。$-x$ 在补码表示下等价于 $\sim x + 1$，因此 $x \mathbin{\&} (-x)$ 提取 $x$ 二进制表示中最低位的 $1$。例如 $\text{lowbit}(6) = 6 \mathbin{\&} (-6) = 2$（$6 = 110_2$，最低位 $1$ 在第 $2$ 位）。

### treearr 结构体

- **字段**：`len[i]` 存储 BIT 节点 $i$ 维护的最大 LIS 长度，`cnt[i]` 存储对应方案数，`ans_len` 和 `ans_cnt` 为查询暂存。
- **设计思路**：将树状数组从"维护前缀和"推广为"维护前缀最大值 + 计数"。每个 BIT 节点不再存一个标量，而是存一个 $(len, cnt)$ 二元组。

### add(idx, num, t_cnt)

- **作用**：将 $(num, t\_cnt)$ 合并到 BIT 节点 $idx$ 上（单节点更新）。
- **合并规则**：
  - 若 $num > len[idx]$：新长度更优，直接替换 $len[idx] = num$，$cnt[idx] = t\_cnt$（旧方案被更优方案淘汰）。
  - 若 $num = len[idx]$：长度相同，方案数累加 $cnt[idx] \mathrel{+}= t\_cnt \pmod{mod}$。
  - 若 $num < len[idx]$：新方案更短，不做任何操作（保持已有更优值）。
- **关键细节**：这里的合并规则是"取最大长度，等长则计数相加"，与普通 BIT 的"求和"完全不同。这要求 BIT 的每个节点维护的不是单一标量，而是 $(max, count)$ 对。

### insert(idx, num, t_cnt)

- **作用**：将 $(num, t\_cnt)$ 插入 BIT 位置 $idx$，并沿 lowbit 链向上传播。
- **算法步骤**：`while (idx <= sz) { add(idx, num, t_cnt); idx += lowbit(idx); }`。
- **关键细节**：与标准 BIT 单点更新完全一致，只是 `add` 内部逻辑变为 $(len, cnt)$ 合并而非简单加法。

### get(idx)

- **作用**：查询前缀 $[1, idx]$ 上的最大 LIS 长度及方案数，结果存入 `ans_len` 和 `ans_cnt`。
- **算法步骤**：`ans_len = 0, ans_cnt = 0; while (idx) { merge BIT[idx] into (ans_len, ans_cnt); idx -= lowbit(idx); }`。
- **合并细节**：遍历覆盖 $[1, idx]$ 的所有 BIT 节点，对每个节点的 $(len, cnt)$ 与当前 $(ans\_len, ans\_cnt)$ 执行同样的合并规则——取较大 $len$，等长则 $cnt$ 相加。
- **关键细节**：查询的是值域前缀 $[1, idx]$ 上的最优值，对应"所有值 $\le idx$ 的元素中最大的 LIS 长度及方案数"。为求严格递增（值 $< A[i]$），调用时传入 $rank(i) - 1$。

### 离散化

- **作用**：将原始值域压缩到 $[1, m]$（$m$ 为不同值个数），使 BIT 能以下标直接索引。
- **步骤**：`sort` → `unique` → `lower_bound` 逐个映射。
- **关键细节**：离散化保证了值的大小关系不变（仅压缩值域），因此"值 $< A[i]$"的约束在离散化后等价于"排名 $< rank(i)$"。

## 逐行代码解析

```cpp
#include<bits/stdc++.h>
using namespace std;
const int mod = 1e9 + 7;          // 方案数取模
inline int lowbit(int x) { return x & -x; }
struct treearr
{
	vector<int> len;               // len[i]: BIT 节点 i 的最大 LIS 长度
	vector<int> cnt;               // cnt[i]: BIT 节点 i 的方案数
	int sz, ans_len, ans_cnt;     // sz: 值域大小; ans_*: 查询暂存

	treearr(int n)
	{
		len.resize(n + 1, 0);
		cnt.resize(n + 1, 0);
		sz = n;
		ans_len = 0, ans_cnt = 0;
	}
	// 单节点合并：(num, t_cnt) 与 BIT[idx] 的 (len, cnt) 比较
	void add(int idx, int num, int t_cnt)
	{
		if (num > len[idx])              // 新长度更优 → 替换
			len[idx] = num, cnt[idx] = t_cnt;
		else if (num == len[idx])        // 长度相同 → 计数累加
			cnt[idx] = (cnt[idx] + t_cnt) % mod;
		// num < len[idx] → 忽略（已有更优）
	}
	// BIT 单点更新（沿 lowbit 链向上传播）
	void insert(int idx, int num, int t_cnt)
	{
		while (idx <= sz)
		{
			add(idx, num, t_cnt);
			idx += lowbit(idx);
		}
	}
	// BIT 前缀查询 [1, idx] 的 (max_len, count)
	void get(int idx)
	{
		ans_len = 0, ans_cnt = 0;
		while (idx)
		{
			if (len[idx] > ans_len)          // 该节点更优
			{
				ans_len = len[idx];
				ans_cnt = cnt[idx];
			}
			else if (len[idx] == ans_len)    // 等长 → 计数累加
				ans_cnt = (ans_cnt + cnt[idx]) % mod;
			idx -= lowbit(idx);
		}
	}
};
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int T;
	cin >> T;
	while (T--)
	{
		int n, sz;
		cin >> n;
		vector<int> A(n + 1);           // 原始序列
		vector<int> tmp(n + 1);        // 离散化辅助数组
		for (int i = 1; i <= n; i++)
		{
			cin >> A[i];
			tmp[i] = A[i];
		}
		// 离散化：排序 + 去重
		sort(tmp.begin() + 1, tmp.end());
		tmp.erase(unique(tmp.begin() + 1, tmp.end()), tmp.end());
		treearr arr(tmp.size());       // BIT 大小 = 不同值个数
		for (int i = 1; i <= n; i++)
		{
			// 将 A[i] 映射到离散化后的排名 [1, m]
			A[i] = lower_bound(tmp.begin() + 1, tmp.end(), A[i]) - tmp.begin();
			// 查询所有值 < A[i] 的元素中，最大 LIS 长度及方案数
			arr.get(A[i] - 1);
			int new_len = arr.ans_len + 1;           // 在前驱最优基础上 +1
			int new_cnt = arr.ans_cnt ? arr.ans_cnt : 1;  // 无前驱时方案数为 1
			// 将当前元素的 (len, cnt) 插入 BIT
			arr.insert(A[i], new_len, new_cnt);
		}
		// 查询整个值域 [1, m] 的全局最优
		arr.get(tmp.size());
		cout << arr.ans_len << ' ' << arr.ans_cnt << '\n';
	}
	return 0;
}
```

**关键解析：**

- `struct treearr`：将树状数组从"前缀和"推广为"前缀最大值 + 计数"。`len` 和 `cnt` 两个数组配合，每个 BIT 节点维护一个 $(max\_len, count)$ 二元组。
- `add` 中的三种分支：`num > len` 替换（更优方案淘汰旧方案），`num == len` 累加（等长方案合并），`num < len` 忽略（不更新）。这是"前缀最大值 BIT"的核心合并逻辑。
- `get(A[i] - 1)` 查询排名 $< rank(A[i])$ 的前缀，对应值严格小于 $A[i]$ 的所有已处理元素。用 $A[i] - 1$ 而非 $A[i]$ 保证严格递增（相同值不参与转移）。
- `new_cnt = arr.ans_cnt ? arr.ans_cnt : 1`：当 `ans_cnt == 0` 时说明没有前驱（当前元素是已处理中最小的），此时 LIS 长度为 $1$，方案数为 $1$（自身单独构成）。当 `ans_cnt > 0` 时，方案数继承前驱的方案数（每个前驱的 LIS 方案都能接上当前元素）。
- `arr.insert(A[i], new_len, new_cnt)`：将当前元素的信息插入 BIT 对应位置，后续元素查询时能看到它。
- 最终 `arr.get(tmp.size())`：查询整个值域 $[1, m]$ 的全局最大长度及方案数，即整个序列的 LIS 长度和方案数。
- 离散化的 `lower_bound`：返回 `tmp` 中第一个等于 $A[i]$ 的位置，由于 `tmp` 从下标 $1$ 开始，返回值即为排名 $[1, m]$。

## 复杂度分析

- **时间复杂度**：离散化（排序 $O(n \log n)$ + 去重 $O(n)$ + 逐个二分映射 $O(n \log n)$）；主循环 $n$ 个元素，每个做一次 `get`（$O(\log m)$）和一次 `insert`（$O(\log m)$），合计 $O(n \log m)$。由于 $m \le n$，总时间复杂度为 $O(n \log n)$。
- **空间复杂度**：`A` 和 `tmp` 各 $O(n)$，BIT 的 `len` 和 `cnt` 各 $O(m) \le O(n)$，总空间复杂度为 $O(n)$。

与朴素 $O(n^2)$ DP 相比，BIT 优化将内层转移从线性扫描降为对数查询。与二分优化的 LIS（$O(n \log n)$ 求长度但不求方案数）相比，BIT 优化在相同时间复杂度下额外支持方案数计数。

## 适用场景

需要同时求解 LIS 长度和方案数的场景（如统计最长上升子序列的个数）、值域较大或含负数需要离散化的场景、需要 $O(n \log n)$ 在线处理（元素逐个到达、实时查询）的场景。常见于竞赛中的 LIS 计数变种问题。

## 常见陷阱与注意事项

1. **严格递增 vs 非递减**：代码中 `get(A[i] - 1)` 查询 $rank - 1$ 保证严格递增（相同值不参与转移）。若需最长非递减子序列，应改为 `get(A[i])`（允许相同值参与转移）。
2. **方案数的模运算**：`cnt` 的累加在 `add` 和 `get` 中都取模，但 `new_cnt = arr.ans_cnt ? arr.ans_cnt : 1` 中若 `ans_cnt` 恰好为 $0$（取模后为 $0$，实际方案数为 $mod$ 的倍数）会被误判为"无前驱"。实际竞赛中方案数 $\equiv 0 \pmod{10^9+7}$ 的概率极低，但严格来说应使用 `ans_len > 0` 判断是否有前驱。
3. **离散化的下标起点**：`tmp` 从下标 $1$ 开始（`tmp.begin() + 1`），`lower_bound` 返回值范围为 $[1, m]$。若误从下标 $0$ 开始，`A[i]` 可能映射到 $0$，导致 `get(A[i] - 1)` 查询 `get(-1)` 越界。
4. **BIT 大小**：BIT 大小应为离散化后的不同值个数 $m$，而非原始序列长度 $n$。当有重复值时 $m < n$，BIT 更省空间。
5. **多组数据的初始化**：每组测试数据新建 `treearr` 对象，自动初始化 `len` 和 `cnt` 为 $0$。若改为全局数组复用，需手动清零。
6. **`ans_cnt` 的初值**：`get` 函数中 `ans_cnt = 0`，当查询结果为空（`idx = 0` 或所有节点 `len = 0`）时返回 $(0, 0)$。主循环中通过 `arr.ans_cnt ? ... : 1` 处理此情况。

## 对比与扩展

与朴素 $O(n^2)$ DP 相比，BIT 优化利用了"值域约束可转化为前缀查询"的性质，将内层转移从 $O(n)$ 降为 $O(\log n)$。与二分优化 LIS 相比，二分法仅求长度（$O(n \log n)$），无法直接计数；BIT 优化在相同复杂度下支持计数，但需要离散化预处理和更大的常数因子。

扩展方向包括：线段树替代 BIT（线段树支持区间查询，更灵活但常数更大）、带权 LIS（每个元素有权值，求最大权值上升子序列，需将 BIT 的合并规则改为权值相关）、二维 LIS（如俄罗斯套娃问题，需多维离散化）、以及将 BIT 优化思路推广到其他"值域约束型 DP"（如最长递增子序列的变种、LCS 转化为 LIS 问题等）。若需还原具体 LIS 路径而不仅计数，需额外维护前驱指针并回溯。
