---
id: dp-linear-maxsub
title: '长度不超过m的最大子段和'
category: 动态规划
subcategory: 线性DP
tags: ["动态规划", "线性DP", "单调队列", "前缀和", "DP"]
timeComplexity: 'O(n)'
spaceComplexity: 'O(n)'
codePath: '动态规划\线性DP\长度不超过 m 的最大子串和\Untitled1.cpp'
---

## 算法原理

求长度不超过 $m$ 的最大连续子段和。朴素做法枚举所有长度不超过 $m$ 的子段，复杂度 $O(n \times m)$。利用前缀和与单调队列可将复杂度降到 $O(n)$。

首先计算前缀和数组 $\text{sum}[i] = \sum_{k=1}^{i} A[k]$。任意子段 $A[l..r]$ 的和可表示为 $\text{sum}[r] - \text{sum}[l-1]$。问题转化为：对每个右端点 $i$，在窗口 $[i-m, i-1]$ 内找到最小的前缀和 $\text{sum}[j]$，使得 $\text{sum}[i] - \text{sum}[j]$ 最大。

这本质上是滑动窗口最小值问题：对每个 $i$，需要在长度为 $m$ 的窗口 $[i-m, i-1]$ 中维护前缀和的最小值。用单调递增队列维护——队首始终是窗口内前缀和最小的位置，队尾弹出所有大于等于当前前缀和的元素以保持单调性。

具体地，单调队列存储 $(\text{sum}[j], j)$。对每个 $i$：首先检查队首是否过期（$i - \text{que.front().second} \geq m$ 表示队首已滑出窗口），若过期则弹出。然后将 $\text{sum}[i-1]$ 入队——入队前弹出队尾所有前缀和值 $\geq \text{sum}[i-1]$ 的元素（因为它们不可能成为后续窗口的最小值）。最后，当前最大子段和为 $\text{sum}[i] - \text{que.front().first}$，更新全局最大值 $\text{ans}$。

每个元素最多入队出队各一次，均摊 $O(1)$，总体 $O(n)$。

## 状态定义与转移方程

前缀和定义：

$$\text{sum}[i] = \sum_{k=1}^{i} A[k]$$

子段和表示：

$$\text{sum}(A[l..r]) = \text{sum}[r] - \text{sum}[l-1]$$

对每个右端点 $i$，求窗口内最小前缀和：

$$\text{ans} = \max_{1 \leq i \leq n} \big(\text{sum}[i] - \min\{\text{sum}[j] : i - m \leq j \leq i - 1\}\big)$$

单调队列维护（$q$ 为单调递增队列）：

$$\text{若 } i - q.\text{front().second} \geq m: \quad q.\text{pop\_front()}$$

$$\text{while } q.\text{back().first} \geq \text{sum}[i-1]: \quad q.\text{pop\_back()}$$

## 逐行代码解析

```cpp
#include<bits/stdc++.h>
using namespace std;
int n, m;
vector<long long> sum;
long long ans = -0x3f3f3f3f3f3f3f3f;
int main()
{
	/*P1714*/
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	cin >> n >> m;
	sum.resize(n + 1,0);
	for(int i = 1;i <= n;i++) 
	{
		long long x;
		cin >> x;
		sum[i] = sum[i - 1] + x;
	}
	deque<pair<long long ,int>> que; 
	for(int i = 1;i <= n;i++)
	{
		if(!que.empty())
		{
			if(i - que.front().second >= m) que.pop_front();
		}
		
		while(!que.empty() && que.back().first >= sum[i - 1]) que.pop_back();
		que.emplace_back(sum[i - 1],i);
		
		ans = max(sum[i] - que.front().first , ans);
	}
	cout << ans;
	return 0;
} 
```

- **`sum` 数组**：前缀和数组，大小 $n+1$，`sum[0] = 0`。`sum[i]` 表示 $A[1] + A[2] + \ldots + A[i]$。
- **`ans` 变量**：全局最大子段和，初始化为极小值 $-0x3f3f3f3f3f3f3f3f$（`long long` 的负无穷），因为元素可能全为负。
- **`que` 单调队列**：`deque<pair<long long, int>>`，存储 $(前缀和值, 下标)$，维护单调递增。
- **队首出队**：`i - que.front().second >= m` 表示队首位置距当前右端点已超过 $m$，滑出窗口。
- **队尾维护**：弹出所有前缀和值 $\geq \text{sum}[i-1]$ 的队尾元素（因为它们更大，不可能成为后续窗口的最小值），保持单调递增。
- **入队**：将 $(\text{sum}[i-1], i)$ 入队——注意入队的是 $\text{sum}[i-1]$（左端点的前缀和），而非 $\text{sum}[i]$。
- **更新答案**：`sum[i] - que.front().first` 为当前右端点 $i$ 的最大子段和，更新全局 `ans`。
- **输出**：`ans` 即为长度不超过 $m$ 的最大子段和。

## 复杂度分析

- **时间复杂度**：$O(n)$。前缀和计算 $O(n)$，单调队列处理每个元素最多入队出队各一次，均摊 $O(1)$，总计 $O(n)$。
- **空间复杂度**：$O(n)$。前缀和数组 $O(n)$，单调队列最坏 $O(n)$。

## 适用场景

适用于有长度上限的最大子段和问题（对应洛谷P1714），也可推广到最大子段积、带限制的最短/最长子段等变种。当子段长度无限制时，退化为经典的最大子段和（Kadane算法），$O(n)$ 无需单调队列。

## 常见陷阱与注意事项

- **`ans` 初始化为负无穷**：因为元素可能全为负，初始化为 $0$ 会得到错误答案。使用 $-0x3f3f3f3f3f3f3f3f$ 作为 `long long` 的负无穷。
- **入队的是 `sum[i-1]` 而非 `sum[i]`**：对右端点 $i$，子段和为 $\text{sum}[i] - \text{sum}[j]$（$j \in [i-m, i-1]$），因此队列中维护的是 $\text{sum}[i-1]$ 及之前的前缀和。
- **队首出队的条件**：`i - que.front().second >= m`（注意是 $\geq$ 不是 $>$），因为窗口范围为 $[i-m, i-1]$，长度为 $m$。
- **`long long` 类型**：前缀和可能很大，使用 `long long` 防止溢出。
- **窗口大小 $m$**：当 $m \geq n$ 时退化为无长度限制的最大子段和，算法仍然正确。

## 对比与扩展

与无限制的最大子段和（Kadane算法，$O(n)$ 无需单调队列）相比，本算法增加了长度约束，需用单调队列维护窗口内最小前缀和。单调队列是DP优化的重要工具，同样适用于多重背包的单调队列优化、固定窗口大小的最值查询等场景。可推广为最小子段和、最大子段积、带权子段和等变种。
