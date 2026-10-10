---
id: dp-interval-stone-merge
title: '区间DP — 环形石子合并（区间DP入门经典）'
category: 动态规划
subcategory: 区间DP
tags: ["区间DP", "动态规划", "石子合并", "环形DP", "断环成链", "前缀和"]
timeComplexity: 'O(n³)'
spaceComplexity: 'O(n²)'
codePath: '动态规划\区间DP\环形石子合并\Untitled1.cpp'
---

## 算法原理

### 什么是区间 DP

区间 DP 是动态规划的一个重要分支，核心特征是：

- 在一个**区间** $[l, r]$ 上定义状态；
- 大区间的答案由若干个**小区间**合并而来；
- 计算顺序：**先算短区间，再算长区间**。

典型应用场景：
- 石子合并（最小/最大代价）
- 能量项链
- 括号匹配 / 最长回文子序列
- 加分二叉树
- 多边形三角剖分

### 石子合并问题

$n$ 堆石子围成一圈，第 $i$ 堆有 $a_i$ 个石子。每次只能合并**相邻**的两堆，合并的代价为这两堆石子的数量之和。求将所有石子合并成一堆的**最小代价**和**最大代价**。

### 核心思想

合并 $[l, r]$ 这一段的最后一步，一定是把左边某段和右边某段合并到一起：

```
左边 [l, k-1]    右边 [k, r]
      \              /
       \            /
        \          /
         合并成一堆
    代价 = 左边代价 + 右边代价 + 区间总和
```

枚举分割点 $k$，取最优值。这就是区间 DP 的标准转移结构。

---

## 状态定义与转移方程

### 状态定义

| 状态 | 含义 |
|------|------|
| `mindp[len][l]` | 把从 $l$ 开始、长度为 `len` 的区间（即 $[l, l+len-1]$）合并成一堆的**最小**代价 |
| `maxdp[len][l]` | 把从 $l$ 开始、长度为 `len` 的区间合并成一堆的**最大**代价 |

> 注：也常用 `dp[l][r]` 的写法，含义相同。`[len][l]` 写法更强调"按长度递推"的顺序。

### 转移方程

枚举分割点 $k$（右区间的起点）：

$$
\text{mindp}[len][l] = \min_{k=l+1}^{l+len-1} \Big( \text{mindp}[k-l][l] + \text{mindp}[len-k+l][k] + \text{sum}(l, l+len-1) \Big)
$$

$$
\text{maxdp}[len][l] = \max_{k=l+1}^{l+len-1} \Big( \text{maxdp}[k-l][l] + \text{maxdp}[len-k+l][k] + \text{sum}(l, l+len-1) \Big)
$$

其中：

- `mindp[k-l][l]`：左半段 $[l, k-1]$ 的代价，长度为 $k-l$
- `mindp[len-k+l][k]`：右半段 $[k, l+len-1]$ 的代价，长度为 $len - (k-l)$
- `sum(l, r)`：区间 $[l, r]$ 的石子总数，即本次合并的代价

### 理解"最后一步"

不管前面怎么合并，最后一步一定是把左右两大堆合并。这一步的代价是固定的（等于区间总和），所以总代价 = 左边最优 + 右边最优 + 本次合并代价。

这就是区间 DP 的"**最优子结构**"：大区间的最优解由小区间的最优解组合而来。

---

## 环形处理：断环成链

### 问题

石子是围成一圈的，首尾相邻。如果直接按线性区间 $[1, n]$ 做，会漏掉"从第 $n$ 堆和第 1 堆之间断开"的方案。

### 技巧：复制一遍数组

把数组复制一份接在后面，长度变成 $2n$：

$$
A[1], A[2], \dots, A[n], A[1], A[2], \dots, A[n]
$$

这样，原环上任意一种断开方式，都对应线性数组中一段长度为 $n$ 的连续区间：

```
原环：  1 - 2 - 3 - 4 - (回到1)
复制后：1 2 3 4 1 2 3 4

从 1 断开 → 区间 [1, 4]
从 2 断开 → 区间 [2, 5]
从 3 断开 → 区间 [3, 6]
从 4 断开 → 区间 [4, 7]
```

### 答案提取

做完 DP 后，枚举所有长度为 $n$ 的区间，取最值：

$$
\text{minans} = \min_{i=1}^{n} \text{mindp}[n][i]
$$

$$
\text{maxans} = \max_{i=1}^{n} \text{maxdp}[n][i]
$$

---

## 前缀和优化

每次合并的代价是区间总和，如果每次都遍历求和会是 $O(n)$，总复杂度变成 $O(n^4)$。

用**前缀和**预处理，可以 $O(1)$ 求区间和：

$$
\text{nsum}[i] = \sum_{j=1}^{i} A[j]
$$

$$
\text{sum}(l, r) = \text{nsum}[r] - \text{nsum}[l-1]
$$

---

## 完整算法流程

### 第一步：初始化

```
1. 读入 n 和每堆石子数 a[1..n]
2. 复制数组：a[n+1..2n] = a[1..n]
3. 计算前缀和 nsum[1..2n]
4. 初始化 DP：
   - mindp[1][i] = 0  （只有一堆，不需要合并）
   - maxdp[1][i] = 0
   - mindp[len][l] = INF  （最小值初始化为无穷大）
   - maxdp[len][l] = 0    （最大值初始化为 0）
```

### 第二步：区间 DP 转移

```
按区间长度从小到大枚举 len = 2 .. n:
    枚举左端点 l:
        枚举分割点 k（右区间起点）:
            cost = nsum[l+len-1] - nsum[l-1]   // 本次合并代价
            mindp[len][l] = min(mindp[len][l],
                                mindp[k-l][l] + mindp[len-k+l][k] + cost)
            maxdp[len][l] = max(maxdp[len][l],
                                maxdp[k-l][l] + maxdp[len-k+l][k] + cost)
```

### 第三步：提取答案

```
minans = INF, maxans = 0
枚举 i = 1 .. n:
    minans = min(minans, mindp[n][i])
    maxans = max(maxans, maxdp[n][i])
输出 minans, maxans
```

---

## 逐行代码解析

### 变量定义

```cpp
const int INF = 0x3f3f3f3f;   // 无穷大，约 1e9
vector<int> A;                 // 石子数量数组（2n 长度）
vector<vector<int>> mindp;     // 最小代价 DP 表
vector<vector<int>> maxdp;     // 最大代价 DP 表
vector<int> nsum;              // 前缀和数组
```

### 初始化

```cpp
int n;
cin >> n;
A.resize(2 * n + 1, 0);       // 开 2n，用于断环成链
nsum.resize(2 * n + 1, 0);
mindp.resize(n + 1, vector<int>(2 * n + 1, INF));   // 最小值初始化为 INF
maxdp.resize(n + 1, vector<int>(2 * n + 1, 0));     // 最大值初始化为 0

for (int i = 1; i <= n; i++)
{
    cin >> A[i];
    A[i + n] = A[i];          // 复制一遍，断环成链
}

for (int i = 1; i <= 2 * n; i++)
{
    nsum[i] = nsum[i - 1] + A[i];   // 前缀和
    mindp[1][i] = 0;                // 长度为 1，代价为 0
}
```

### DP 转移

```cpp
for (int len = 2; len <= n; len++)           // 枚举区间长度
{
    for (int l = 1; l <= 2 * n - len + 1; l++)  // 枚举左端点
    {
        for (int k = l + 1; k < l + len; k++)   // 枚举分割点
        {
            int cost = nsum[l + len - 1] - nsum[l - 1];  // 区间和 = 本次合并代价
            // 左区间：[l, k-1]，长度 k-l
            // 右区间：[k, l+len-1]，长度 len - (k-l)
            mindp[len][l] = min(mindp[len][l],
                mindp[k - l][l] + mindp[len - k + l][k] + cost);
            maxdp[len][l] = max(maxdp[len][l],
                maxdp[k - l][l] + maxdp[len - k + l][k] + cost);
        }
    }
}
```

### 提取答案

```cpp
int minarr = INF;
int maxarr = 0;
for (int i = 1; i <= n; i++)
{
    minarr = min(minarr, mindp[n][i]);   // 枚举所有可能的断开点
    maxarr = max(maxarr, maxdp[n][i]);
}
cout << minarr << endl << maxarr;
```

### 关键变量对照表

| 变量 | 含义 |
|------|------|
| `len` | 当前处理的区间长度 |
| `l` | 区间左端点 |
| `k` | 分割点，右区间的起点 |
| `k - l` | 左区间长度 |
| `len - k + l` | 右区间长度 |
| `nsum[l + len - 1] - nsum[l - 1]` | 区间和 = 本次合并代价 |

---

## 复杂度分析

### 时间复杂度

三层循环：
- 外层枚举长度：$O(n)$
- 中层枚举左端点：$O(n)$
- 内层枚举分割点：$O(n)$

总时间复杂度：

$$
O(n^3)
$$

对于 $n \le 100$ 完全没问题，$n \le 300$ 也能过。如果 $n$ 更大（如 1000+），需要用四边形不等式优化到 $O(n^2)$。

### 空间复杂度

DP 表是 $(n+1) \times (2n+1)$，空间复杂度：

$$
O(n^2)
$$

---

## 适用场景

1. **区间合并类问题**：石子合并、能量项链
2. **区间最优决策**：加分二叉树、多边形三角剖分
3. **回文/括号类**：最长回文子序列、括号匹配
4. **环形问题**：通过"断环成链"技巧转化为线性区间 DP

---

## 常见陷阱与注意事项

### 1. 长度从 2 开始枚举

长度为 1 的区间不需要合并，代价为 0，是初始条件。DP 从 `len = 2` 开始。

### 2. 分割点不能等于右端点

分割点 $k$ 的范围是 $l+1 \le k < l+len$，不能取到 $l+len$（否则右区间为空，没有意义）。

### 3. 环形数组要开 2n

数组 `A`、前缀和 `nsum`、DP 表都要考虑 $2n$ 的长度。只开 $n$ 会越界。

### 4. 答案不是 mindp[n][1]

环形问题要枚举所有长度为 $n$ 的区间取最值，不能直接用 `mindp[n][1]`。这是最容易犯的错误。

### 5. 最小值要初始化为 INF

`mindp` 必须初始化为一个足够大的值（如 `0x3f3f3f3f`），然后再用 `min` 更新。如果初始化为 0，永远取不出最小值。

### 6. 最大值的初始化要小心

如果石子重量**全为正**，`maxdp` 初始化为 0 没问题（因为每次合并都是加正数）。

但如果石子可能为**负数**，`maxdp` 必须初始化为 `-INF`，否则会被 0 卡住，取不到正确的负数值。

### 7. 数据类型

当 $n = 100$、每堆 $100$ 时，总代价约 $100 \times 100 \times 100 = 10^6$，`int` 够。但当 $n$ 更大或石子更大时，要考虑 `long long`。

### 8. 下标从 1 开始

前缀和和区间 DP 用 1-based 下标最方便，`nsum[0] = 0`，区间 $[l, r]$ 的和直接是 `nsum[r] - nsum[l-1]`。

### 9. 转移时两边都要用同一种 DP

求最小值就两边都用 `mindp`，求最大值就两边都用 `maxdp`。不要写混了（比如最小值里不小心用了 maxdp）。

---

## 区间 DP 的通用模板

### 线性版

```cpp
for (int len = 2; len <= n; len++) {
    for (int l = 1; l + len - 1 <= n; l++) {
        int r = l + len - 1;
        dp[l][r] = INF;
        for (int k = l; k < r; k++) {
            dp[l][r] = min(dp[l][r],
                dp[l][k] + dp[k + 1][r] + cost(l, r));
        }
    }
}
```

### 环形版

```cpp
// 断环成链：a[i+n] = a[i]
for (int len = 2; len <= n; len++) {
    for (int l = 1; l + len - 1 <= 2 * n; l++) {
        int r = l + len - 1;
        for (int k = l; k < r; k++) {
            dp[l][r] = min(dp[l][r],
                dp[l][k] + dp[k + 1][r] + cost(l, r));
        }
    }
}
// 答案：枚举 i=1..n 的 dp[i][i+n-1] 取最值
```

---

## 记忆口诀

> **定状态，按长度；**
> **找分割，加代价；**
> **环复制，取最值；**
> **先小区间，再大区间。**
