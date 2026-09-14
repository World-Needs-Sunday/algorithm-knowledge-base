---
id: math-crt
title: '中国剩余定理（CRT）'
category: '数学'
subcategory: '数论基础'
difficulty: '提高'
tags: ['数学', '数论', '中国剩余定理', 'CRT', '模逆元', '费马小定理', '同余方程组']
prerequisites: ['math-modular-inverse', 'math-gcd-lcm', 'math-quick-pow-basic']
codePath: '数学\数论基础\中国剩余定理\源.cpp'
description: '中国剩余定理（CRT）求解两两互质模数的同余方程组 x ≡ a_i (mod m_i)，解为 x ≡ Σ a_i · M_i · M_i^{-1} (mod M)，其中 M = Π m_i，M_i = M/m_i。本实现基于费马小定理求逆元，要求模数为质数。'
---

# 中国剩余定理（CRT）

> ⚠️ **代码质量警告：本实现大数容易溢出**
>
> 本代码使用 `long long` 存储模数乘积 $M = \prod r_i$，在计算过程中 `Mi * ti`、`x * Mi` 等中间乘法可能在取模前就已经溢出。
>
> **支持的数值范围**：
> - 当模数都是 int 范围内的小质数时（如 $r_i \le 10^9$），最多只能支持约 **2~3 个**模数，再多 $M$ 就会超过 `long long` 上限（约 $9 \times 10^{18}$）
> - 若模数都很小（如 $r_i \le 100$），最多支持约 **15 个**模数
> - 中间乘法 `ans * gap`、`gap * gap` 在模数接近 $10^9$ 时也可能溢出
>
> **改进建议**：使用 `__int128` 或龟速乘（快速乘）避免中间溢出；或使用扩展欧几里得法的 EXCRT 版本（不要求模数互质且更安全）。

## 一、算法原理

### 1.1 问题描述

给定 $n$ 个同余方程，模数两两互质：

$$
\begin{cases}
x \equiv a_1 \pmod{r_1} \\
x \equiv a_2 \pmod{r_2} \\
\quad \vdots \\
x \equiv a_n \pmod{r_n}
\end{cases}
$$

其中 $\gcd(r_i, r_j) = 1$（$i \ne j$），求 $x$ 在模 $M = \prod_{i=1}^{n} r_i$ 意义下的唯一解。

### 1.2 核心构造

令 $M = \prod_{i=1}^{n} r_i$，$M_i = \dfrac{M}{r_i}$（即去掉第 $i$ 个模数后的乘积）。

由于所有模数两两互质，$M_i$ 与 $r_i$ 互质，因此 $M_i$ 在模 $r_i$ 意义下存在逆元，记为 $t_i = M_i^{-1} \pmod{r_i}$。

则方程组的解为：

$$
x \equiv \sum_{i=1}^{n} a_i \cdot M_i \cdot t_i \pmod{M}
$$

### 1.3 正确性证明

对第 $k$ 个方程验证：

- 当 $i \ne k$ 时，$M_i$ 包含因子 $r_k$，所以 $M_i \equiv 0 \pmod{r_k}$，因此 $a_i \cdot M_i \cdot t_i \equiv 0 \pmod{r_k}$
- 当 $i = k$ 时，$M_k \cdot t_k \equiv M_k \cdot M_k^{-1} \equiv 1 \pmod{r_k}$，因此 $a_k \cdot M_k \cdot t_k \equiv a_k \pmod{r_k}$

所以总和满足 $x \equiv a_k \pmod{r_k}$，对所有 $k$ 成立。

### 1.4 逆元的求法

本代码使用**费马小定理**求逆元：因为 $r_i$ 是质数，所以 $t_i = M_i^{r_i - 2} \bmod r_i$。

也可以用扩展欧几里得算法求逆元，效率相同且不要求 $r_i$ 为质数（只要求互质）。

## 二、核心公式与状态定义

### 2.1 CRT 核心公式

$$
\boxed{x \equiv \sum_{i=1}^{n} a_i \cdot \frac{M}{r_i} \cdot \left(\frac{M}{r_i}\right)^{-1}_{r_i} \pmod{M}, \quad M = \prod_{i=1}^{n} r_i}
$$

### 2.2 变量定义

| 变量 | 含义 |
|------|------|
| $n$ | 同余方程的个数 |
| $r_i$ | 第 $i$ 个模数（两两互质，且为质数） |
| $a_i$ | 第 $i$ 个余数 |
| $M$ | 所有模数的乘积 $\prod r_i$ |
| $M_i$ | $M / r_i$（去掉第 $i$ 个模数后的乘积） |
| $t_i$ | $M_i$ 在模 $r_i$ 下的逆元 |
| `ans` | 累加的结果，最终即 $x \bmod M$ |

### 2.3 求解流程

```
输入: n 个同余方程 (r_i, a_i)
输出: x mod M

1. 计算 M = r_1 * r_2 * ... * r_n
2. ans = 0
3. 对每个 i = 1..n:
   - Mi = M / r_i
   - ti = Mi^{-1} mod r_i    （费马小定理 / 扩展欧几里得）
   - ans = (ans + a_i * Mi * ti) mod M
4. 返回 ans
```

## 三、逐行代码解析

### 3.1 全局变量与 init

```cpp
vector<long long> a, r;
void init(int n)
{
    a.assign(n + 1, 0);
    r.assign(n + 1, 0);
}
```

`a[i]` 存余数，`r[i]` 存模数。数组下标从 1 开始，方便按方程编号访问。

### 3.2 _gcd —— 欧几里得算法

```cpp
long long _gcd(long long a, long long b)
{
    return b ? _gcd(b, a % b) : a;
}
```

递归实现的 GCD，本代码中实际未被 CRT 函数调用（因为假设模数两两互质且为质数），保留作通用工具函数。

### 3.3 fastpow —— 快速幂

```cpp
long long fastpow(long long a, long long b, long long q)
{
    long long ans = 1;
    long long gap = a % q;
    while (b)
    {
        if (b & 1) ans = (ans * gap) % q;
        gap = (gap * gap) % q;
        b >>= 1;
    }
    return ans;
}
```

计算 $a^b \bmod q$，用于费马小定理求逆元。注意首行 `gap = a % q` 确保底数先取模。

### 3.4 CRT —— 中国剩余定理主函数

```cpp
long long CRT(int n, long long M)
{
    long long ans = 0;
    for (int i = 1; i <= n; i++)
    {
        long long x = a[i] % M;
        long long Mi = M / r[i];
        long long ti = fastpow(Mi, r[i] - 2, r[i]);
        x = (x * (Mi % M)) % M;
        x = (x * ti) % M;
        ans = (ans + x) % M;
    }
    return (ans % M + M) % M;
}
```

| 行 | 代码 | 解析 |
|----|------|------|
| 1 | `long long ans = 0;` | 累加器初始化 |
| 2 | `long long x = a[i] % M;` | 取余数的模 M 形式 |
| 3 | `long long Mi = M / r[i];` | $M_i = M / r_i$ |
| 4 | `long long ti = fastpow(Mi, r[i] - 2, r[i]);` | 费马小定理求 $t_i = M_i^{-1} \bmod r_i$ |
| 5 | `x = (x * (Mi % M)) % M;` | $x \leftarrow a_i \cdot M_i \pmod{M}$ |
| 6 | `x = (x * ti) % M;` | $x \leftarrow a_i \cdot M_i \cdot t_i \pmod{M}$ |
| 7 | `ans = (ans + x) % M;` | 累加到答案 |
| 8 | `return (ans % M + M) % M;` | 确保结果非负 |

### 3.5 main —— 输入输出

```cpp
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    int n;
    long long M = 1;
    cin >> n;
    init(n);
    for (int i = 1; i <= n; i++)
    {
        cin >> r[i] >> a[i];
        M *= r[i];
    }
    cout << CRT(n, M);
    return 0;
}
```

读取 $n$ 个方程，逐次累乘计算 $M$，调用 CRT 求解并输出。

### 3.6 运行示例

输入：
```
3
3 2
5 3
7 2
```

即求解：
$$
\begin{cases}
x \equiv 2 \pmod{3} \\
x \equiv 3 \pmod{5} \\
x \equiv 2 \pmod{7}
\end{cases}
$$

输出：
```
23
```

验证：$23 \bmod 3 = 2$，$23 \bmod 5 = 3$，$23 \bmod 7 = 2$ ✓

$M = 3 \times 5 \times 7 = 105$，通解为 $x \equiv 23 \pmod{105}$。

## 四、复杂度分析

| 维度 | 复杂度 | 说明 |
|------|--------|------|
| 时间 | $O(n \log r_{\max})$ | 每个方程需要一次快速幂求逆元 $O(\log r_i)$，共 $n$ 个方程 |
| 空间 | $O(n)$ | 存储 $a_i$ 和 $r_i$ |

## 五、适用场景

| 场景 | 说明 |
|------|------|
| **同余方程组求解** | 模数两两互质的同余方程组 |
| **组合数取模** | 模数为合数但可分解为互质因子的乘积时，分别求解后用 CRT 合并 |
| **NTT 合并** | 用多个 NTT 友好模数计算后，用 CRT 合并出真实结果 |
| **数论构造题** | 需要构造满足多个同余约束的数 |

## 六、常见陷阱与注意事项

### 6.1 大数溢出（最严重问题）

本代码的最大问题是溢出。`long long` 的上限约为 $9.2 \times 10^{18}$，当模数乘积 $M$ 或中间乘法超过这个值时，结果完全错误。

**典型溢出点**：
- `M *= r[i]` —— 累乘 $M$ 时溢出
- `ans * gap` 和 `gap * gap` —— 快速幂内部乘法溢出
- `x * (Mi % M)` —— CRT 中的乘法溢出

**解决方案**：
1. 使用 `__int128` 做中间乘法（部分编译器支持）
2. 使用龟速乘（快速乘）模拟乘法，避免溢出
3. 使用 Python 等自带高精度的语言

### 6.2 模数必须两两互质

CRT 的前提条件是模数两两互质。如果模数不互质，应当使用扩展中国剩余定理（EXCRT），通过合并方程逐步求解。

### 6.3 费马小定理要求模数为质数

本代码用 `fastpow(Mi, r[i] - 2, r[i])` 求逆元，要求 $r_i$ 是质数。如果 $r_i$ 只是与 $M_i$ 互质但不是质数，应改用扩展欧几里得求逆元。

### 6.4 结果可能为负

虽然代码末尾做了 `(ans % M + M) % M` 确保非负，但如果中间步骤处理不当（如 $a_i$ 为负数），仍可能出问题。建议输入前先将 $a_i$ 调整到 $[0, r_i)$ 范围内。

### 6.5 M 可能等于 0

如果模数太多导致 $M$ 溢出变为 0，后续 `M / r[i]` 会触发除零错误。使用前务必确认数据范围。

## 七、对比与扩展

### 7.1 三种 CRT 实现对比

| 维度 | 费马小定理版（本代码） | 扩展欧几里得版 | EXCRT（扩展 CRT） |
|------|---------------------|-------------|-----------------|
| 模数要求 | 两两互质且为质数 | 两两互质 | 无需互质 |
| 求逆方式 | 快速幂 | 扩展欧几里得 | 合并方程时用 exgcd |
| 时间复杂度 | $O(n \log r)$ | $O(n \log r)$ | $O(n \log r)$ |
| 代码复杂度 | 简单 | 中等 | 稍复杂 |
| 适用场景 | 模数为质数的竞赛题 | 模数互质但不一定是质数 | 模数不互质的一般情况 |

### 7.2 龟速乘（快速乘）防溢出

```cpp
long long mul(long long a, long long b, long long mod)
{
    long long ans = 0;
    while (b)
    {
        if (b & 1) ans = (ans + a) % mod;
        a = (a + a) % mod;
        b >>= 1;
    }
    return ans;
}
```

用加法模拟乘法，复杂度 $O(\log b)$，避免中间溢出。配合 CRT 使用可处理更大的模数。

### 7.3 EXCRT（扩展中国剩余定理）

当模数不互质时，使用 EXCRT 逐次合并方程：

```cpp
long long excrt(int n, long long r[], long long a[])
{
    long long M = r[1], ans = a[1];
    for (int i = 2; i <= n; i++)
    {
        long long c = ((a[i] - ans) % r[i] + r[i]) % r[i];
        long long x, y;
        long long d = ex_gcd(M, r[i], x, y);
        if (c % d != 0) return -1; // 无解
        long long t = r[i] / d;
        x = mul(x, c / d, t);
        ans += x * M;
        M = M / d * r[i];
        ans = (ans % M + M) % M;
    }
    return ans;
}
```

每次合并两个同余方程，通过扩展欧几里得求解新的模数和余数。

### 7.4 一句话总结

中国剩余定理求解两两互质模数的同余方程组，核心是构造 $M = \prod r_i$ 和 $M_i = M/r_i$，利用逆元 $M_i^{-1}$ 使得第 $i$ 项仅在模 $r_i$ 下贡献 $a_i$，其余模下为 0。本实现基于费马小定理求逆元，**代码简洁但大数容易溢出**，仅适合模数较少、数值较小的场景。生产级实现应配合龟速乘或 `__int128` 使用。
