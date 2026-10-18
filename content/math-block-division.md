---
id: math-block-division
title: '整除分块（数论分块）'
category: 数学
subcategory: 数论基础
subSubcategory: ''
tags: ["数学", "数论", "整除分块", "根号复杂度"]
timeComplexity: 'O(√n)'
spaceComplexity: 'O(1)'
level: A
noCode: true
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 算法原理

整除分块（也叫数论分块）用于解决一类**整除求和**问题：给定 $n$，求

$$
\sum_{i=1}^{n} \left\lfloor \frac{n}{i} \right\rfloor
$$

朴素的直接枚举是 $O(n)$，当 $n \le 10^9$ 时就不可接受了。整除分块的核心观察是：**$\lfloor n/i \rfloor$ 的取值个数远小于 $n$**。

- 随着 $i$ 增大，$\lfloor n/i \rfloor$ 单调不增，且每个取值在 $i$ 上**连续成段**（例如 $n=21$ 时 $\lfloor 21/i \rfloor$ 依次是 $21,10,7,5,5,4,3,3,3,2,2,\dots,1$，越往后越稀疏）。
- 可以证明 $\lfloor n/i \rfloor$ 的不同取值个数 $\le 2\sqrt n$：当 $i \le \sqrt n$ 时最多 $\sqrt n$ 种；当 $i > \sqrt n$ 时 $\lfloor n/i \rfloor < \sqrt n$，也最多 $\sqrt n$ 种。

于是我们把 $i$ 按「$\lfloor n/i \rfloor$ 相同」分成若干块，每块只算一次，复杂度降到 $O(\sqrt n)$。

## 核心公式

对每个块，已知左端点 $l$，其右端点是

$$
r = \left\lfloor \frac{n}{\lfloor n/l \rfloor} \right\rfloor
$$

即 `r = n / (n / l)`（注意这里的除法都是下取整，且 $r$ 要再和 $n$ 取 $\min$ 防止越界）。

## 逐行代码解析

伪代码如下：

```cpp
for (int l = 1; l <= n; l = r + 1) {
    r = min(n, n / (n / l));      // 当前块右端点
    cnt += s(l, r) * (n / l);     // s(l,r) 是该块的"贡献"，n/l 是整块共同的 ⌊n/i⌋ 值
}
```

- `l = r + 1`：跳到下一块的左端点。
- `r = n / (n / l)`：由「$l$ 所在块右端点」公式直接得出。
- `s(l, r)`：表示区间 $[l, r]$ 上的额外贡献（如区间长度、区间和、某函数值之和）；若它能 $O(1)$ 求出，则整体就是 $O(\sqrt n)$。

## 复杂度分析

- **时间复杂度**：块数 $O(\sqrt n)$，每块 $O(1)$，总计 $O(\sqrt n)$。
- **空间复杂度**：$O(1)$。

## 适用场景

- 求 $\sum_{i=1}^n \lfloor n/i \rfloor$ 这类整除求和。
- 统计 $[1,n]$ 中约数个数、约数和等需要按 $\lfloor n/i \rfloor$ 分段的计数问题。
- 是莫比乌斯反演、杜教筛等更复杂数论推导的基础工具。

## 常见陷阱与注意事项

1. **右端点要取 $\min(n, \cdot)$**：`n / (n / l)` 在 $l$ 较小时可能超过 $n$，必须 `min(n, ...)`。
2. **死循环**：忘记 `l = r + 1` 会在同一块原地打转。
3. **整除顺序**：`n / (n / l)` 里两次除法都要下取整，写成 `n / n / l` 就错了。
4. **溢出**：$n$ 很大（接近 `int` 上限）时，块内累加用 `long long` 防溢出。
5. **边界**：$l$ 从 $1$ 开始；$n=0$ 需特判（此时无意义）。

## 对比与扩展

| 方法 | 复杂度 | 适用 |
|---|---|---|
| 直接枚举 | $O(n)$ | $n$ 很小 |
| 整除分块 | $O(\sqrt n)$ | $n \le 10^9 \sim 10^{18}$ 的整除求和 |
| 杜教筛 | $O(n^{2/3})$ | 前缀和型的数论函数求和 |
| 莫比乌斯反演 | 视推导 | 带卷积/容斥的计数，常与整除分块结合 |
