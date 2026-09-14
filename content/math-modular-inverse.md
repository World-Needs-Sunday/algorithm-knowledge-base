---
id: math-modular-inverse
title: '模逆元（费马小定理 + 扩展欧几里得）'
category: '数学'
subcategory: '数论基础'
difficulty: '普及+'
tags: ['数学', '数论', '模逆元', '费马小定理', '欧拉定理', '快速幂', '扩展欧几里得']
prerequisites: ['math-quick-pow-basic', 'math-euler-phi', 'math-gcd-lcm']
codePath: '数学\数论基础\模逆元\源.cpp'
description: '模逆元 a^{-1} mod q 满足 a * a^{-1} ≡ 1 (mod q)。当 q 为质数时，由费马小定理 a^{q-2} ≡ a^{-1} (mod q)，用快速幂 O(log q) 求解；当 q 不为质数但 gcd(a,q)=1 时，由欧拉定理 a^{φ(q)-1} ≡ a^{-1} (mod q)。逆元是模意义下做除法的核心工具。'
---

# 模逆元（费马小定理 + 扩展欧几里得）

## 一、算法原理

### 1.1 定义

在模 $q$ 意义下，$a$ 的**模逆元**（modular inverse）$a^{-1}$ 满足：

$$a \cdot a^{-1} \equiv 1 \pmod{q}$$

逆元存在的**充要条件**是 $\gcd(a, q) = 1$，即 $a$ 与 $q$ 互质。

**直觉理解**：在实数域中，$a$ 的倒数是 $1/a$，满足 $a \times (1/a) = 1$。模逆元就是模意义下的"倒数"——把除法转化为乘法。$a / b \bmod q$ 等价于 $a \times b^{-1} \bmod q$。

### 1.2 费马小定理

**费马小定理**：若 $p$ 为质数且 $\gcd(a, p) = 1$，则

$$a^{p-1} \equiv 1 \pmod{p}$$

两边同乘 $a^{-1}$：

$$a^{p-2} \equiv a^{-1} \pmod{p}$$

因此，当模数 $q$ 为**质数**时，$a^{-1} \equiv a^{q-2} \pmod{q}$，用快速幂在 $O(\log q)$ 内求出。

### 1.3 欧拉定理（一般情况）

当模数 $q$ **不一定为质数**时，用欧拉定理：

$$a^{\varphi(q)} \equiv 1 \pmod{q} \quad (\gcd(a, q) = 1)$$

因此 $a^{-1} \equiv a^{\varphi(q)-1} \pmod{q}$。

当 $q$ 为质数时 $\varphi(q) = q - 1$，退化为费马小定理。所以费马小定理是欧拉定理的特例。

### 1.4 扩展欧几里得法

逆元也可以通过求解方程 $ax + qy = 1$（$\gcd(a,q)=1$ 保证有解）来获得。用扩展欧几里得算法求出 $x$，则 $a^{-1} \equiv x \pmod{q}$。这种方法不要求 $q$ 为质数，只要求 $\gcd(a,q) = 1$。

### 1.5 三种方法对比

| 方法 | 条件 | 复杂度 | 代码难度 |
|------|------|--------|---------|
| 费马小定理 + 快速幂 | $q$ 为质数 | $O(\log q)$ | 简单 |
| 欧拉定理 + 快速幂 | $\gcd(a,q)=1$ | $O(\sqrt{q} + \log q)$（需先求 $\varphi(q)$） | 中等 |
| 扩展欧几里得 | $\gcd(a,q)=1$ | $O(\log q)$ | 中等 |

本代码实现的是**费马小定理 + 快速幂**法。

## 二、核心公式/状态定义与转移方程

### 2.1 核心公式

$$\boxed{a^{-1} \equiv a^{q-2} \pmod{q} \quad (q \text{ 为质数}, \gcd(a,q)=1)}$$

### 2.2 快速幂递推

将指数 $q - 2$ 按二进制分解，利用平方倍增：

$$a^b = \left(a^{b/2}\right)^2 \times (a \text{ if } b \text{ odd})$$

| 迭代 | 操作 | 含义 |
|------|------|------|
| `ans = 1` | 初始化 | 累积器 |
| `gap = a` | 初始化 | 当前平方基 |
| `if (b & 1) ans = ans * gap % q` | 奇数位 | 乘入当前基 |
| `gap = gap * gap % q` | 平方 | 基倍增 |
| `b >>= 1` | 右移 | 处理下一位 |

### 2.3 逆元函数的状态转移

```
inv(a, q):
  若 gcd(a, q) == 1 → 返回 fastpow(a, q-2, q)   // 费马小定理
  否则 → 返回 -1                                    // 逆元不存在
```

### 2.4 GCD 递推（欧几里得算法）

$$\gcd(a, b) = \gcd(b, a \bmod b), \quad \gcd(a, 0) = a$$

### 2.5 重要性质速查

| 性质 | 公式 | 条件 |
|------|------|------|
| 逆元定义 | $a \cdot a^{-1} \equiv 1 \pmod{q}$ | $\gcd(a,q)=1$ |
| 费马小定理 | $a^{q-2} \equiv a^{-1} \pmod{q}$ | $q$ 为质数 |
| 欧拉定理 | $a^{\varphi(q)-1} \equiv a^{-1} \pmod{q}$ | $\gcd(a,q)=1$ |
| 乘法转除法 | $a/b \equiv a \cdot b^{-1} \pmod{q}$ | $\gcd(b,q)=1$ |
| 逆元的逆元 | $(a^{-1})^{-1} \equiv a \pmod{q}$ | $\gcd(a,q)=1$ |
| 逆元可乘 | $(ab)^{-1} \equiv a^{-1} \cdot b^{-1} \pmod{q}$ | $\gcd(a,q)=\gcd(b,q)=1$ |
| 线性递推 | $i^{-1} \equiv -(q/i) \cdot (q \bmod i)^{-1} \pmod{q}$ | $q$ 为质数，$i < q$ |

## 三、逐行代码解析

```cpp
#include<bits/stdc++.h>
using namespace std;
```

标准头文件与命名空间。

### 3.1 _gcd —— 欧几里得算法

```cpp
long long _gcd(long long a, long long b)
{
    return b ? _gcd(b, a % b) : a;
}
```

递归实现的欧几里得算法。`b` 为 0 时返回 `a`（递归终点），否则递归调用 `_gcd(b, a % b)`。函数名加下划线前缀避免与标准库 `gcd` 冲突。

| 行 | 代码 | 解析 |
|----|------|------|
| 1 | `long long _gcd(long long a, long long b)` | 输入 $a, b$，返回 $\gcd(a, b)$ |
| 2 | `return b ? _gcd(b, a % b) : a;` | 三目运算：$b \ne 0$ 时递归，$b = 0$ 时返回 $a$ |

### 3.2 fastpow —— 快速幂

```cpp
long long fastpow(long long a, long long b, long long q)
{
    if (b == 0) return 1;
    long long ans = 1;
    long long gap = a;
    while (b)
    {
        if (b & 1) ans = (ans * gap) % q;
        gap = (gap * gap) % q;
        b >>= 1;
    }
    return ans;
}
```

计算 $a^b \bmod q$。

| 行 | 代码 | 解析 |
|----|------|------|
| 1 | `if (b == 0) return 1;` | 特判 $b = 0$：$a^0 = 1$ |
| 2-3 | `long long ans = 1; long long gap = a;` | `ans` 为累积器，`gap` 为当前平方基 |
| 4 | `while (b)` | 遍历 $b$ 的每一位二进制位 |
| 5 | `if (b & 1) ans = (ans * gap) % q;` | 当前位为 1 → 乘入 `gap` |
| 6 | `gap = (gap * gap) % q;` | 平方倍增 |
| 7 | `b >>= 1;` | 右移处理下一位 |

### 3.3 inv —— 模逆元（费马小定理）

```cpp
long long inv(long long a, long long q)
{
    if (_gcd(a, q) == 1) return fastpow(a, q - 2, q);
    return -1;
}
```

| 行 | 代码 | 解析 |
|----|------|------|
| 1 | `long long inv(long long a, long long q)` | 输入 $a$ 和质数 $q$，返回 $a^{-1} \bmod q$ |
| 2 | `if (_gcd(a, q) == 1) return fastpow(a, q - 2, q);` | $\gcd(a,q)=1$ → 费马小定理：$a^{-1} \equiv a^{q-2}$ |
| 3 | `return -1;` | 不互质 → 逆元不存在，返回 $-1$ |

### 3.4 main —— 求逆元

```cpp
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    //q要为质数
    long long a, q;
    cin >> a >> q;
    cout << inv(a, q);
    return 0;
}
```

| 行 | 代码 | 解析 |
|----|------|------|
| 1-2 | `ios::sync_with_stdio(false); cin.tie(nullptr);` | 快速 IO |
| 3 | `//q要为质数` | 注释提醒：费马小定理要求 $q$ 为质数 |
| 4-5 | `long long a, q; cin >> a >> q;` | 读入 $a$ 和模数 $q$ |
| 6 | `cout << inv(a, q);` | 输出 $a^{-1} \bmod q$（或 $-1$ 表示不存在） |

### 3.5 运行示例

输入：
```
3 7
```

输出：
```
5
```

因为 $3 \times 5 = 15 \equiv 1 \pmod{7}$，所以 $3^{-1} \equiv 5 \pmod{7}$。

验证：$3^{7-2} = 3^5 = 243 = 34 \times 7 + 5 \equiv 5 \pmod{7}$ ✓

输入：
```
2 4
```

输出：
```
-1
```

因为 $\gcd(2, 4) = 2 \ne 1$，逆元不存在。

## 四、复杂度分析

| 维度 | 复杂度 | 说明 |
|------|--------|------|
| 时间 | $O(\log q)$ | 快速幂 $O(\log q)$ + GCD $O(\log q)$ |
| 空间 | $O(1)$ | 仅用几个变量（递归 GCD 的栈深度为 $O(\log q)$） |

## 五、适用场景

| 场景 | 说明 |
|------|------|
| **模意义除法** | $a/b \bmod q$ 转化为 $a \times b^{-1} \bmod q$ |
| **组合数取模** | $C(n,k) = n! / (k!(n-k)!)$ 取模需要求阶乘的逆元 |
| **模方程求解** | 求解 $ax \equiv b \pmod{q}$ 转化为 $x \equiv b \cdot a^{-1} \pmod{q}$ |
| **RSA 加密** | 生成密钥时需要模逆元 |
| **分数取模** | 将分数 $a/b$ 在模 $q$ 下表示为整数 $a \cdot b^{-1} \bmod q$ |

## 六、常见陷阱与注意事项

### 6.1 q 必须为质数

费马小定理要求模数 $q$ 为质数。若 $q$ 为合数，$a^{q-2} \bmod q$ **不一定**等于 $a^{-1}$。例如 $q = 4$，$a = 3$：$3^{4-2} = 9 \equiv 1 \pmod{4}$，但 $3^{-1} \equiv 3 \pmod{4}$（$3 \times 3 = 9 \equiv 1 \pmod{4}$），两者恰好相等是巧合。另一个反例：$q = 6$，$a = 5$，$5^{6-2} = 5^4 = 625 \equiv 1 \pmod{6}$，而 $5^{-1} \equiv 5 \pmod{6}$，不相等。

### 6.2 gcd 检查的必要性

即使 $q$ 为质数，仍需检查 $\gcd(a, q) = 1$。当 $a$ 是 $q$ 的倍数时（$q$ 为质数即 $a$ 是 $q$ 的倍数），逆元不存在。代码返回 $-1$ 表示这种情况。若不做检查直接计算，结果为 0 而非 $-1$，可能导致下游逻辑错误。

### 6.3 long long 的必要性

中间计算 `ans * gap` 和 `gap * gap` 可能超过 `int` 上限。当 $q$ 接近 $10^9$ 时，两个 $< q$ 的数相乘结果可达 $10^{18}$，必须用 `long long`。

### 6.4 负数取模

C++ 中负数取模结果为负数。若 $a$ 可能为负，需先 `a = ((a % q) + q) % q` 转为正数再求逆元。

### 6.5 逆元不改变"整除性"

$a \cdot b^{-1} \bmod q$ 的结果只有在 $b \mid a$ 时才等于 $a/b$ 的整数部分取模。当 $b \nmid a$ 时，结果是"模 $q$ 意义下的商"，而非真实除法的结果。

### 6.6 函数名冲突

标准库 `<algorithm>` 在 C++17 中提供了 `std::gcd`，本代码用 `_gcd` 避免命名冲突。若使用 `using namespace std` 且不加分隔符直接定义 `gcd`，可能产生歧义。

## 七、对比与扩展

### 7.1 三种求逆元方法对比

| 维度 | 费马小定理（本代码） | 欧拉定理 | 扩展欧几里得 |
|------|---------------------|---------|-------------|
| 模数要求 | $q$ 为质数 | $\gcd(a,q)=1$ | $\gcd(a,q)=1$ |
| 核心公式 | $a^{q-2}$ | $a^{\varphi(q)-1}$ | 解 $ax+qy=1$ |
| 复杂度 | $O(\log q)$ | $O(\sqrt{q} + \log q)$ | $O(\log q)$ |
| 需要快速幂 | 是 | 是 | 否 |
| 需要求 $\varphi$ | 否 | 是 | 否 |
| 适用场景 | 模数为质数（竞赛常见） | 模数为合数 | 模数为合数或需同时求 GCD |

### 7.2 线性递推求逆元

当需要 $[1, n]$ 内所有数的逆元（$n \le q$，$q$ 为质数）时，可线性递推：

$$i^{-1} \equiv -(q/i) \cdot (q \bmod i)^{-1} \pmod{q}$$

推导：设 $q = ki + r$（$k = q/i$，$r = q \bmod i$），则 $ki + r \equiv 0 \pmod{q}$，两边乘 $i^{-1} r^{-1}$ 得 $k \cdot r^{-1} + i^{-1} \equiv 0$，所以 $i^{-1} \equiv -k \cdot r^{-1} = -(q/i) \cdot (q \bmod i)^{-1}$。

```cpp
inv[1] = 1;
for (int i = 2; i <= n; i++)
    inv[i] = (q - q / i) * inv[q % i] % q;
```

复杂度 $O(n)$，适合批量预处理。

### 7.3 逆元与组合数

组合数 $C(n, k) = \frac{n!}{k!(n-k)!}$ 取模时，需要预处理阶乘及其逆元：

$$C(n, k) \bmod q = n! \cdot (k!)^{-1} \cdot ((n-k)!)^{-1} \bmod q$$

预处理 `fact[i] = i!` 和 `invfact[i] = (i!)^{-1}`，即可 $O(1)$ 查询组合数。

### 7.4 扩展欧几里得求逆元

```cpp
long long ex_gcd(long long a, long long b, long long &x, long long &y)
{
    if (!b) { x = 1; y = 0; return a; }
    long long d = ex_gcd(b, a % b, y, x);
    y -= a / b * x;
    return d;
}

long long inv(long long a, long long q)
{
    long long x, y;
    ex_gcd(a, q, x, y);
    return (x % q + q) % q;
}
```

不要求 $q$ 为质数，只要求 $\gcd(a, q) = 1$。

### 7.5 一句话总结

模逆元是模意义下的"倒数"，将除法转化为乘法。当模数 $q$ 为质数时，由费马小定理 $a^{-1} \equiv a^{q-2} \pmod{q}$，用快速幂 $O(\log q)$ 求解；当 $q$ 为合数时，由欧拉定理 $a^{-1} \equiv a^{\varphi(q)-1} \pmod{q}$ 或扩展欧几里得求解。逆元是组合数取模、模方程求解等问题的核心工具。
