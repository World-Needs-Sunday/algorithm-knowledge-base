---
id: math-pollard-rho
title: 'Pollard-Rho 质因数分解'
category: 数学
subcategory: 数论基础
subSubcategory: ''
tags: ["数学", "数论", "质因数分解", "随机化算法", "生日悖论"]
timeComplexity: 'O(n^{1/4})'
spaceComplexity: 'O(1)'
level: A
noCode: true
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 算法原理

Pollard-Rho 用于**大整数的质因数分解**。当 $n$ 大到 $10^{18}$ 级别，试除法需要枚举到 $\sqrt{n} \approx 10^9$，直接超时。Pollard-Rho 把「逐个试因子」换成「**随机猜因子**」，借助**生日悖论**大幅降低尝试次数。

**生日悖论**：一个房间要多少人，才能让「至少两个人同一天生日」的概率超过 50%？直觉是 183 人（365 的一半），但答案是 **23 人**。原因在于比较对象不是「某个人和你是否同天」，而是「任意两个人之间」：$k$ 个人两两配对的组合数是 $\binom{k}{2} \approx \frac{k^2}{2}$，随 $k$ 平方增长，碰撞来得远比直觉早。

把生日悖论搬到因数分解上：设 $n$ 有一个（未知的）质因子 $p$，必有 $p \le \sqrt{n}$。若随机生成 $k$ 个数并对 $p$ 取模，当 $k \approx \sqrt{p}$ 时，几乎必然有两个数同余，即 $x_i \equiv x_j \pmod{p}$。此时 $p \mid (x_i - x_j)$，于是 $\gcd(\lvert x_i - x_j \rvert, n)$ 是 $p$ 的倍数，极大概率就是 $n$ 的一个**非平凡因子**（既不是 $1$ 也不是 $n$）。

关键点在于 $p \le \sqrt{n}$，所以 $\sqrt{p} \le n^{1/4}$：只需约 $n^{1/4}$ 次随机「碰撞」就能挖出一个因子，比试除法的 $\sqrt{n}$ 快得多。找到因子后递归分解，直到全部拆成质数。

具体怎么生成随机数？Pollard-Rho 用一个**伪随机递推** $x_{i+1} \equiv (x_i^2 + c) \pmod{n}$。因为状态只有 $n$ 种，序列必然进入循环；「尾巴 + 环」整体形似希腊字母 $\rho$，算法因此得名。对 $p$ 取模后状态只有 $p$ 种，环更短（约 $\sqrt{p}$），所以在模 $p$ 意义下先碰撞、在模 $n$ 意义下尚未碰撞——正是这种「差」暴露了因子。

## 核心公式 / 状态定义与转移方程

**伪随机递推**：

$$
x_{i+1} \equiv (x_i^2 + c) \pmod{n}, \qquad c \not\equiv 0,\ -2 \pmod{n}
$$

**生日悖论（碰撞概率）**：$k$ 个数中至少有一对模 $p$ 同余的概率约为

$$
P \approx 1 - e^{-k^2 / (2p)}
$$

当 $k \approx \sqrt{p}$ 时 $P$ 超过 $50\%$。

**碰撞给出因子**：若 $x_i \equiv x_j \pmod{p}$ 且 $x_i \not\equiv x_j \pmod{n}$，则

$$
d = \gcd(\lvert x_i - x_j \rvert, n), \qquad 1 < d < n
$$

**复杂度依据**：$p \le \sqrt{n} \implies \sqrt{p} \le n^{1/4}$，因此期望尝试次数为 $O(n^{1/4})$。

## 逐行代码解析

Pollard-Rho 的完整实现还需配合 Miller-Rabin 素性测试与防溢出模乘，下面用伪代码分模块讲解核心流程。

### 模块一：伪随机递推与 ρ 形结构

```cpp
// 伪随机递推：x_{i+1} ≡ (x_i^2 + c) (mod n)
long long f(long long x, long long c, long long n) {
    return ((__int128)x * x + c) % n;   // __int128 防止 x*x 溢出
}
```

- 递推是「伪随机」的：给定 $n$ 与 $c$ 后序列完全确定，但取值在 $[0, n)$ 上乱跳。
- 状态只有 $n$ 种，序列必然进入循环；「尾巴 + 环」构成 $\rho$ 形，是算法名字的由来。
- 平方项让相邻值快速散开；改变 $c$ 可换一条新序列，供失败时重试。

### 模块二：Floyd 判环（龟兔赛跑）

```cpp
// 快慢指针在「模 p 的环」上必然相遇，差值暴露因子
x = f(x, c, n);              // 慢指针（乌龟）走一步
y = f(f(y, c, n), c, n);     // 快指针（兔子）走两步
d = gcd(abs(x - y), n);      // 相遇时 |x - y| 是 p 的倍数
```

- 无需存下整条序列：只维护两个指针 $x$（慢）与 $y$（快）。
- 当序列在模 $p$ 上成环时，快慢指针必然在同一环内追上，即 $x \equiv y \pmod{p}$。
- 此时 $p \mid (x - y)$，$\gcd(\lvert x - y \rvert, n)$ 就是候选因子 $d$。

### 模块三：单次 Pollard-Rho 找非平凡因子

```cpp
long long pollard_rho(long long n) {
    if (n % 2 == 0) return 2;             // 偶数特判，直接拆出 2
    long long c = rand() % (n - 1) + 1;   // 随机常数 c（避开 0 与 -2）
    long long x = rand() % n, y = x, d = 1;
    while (d == 1) {                      // 未碰撞则继续推进
        x = f(x, c, n);
        y = f(f(y, c, n), c, n);
        d = gcd(abs(x - y), n);
    }
    return d == n ? pollard_rho(n) : d;   // d==n 撞在模 n 上，换 c 重试
}
```

- $d = 1$ 说明还没碰撞，继续推进序列。
- $d = n$ 说明 $x \equiv y \pmod{n}$，这次运气不好，换随机 $c$ 重新跑。
- $1 < d < n$ 时，$d$ 就是一个非平凡因子。

### 模块四：Miller-Rabin 判质数 + 递归分解

```cpp
void factorize(long long n, vector<long long>& fac) {
    if (n == 1) return;
    if (is_prime(n)) { fac.push_back(n); return; }   // Miller-Rabin 判定质数
    long long d = pollard_rho(n);                    // 拆出非平凡因子
    factorize(d, fac);                               // 递归分解 d
    factorize(n / d, fac);                           // 递归分解另一半
}
```

- `is_prime` 用 Miller-Rabin 随机化素性测试，误判概率可忽略（详见「对比与扩展」）。
- 找到因子 $d$ 后对 $d$ 与 $n/d$ 分别递归，直到全部拆成质数。
- 分工：Pollard-Rho 负责「拆」，Miller-Rabin 负责「判定是否还要拆」。

## 复杂度分析

| 维度 | 复杂度 | 说明 |
|------|--------|------|
| 时间 | $O(n^{1/4})$（期望） | 每个因子期望 $O(n^{1/4})$ 次迭代找到；每次迭代一次模乘与一次 gcd |
| 空间 | $O(1)$ | Floyd 判环只需两个指针，不存整条序列 |

**与试除法对比**：试除法 $O(\sqrt{n})$，Pollard-Rho $O(n^{1/4})$。当 $n = 10^{18}$ 时，$\sqrt{n} \approx 10^9$ 已经扛不住，而 $n^{1/4} \approx 3 \times 10^4$ 瞬间完成。

## 适用场景

- 大整数质因数分解：$n$ 到 $10^{18}$ 甚至更大（配合 Miller-Rabin），试除法顶不住时。
- 只需单个非平凡因子（不要求全部质因子）时，可只调用一次 `pollard_rho`。
- 数论题里对 $n$ 做唯一分解、求 $\varphi(n)$、求约数个数/约数和等，当 $\sqrt{n}$ 过不了时。

## 常见陷阱与注意事项

1. **模乘溢出**：$x \cdot x$ 在 $n$ 接近 $2^{63}$ 时会溢出，必须用 `__int128` 或龟速乘做模乘。
2. **$d = n$ 的失败**：撞在模 $n$ 上得到的 $\gcd = n$ 不是有效因子，必须换 $c$ 重试（递归重跑）。
3. **$c$ 的取值**：$c = 0$ 或 $c = -2$ 时序列会退化（很快循环），应避开。
4. **初值固定**：随机化初始 $x$ 与 $c$，否则对某些 $n$ 可能反复失败。
5. **必须配合素性测试**：Pollard-Rho 只能拆合数，需 Miller-Rabin 先判质数，否则会无限递归。
6. **偶数特判**：偶数 $n$ 直接返回 $2$，否则平方递推在偶数上表现差。

## 对比与扩展

| 方法 | 复杂度 | 适用 |
|------|--------|------|
| 试除法 | $O(\sqrt{n})$ | $n \le 10^{12}$ 左右 |
| Pollard-Rho | $O(n^{1/4})$（期望） | $n$ 到 $10^{18}$ 甚至更大 |
| 埃氏筛 / 线性筛 | $O(n\log\log n)$ / $O(n)$ | 批量筛出 $[1,n]$ 的质因子，$n \le 10^7$ |

扩展：

- **Miller-Rabin**：随机化素性测试，与 Pollard-Rho 组成完整的「大整数分解」工具链。
- **Brent 判环**：Floyd 判环的改进版，减少 gcd 调用次数，常数更小。
- **平方根攻击思想**：Pollard-Rho 是「用碰撞把 $O(n)$ 降到 $O(\sqrt{n})$」的典型例子，同源的还有 BSGS（大步小步）、Meet-in-the-Middle 等，核心都是生日悖论。
