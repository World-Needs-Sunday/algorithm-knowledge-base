---
id: dp-multi-bag-binary
title: '多重背包（二进制优化）'
category: 动态规划
subcategory: 背包问题
subSubcategory: 多重背包
tags: ["动态规划", "背包问题", "多重背包", "二进制优化", "DP"]
timeComplexity: 'O(W×Σlog(m_i))'
spaceComplexity: 'O(W+Σlog(m_i))'
codePath: '动态规划\背包问题模板\多重背包\多重背包2进制优化\Untitled1.cpp'
---

## 算法原理

多重背包中第 $i$ 种物品有数量上限 $m_i$ 件，每件重量为 $w_i$、价值为 $u_i$。朴素做法是将每种物品展开为 $m_i$ 个独立物品按01背包处理，时间复杂度为 $O(W \times \sum m_i)$，当 $m_i$ 很大时不可接受。二进制优化将复杂度降到 $O\big(W \times \sum \log m_i\big)$。

核心思想基于二进制拆分原理：任何正整数 $m$ 可以表示为若干 $2$ 的幂次之和加上一个余数。具体地，令 $k = 1, 2, 4, 8, \ldots$，依次从 $m$ 中减去 $k$，将 $(u \times k, w \times k)$ 作为一个"打包物品"存入列表，直到 $m$ 不足以凑出下一个 $2$ 的幂，此时将剩余的 $m$ 作为一个打包物品。例如 $m = 13$ 拆分为 $1 + 2 + 4 + 6$，共 $4$ 个打包物品。

拆分后，这 $\lfloor \log_2 m \rfloor + 1$ 个打包物品的组合可以凑出 $0$ 到 $m$ 之间的任意整数件原物品。这是因为 $1, 2, 4, \ldots, 2^t$ 的子集和可以表示 $0$ 到 $2^{t+1} - 1$ 的所有整数，再加上余数项即可覆盖 $0$ 到 $m$。因此，对打包物品按01背包处理，等价于原问题。

拆分后物品数为 $\sum_{i=1}^{n} \big(\lfloor \log_2 m_i \rfloor + 1\big)$，远小于 $\sum m_i$。对拆分后的所有打包物品执行标准01背包（容量逆序遍历），即可在 $O\big(W \times \sum \log m_i\big)$ 的复杂度内求解。

## 状态定义与转移方程

二进制拆分（对第 $i$ 种物品，价值 $u$，重量 $w$，数量 $m$）：

$$k = 1, 2, 4, \ldots, 2^t, \quad \text{打包物品 } (u \times k, \; w \times k), \quad m \leftarrow m - k$$

当 $m$ 有剩余时：

$$\text{打包物品 } (u \times m_{\text{剩余}}, \; w \times m_{\text{剩余}})$$

拆分后01背包转移：

$$dp[j] = \max\big(dp[j],\; dp[j - w_k] + v_k\big), \quad j \geq w_k$$

最终答案：

$$\text{ans} = dp[W]$$

## 逐行代码解析

下面按逻辑阶段把整段代码拆成 5 个模块：头文件与全局变量 → main 与读入 → 读入并二进制拆分 → 01背包转移 → 输出。

### 头文件与全局变量

```cpp
#include<bits/stdc++.h>
using namespace std;
int N , W;
vector<long long> dp;
vector<pair<long long , long long>> A;
/*洛谷P1776*/
```

- `N`、`W` 分别为物品种类数与背包容量，声明为全局变量，方便在 `main` 与各循环中直接使用。
- `dp` 是一维滚动数组，大小为 $W+1$、初始化为 $0$，使用 `long long` 防溢出；`dp[j]` 表示容量 $j$ 下能达到的最大价值。
- `A` 存储拆分后的打包物品，每个 `pair` 的 `first` 为打包价值（$u \times k$），`second` 为打包重量（$w \times k$）。

### main 与读入

```cpp
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	cin >> N >> W;
	dp.resize(W + 1, 0);
```

- `ios::sync_with_stdio(false)` 关闭 C++ 与 C 标准输入输出的同步，`cin.tie(nullptr)` 解除 `cin`/`cout` 绑定，用于加快 IO。
- 读入物品种类数 `N` 与背包容量 `W` 后，把 `dp` 扩容到 $W+1$ 并全部置 $0$，保证下标 `dp[W]` 合法。

### 读入物品并二进制拆分

```cpp
	for (int i = 1; i <= N; i++)
	{
		long long u, w, m;
		cin >> u >> w >> m;
		for (int k = 1; k <= m; k <<= 1)
		{
			A.emplace_back(u * k , w * k);
			m -= k;
		}
		if (m) A.emplace_back(u * m, w * m);
	}
```

- 每次读入一种物品的价值 `u`、重量 `w`、数量 `m`；三者用 `long long`，因为乘以 $k$ 后数值可能很大。
- 内层 `k` 从 $1$ 开始每次左移一位（`k <<= 1`，即翻倍），把打包物品 $(u \times k,\ w \times k)$ 存入 `A`，并从 `m` 中减去 `k`；循环条件 `k <= m` 保证每次打包不超过剩余数量。
- 余数处理：拆分循环结束后若 `m` 仍大于 $0$，把剩余 `m` 件作为一个打包物品存入 `A`（如 $m = 13$ 拆成 $1 + 2 + 4 + 6$）。

### 01背包转移

```cpp
	for (int i = 0; i < A.size(); i++)
	{
		for (int j = W; j >= 1; j--)
		{
			if (j >= A[i].second) dp[j] = max(dp[j], dp[j - A[i].second] + A[i].first);
		}
	}
```

- 对 `A` 里所有打包物品做标准01背包：外层遍历每个打包物品，内层容量 `j` 从 $W$ 逆序到 $1$，保证每个打包物品最多被选一次。
- `if (j >= A[i].second)` 判断当前容量装得下该物品（$j \ge$ 打包重量），装得下才转移，避免下标越界。
- `A[i].first` 是打包价值、`A[i].second` 是打包重量，对应转移方程 $dp[j] = \max(dp[j],\ dp[j - w_k] + v_k)$。

### 输出

```cpp
	cout << dp[W];
	return 0;
}
```

- `dp[W]` 即容量 $W$ 下背包的最大价值，作为最终答案输出。
- `return 0;` 正常结束程序。

## 复杂度分析

- **时间复杂度**：$O\big(W \times \sum_{i=1}^{N} \log m_i\big)$。拆分后打包物品总数为 $\sum_{i=1}^{N} \big(\lfloor \log_2 m_i \rfloor + 1\big) = O\big(\sum \log m_i\big)$，对每个打包物品遍历容量 $W$，故总时间为 $O\big(W \times \sum \log m_i\big)$。
- **空间复杂度**：$O\big(W + \sum \log m_i\big)$。`dp` 数组 $O(W)$，打包物品列表 $O\big(\sum \log m_i\big)$。

## 适用场景

适用于每种物品有数量上限的多重背包问题（对应洛谷P1776），当 $m_i$ 较大（$10^4$ 级别）且 $W$ 也在 $10^4$ 级别时使用。当 $m_i$ 和 $W$ 均达到 $10^5$ 级别时需进一步用单调队列优化。

## 常见陷阱与注意事项

- **`k <<= 1` 的含义**：`k` 每次翻倍（$1 \to 2 \to 4 \to 8 \to \ldots$），这是二进制拆分的核心。注意 `k <<= 1` 等价于 `k *= 2`。
- **拆分循环的终止条件**：`k <= m` 确保每次打包不超过剩余数量。若写成 `k <= m` 的判断有误，可能导致打包数量超过实际。
- **余数项不可遗漏**：拆分后若 $m$ 仍有剩余（如 $m = 13$，拆出 $1+2+4=7$，剩余 $6$），必须将剩余作为最后一个打包物品，否则无法凑出所有 $0$ 到 $m$ 的数量。
- **`long long` 防溢出**：价值和重量乘以 $k$ 后可能很大，务必使用 `long long`。
- **容量逆序遍历**：打包物品按01背包处理，容量必须逆序，否则同一打包物品会被重复选取。

## 对比与扩展

与朴素多重背包（$O(W \times \sum m_i)$）相比，二进制优化通过拆分将物品数从 $\sum m_i$ 降到 $\sum \log m_i$，大幅降低复杂度。与单调队列优化（$O(N \times W)$）相比，二进制优化实现简单但复杂度略高。当 $W$ 和 $m_i$ 都很大（$10^5$ 级别）时，单调队列优化是更优选择。二进制拆分的思想也可用于其他需要"将大数量拆分为对数级别"的场景。
