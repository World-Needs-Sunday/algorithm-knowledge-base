---
id: math-linear-sieve-divisor-sum
title: '线性筛求约数和（欧拉筛批量预处理）'
category: '数学'
subcategory: '数论基础'
difficulty: '进阶'
tags: ['数学', '数论', '约数和', '线性筛', '欧拉筛', '积性函数', '预处理']
prerequisites: ['math-sieve-eratosthenes', 'math-linear-sieve-phi', 'math-linear-sieve-divisor-count']
codePath: '数学\数论基础\线性筛求约数和\源.cpp'
description: '在线性筛（欧拉筛）框架上同时计算约数和函数 σ(n)（即 f(n)）：每个合数仅被其最小质因子筛一次，利用 σ 的积性分两种情况递推——当 prime[j] 不整除 i 时 σ(i*p) = σ(i)*(1+p)，整除时需要额外维护最小质因子幂和 g，递推 g(i*p) = g(i)*p+1，σ(i*p) = σ(i)/g(i)*g(i*p)。复杂度 O(n)，适合需要批量查询约数和的场景。'
---

# 线性筛求约数和（欧拉筛批量预处理）

## 一、算法原理

### 1.1 问题背景

约数和函数 $\sigma(n)$（即 $f(n)$）表示 $n$ 的所有正约数之和。当需要查询 $[1, n]$ 内大量数的约数和（如 $q = 10^6$ 次查询，每个数 $\le 10^7$）时，单次 $O(\sqrt{n})$ 的试除法会超时。

**思路**：利用约数和的积性，结合线性筛框架，在 $O(n)$ 时间内一次性求出 $[1, n]$ 内所有 $\sigma(i)$，之后每次查询 $O(1)$。

### 1.2 约数和公式

设 $n$ 的质因数分解为 $n = p_1^{\alpha_1} \cdot p_2^{\alpha_2} \cdots p_s^{\alpha_s}$，则：

$$\sigma(n) = \prod_{i=1}^{s} \left(1 + p_i + p_i^2 + \cdots + p_i^{\alpha_i}\right)$$

**直觉理解**：每个质因子 $p_i$ 的贡献是一个等比数列和 $1 + p_i + p_i^2 + \cdots + p_i^{\alpha_i}$。各质因子的约数独立组合（由积性），所以总和是各等比数列和的乘积。

**示例**：

| $n$ | 分解 | $\sigma(n)$ | 约数 |
|-----|------|------------|------|
| 1 | 1 | 1 | {1} |
| 2 | $2^1$ | $1+2 = 3$ | {1, 2} |
| 4 | $2^2$ | $1+2+4 = 7$ | {1, 2, 4} |
| 6 | $2^1 \times 3^1$ | $(1+2)(1+3) = 12$ | {1, 2, 3, 6} |
| 12 | $2^2 \times 3^1$ | $(1+2+4)(1+3) = 28$ | {1, 2, 3, 4, 6, 12} |

### 1.3 积性与线性筛

约数和函数 $\sigma(n)$ 是**积性函数**：当 $\gcd(a, b) = 1$ 时，$\sigma(ab) = \sigma(a) \cdot \sigma(b)$。

在线性筛中，每个合数 $m = i \times p_j$（$p_j$ 是 $m$ 的最小质因子）被标记时，分两种情况计算 $\sigma(m)$。关键在于额外维护一个辅助数组 $g[\,]$，记录每个数的最小质因子对应的等比数列和 $1 + p + p^2 + \cdots + p^k$。

### 1.4 两种递推情况

**情况一：$i \bmod p_j \neq 0$（$p_j$ 不整除 $i$）**

此时 $\gcd(i, p_j) = 1$（因为 $p_j$ 是质数且不整除 $i$），由积性得：

$$\sigma(i \times p_j) = \sigma(i) \times \sigma(p_j) = \sigma(i) \times (1 + p_j)$$

因为 $p_j$ 是 $i \times p_j$ 的新质因子（指数为 1），对应的等比数列和为 $1 + p_j$，所以 $g(i \times p_j) = 1 + p_j$。

**情况二：$i \bmod p_j = 0$（$p_j$ 整除 $i$）**

此时 $p_j$ 已经是 $i$ 的最小质因子。设 $i$ 中 $p_j$ 的指数为 $k$，则 $i \times p_j$ 中 $p_j$ 的指数变为 $k+1$。

$i$ 的等比数列和为 $g(i) = 1 + p_j + p_j^2 + \cdots + p_j^k$。

$i \times p_j$ 的等比数列和为 $g(i \times p_j) = 1 + p_j + p_j^2 + \cdots + p_j^{k+1} = p_j \times g(i) + 1$。

因为 $\sigma(i) = g(i) \times \sigma(t)$（$t = i / p_j^k$，$p_j \nmid t$），所以：

$$\sigma(i \times p_j) = g(i \times p_j) \times \sigma(t) = \frac{g(i \times p_j)}{g(i)} \times \sigma(i) = \sigma(i) \, / \, g(i) \times g(i \times p_j)$$

> **为什么先除后乘？** $g(i) = 1 + p_j + \cdots + p_j^k$ 整除 $\sigma(i) = g(i) \times \sigma(t)$，所以 $\sigma(i) / g(i) = \sigma(t)$ 是整数。先除保证整除，再乘避免分数运算。

### 1.5 边界与初始值

- $\sigma(1) = 1$（1 只有一个约数——自身），$g(1) = 1$
- 质数 $p$ 的 $\sigma(p) = 1 + p$，$g(p) = 1 + p$（最小质因子就是自身，指数为 1）

## 二、核心公式/状态定义与转移方程

### 2.1 状态定义

| 数组 | 含义 |
|------|------|
| `vis[i]` | $i$ 是否已被筛过（是否为合数） |
| `prime` | 已发现的质数表，按升序存储 |
| `g[i]` | $i$ 的最小质因子对应的等比数列和 $1 + p + p^2 + \cdots + p^k$ |
| `f[i]` | 约数和 $\sigma(i)$ |

### 2.2 转移方程

对每个 $i$（从 2 到 $n$），对每个质数 $p_j$（$p_j \le n / i$），设 $m = i \times p_j$：

$$
g(m) =
\begin{cases}
1 + p_j, & i \bmod p_j \neq 0 \\
p_j \cdot g(i) + 1, & i \bmod p_j = 0
\end{cases}
$$

$$
\sigma(m) =
\begin{cases}
\sigma(i) \cdot (1 + p_j), & i \bmod p_j \neq 0 \quad (\text{新质因子，积性}) \\
\sigma(i) \, / \, g(i) \cdot g(m), & i \bmod p_j = 0 \quad (\text{指数}+1\text{，替换等比和})
\end{cases}
$$

### 2.3 线性时间证明

每个合数仅被它的**最小质因子**筛掉一次：当内层循环遇到 $i \bmod p_j = 0$ 时立即 `break`，保证 $i \times p_k$（$k > j$）不会在此时被标记——因为它们的最小质因子是 $p_j$ 而不是 $p_k$，会在 $i' = i \times p_k / p_j$ 时由 $p_j$ 筛掉。

因此总操作次数恰好等于合数个数加上质数个数，总共 $O(n)$。

### 2.4 辅助数组 g 的作用

$g[\,]$ 数组记录每个数的最小质因子对应的等比数列和 $1 + p + p^2 + \cdots + p^k$（$k$ 为该质因子在数中的指数），是情况二递推的关键。

在线性筛中，$p_j$ 总是 $i \times p_j$ 的最小质因子（因为质数表按升序遍历），所以：

- 当 $p_j$ 不整除 $i$ 时，$p_j$ 是新质因子，指数为 1，$g(m) = 1 + p_j$
- 当 $p_j$ 整除 $i$ 时，$p_j$ 是 $i$ 的最小质因子，$i \times p_j$ 中 $p_j$ 的指数 +1，等比数列和从 $g(i)$ 扩展为 $p_j \cdot g(i) + 1$

没有 $g[\,]$ 数组，就无法在情况二中正确递推——因为需要知道"旧等比和"才能算"新等比和"，进而更新 $\sigma$ 值。

### 2.5 f 和 g 的关系

对于任意 $n$，设其最小质因子为 $p$，指数为 $k$，则 $n = p^k \times t$（$p \nmid t$）：

$$g(n) = 1 + p + p^2 + \cdots + p^k$$

$$\sigma(n) = g(n) \times \sigma(t)$$

当 $n$ 是质数时，$k = 1$，$t = 1$，$g(p) = 1 + p$，$\sigma(p) = (1+p) \times \sigma(1) = 1 + p$，两者相等。

当 $n = 1$ 时，$g(1) = \sigma(1) = 1$，也是一致的。

## 三、逐行代码解析

```cpp
#include<bits/stdc++.h>
using namespace std;
vector<int> prime;
vector<long long> g, f;
```

全局变量：`prime` 存质数表，`g` 存最小质因子等比数列和，`f` 存约数和。注意 `g` 和 `f` 用 `long long`，因为约数和可能很大。

### 3.1 f_sieve —— 线性筛求约数和

```cpp
void f_sieve(int n)
{
    prime.clear();
    prime.reserve(n / 15 + 16);
    g.assign(n + 1, 0);
    f.assign(n + 1, 0);
    f[1] = g[1] = 1;
    vector<bool> vis(n + 1, false);
    for (int i = 2; i <= n; i++)
    {
        if (!vis[i])
        {
            g[i] = f[i] = i + 1;
            prime.emplace_back(i);
        }
        for (int j = 0; prime[j] <= n / i; j++)
        {
            long long m = prime[j] * i;
            vis[m] = true;
            if (i % prime[j])
            {
                g[m] = 1 + prime[j];
                f[m] = f[i] * g[m];
            }
            else
            {
                g[m] = g[i] * prime[j] + 1;
                f[m] = f[i] / g[i] * g[m];
                break;
            }
        }
    }
}
```

| 行 | 代码 | 解析 |
|----|------|------|
| 1 | `prime.clear(); prime.reserve(n / 15 + 16);` | 清空质数表，预留空间（质数密度约 $n / \ln n$） |
| 2-3 | `g.assign(n + 1, 0); f.assign(n + 1, 0);` | 辅助数组和约数和数组初始化为 0 |
| 4 | `f[1] = g[1] = 1;` | 边界 $\sigma(1) = g(1) = 1$ |
| 5 | `vector<bool> vis(n + 1, false);` | 标记数组，初始全 false（未被筛） |
| 6 | `for (int i = 2; i <= n; i++)` | 外层遍历每个数 |
| 7-10 | `if (!vis[i]) { g[i] = f[i] = i + 1; prime.emplace_back(i); }` | $i$ 未被筛 → 质数，$g(p) = f(p) = 1 + p$，加入质数表 |
| 11 | `for (int j = 0; prime[j] <= n / i; j++)` | 内层遍历质数，`n/i` 防止越界 |
| 12-13 | `long long m = prime[j] * i; vis[m] = true;` | 计算 $m = i \times p_j$ 并标记为合数（用 `long long` 防溢出） |
| 14-17 | `if (i % prime[j]) { g[m] = 1 + prime[j]; f[m] = f[i] * g[m]; }` | 情况一：$p_j$ 不整除 $i$ → 新质因子，$g(m) = 1+p$，$\sigma(m) = \sigma(i) \cdot (1+p)$ |
| 18-21 | `else { g[m] = g[i]*p+1; f[m] = f[i]/g[i]*g[m]; break; }` | 情况二：$p_j$ 整除 $i$ → 扩展等比和 $g(m) = p \cdot g(i) + 1$，$\sigma(m) = \sigma(i)/g(i) \cdot g(m)$；`break` 保证线性 |

### 3.2 手动模拟（n = 12）

| $i$ | 操作 | 新增质数 | $g$ 值更新 | $f$ 值更新 |
|-----|------|---------|-----------|-----------|
| 2 | `vis[2]=false` → 质数 | 2 | $g(2)=3$ | $f(2)=3$ |
| 2 | `j=0, p=2, 2≤6` | — | $g(4)=g(2) \times 2+1=7$ | $f(4)=f(2)/g(2) \times 7 = 7$（2%2=0, break） |
| 3 | `vis[3]=false` → 质数 | 3 | $g(3)=4$ | $f(3)=4$ |
| 3 | `j=0, p=2, 2≤4` | — | $g(6)=1+2=3$ | $f(6)=f(3) \times 3 = 12$（3%2≠0） |
| 3 | `j=1, p=3, 3≤4` | — | $g(9)=g(3) \times 3+1=13$ | $f(9)=f(3)/g(3) \times 13 = 13$（3%3=0, break） |
| 4 | `vis[4]=true` → 非质数 | — | — | — |
| 4 | `j=0, p=2, 2≤3` | — | $g(8)=g(4) \times 2+1=15$ | $f(8)=f(4)/g(4) \times 15 = 15$（4%2=0, break） |
| 5 | `vis[5]=false` → 质数 | 5 | $g(5)=6$ | $f(5)=6$ |
| 5 | `j=0, p=2, 2≤2` | — | $g(10)=1+2=3$ | $f(10)=f(5) \times 3 = 18$（5%2≠0） |
| 5 | `j=1, p=3, 3≤2` | — | — | （3 > 12/5=2，内层结束） |
| 6 | `vis[6]=true` → 非质数 | — | — | — |
| 6 | `j=0, p=2, 2≤2` | — | $g(12)=g(6) \times 2+1=7$ | $f(12)=f(6)/g(6) \times 7 = 28$（6%2=0, break） |

**验证**：
- $\sigma(4) = 1+2+4 = 7$ ✓
- $\sigma(6) = 1+2+3+6 = 12$ ✓
- $\sigma(8) = 1+2+4+8 = 15$ ✓
- $\sigma(9) = 1+3+9 = 13$ ✓
- $\sigma(10) = 1+2+5+10 = 18$ ✓
- $\sigma(12) = 1+2+3+4+6+12 = 28$ ✓

全部正确 ✓

### 3.3 main —— 预处理 + 多组查询

```cpp
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    int n, q;
    cin >> n >> q;
    f_sieve(n);
    for (int i = 1; i <= q; i++)
    {
        int x;
        cin >> x;
        cout << f[x] << '\n';
    }
    return 0;
}
```

| 行 | 代码 | 解析 |
|----|------|------|
| 1-2 | `int n, q; cin >> n >> q;` | 读入范围 $n$ 和查询次数 $q$ |
| 3 | `f_sieve(n);` | 一次预处理，求出所有 $\sigma(1..n)$ |
| 4-7 | `for (int i = 1; i <= q; i++) { cin >> x; cout << f[x]; }` | 每次查询 $O(1)$ |

### 3.4 运行示例

输入：
```
12 4
4
6
9
12
```

输出：
```
7
12
13
28
```

## 四、复杂度分析

| 维度 | 复杂度 | 说明 |
|------|--------|------|
| 预处理时间 | $O(n)$ | 每个合数被最小质因子恰好筛一次 |
| 单次查询 | $O(1)$ | 数组下标访问 |
| 空间 | $O(n)$ | `vis` + `g` + `f` + `prime`（质数约 $n / \ln n$ 个） |

**与单次试除法对比**：

| 方法 | 预处理 | 单次查询 | $q=10^6, n=10^7$ 总耗时 |
|------|--------|---------|------------------------|
| 单次试除 | 无 | $O(\sqrt{n})$ | $10^6 \times 3162 \approx 3.2 \times 10^9$（超时） |
| 线性筛 | $O(n)$ | $O(1)$ | $10^7 + 10^6 \approx 1.1 \times 10^7$（瞬间） |

## 五、适用场景

| 场景 | 说明 |
|------|------|
| **大约数和查询** | 预处理后每次 $O(1)$，适合百万级查询 |
| **约数相关计数** | 如 $\sum_{i=1}^{n} \sigma(i)$、亲和数判定等问题的预处理步骤 |
| **积性函数筛法进阶** | $\sigma$ 需要额外维护等比数列和 $g$，是线性筛积性函数中最复杂的经典案例 |
| **配合其他积性函数** | 线性筛可同时求 $\sigma$、$d$、$\varphi$、$\mu$ 等，一次筛法多个产出 |

## 六、常见陷阱与注意事项

### 6.1 g 数组的含义

`g[i]` 存的是 $i$ 的**最小质因子**对应的等比数列和 $1 + p + p^2 + \cdots + p^k$，不是任意质因子的等比和，也不是约数和本身。对于质数 $p$，$g(p) = 1 + p = \sigma(p)$，两者恰好相等，这容易造成误解。但对于质数幂 $p^k$（$k \ge 2$），$g(p^k) = 1 + p + \cdots + p^k \ne \sigma(p^k) = 1 + p + \cdots + p^k$...实际上两者相等，因为 $p^k$ 只有一个质因子。真正的区别体现在有多个质因子时。

### 6.2 先除后乘防溢出

`f[m] = f[i] / g[i] * g[m]` 必须先除后乘。因为 $g(i) = 1 + p + \cdots + p^k$ 整除 $\sigma(i) = g(i) \times \sigma(t)$，所以 $\sigma(i) / g(i) = \sigma(t)$ 是整数。先除得到整数 $\sigma(t)$，再乘 $g(m)$ 不溢出。若写成 `f[i] * g[m] / g[i]`，中间值 `f[i] * g[m]` 可能溢出 `long long`。

### 6.3 vis 数组不能省去

与线性筛求欧拉函数不同，本代码**不能**用 `!f[i]` 代替 `vis[i]` 判质数。因为 $f(1) = 1$ 非零，质数的 $f(p) = 1 + p$ 也非零，合数的 $f$ 值也非零——$f$ 数组中没有值为 0 的元素可用于标记"未访问"。因此必须额外维护 `vis` 数组。

### 6.4 long long 的必要性

约数和 $\sigma(n)$ 可能远大于 $n$。例如 $\sigma(2520) = \sigma(2^3 \times 3^2 \times 5 \times 7) = 15 \times 13 \times 6 \times 8 = 9360$，是 $n$ 的 3.7 倍。当 $n = 10^6$ 时，$\sigma$ 值可能超过 `int` 上限。因此 `g` 和 `f` 数组必须用 `long long`。

### 6.5 m 的类型

`long long m = prime[j] * i;` 中 `m` 用 `long long` 而非 `int`。虽然 `prime[j] * i <= n` 不会溢出 `int`，但后续 `g[m]` 和 `f[m]` 的下标访问需要正确的下标值，使用 `long long` 更安全。

### 6.6 break 的位置

必须在 `i % prime[j] == 0` 时立即 `break`，否则合数会被多个质因子重复标记，复杂度退化为 $O(n \log \log n)$。`break` 是保证线性时间的关键。

### 6.7 质数 f 和 g 值的赋值位置

$g(p) = f(p) = 1 + p$ 在发现质数时赋值（外层 `if (!vis[i])` 分支），而不是在内层循环中。因为质数只会在外层被识别，不会作为合数 $i \times p_j$ 被标记。

## 七、对比与扩展

### 7.1 三种线性筛积性函数对比

| 维度 | 求 $\varphi$ | 求 $d$ | 求 $\sigma$ |
|------|-------------|--------|------------|
| 辅助数组 | 无 | $a[\,]$（最小质因子指数） | $g[\,]$（最小质因子等比和） |
| 质数标记 | `!phi[i]` 兼作标记 | 必须额外 `vis[]` | 必须额外 `vis[]` |
| 情况一递推 | $(p-1) \cdot \varphi(i)$ | $2 \cdot d(i)$ | $(1+p) \cdot \sigma(i)$ |
| 情况二递推 | $p \cdot \varphi(i)$ | $d(i)/a(m) \cdot (a(m)+1)$ | $\sigma(i)/g(i) \cdot g(m)$ |
| 辅助数组更新 | 无 | $a(m) = a(i)+1$ | $g(m) = g(i) \cdot p + 1$ |
| 数据类型 | `int` 足够 | `int` 足够 | 必须 `long long` |
| 递推复杂度 | 直接乘法 | 先除后乘 | 先除后乘 |

求 $\sigma$ 是三种中最复杂的：需要维护等比数列和 $g$，且结果可能溢出 `int`。但核心思想一致——利用积性，对最小质因子"新出现"和"重复出现"分两种情况递推。

### 7.2 等比数列和 g 的递推公式

$g$ 的递推 $g(m) = g(i) \times p + 1$ 来自等比数列的扩展：

$$g(i) = 1 + p + p^2 + \cdots + p^k$$

$$g(m) = 1 + p + p^2 + \cdots + p^{k+1} = p \times (1 + p + \cdots + p^k) + 1 = p \times g(i) + 1$$

这个递推避免了直接计算 $p^{k+1}$，只需一次乘法和一次加法。

### 7.3 用线性筛求其他积性函数

线性筛的框架是通用的，只要知道函数在"最小质因子新出现"和"最小质因子重复出现"两种情况下的递推关系，就能在线性时间内求出。常见的：

| 函数 | $i \bmod p \neq 0$ | $i \bmod p = 0$ | 额外信息 |
|------|---------------------|------------------|---------|
| 欧拉函数 $\varphi$ | $(p-1) \cdot \varphi(i)$ | $p \cdot \varphi(i)$ | 无 |
| 约数个数 $d$ | $2 \cdot d(i)$ | $d(i) / a(m) \cdot (a(m)+1)$ | 最小质因子指数 $a$ |
| 约数和 $\sigma$ | $(1+p) \cdot \sigma(i)$ | $\sigma(i)/g(i) \cdot g(m)$ | 最小质因子等比和 $g$ |
| 莫比乌斯函数 $\mu$ | $-\mu(i)$ | $0$ | 无 |

### 7.4 三种筛法的统一框架

三个线性筛知识点（求 $\varphi$、求 $d$、求 $\sigma$）共享相同的外层循环和内层 `break` 机制，区别仅在于：

1. **质数时的赋值**：$\varphi(p) = p-1$，$d(p) = 2$，$\sigma(p) = 1+p$
2. **情况一的递推**：分别用 $(p-1)$、$2$、$(1+p)$ 乘
3. **情况二的递推**：$\varphi$ 直接乘 $p$；$d$ 需要辅助数组 $a$；$\sigma$ 需要辅助数组 $g$
4. **辅助数组的更新**：$a(m) = a(i)+1$（指数+1），$g(m) = g(i) \cdot p + 1$（等比和扩展）

理解了这套统一框架，就能举一反三地用线性筛求任意积性函数。

### 7.5 一句话总结

线性筛求约数和是线性筛积性函数中最复杂的经典案例：在 $O(n)$ 时间内求出 $[1, n]$ 内所有 $\sigma(i)$。核心在于额外维护最小质因子等比数列和数组 $g$，对每个合数 $i \cdot p_j$ 分两种情况递推——$p_j$ 不整除 $i$ 时 $\sigma(ip) = (1+p) \cdot \sigma(i)$，整除时 $g(ip) = p \cdot g(i) + 1$，$\sigma(ip) = \sigma(i)/g(i) \cdot g(ip)$。与求 $d$ 类似，求 $\sigma$ 也无法用 $f$ 数组本身判质数，必须额外维护 `vis` 标记数组，且 $g$ 和 $f$ 必须用 `long long` 防溢出。
