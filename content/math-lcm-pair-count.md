---
id: math-lcm-pair-count
title: 'LCM数对计数问题'
category: 数学
subcategory: 数论计数
tags: ["数学", "数论", "LCM", "计数", "质因数分解", "DFS"]
timeComplexity: '分解O(√n)，枚举O(∏(2e_i+1))'
spaceComplexity: 'O(质因子数)'
codePath: '数学\LCM数对计数问题\Untitled1.cpp'
---

## 算法原理

本算法求解满足 $\text{lcm}(a, b) = n$ 的有序对 $(a, b)$ 的个数。关键观察是：LCM 由各质因子的最大指数决定，因此可以将问题分解到每个质因子维度上独立处理，再利用乘法原理将各维度的方案数相乘。

设 $n$ 的质因数分解为 $n = \prod_{i} p_i^{e_i}$。对于每个质因子 $p_i$，设 $a$ 中 $p_i$ 的指数为 $\alpha_i$，$b$ 中 $p_i$ 的指数为 $\beta_i$，则 $\text{lcm}(a, b)$ 中 $p_i$ 的指数为 $\max(\alpha_i, \beta_i)$。要求 $\max(\alpha_i, \beta_i) = e_i$，即 $\alpha_i$ 和 $\beta_i$ 中至少有一个等于 $e_i$，且两者都不超过 $e_i$。

满足 $\max(\alpha, \beta) = e$ 且 $0 \le \alpha, \beta \le e$ 的有序对 $(\alpha, \beta)$ 共有 $2e + 1$ 种：当 $\alpha = e$ 时 $\beta$ 可取 $0, 1, \ldots, e$ 共 $e+1$ 种；当 $\beta = e$ 且 $\alpha \ne e$ 时 $\alpha$ 可取 $0, 1, \ldots, e-1$ 共 $e$ 种。两者合计 $2e + 1$。由于各质因子维度独立，总方案数为各维度方案数之积 $\prod_{i} (2e_i + 1)$。

算法实现分两步：先用 `init` 对 $n$ 做质因数分解（试除法，$O(\sqrt{n})$），将每个质因子及其指数存入 `AM` 向量；再用 DFS 在质因子维度上枚举每个质因子的指数分配方案。对于每个质因子 $p^e$，DFS 分两段枚举：第一段固定 $a$ 的指数为 $e$，$b$ 的指数取 $0 \sim e$（共 $e+1$ 种）；第二段固定 $b$ 的指数为 $e$，$a$ 的指数取 $0 \sim e-1$（共 $e$ 种，排除 $a = e$ 避免与第一段重复计数）。两段合计恰好 $2e + 1$ 种，与公式吻合。

## 核心公式

$n$ 的质因数分解：
$$n = \prod_{i=1}^{m} p_i^{e_i}$$

LCM 的质因子条件：
$$\max(\alpha_i, \beta_i) = e_i, \quad 0 \le \alpha_i, \beta_i \le e_i$$

单个质因子的合法方案数：
$$|\{(\alpha, \beta) : \max(\alpha, \beta) = e\}| = (e + 1) + e = 2e + 1$$

有序对总数（乘法原理）：
$$\text{ans} = \prod_{i=1}^{m} (2e_i + 1)$$

## 逐行代码解析

```cpp
#include<bits/stdc++.h>
using namespace std;
vector<pair<long long,long long>> AM;   // 存储 (质因子, 指数) 对
long long ans;

// 质因数分解：将 n 分解为 AM = {(p1,e1), (p2,e2), ...}
void init(long long n)
{
	ans = 0;
	AM.clear();
	for(long long i = 2; i * i <= n; i++)     // 试除法
	{
		long long cnt = 0;
		while(n % i == 0) { ++cnt; n /= i; }
		if(cnt) AM.emplace_back(i, cnt);
	}
	if(n > 1) AM.emplace_back(n, 1);          // 剩余的大于 1 的因子
}

// DFS 枚举每个质因子的指数分配
void dfs(long long idx, long long a, long long b)
{
	if(idx == AM.size())                      // 所有质因子已分配完毕
	{
		cout << a << " " << b << '\n';        // 输出合法对
		++ans;
		return;
	}
	// 预处理 p^0, p^1, ..., p^e
	vector<long long> tmp(AM[idx].second + 1);
	tmp[0] = 1;
	for(long long i = 1; i <= AM[idx].second; i++)
		tmp[i] = tmp[i - 1] * AM[idx].first;

	// 第一段：a 的指数固定为 e，b 的指数取 0~e（共 e+1 种）
	for(long long i = 0; i <= AM[idx].second; i++)
		dfs(idx + 1, a * tmp[AM[idx].second], b * tmp[i]);

	// 第二段：b 的指数固定为 e，a 的指数取 0~e-1（共 e 种，排除 a=e 避免重复）
	for(long long i = 0; i < AM[idx].second; i++)
		dfs(idx + 1, a * tmp[i], b * tmp[AM[idx].second]);
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int t;
	cin >> t;
	for(int i = 1; i <= t; i++)
	{
		long long n;
		cin >> n;
		init(n);
		dfs(0, 1, 1);
		cout << "Case " << i << ": " << ans << '\n';
	}
	return 0;
}
```

**关键解析：**

- `init` 中的试除法：遍历 $i$ 从 $2$ 到 $\sqrt{n}$，对每个能整除 $n$ 的 $i$ 统计指数。若循环结束后 $n > 1$，说明剩余部分是一个大于 $\sqrt{n}$ 的质因子（指数为 $1$），单独加入。
- `tmp[i] = tmp[i-1] * AM[idx].first`：预计算 $p^0, p^1, \ldots, p^e$，避免在 DFS 中重复计算幂值。
- 第一段 `for(i=0; i<=e; i++)`：固定 $a$ 的指数为 $e$（`a * tmp[e]`），$b$ 的指数取 $0 \sim e$。这覆盖了所有 $\alpha = e$ 的情形。
- 第二段 `for(i=0; i<e; i++)`：固定 $b$ 的指数为 $e$（`b * tmp[e]`），$a$ 的指数取 $0 \sim e-1$。上界为 $e-1$ 排除了 $\alpha = e$（已在第一段计数），避免 $(e, e)$ 重复。
- `ios::sync_with_stdio(false)`：取消 C/C++ 流同步，加速大量 `cout` 输出。
- DFS 的乘法累积：`a` 和 `b` 在递归过程中累积各质因子的贡献，到达叶子节点时即为完整的 $(a, b)$ 值。

## 复杂度分析

质因数分解 `init` 使用试除法，时间复杂度为 $O(\sqrt{n})$。DFS 枚举的总方案数为 $\prod_{i} (2e_i + 1)$，每个质因子贡献 $2e_i + 1$ 种选择。由于 $\prod (2e_i + 1) \le \prod (2e_i + 1) \le d(n)^2$（$d(n)$ 为约数个数），且 $d(n) = O(n^\epsilon)$（对任意 $\epsilon > 0$），实际方案数通常远小于 $n$。空间上 `AM` 存储质因子对，个数为 $O(\log n / \log \log n)$（质因子数的上界），DFS 递归深度等于质因子个数，总空间复杂度为 $O(\text{质因子数})$。

## 适用场景

数论计数问题、约数与 LCM/GCD 组合计数、需要枚举所有满足特定 LCM 或 GCD 条件的数对的场景。这类问题在竞赛数论中较为常见，核心是将多元条件分解到各质因子维度独立处理。

## 常见陷阱与注意事项

1. **重复计数**：$(e, e)$ 即 $\alpha = \beta = e$ 同时满足"$\alpha = e$"和"$\beta = e$"两个条件。若两段枚举的上界都设为 $e$，则该情况被计数两次。第二段上界设为 $e - 1$ 是避免重复的关键。
2. **有序对 vs 无序对**：本代码求解有序对 $(a, b)$，即 $(a, b)$ 与 $(b, a)$ 视为不同。若题目要求无序对，需对 $a \ne b$ 的情况除以 $2$，再加回 $a = b$ 的 $1$ 种。
3. **$n = 1$ 的退化**：当 $n = 1$ 时质因数分解为空，`AM.size() = 0`，DFS 直接到达叶子节点输出 $(1, 1)$，答案为 $1$（因为 $\text{lcm}(1,1) = 1$）。
4. **溢出**：`a` 和 `b` 在 DFS 中累积乘积可能超过 `long long` 范围。若题目保证 $a, b \le n$ 则不会溢出（因为 $a, b$ 均为 $n$ 的约数），但若 $n$ 接近 `long long` 上界仍需注意。
5. **大数质因数分解**：试除法为 $O(\sqrt{n})$，当 $n$ 达到 $10^{14}$ 以上时需考虑 Pollard-Rho 等更快的分解算法。
6. **输出量过大**：代码在 DFS 叶子节点输出每个合法对 $(a, b)$，当方案数极大时输出会成为瓶颈，实际使用中可去掉输出仅计数。

## 对比与扩展

与直接枚举所有约数对再验证 LCM 的 $O(d(n)^2)$ 做法相比，本算法利用质因子维度的独立性将复杂度降至 $O(\prod(2e_i+1))$，通常远小于 $d(n)^2$。类似思路可推广到 GCD 计数（$\min(\alpha, \beta) = e_i$ 的方案数为 $(2e_i + 1)$ 但分配方式不同）、以及同时约束 LCM 和 GCD 的计数问题。扩展方向包括：利用莫比乌斯反演处理 $\sum \text{lcm}$ 类求和问题、结合容斥原理处理多约束计数、以及推广到多元组 $(a_1, a_2, \ldots, a_k)$ 的 LCM 计数。
