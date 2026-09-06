---
id: math-euler-phi
title: '欧拉函数（单次求值）'
category: '数学'
subcategory: '数论基础'
difficulty: '入门'
tags: ['数学', '数论', '欧拉函数', '积性函数', '质因数分解']
prerequisites: ['math-gcd-lcm', 'math-sieve-eratosthenes']
codePath: '数学\数论基础\欧拉函数\源.cpp'
description: '欧拉函数 φ(n) 表示 [1, n] 中与 n 互素的正整数个数。利用质因数分解，在 O(√n) 时间内单次求值。核心公式：φ(n) = n × ∏(1 - 1/pᵢ)，仅由 n 的不同质因子决定，与质因子的幂次无关。'
---

# 欧拉函数（单次求值）

## 一、算法原理

### 1.1 定义

欧拉函数 $\varphi(n)$ 表示 $[1, n]$ 中与 $n$ 互质（$\gcd(k, n) = 1$）的正整数个数。

**示例**：

| $n$ | 与 $n$ 互质的数 | $\varphi(n)$ |
|-----|----------------|-------------|
| 1 | {1} | 1 |
| 2 | {1} | 1 |
| 3 | {1, 2} | 2 |
| 4 | {1, 3} | 2 |
| 5 | {1, 2, 3, 4} | 4 |
| 6 | {1, 5} | 2 |
| 12 | {1, 5, 7, 11} | 4 |

### 1.2 三条基本性质

**性质 1**：若 $p$ 为质数，则 $\varphi(p) = p - 1$

质数 $p$ 与 $[1, p-1]$ 中的每个数都互质，共 $p - 1$ 个。

**性质 2**：若 $p$ 为质数，$k$ 为正整数，则 $\varphi(p^k) = (p-1) \cdot p^{k-1}$

在 $[1, p^k]$ 中，与 $p^k$ 不互质的数恰好是 $p$ 的倍数：$p, 2p, 3p, \ldots, p^{k-1} \cdot p$，共 $p^{k-1}$ 个。因此 $\varphi(p^k) = p^k - p^{k-1} = (p-1) \cdot p^{k-1}$。

**性质 3（积性）**：若 $\gcd(m, n) = 1$，则 $\varphi(m \cdot n) = \varphi(m) \cdot \varphi(n)$

这是欧拉函数最重要的性质——当两数互质时，欧拉函数可拆分为乘积。利用积性 + 质因数分解，就能把任意 $n$ 的 $\varphi(n)$ 归约到质数幂的情况。

### 1.3 计算公式推导

设 $n$ 的质因数分解为 $n = p_1^{\alpha_1} \cdot p_2^{\alpha_2} \cdots p_s^{\alpha_s}$，则：

$$\varphi(n) = \varphi(p_1^{\alpha_1}) \cdot \varphi(p_2^{\alpha_2}) \cdots \varphi(p_s^{\alpha_s})$$

（积性，因为不同质数的幂两两互质）

$$= \prod_{i=1}^{s} (p_i - 1) \cdot p_i^{\alpha_i - 1}$$

（性质 2）

$$= \prod_{i=1}^{s} p_i^{\alpha_i} \cdot \frac{p_i - 1}{p_i}$$

（提取 $p_i^{\alpha_i}$，把 $(p_i-1) \cdot p_i^{\alpha_i - 1}$ 写成 $p_i^{\alpha_i} \cdot \frac{p_i-1}{p_i}$）

$$= n \cdot \prod_{i=1}^{s} \frac{p_i - 1}{p_i}$$

（$\prod p_i^{\alpha_i} = n$ 合并）

$$= n \cdot \prod_{i=1}^{s} \left(1 - \frac{1}{p_i}\right)$$

**关键结论**：欧拉函数仅由 $n$ 和它的**不同质因子**决定，与各质因子的幂次 $\alpha_i$ 无关。

**验证**：$\varphi(12) = \varphi(2^2 \times 3) = 12 \times \frac{1}{2} \times \frac{2}{3} = 12 \times \frac{1}{3} = 4$ ✓

互质的数：{1, 5, 7, 11}，恰好 4 个 ✓

### 1.4 朴素法 vs 公式法

| 方法 | 思路 | 复杂度 |
|------|------|--------|
| 朴素枚举 | 遍历 $k = 1 \ldots n$，逐个检查 $\gcd(k, n) = 1$ | $O(n \log n)$ |
| 公式法 | 质因数分解 + 乘积公式 | $O(\sqrt{n})$ |

当 $n = 10^{12}$ 时，朴素法完全不可行，公式法只需约 $10^6$ 次操作。

## 二、核心公式/状态定义与转移方程

### 2.1 核心公式

$$\boxed{\varphi(n) = n \cdot \prod_{p \mid n} \left(1 - \frac{1}{p}\right) = n \cdot \prod_{p \mid n} \frac{p - 1}{p}}$$

其中 $p$ 遍历 $n$ 的所有**不同质因子**。

### 2.2 算法转移

对 $n$ 做质因数分解，每找到一个质因子 $p$，就执行：

$$\text{res} \leftarrow \text{res} / p \times (p - 1)$$

等价于 $\text{res} \leftarrow \text{res} \times \frac{p-1}{p}$。

**为什么先除后乘？** $\text{res}$ 初始为 $n$，而 $p \mid n$，所以 $\text{res} / p$ 必为整数。先除保证整除，再乘避免分数运算。若写成 `res * (p-1) / p`，中间值更大，可能溢出。

### 2.3 质数判定副产物

由性质 1，$\varphi(p) = p - 1$。反过来，若 $\varphi(n) = n - 1$，则 $n$ 是质数——因为只有质数才与 $[1, n-1]$ 中所有数都互质。本代码利用这一性质附带判定了质数。

> **注意**：这只是判质数的一种方式，效率（$O(\sqrt{n})$）与试除法相同，不如筛法预处理后 $O(1)$ 查询。

### 2.4 重要性质速查

| 性质 | 公式 | 条件 |
|------|------|------|
| 质数 | $\varphi(p) = p - 1$ | $p$ 为质数 |
| 质数幂 | $\varphi(p^k) = p^k - p^{k-1}$ | $p$ 为质数，$k \ge 1$ |
| 积性 | $\varphi(mn) = \varphi(m)\varphi(n)$ | $\gcd(m, n) = 1$ |
| 完全积性？ | ❌ 不成立 | $\gcd(m,n) > 1$ 时不保证 |
| 欧拉定理 | $a^{\varphi(n)} \equiv 1 \pmod{n}$ | $\gcd(a, n) = 1$ |
| 费马小定理 | $a^{p-1} \equiv 1 \pmod{p}$ | $p$ 为质数，$\gcd(a,p)=1$（欧拉定理的特例） |
| 求和公式 | $\sum_{d \mid n} \varphi(d) = n$ | 任意正整数 $n$ |

## 三、逐行代码解析

```cpp
#include<bits/stdc++.h>
using namespace std;
```

标准头文件与命名空间。

### 3.1 phi —— 欧拉函数单次求值

```cpp
int phi(int n)
{
    int res = n;
    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0) res = res / i * (i - 1);
        while (n % i == 0) n /= i;
    }
    if(n > 1) res = res / n * (n - 1);
    return res;
}
```

| 行 | 代码 | 解析 |
|----|------|------|
| 1 | `int phi(int n)` | 输入 $n$，返回 $\varphi(n)$ |
| 2 | `int res = n;` | 初始化 $\text{res} = n$，后续逐个质因子乘入 $\frac{p-1}{p}$ |
| 3 | `for (int i = 2; i * i <= n; i++)` | 试除法找质因子，外层到 $\sqrt{n}$，用 `i*i` 避免浮点 |
| 4 | `if (n % i == 0) res = res / i * (i - 1);` | $i$ 是质因子 → 乘入 $\frac{i-1}{i}$（先除后乘防溢出） |
| 5 | `while (n % i == 0) n /= i;` | 把 $n$ 中所有 $i$ 的因子除干净，保证下次找到的是**不同**质因子 |
| 6 | `if(n > 1) res = res / n * (n - 1);` | 循环结束后若 $n > 1$，说明残留了一个大于 $\sqrt{\text{原}n}$ 的质因子，补乘 |
| 7 | `return res;` | 返回最终结果 |

**执行示例**（$n = 12$）：

| 步骤 | $i$ | $n$（当前） | `i*i <= n`? | 动作 | res |
|------|-----|------------|------------|------|-----|
| 初始 | — | 12 | — | res = 12 | 12 |
| $i=2$ | 2 | 12 | 4 ≤ 12 ✓ | res = 12/2*1 = 6; n = 12→6→3 | 6 |
| $i=3$ | 3 | 3 | 9 ≤ 3 ✗ | 循环退出 | 6 |
| 收尾 | — | 3 | — | n>1 → res = 6/3*2 = 4 | 4 |

$\varphi(12) = 4$ ✓ 互质的数：{1, 5, 7, 11}

### 3.2 main —— 求值 + 质数判定

```cpp
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    int n,q;
    //注意: n >= 2
    cin >> n;
    q = phi(n);
    if (q == n - 1) cout << n << "为质数\n";
    else cout << n << "为合数\n";
    cout << n << "的欧拉函数的值为: " << q;
    return 0;
}
```

| 行 | 代码 | 解析 |
|----|------|------|
| 1-2 | `ios::sync_with_stdio(false); cin.tie(nullptr);` | 快速 IO |
| 3-4 | `int n, q; cin >> n;` | 读入 $n$（注释提醒 $n \ge 2$） |
| 5 | `q = phi(n);` | 计算欧拉函数 |
| 6-7 | `if (q == n - 1) ...` | 利用 $\varphi(n) = n-1 \iff n$ 为质数的性质判定质数 |
| 8 | `cout << ... << q;` | 输出欧拉函数值 |

### 3.3 运行示例

输入：
```
12
```

输出：
```
12为合数
12的欧拉函数的值为: 4
```

输入：
```
7
```

输出：
```
7为质数
7的欧拉函数的值为: 6
```

## 四、复杂度分析

| 维度 | 复杂度 | 说明 |
|------|--------|------|
| 时间 | $O(\sqrt{n})$ | 试除法质因数分解，外层循环到 $\sqrt{n}$ |
| 空间 | $O(1)$ | 仅用几个变量 |

**与朴素枚举法对比**：

| 方法 | 复杂度 | $n = 10^{12}$ |
|------|--------|--------------|
| 朴素枚举 | $O(n \log n)$ | $\approx 10^{13}$，不可行 |
| 公式法 | $O(\sqrt{n})$ | $\approx 10^6$，瞬间完成 |

## 五、适用场景

| 场景 | 说明 |
|------|------|
| **模逆元** | 由欧拉定理 $a^{\varphi(n)-1} \equiv a^{-1} \pmod{n}$（$\gcd(a,n)=1$ 时），需先求 $\varphi(n)$ |
| **RSA 加密** | 公钥密码系统的核心运算，需计算 $\varphi(n)$ 来生成密钥 |
| **质数判定** | $\varphi(n) = n - 1$ 当且仅当 $n$ 为质数 |
| **简化幂运算** | 计算 $a^k \bmod n$ 时，若 $\gcd(a,n)=1$，可把 $k$ 对 $\varphi(n)$ 取模来缩小指数 |
| **数论计数** | 欧拉函数在各种数论计数问题中作为中间量出现 |
| **分数化简** | 利用 $\sum_{d \mid n} \varphi(d) = n$ 统计分母为 $n$ 的约数的既约分数个数 |

## 六、常见陷阱与注意事项

### 6.1 n = 1 的边界

$\varphi(1) = 1$（1 与自身互质）。但本代码注释标注 $n \ge 2$，若输入 $n = 1$：循环不执行，`n > 1` 为 false，返回 `res = 1`。结果正确，但质数判定 `q == n - 1` → `1 == 0` → 判为合数，语义上 1 既不是质数也不是合数，需特判。

### 6.2 先除后乘防溢出

`res = res / i * (i - 1)` 必须先除后乘。因为 $i$ 是 $n$ 的质因子，而 `res` 初始为 $n$ 并在每步乘入 $\frac{p-1}{p}$，始终保持整除性。若写成 `res * (i-1) / i`，中间值 `res * (i-1)` 可能溢出 `int`。

### 6.3 循环中 n 被修改

`while (n % i == 0) n /= i;` 修改了 $n$ 的值，因此外层 `i * i <= n` 比较的是**当前剩余的 $n$**，不是原始 $n$。这是一个容易忽略的细节——当 $n$ 被除掉小质因子后，$\sqrt{n}$ 变小，循环可能提前结束，残留的大质因子由末尾的 `if (n > 1)` 处理。

### 6.4 int 溢出

当 $n$ 接近 `int` 上限（$\approx 2.1 \times 10^9$）时，`res`、`i * i` 可能溢出。建议参数和返回值用 `long long`：

```cpp
long long phi(long long n)
{
    long long res = n;
    for (long long i = 2; i * i <= n; i++)
    { ... }
    ...
}
```

### 6.5 欧拉函数不是完全积性的

$\varphi(mn) = \varphi(m)\varphi(n)$ **仅当** $\gcd(m, n) = 1$。若 $\gcd(m, n) > 1$，此式不成立。例如 $\varphi(2) \cdot \varphi(2) = 1 \neq \varphi(4) = 2$。

### 6.6 多次查询的效率

单次求值 $O(\sqrt{n})$ 适合少量查询。若需查询 $[1, n]$ 内所有 $\varphi$ 值，应改用线性筛批量预处理，复杂度 $O(n)$。

## 七、对比与扩展

### 7.1 单次求值 vs 筛法批量求值

| 维度 | 单次求值（本代码） | 线性筛批量预处理 |
|------|-------------------|-----------------|
| 单次复杂度 | $O(\sqrt{n})$ | $O(1)$（预处理后） |
| 预处理 | 无 | $O(n)$ |
| 适用场景 | 少量查询、$n$ 极大 | 大量查询、$n \le 10^7$ |
| 空间 | $O(1)$ | $O(n)$ |
| 额外功能 | 可附带判质数 | 可同时求 $\varphi$、$\mu$、最小质因子 |

### 7.2 欧拉定理与费马小定理

**欧拉定理**：若 $\gcd(a, n) = 1$，则 $a^{\varphi(n)} \equiv 1 \pmod{n}$。

**费马小定理**（欧拉定理的特例）：若 $p$ 为质数，$\gcd(a, p) = 1$，则 $a^{p-1} \equiv 1 \pmod{p}$（因为 $\varphi(p) = p - 1$）。

**应用**：求模逆元 $a^{-1} \equiv a^{\varphi(n)-1} \pmod{n}$；计算大幂 $a^k \bmod n$ 时把 $k$ 对 $\varphi(n)$ 取模。

### 7.3 线性筛求欧拉函数

利用积性和质因数分解，线性筛可在 $O(n)$ 内求出 $[1, n]$ 内所有 $\varphi$ 值：

- $\varphi(1) = 1$
- 若 $p$ 为质数：$\varphi(p) = p - 1$
- 若 $i \bmod p \neq 0$（$p$ 不整除 $i$，即 $\gcd(i, p) = 1$）：$\varphi(i \cdot p) = \varphi(i) \cdot \varphi(p) = \varphi(i) \cdot (p - 1)$
- 若 $i \bmod p = 0$（$p$ 整除 $i$，即 $p$ 已是 $i$ 的质因子）：$\varphi(i \cdot p) = \varphi(i) \cdot p$（因为 $ip$ 与 $i$ 的质因子集合相同，$\varphi(ip) = ip \cdot \prod \frac{q-1}{q} = p \cdot \left(i \cdot \prod \frac{q-1}{q}\right) = p \cdot \varphi(i)$）

### 7.4 欧拉函数求和公式

$$\sum_{d \mid n} \varphi(d) = n$$

**直觉理解**：考虑分数 $\frac{1}{n}, \frac{2}{n}, \ldots, \frac{n}{n}$，化简后分母为 $d$（$d \mid n$）的既约分数恰好有 $\varphi(d)$ 个，总数 $n$ 个。

### 7.5 一句话总结

欧拉函数 $\varphi(n)$ 统计 $[1, n]$ 中与 $n$ 互质的数的个数，核心公式 $\varphi(n) = n \cdot \prod_{p \mid n} \frac{p-1}{p}$ 仅由不同质因子决定、与幂次无关。单次求值用试除法质因数分解在 $O(\sqrt{n})$ 完成，先除后乘防溢出；批量求值用线性筛在 $O(n)$ 内完成全部预处理。欧拉定理 $a^{\varphi(n)} \equiv 1 \pmod{n}$ 是模逆元和大幂运算的理论基础。
