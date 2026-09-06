---
id: math-gcd-lcm
title: 'GCD 与 LCM（欧几里得算法）'
category: '数学'
subcategory: '数论基础'
difficulty: '入门'
tags: ['数学', '数论', '最大公约数', '最小公倍数', '欧几里得算法', '递归']
codePath: '数学\数论基础\GCD与LCM\源.cpp'
description: '用欧几里得算法（辗转相除法）求最大公约数 GCD，再利用 gcd(a,b) × lcm(a,b) = a × b 求最小公倍数 LCM。递归写法仅需一行核心代码，时间复杂度 O(log min(a,b))，是数论中最基础也是最重要的算法之一。'
---

# GCD 与 LCM（欧几里得算法）

## 一、算法原理

### 1.1 最大公约数 GCD

**定义**：对于两个正整数 $a, b$，它们的最大公约数（Greatest Common Divisor, GCD）是能同时整除 $a$ 和 $b$ 的最大正整数，记作 $\gcd(a, b)$。

例：$\gcd(12, 18) = 6$，$\gcd(7, 13) = 1$（两数互质）。

### 1.2 欧几里得算法（辗转相除法）

**核心等式**：

$$\gcd(a, b) = \gcd(b, a \bmod b)$$

**为什么成立？** 设 $d = \gcd(a, b)$，即 $d \mid a$ 且 $d \mid b$。令 $a = q \cdot b + r$（其中 $0 \le r < b$，$r = a \bmod b$），则：

- $d \mid a$ 且 $d \mid b$ → $d \mid (a - q \cdot b) = r$ → $d$ 也是 $b$ 和 $r$ 的公约数
- 反过来，若 $d' \mid b$ 且 $d' \mid r$，则 $d' \mid (q \cdot b + r) = a$ → $d'$ 也是 $a$ 和 $b$ 的公约数

因此 $\{a, b\}$ 的公约数集合与 $\{b, a \bmod b\}$ 的公约数集合完全相同，最大元自然也相同 ✓

**终止条件**：当 $b = 0$ 时，$\gcd(a, 0) = a$（任何数与 0 的最大公约数是它本身）。

### 1.3 最小公倍数 LCM

**定义**：对于两个正整数 $a, b$，它们的最小公倍数（Least Common Multiple, LCM）是能同时被 $a$ 和 $b$ 整除的最小正整数，记作 $\text{lcm}(a, b)$。

例：$\text{lcm}(12, 18) = 36$，$\text{lcm}(7, 13) = 91$。

**核心公式**：

$$\gcd(a, b) \times \text{lcm}(a, b) = a \times b$$

变形得：

$$\text{lcm}(a, b) = \frac{a \times b}{\gcd(a, b)}$$

**为什么成立？** 从质因数分解角度看：设 $a = \prod p_i^{e_i}$，$b = \prod p_i^{f_i}$，则

- $\gcd(a, b) = \prod p_i^{\min(e_i, f_i)}$
- $\text{lcm}(a, b) = \prod p_i^{\max(e_i, f_i)}$

显然 $\min(e_i, f_i) + \max(e_i, f_i) = e_i + f_i$，对应指数相加即乘积等式成立 ✓

### 1.4 防溢出技巧：先除后乘

直接写 `a * b / gcd(a, b)` 有溢出风险：当 $a, b$ 都是 $10^9$ 时，$a \times b$ 可达 $10^{18}$，`int` 存不下。即使 `long long`，若 $a, b$ 更大也会溢出。

正确写法：

$$\text{lcm}(a, b) = \frac{a}{\gcd(a, b)} \times b$$

先除后乘，中间值更小，溢出概率大幅降低。本代码即采用此写法。

## 二、核心公式/状态定义与转移方程

### 2.1 欧几里得算法递归式

$$
\gcd(a, b) =
\begin{cases}
a, & b = 0 \\
\gcd(b, a \bmod b), & b \neq 0
\end{cases}
$$

### 2.2 复杂度分析

欧几里得算法的时间复杂度为 $O(\log \min(a, b))$。

**证明思路**（拉梅定理，Lamé's Theorem）：欧几里得算法的步数不超过较小数的十进制位数的 5 倍。

直觉上，$a \bmod b$ 至少比 $a$ 小一半（当 $b \le a/2$ 时显然；当 $b > a/2$ 时，$a \bmod b = a - b < a/2$），因此每次递归规模至少减半，步数为对数级。

### 2.3 性质速查

| 性质 | 公式 |
|------|------|
| 交换律 | $\gcd(a, b) = \gcd(b, a)$ |
| 结合律 | $\gcd(\gcd(a, b), c) = \gcd(a, \gcd(b, c))$ |
| 分配律 | $\gcd(k \cdot a, k \cdot b) = k \cdot \gcd(a, b)$ |
| 互质条件 | $\gcd(a, b) = 1 \iff a, b$ 互质 |
| 贝祖定理 | 存在整数 $x, y$ 使得 $a \cdot x + b \cdot y = \gcd(a, b)$ |
| GCD-LCM 关系 | $\gcd(a, b) \times \text{lcm}(a, b) = a \times b$ |
| LCM 分配律 | $\text{lcm}(k \cdot a, k \cdot b) = k \cdot \text{lcm}(a, b)$ |

## 三、逐行代码解析

```cpp
#include<bits/stdc++.h>
using namespace std;
```

标准头文件与命名空间。

### 3.1 _gcd —— 欧几里得算法递归实现

```cpp
long long _gcd(long long a, long long b)
{
    return b ? _gcd(b, a % b) : a;
}
```

| 行 | 代码 | 解析 |
|----|------|------|
| 1 | `long long _gcd(long long a, long long b)` | 参数用 `long long`，支持大数；函数名前加下划线避免与 `std::gcd`（C++17）冲突 |
| 2 | `return b ? _gcd(b, a % b) : a;` | 三元运算符一行实现：若 $b \neq 0$，递归调用 `_gcd(b, a % b)`；若 $b = 0$，返回 $a$（终止条件） |

**执行示例**：$\gcd(48, 18)$

$$
\begin{align*}
\gcd(48, 18) &= \gcd(18, 48 \bmod 18) = \gcd(18, 12) \\
&= \gcd(12, 18 \bmod 12) = \gcd(12, 6) \\
&= \gcd(6, 12 \bmod 6) = \gcd(6, 0) \\
&= 6 \quad ✓
\end{align*}
$$

递归 3 层即得出结果。

### 3.2 _lcm —— 最小公倍数

```cpp
long long _lcm(long long a, long long b)
{
    return a / _gcd(a, b) * b;
}
```

| 行 | 代码 | 解析 |
|----|------|------|
| 1 | `a / _gcd(a, b) * b` | **先除后乘**防溢出：$a / \gcd$ 必为整数（$\gcd$ 整除 $a$），再乘 $b$ 结果与 $a \times b / \gcd$ 相同但中间值更小 |

> **注意**：`a / gcd * b` 与 `b / gcd * a` 结果相同，但从溢出角度看，先用较大的数去除更安全。

### 3.3 main —— 多组测试

```cpp
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    int t;
    cin >> t;
    while (t--)
    {
        long long a, b;
        cin >> a >> b;
        cout << _gcd(a, b) << ' ' << _lcm(a, b) << '\n';
    }
    return 0;
}
```

| 行 | 代码 | 解析 |
|----|------|------|
| 1-2 | `ios::sync_with_stdio(false); cin.tie(nullptr);` | 快速 IO，多组数据时加速输入输出 |
| 3-4 | `int t; cin >> t;` | 读入测试用例组数 |
| 5-8 | `while (t--) { cin >> a >> b; cout << ... << '\n'; }` | 每组读入两个数，输出 GCD 和 LCM，用 `'\n'` 代替 `endl` 避免刷新缓冲区 |

### 3.4 运行示例

输入：
```
3
12 18
7 13
100 25
```

输出：
```
6 36
1 91
25 100
```

## 四、复杂度分析

| 维度 | 复杂度 | 说明 |
|------|--------|------|
| 时间（GCD） | $O(\log \min(a, b))$ | 欧几里得算法，递归深度为对数级 |
| 时间（LCM） | $O(\log \min(a, b))$ | 多一次除法和乘法，常数级额外开销 |
| 空间 | $O(\log \min(a, b))$ | 递归栈深度；迭代版为 $O(1)$ |

对于 $a, b \le 10^{18}$，递归深度约 60 层，几乎瞬间完成。

## 五、适用场景

| 场景 | 说明 |
|------|------|
| **约分 / 化简分数** | 分子分母同除以 GCD，得到最简分数 |
| **求最小公倍数** | 周期问题、调度问题中求重复周期 |
| **扩展欧几里得** | 求模逆元、解贝祖方程（GCD 是基础） |
| **数论计数** | 利用 GCD/LCM 性质统计满足条件的数对 |
| **判断互质** | $\gcd(a, b) = 1$ 即互质，数论中常见条件 |
| **多个数的 GCD/LCM** | 结合律保证可依次迭代计算：$\gcd(a, b, c) = \gcd(\gcd(a, b), c)$ |

## 六、常见陷阱与注意事项

### 6.1 a < b 时算法仍正确

$\gcd(18, 48)$ → $\gcd(48, 18 \bmod 48) = \gcd(48, 18)$，自动交换。**不需要**在调用前保证 $a \ge b$。

### 6.2 参数为 0 的情况

- $\gcd(a, 0) = a$（算法本身已处理，是终止条件）
- $\gcd(0, 0)$ 无定义，数学上不存在，代码会返回 0
- $\text{lcm}(a, 0) = 0$（任何数都是 0 的倍数），但代码中 `a / gcd(a,0) * 0 = a / a * 0 = 0`，结果正确

### 6.3 负数的 GCD

数学上 GCD 始终为正。若输入可能为负，需取绝对值：`_gcd(abs(a), abs(b))`。本代码假设输入为正整数。

### 6.4 溢出问题

- `a * b / gcd` 写法可能溢出，必须先除后乘：`a / gcd * b`
- 即使先除后乘，若 $a, b$ 都接近 `long long` 上限（$9 \times 10^{18}$），LCM 仍可能溢出；此时需用 `__int128` 或高精度

### 6.5 递归深度

对 $10^{18}$ 以内的数，递归深度约 60 层，不会栈溢出。但若有严格要求，可改写为迭代版：

```cpp
long long _gcd(long long a, long long b)
{
    while (b) { a %= b; swap(a, b); }
    return a;
}
```

### 6.6 C++17 内置 gcd

C++17 在 `<numeric>` 中提供了 `std::gcd` 和 `std::lcm`，但竞赛环境不一定支持，手写版本更稳妥。

### 6.7 多个数的 GCD/LCM

利用结合律迭代即可：

```cpp
long long gcd_n(vector<long long>& v)
{
    long long res = v[0];
    for (int i = 1; i < v.size(); i++)
        res = _gcd(res, v[i]);
    return res;
}
```

## 七、对比与扩展

### 7.1 递归版 vs 迭代版

| 维度 | 递归版 | 迭代版 |
|------|--------|--------|
| 代码长度 | 一行，极其简洁 | 三行循环 |
| 空间 | $O(\log n)$ 栈空间 | $O(1)$ |
| 可读性 | 接近数学定义，直观 | 稍低 |
| 栈溢出风险 | 极低（约 60 层） | 无 |
| 性能 | 函数调用开销略大 | 更快 |

### 7.2 更慢的方法：质因数分解法

先分别分解 $a, b$ 的质因数，再取指数的 min/max。复杂度为 $O(\sqrt{a} + \sqrt{b})$，远劣于欧几里得算法的 $O(\log \min(a,b))$，仅在需要质因数信息时使用。

### 7.3 扩展欧几里得算法

欧几里得算法的扩展版本，不仅求 $\gcd(a, b)$，还求贝祖系数 $x, y$ 使得 $a \cdot x + b \cdot y = \gcd(a, b)$。是求模逆元、解线性同余方程的基础。

### 7.4 二进制 GCD（Stein 算法）

用移位和减法代替取模，在某些硬件上更快，且特别适合大整数（高精度）实现。核心利用：

- $\gcd(2a, 2b) = 2 \cdot \gcd(a, b)$
- $\gcd(2a, b) = \gcd(a, b)$（$b$ 为奇数）
- $\gcd(a, b) = \gcd(|a-b|, \min(a, b))$（$a, b$ 均为奇数）

### 7.5 一句话总结

GCD 是数论的基石算法，欧几里得算法用一行递归实现了 $O(\log n)$ 的极致效率；LCM 通过 `先除后乘` 的公式从 GCD 衍生而来，两者共同构成了数论计数、模运算、扩展欧几里得等高级算法的基础。掌握 GCD 的性质与实现，是学习数论的第一步。
