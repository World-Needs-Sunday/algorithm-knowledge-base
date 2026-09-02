---
id: dp-multi-bag-monotone
title: '多重背包（单调队列优化）'
category: 动态规划
subcategory: 背包问题
subSubcategory: 多重背包
tags: ["动态规划", "背包问题", "多重背包", "单调队列", "DP"]
timeComplexity: 'O(n×W)'
spaceComplexity: 'O(W)'
codePath: '动态规划\背包问题模板\多重背包\多重背包单调队列优化\Untitled1.cpp'
---

## 算法原理

单调队列优化是多重背包的最高效优化，将复杂度降到 $O(n \times W)$，适用于 $n$ 和 $W$ 均达到 $10^5$ 级别的场景。核心思想是将转移方程转化为滑动窗口最值问题，用单调队列在 $O(1)$ 均摊时间内完成每次转移。

对于第 $i$ 种物品（价值 $v$，重量 $w$，数量 $m$），转移方程为 $dp[j] = \max_{0 \leq k \leq \min(m, \lfloor j/w \rfloor)} \big(dp[j - k \times w] + k \times v\big)$。观察发现，$j$ 的转移只依赖于 $j' = j - k \times w$（即与 $j$ 模 $w$ 同余的位置），因此将所有容量按 $\bmod \; w$ 分为 $w$ 组，每组内的转移是独立的。

在同余数类 $r$（$r = 0, 1, \ldots, w-1$）中，容量序列为 $r, r+w, r+2w, \ldots$。对位置 $j = r + t \times w$，转移为 $dp[j] = \max_{0 \leq k \leq m} \big(dp[r + (t-k) \times w] + k \times v\big)$。这等价于在滑动窗口 $[t-m, t]$ 内求 $dp[r + s \times w] - s \times v$ 的最大值再加上 $t \times v$——这是经典的滑动窗口最大值问题，用单调递减队列维护即可。

单调队列存储 $(位置 j, dp[j]值)$。队首出队条件为窗口距离超过 $m$ 件物品（即 $(j - \text{que.front().first}) / w > m$）。队尾维护时弹出所有不如当前优的元素（即 $dp[j] \geq \text{que.back().second} + (j - \text{que.back().first}) / w \times v$）。更新 $dp[j]$ 为队首值加上偏移量。

## 状态定义与转移方程

原始转移方程：

$$dp[j] = \max_{0 \leq k \leq \min(m,\; \lfloor j/w \rfloor)} \big(dp[j - k \times w] + k \times v\big)$$

同余类分组后，对 $j = r + t \times w$：

$$dp[j] = \max_{0 \leq k \leq m} \big(dp[r + (t-k) \times w] + k \times v\big)$$

单调队列更新（$q$ 为单调递减队列，存储位置对）：

$$dp[j] = \text{q.front().second} + \frac{j - \text{q.front().first}}{w} \times v$$

最终答案：

$$\text{ans} = dp[W]$$

## 逐行代码解析

```cpp
#include<bits/stdc++.h>
using namespace std;
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	long long n,W;
	cin >> n >> W;
	vector<long long> dp(W + 1,0);
	deque<pair<long long,long long>> que;
	while(n--)
	{
		long long v,w,m;
		cin >> v >> w >> m;
		for(long long i = 0;i < w;i++)
		{
			que.clear(); 
			for(long long j = i;j <= W;j += w)
			{
				if(!que.empty() && (j - que.front().first) / w > m) que.pop_front(); 
				while(!que.empty() && dp[j] >= que.back().second + (j - que.back().first) / w * v) que.pop_back();
				que.emplace_back(j,dp[j]);
				dp[j] = que.front().second + (j - que.front().first) / w * v;
			}
		}
	}
	cout << dp[W];
	return 0;
} 
```

- **`dp` 数组**：一维滚动数组，大小 $W+1$，初始化为 $0$，使用 `long long` 防溢出。
- **`que` 单调队列**：`deque<pair<long long,long long>>`，存储 $(位置j, dp[j]值)$，维护单调递减。
- **外层物品循环**：`while(n--)` 逐个处理每种物品。
- **同余类遍历**：外层 `i` 从 $0$ 到 $w-1$，枚举所有余数类。内层 `j` 以步长 $w$ 遍历同余数容量（$j = i, i+w, i+2w, \ldots$）。
- **队首出队**：`(j - que.front().first) / w > m` 表示队首位置距离当前位置超过 $m$ 件物品，已超出窗口范围。
- **队尾维护**：`dp[j] >= que.back().second + (j - que.back().first) / w * v` 表示当前值已不劣于队尾元素（考虑偏移量后），弹出队尾保持单调递减。
- **入队与更新**：先将当前 $dp[j]$ 原始值入队，然后用队首值加上偏移量更新 $dp[j]$。
- **输出**：`dp[W]` 即为最大价值。

## 复杂度分析

- **时间复杂度**：$O(n \times W)$。每种物品遍历所有容量一次（$w$ 个同余类覆盖全部 $W+1$ 个位置），每个元素最多入队出队各一次，均摊 $O(1)$。总计 $n$ 种物品，总时间 $O(n \times W)$。
- **空间复杂度**：$O(W)$。`dp` 数组 $O(W)$，单调队列最坏 $O(W)$，但通常远小于 $W$。

## 适用场景

适用于数据范围极大的多重背包问题（$n$ 和 $W$ 均达到 $10^4$ 至 $10^5$ 级别），当二进制优化的 $O\big(W \times \sum \log m_i\big)$ 仍然不够快时使用。是多重背包的最优解法。

## 常见陷阱与注意事项

- **同余类分组**：必须按 $\bmod \; w$ 分组，每组独立处理。若不分组合并处理，转移会依赖错误的位置。
- **队列出队的判断条件**：用 $(j - \text{que.front().first}) / w > m$ 而非 $j - \text{que.front().first} > m \times w$，两者等价但前者更直接。
- **队尾维护的偏移量**：比较时必须加上 $(j - \text{que.back().first}) / w \times v$ 的偏移量，因为不同位置的 $dp$ 值加上不同件数的价值后才能公平比较。
- **入队时机**：先将原始 $dp[j]$ 入队，再用队首更新 $dp[j]$。顺序不能颠倒，否则会用到已更新的值。
- **`long long` 类型**：价值和偏移量乘积可能很大，务必使用 `long long`。
- **队列清空**：处理每种物品的每个同余类前需 `que.clear()`，确保队列从空开始。

## 对比与扩展

与二进制优化（$O\big(W \times \sum \log m_i\big)$）相比，单调队列优化进一步将复杂度降到 $O(n \times W)$，是多重背包的理论最优。但实现更复杂，需要理解同余分组和滑动窗口的思想。单调队列优化是DP优化的重要工具，同样适用于"长度不超过 $m$ 的最大子段和"等滑动窗口最值问题。理解本算法后可推广到其他需要"在窗口内取最值"的DP转移。
