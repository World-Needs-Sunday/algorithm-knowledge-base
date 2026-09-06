---
id: math-linear-sieve-phi
title: '线性筛求欧拉函数（欧拉筛批量预处理）'
category: '数学'
subcategory: '数论基础'
difficulty: '进阶'
tags: ['数学', '数论', '欧拉函数', '线性筛', '欧拉筛', '积性函数', '预处理']
prerequisites: ['math-euler-phi', 'math-sieve-eratosthenes']
codePath: '数学\数论基础\线性筛求欧拉函数\源.cpp'
description: '在线性筛（欧拉筛）框架上同时计算欧拉函数 φ(n)：每个合数仅被其最小质因子筛一次，利用 φ 的积性分两种情况递推——当 prime[j] 整除 i 时 φ(i*p) = p*φ(i)，不整除时 φ(i*p) = (p-1)*φ(i)。复杂度 O(n)，适合需要批量查询 φ 值的场景。'
---

# 线性筛求欧拉函数（欧拉筛批量预处理）

## 一、算法原理

### 1.1 问题背景

当需要查询 $[1, n]$ 内大量数的欧拉函数值（如 $q = 10^6$ 次查询，每个数 $\le 10^7$）时，单次 $O(\sqrt{n})$ 的试除法会超时。

**思路转变**：利用欧拉函数的积性，结合线性筛（欧拉筛）框架，在 $O(n)$ 时间内一次性求出 $[1, n]$ 内所有 $\varphi(i)$，之后每次查询 $O(1)$。

### 1.2 线性筛回顾

线性筛（欧拉筛）是埃氏筛的优化版本，**每个合数只被它的最小质因子筛掉一次**，因此时间复杂度严格 $O(n)$。

核心机制：

- 维护质数表 `prime`，按从小到大顺序存储已发现的质数
- 对每个 $i$ 从 2 到 $n$，若 $i$ 未被标记，则 $i$ 是质数，加入 `prime`
- 对每个质数 $p_j$，标记 $i \times p_j$ 为合数；当 $i \bmod p_j = 0$ 时 `break`——因为 $p_j$ 是 $i$ 的最小质因子，再往后的 $p_k > p_j$ 会导致 $i \times p_k$ 的最小质因子不是 $p_k$，会被重复标记

### 1.3 φ 的两种递推情况

欧拉函数是积性函数：当 $\gcd(a, b) = 1$ 时，$\varphi(ab) = \varphi(a)\varphi(b)$。

在线性筛中，每个合数 $m = i \times p_j$（$p_j$ 是 $m$ 的最小质因子）被标记时，分两种情况计算 $\varphi(m)$：

**情况一：$i \bmod p_j \neq 0$（$p_j$ 不整除 $i$）**

此时 $\gcd(i, p_j) = 1$（因为 $p_j$ 是质数且不整除 $i$），由积性得：

$$\varphi(i \times p_j) = \varphi(i) \times \varphi(p_j) = \varphi(i) \times (p_j - 1)$$

**情况二：$i \bmod p_j = 0$（$p_j$ 整除 $i$）**

此时 $p_j$ 已经是 $i$ 的质因子，所以 $i \times p_j$ 与 $i$ 的**质因子集合完全相同**（只是 $p_j$ 的指数加了 1）。

设 $i = p_j^k \times t$（$\gcd(t, p_j) = 1$），则 $\varphi(i) = \varphi(p_j^k) \times \varphi(t) = p_j^k \cdot \frac{p_j-1}{p_j} \times \varphi(t)$。

而 $\varphi(i \times p_j) = \varphi(p_j^{k+1} \times t) = \varphi(p_j^{k+1}) \times \varphi(t) = p_j^{k+1} \cdot \frac{p_j-1}{p_j} \times \varphi(t) = p_j \times \left(p_j^k \cdot \frac{p_j-1}{p_j} \times \varphi(t)\right) = p_j \times \varphi(i)$。

即：

$$\varphi(i \times p_j) = p_j \times \varphi(i)$$

> **直觉理解**：$i$ 和 $ip$ 的不同质因子集合完全一样，所以 $\varphi(ip) = ip \cdot \prod \frac{q-1}{q} = p \cdot \left(i \cdot \prod \frac{q-1}{q}\right) = p \cdot \varphi(i)$。

### 1.4 边界与初始值

- $\varphi(1) = 1$（1 与自身互质）
- 质数 $p$ 的 $\varphi(p) = p - 1$（质数与 $[1, p-1]$ 全部互质）

这两个值分别在初始化和发现质数时直接赋值，作为递推的起点。

## 二、核心公式/状态定义与转移方程

### 2.1 状态定义

| 数组 | 含义 |
|------|------|
| `prime_vis[i]` | $i$ 是否可能为质数（未被筛掉为 `true`） |
| `prime` | 已发现的质数表，按升序存储 |
| `phi[i]` | 欧拉函数值 $\varphi(i)$ |

### 2.2 转移方程

对每个 $i$（从 2 到 $n$），对每个质数 $p_j$：

$$
\varphi(i \cdot p_j) =
\begin{cases}
(p_j - 1) \cdot \varphi(i), & i \bmod p_j \neq 0 \quad (\text{互质，直接用积性}) \\
p_j \cdot \varphi(i), & i \bmod p_j = 0 \quad (\text{不互质，质因子集合不变})
\end{cases}
$$

### 2.3 线性时间证明

每个合数仅被它的**最小质因子**筛掉一次：当内层循环遇到 $i \bmod p_j = 0$ 时立即 `break`，保证 $i \times p_k$（$k > j$）不会在此时被标记——因为它们的最小质因子是 $p_j$ 而不是 $p_k$，会在 $i' = i \times p_k / p_j$ 时由 $p_j$ 筛掉。

因此总操作次数恰好等于合数个数，加上质数个数，总共 $n$ 次级别，即 $O(n)$。

### 2.4 附带功能

线性筛一次运行可以同时得到：
- **质数表** `prime`：所有 $\le n$ 的质数
- **质数判定** `prime_vis[i]`：$O(1)$ 判断 $i$ 是否为质数
- **欧拉函数** `phi[i]`：所有 $\varphi(i)$ 的值
- **最小质因子**（可扩展）：只需多开一个数组记录每个合数被哪个质数筛掉

这种"一次筛法、多个产出"的特性是线性筛最强大的地方。

## 三、逐行代码解析

```cpp
#include<bits/stdc++.h>
using namespace std;
vector<int> prime, phi;
```

全局变量：`prime` 存质数表，`phi` 存欧拉函数值。

### 3.1 phi_sieve —— 线性筛求欧拉函数

```cpp
void phi_sieve(int n)
{
    vector<bool> prime_vis(n + 1, true);
    prime.clear();
    prime.reserve(n + 1);
    phi.assign(n + 1, 0);
    phi[1] = 1;
    for (int i = 2; i <= n; i++)
    {
        if (prime_vis[i])
        {
            prime.emplace_back(i);
            phi[i] = i - 1;
        }
        for (int j = 0; prime[j] <= n / i; j++)
        {
            prime_vis[i * prime[j]] = false;
            if (i % prime[j] == 0)
            {
                phi[i * prime[j]] = prime[j] * phi[i];
                break;
            }
            else phi[i * prime[j]] = (prime[j] - 1) * phi[i];
        }
    }
    prime.shrink_to_fit();
}
```

| 行 | 代码 | 解析 |
|----|------|------|
| 1 | `vector<bool> prime_vis(n + 1, true);` | 标记数组，初始全为 true（未被筛） |
| 2-3 | `prime.clear(); prime.reserve(n + 1);` | 清空质数表，预留空间避免多次扩容 |
| 4-5 | `phi.assign(n + 1, 0); phi[1] = 1;` | φ 数组初始化为 0，边界 φ(1) = 1 |
| 6 | `for (int i = 2; i <= n; i++)` | 外层遍历每个数 |
| 7-10 | `if (prime_vis[i]) { prime.push_back(i); phi[i] = i - 1; }` | $i$ 是质数 → 加入质数表，$\varphi(i) = i-1$ |
| 11 | `for (int j = 0; prime[j] <= n / i; j++)` | 内层遍历质数，用 `n/i` 防止 `i*prime[j]` 越界 |
| 12 | `prime_vis[i * prime[j]] = false;` | 标记 $i \times p_j$ 为合数 |
| 13-16 | `if (i % prime[j] == 0) { phi[...] = p * phi[i]; break; }` | 情况二：$p_j$ 整除 $i$ → φ 乘 $p$；然后 break（保证线性） |
| 17 | `else phi[...] = (p - 1) * phi[i];` | 情况一：$p_j$ 不整除 $i$ → 积性，φ 乘 $(p-1)$ |
| 18 | `prime.shrink_to_fit();` | 释放质数表多余空间（可选优化） |

### 3.2 手动模拟（n = 12）

| $i$ | 操作 | 新增质数 | 标记的合数 | φ 值更新 |
|-----|------|---------|-----------|---------|
| 2 | `prime_vis[2]=true` → 质数 | 2 | — | $\varphi(2)=1$ |
| 2 | `j=0, prime[0]=2, 2<=6` | — | 4 | $\varphi(4) = 2 \times \varphi(2) = 2$（2%2=0, break） |
| 3 | `prime_vis[3]=true` → 质数 | 3 | — | $\varphi(3)=2$ |
| 3 | `j=0, prime[0]=2, 2<=4` | — | 6 | $\varphi(6) = (2-1) \times \varphi(3) = 2$（3%2≠0） |
| 3 | `j=1, prime[1]=3, 3<=4` | — | 9 | $\varphi(9) = 3 \times \varphi(3) = 6$（3%3=0, break） |
| 4 | `prime_vis[4]=false` | — | — | — |
| 4 | `j=0, prime[0]=2, 2<=3` | — | 8 | $\varphi(8) = 2 \times \varphi(4) = 4$（4%2=0, break） |
| 5 | `prime_vis[5]=true` → 质数 | 5 | — | $\varphi(5)=4$ |
| 5 | `j=0, prime[0]=2, 2<=2` | — | 10 | $\varphi(10) = (2-1) \times \varphi(5) = 4$（5%2≠0） |
| 5 | `j=1, prime[1]=3, 3<=2` | — | — | （3 > 12/5=2，内层结束） |
| 6 | `prime_vis[6]=false` | — | — | — |
| 6 | `j=0, prime[0]=2, 2<=2` | — | 12 | $\varphi(12) = 2 \times \varphi(6) = 4$（6%2=0, break） |

**验证**：
- $\varphi(4) = 2$ ✓ {1, 3}
- $\varphi(6) = 2$ ✓ {1, 5}
- $\varphi(9) = 6$ ✓ {1, 2, 4, 5, 7, 8}
- $\varphi(8) = 4$ ✓ {1, 3, 5, 7}
- $\varphi(10) = 4$ ✓ {1, 3, 7, 9}
- $\varphi(12) = 4$ ✓ {1, 5, 7, 11}

全部正确 ✓

### 3.3 main —— 预处理 + 多组查询

```cpp
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    int n, q;
    cin >> n >> q;
    phi_sieve(n);
    while (q--)
    {
        int n;
        cin >> n;
        if (phi[n] == n - 1) cout << n << "为质数\n";
        else cout << n << "为合数\n";
        cout << n << "的欧拉函数的值为: " << phi[n];
    }
    return 0;
}
```

| 行 | 代码 | 解析 |
|----|------|------|
| 1-2 | `int n, q; cin >> n >> q;` | 读入范围 $n$ 和查询次数 $q$ |
| 3 | `phi_sieve(n);` | 一次预处理，求出所有 $\varphi(1..n)$ |
| 4-7 | `while (q--) { ... }` | 每次查询 $O(1)$，同时判定质数 + 输出 φ 值 |

### 3.4 运行示例

输入：
```
12 3
4
7
12
```

输出：
```
4为合数
4的欧拉函数的值为: 2
7为质数
7的欧拉函数的值为: 6
12为合数
12的欧拉函数的值为: 4
```

## 四、复杂度分析

| 维度 | 复杂度 | 说明 |
|------|--------|------|
| 预处理时间 | $O(n)$ | 每个合数被最小质因子恰好筛一次 |
| 单次查询 | $O(1)$ | 数组下标访问 |
| 空间 | $O(n)$ | `prime_vis` + `phi` + `prime`（质数约 $n / \ln n$ 个） |

**与单次求值对比**：

| 方法 | 预处理 | 单次查询 | $q=10^6, n=10^7$ 总耗时 |
|------|--------|---------|------------------------|
| 单次试除 | 无 | $O(\sqrt{n})$ | $10^6 \times 3162 \approx 3.2 \times 10^9$（超时） |
| 线性筛 | $O(n)$ | $O(1)$ | $10^7 + 10^6 \approx 1.1 \times 10^7$（瞬间） |

当查询量大时，线性筛批量预处理碾压单次求值。

## 五、适用场景

| 场景 | 说明 |
|------|------|
| **大量 φ 值查询** | 预处理后每次 $O(1)$，适合百万级查询 |
| **数论计数** | 很多计数问题需要枚举 φ 值或利用 φ 的求和公式 |
| **同时需要质数表和 φ** | 一次筛法同时得到，效率最高 |
| **莫比乌斯函数/其他积性函数** | 同样的线性筛框架，换递推公式即可 |
| **最小质因子查询** | 略加修改可同时记录每个数的最小质因子 |

## 六、常见陷阱与注意事项

### 6.1 内层循环的终止条件

`prime[j] <= n / i` 而非 `i * prime[j] <= n`。前者用除法避免了乘法溢出，同时保证 `i * prime[j]` 不会越界。若写 `i * prime[j] <= n`，当 $n$ 接近 `int` 上限时可能溢出。

### 6.2 break 的位置

必须在 `i % prime[j] == 0` 时立即 `break`，否则合数会被多个质因子重复标记，复杂度退化为 $O(n \log \log n)$（退化为埃氏筛）。`break` 是保证线性时间的关键。

### 6.3 phi[1] 的初始化

$\varphi(1) = 1$ 必须手动赋值，因为循环从 $i=2$ 开始，1 不会被处理。若遗漏，所有包含因子 1 的积性递推都会出错（不过本题中 1 不参与递推，影响较小，但仍是标准做法）。

### 6.4 质数 φ 值的赋值位置

$\varphi(p) = p - 1$ 在发现质数时赋值，而不是在内层循环中。因为质数 $p$ 只会在外层 `if (prime_vis[i])` 分支中被识别，不会作为合数 $i \times p_j$ 被标记。

### 6.5 数据类型选择

当 $n$ 较大时（如 $n \ge 10^6$），$\varphi(n)$ 可能接近 $n$，`int` 可能不够。建议用 `long long` 存储 `phi` 数组。质数表 `prime` 用 `int` 通常没问题（$10^7$ 以内质数约 66 万个，存下标完全够）。

### 6.6 vector\<bool\> 的位压缩

`prime_vis` 用 `vector<bool>` 位压缩节省内存，但访问速度略慢。若卡常可改用 `vector<char>`。

### 6.7 全局变量 vs 局部变量

代码中 `prime` 和 `phi` 是全局变量。好处是 `phi_sieve` 只需一个参数 $n$；坏处是多组测试数据间可能冲突。竞赛中常用全局变量（方便调试），但应注意在每组测试前调用 `clear()`。

## 七、对比与扩展

### 7.1 埃氏筛 vs 线性筛求 φ

| 维度 | 埃氏筛 + φ | 线性筛求 φ |
|------|-----------|-----------|
| 时间复杂度 | $O(n \log \log n)$ | $O(n)$ |
| 实现难度 | 简单（埃氏筛框架 + 每质数更新 φ） | 中等（需理解两种递推情况 + break 机制） |
| 附带产物 | 质数判定 | 质数表 + 质数判定 + 可扩展最小质因子 |
| 实际速度 | $n \le 10^7$ 时相近 | $n > 10^7$ 时更快 |
| 空间 | 少（只有 prime_vis + phi） | 多一个质数表（约 $n / \ln n$ 个元素） |

### 7.2 埃氏筛求 φ 的写法

埃氏筛也能求 φ，思路是初始化 `phi[i] = i`，然后对每个质数 $p$，遍历 $p$ 的倍数 $j$，执行 `phi[j] = phi[j] / p * (p - 1)`。每个质数只更新一次倍数，因此不同质因子各乘一次 $\frac{p-1}{p}$，结果正确。

```cpp
vector<int> phi(n + 1);
for (int i = 1; i <= n; i++) phi[i] = i;
for (int i = 2; i <= n; i++)
    if (phi[i] == i)  // i 是质数
        for (int j = i; j <= n; j += i)
            phi[j] = phi[j] / i * (i - 1);
```

### 7.3 用线性筛求其他积性函数

线性筛的框架是通用的，只要知道函数在"最小质因子新出现"和"最小质因子重复出现"两种情况下的递推关系，就能在线性时间内求出。常见的：

| 函数 | $i \bmod p \neq 0$ | $i \bmod p = 0$ |
|------|---------------------|------------------|
| 欧拉函数 $\varphi$ | $(p-1) \cdot \varphi(i)$ | $p \cdot \varphi(i)$ |
| 莫比乌斯函数 $\mu$ | $-\mu(i)$ | $0$ |
| 约数个数 $d$ | $2 \cdot d(i)$ | 需额外记录最小质因子的指数 |
| 约数和 $\sigma$ | $(1+p) \cdot \sigma(i)$ | 需额外记录等比数列信息 |
| 最小质因子 | $p$ | $p$（直接赋值） |

### 7.4 线性筛求最小质因子

略加修改，把 `prime_vis` 替换为 `min_prime_factor` 数组，在标记合数时记录最小质因子：

```cpp
vector<int> mpf(n + 1, 0);  // 最小质因子
...
mpf[i * prime[j]] = prime[j];
```

有了最小质因子，可以快速对任意数做质因数分解（每次除以最小质因子）。

### 7.5 一句话总结

线性筛求欧拉函数是"一次筛法、多个产出"的典范：在 $O(n)$ 时间内同时得到质数表、质数判定和所有 φ 值。核心在于利用 φ 的积性，对每个合数 $i \cdot p_j$ 分两种情况递推——$p_j$ 整除 $i$ 时 $\varphi(ip) = p \cdot \varphi(i)$，不整除时 $\varphi(ip) = (p-1) \cdot \varphi(i)$。配合 `break` 保证每个合数只被最小质因子筛一次，实现严格线性复杂度。当查询量大时，线性筛远优于单次试除求值。
