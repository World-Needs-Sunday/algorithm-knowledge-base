---
id: math-modular-arithmetic
title: '模运算相关算法'
category: 数学
subcategory: 数论基础
subSubcategory: ''
tags: ['数学', '数论', '模运算', '同余', '逆元']
timeComplexity: '视具体实现'
spaceComplexity: '视具体实现'
level: A
noCode: true
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 算法原理

模运算（modular arithmetic）即**取余**运算。给定整数 $a$ 与正整数 $n$，存在唯一的余数 $r$ 与整数 $q$，使得

$$a = q \cdot n + r, \quad 0 \le r < n$$

记 $a \bmod n = r$，称 $r$ 为 $a$ 除以 $n$ 的**最小非负余数**。C++ 中写作 `a % n`，但 C++ 的 `%` 结果符号与被除数一致（向零取整），而数学上通常只关心非负余数，二者在负数上并不一致。

一个直观的例子：今天是星期三，问 $5$ 天后是星期几。因为 $3 + 5 = 8$，$8 \bmod 7 = 1$，所以是星期一。若问 $1145$ 天后，只需算 $1145 \bmod 7$ 即可，无需反复减 $7$。

模运算之所以重要，是因为竞赛题经常要求结果对某个大数（如 $10^9 + 7$）取模。在模意义下，**加、减、乘**都可以"先取模、再运算、最后再取模"，唯独**除法**不能直接把分子分母分别取模后相除，而必须借助**模逆元**。

围绕模运算，延伸出一整套数论算法，本文按依赖顺序展开：

1. **同余与基本运算**：同余的定义、性质，以及模意义下加减乘的恒等变形。
2. **模逆元**：模意义下做除法的核心工具。
3. **裴蜀定理与扩展欧几里得（EXGCD）**：解线性同余方程与不定方程，并求逆元。
4. **费马小定理与欧拉定理**：快速求逆元与"降幂"。
5. **中国剩余定理（CRT）**：解一元线性同余方程组。
6. **BSGS 与原根**：解高次同余方程，以及 NTT 等进阶应用的基础。

## 核心公式

### 2.1 同余与基本运算

若 $a \bmod n = b \bmod n$，称 $a$ 与 $b$ 模 $n$ **同余**，记作

$$a \equiv b \pmod{n}$$

等价于 $n \mid (a - b)$。同余关系满足自反性、对称性、传递性，并在加减乘与幂运算下**保持**：

$$a \equiv b,\ c \equiv d \pmod{m} \implies a+c \equiv b+d,\ a-c \equiv b-d,\ ac \equiv bd,\ a^k \equiv b^k \pmod{m}$$

模意义下加减乘的恒等式：

$$(a + b) \bmod m = \big((a \bmod m) + (b \bmod m)\big) \bmod m$$

$$(a - b) \bmod m = \big((a \bmod m) - (b \bmod m) + m\big) \bmod m$$

$$(a \times b) \bmod m = \big((a \bmod m) \times (b \bmod m)\big) \bmod m$$

减法式中先加 $m$ 是为了把负数"抬"回非负范围。

### 2.2 模逆元

若存在整数 $x$ 使 $a \cdot x \equiv 1 \pmod{m}$，称 $x$ 为 $a$ 模 $m$ 的**逆元**，记作 $a^{-1} \pmod{m}$。逆元存在的充要条件是 $\gcd(a, m) = 1$。有了逆元即可做除法：

$$a / b \equiv a \cdot b^{-1} \pmod{m}$$

常用求法：

- **费马小定理**（$m$ 为质数）：$a^{-1} \equiv a^{m-2} \pmod{m}$。
- **扩展欧几里得**（不要求 $m$ 为质数）：解 $ax + my = 1$ 得 $x$。
- **线性递推**（$m$ 为质数，批量求 $1 \sim n$ 的逆元）：$inv[i] = m - (m / i) \cdot inv[m \bmod i] \bmod m$。

### 2.3 裴蜀定理

对不全为零的整数 $a, b$，一定存在整数 $x, y$ 使

$$ax + by = \gcd(a, b)$$

反过来，方程 $ax + by = c$ 有整数解**当且仅当** $\gcd(a, b) \mid c$。这一结论是判断不定方程与线性同余方程是否有解的直接依据。

### 2.4 扩展欧几里得的递推

由 $\gcd(a, b) = \gcd(b, a \bmod b)$ 且 $a \bmod b = a - \lfloor a/b \rfloor \cdot b$，代入整理得：

$$x_1 = y_2, \quad y_1 = x_2 - \lfloor a/b \rfloor \cdot y_2$$

边界为 $b = 0$ 时 $\gcd(a, 0) = a$，此时 $x = 1, y = 0$。

### 2.5 费马小定理与欧拉定理

费马小定理：设 $p$ 为质数且 $\gcd(a, p) = 1$，则

$$a^{p-1} \equiv 1 \pmod{p}$$

等价形式（不要求互质，对所有整数成立）：

$$a^p \equiv a \pmod{p}$$

欧拉定理（费马小定理的推广）：设 $\gcd(a, m) = 1$，则

$$a^{\varphi(m)} \equiv 1 \pmod{m}$$

用于"降幂"：当 $\gcd(a, m) = 1$ 时 $a^b \equiv a^{b \bmod \varphi(m)} \pmod{m}$。若不互质，可用**扩展欧拉定理**（当 $b \ge \varphi(m)$）：

$$a^b \equiv a^{\,b \bmod \varphi(m) + \varphi(m)} \pmod{m}$$

### 2.6 中国剩余定理（CRT）

求解方程组 $x \equiv a_i \pmod{m_i}$（$m_i$ 两两互质）。令 $M = \prod m_i$，$M_i = M / m_i$，$t_i \equiv M_i^{-1} \pmod{m_i}$，则

$$x \equiv \sum_{i=1}^{k} a_i \, M_i \, t_i \pmod{M}$$

### 2.7 BSGS 与原根

BSGS 求 $a^x \equiv b \pmod{p}$（$a \perp p$）的最小非负解：令 $m = \lfloor\sqrt{p}\rfloor$，$x = im - j$，转化为

$$(a^m)^i \equiv b \cdot a^j \pmod{p}$$

分"小步"枚举 $j$ 建表、"大步"枚举 $i$ 查表。原根：若 $g$ 模 $n$ 的阶恰为 $\varphi(n)$，则称 $g$ 为 $n$ 的原根。

## 逐行代码解析

本节给出各算法的**伪代码 / 思想步骤**，与任何具体 `.cpp` 实现无关。

### 3.1 模意义下的加减乘

```cpp
add(a, b, m) = ((a % m) + (b % m)) % m          // 加法：先取模再相加取模
sub(a, b, m) = ((a % m) - (b % m) + m) % m      // 减法：加 m 保证结果非负
mul(a, b, m) = (1LL * (a % m) * (b % m)) % m    // 乘法：中间转 long long 防溢出
```

### 3.2 快速幂（求 $a^b \bmod m$）

```cpp
fastpow(a, b, m):
    ans = 1, base = a % m
    while b > 0:
        if b & 1: ans = ans * base % m   // 当前二进制位为 1 就乘入
        base = base * base % m           // 底数平方倍增
        b >>= 1                          // 处理下一个二进制位
    return ans
```

### 3.3 扩展欧几里得（EXGCD）

```cpp
exgcd(a, b):
    若 b == 0: return (a, 1, 0)          // 边界：gcd(a,0)=a，解 x=1,y=0
    (d, x2, y2) = exgcd(b, a % b)        // 递归求解子问题
    x1 = y2
    y1 = x2 - (a / b) * y2               // 递推更新
    return (d, x1, y1)
```

返回 $d = \gcd(a, b)$，且满足 $a x_1 + b y_1 = d$。

### 3.4 费马小定理求逆元（$m$ 为质数）

```cpp
inv(a, m) = fastpow(a, m - 2, m)   // a^{-1} ≡ a^{m-2} (mod m)
```

### 3.5 中国剩余定理（CRT）

```cpp
crt(a[1..k], m[1..k]):              // m[i] 两两互质
    M = m[1] * m[2] * ... * m[k]    // 模数乘积
    res = 0
    for i = 1..k:
        Mi = M / m[i]               // 除 m[i] 外其余模数的乘积
        ti = inv(Mi, m[i])          // Mi 在模 m[i] 下的逆元
        res = (res + a[i] * Mi % M * ti % M) % M
    return res
```

### 3.6 BSGS

```cpp
bsgs(a, b, p):                       // 求 a^x ≡ b (mod p) 的最小非负解，a ⊥ p
    m = ceil(sqrt(p))
    for j = 0 .. m-1:                // Baby step：小步建表
        记录 (b * a^j mod p, j)，重复时取更大的 j
    for i = 1 .. m:                  // Giant step：大步查表
        t = (a^m)^i mod p
        若 t 在表中，return i * m - 表值
    return -1                        // 无解
```

### 3.7 原根判断（$p$ 为质数）

```cpp
is_primitive_root(a, p):
    若 fastpow(a, p-1, p) != 1: return false          // 不满足费马小定理
    对 p-1 的每个质因子 q:
        若 fastpow(a, (p-1)/q, p) == 1: return false  // 阶更小，非原根
    return true
```

## 复杂度分析

本知识点涵盖多个独立算法，整体无统一复杂度；各算法单独如下：

| 算法 | 时间复杂度 | 空间复杂度 |
|---|---|---|
| 模加减乘 | $O(1)$ | $O(1)$ |
| 快速幂 / 费马小定理求逆元 | $O(\log m)$ | $O(1)$ |
| 扩展欧几里得 | $O(\log\min(a, b))$ | $O(\log\min(a, b))$（递归栈） |
| 线性递推求 $1 \sim n$ 逆元 | $O(n)$ | $O(n)$ |
| 中国剩余定理 CRT | $O(k \log M)$ | $O(1)$（不含输入数组） |
| 扩展 CRT（exCRT） | $O(k \log m)$ | $O(1)$ |
| BSGS | $O(\sqrt{p})$ | $O(\sqrt{p})$（哈希表） |
| 原根判断 | $O(\sqrt{p} \log p)$ | $O(1)$ |

## 适用场景

- **模意义四则运算**：竞赛中答案对 $10^9+7$ 等大数取模时，加减乘直接按恒等式变形，除法转为乘逆元。
- **组合数 / 分数取模**：$C(n, k) = n!/(k!(n-k)!)$ 取模时，预处理阶乘及其逆元，实现 $O(1)$ 查询。
- **解线性同余方程 / 不定方程**：$ax \equiv b \pmod{m}$ 或 $ax + by = c$ 用 EXGCD 判断有解并求特解（依据裴蜀定理）。
- **解同余方程组**：模数两两互质用 CRT，不互质用 exCRT。
- **解高次同余方程**：$a^x \equiv b \pmod{p}$（$a \perp p$）用 BSGS，是离散对数问题的基础解法。
- **NTT 与原根**：快速数论变换需要模数的原根，离散对数相关问题也依赖"阶"与"原根"的概念。

## 常见陷阱与注意事项

1. **负数取模**：C++ 中 `-3 % 5 == -3`，而数学期望得到 $2$。规范写法是 `(x % m + m) % m` 保证非负。
2. **除法不能直接取模**：$\dfrac{a}{b} \bmod m$ 不能写成 $\dfrac{a \bmod m}{b \bmod m}$，必须乘逆元 $a \cdot b^{-1} \bmod m$，且要求 $\gcd(b, m) = 1$。
3. **费马小定理求逆元要求模数为质数**：模数为合数时 $a^{m-2}$ 未必等于 $a^{-1}$，应改用欧拉定理或 EXGCD（都要求 $\gcd(a, m) = 1$）。
4. **防溢出**：两个接近 $m$ 的数相乘可能超过 `int` 上限，中间结果需用 `long long`。
5. **CRT 要求模数两两互质**：不满足时用 exCRT 通过两两合并处理，合并中可能出现无解（$\gcd$ 不整除差时），需特判。
6. **BSGS 要求 $a \perp p$**：不互质时算法失效，需用扩展 BSGS（先约去 $\gcd$ 再求解）。
7. **原根存在条件**：正整数 $n$ 有原根当且仅当 $n$ 为 $1, 2, 4$、$p^e$ 或 $2p^e$（$p$ 为奇质数，$e$ 为正整数），并非所有模数都有原根。
8. **降幂的前提**：$\gcd(a, m) = 1$ 时才有 $a^b \equiv a^{b \bmod \varphi(m)} \pmod{m}$；不互质时改用扩展欧拉定理，并注意其成立条件 $b \ge \varphi(m)$。

## 对比与扩展

### 7.1 三种求逆元方法对比

| 方法 | 条件 | 时间复杂度 | 适用 |
|---|---|---|---|
| 费马小定理 + 快速幂 | $m$ 为质数 | $O(\log m)$ | 竞赛最常见（模数多为质数） |
| 欧拉定理 + 快速幂 | $\gcd(a,m)=1$ | $O(\sqrt{m} + \log m)$ | 合数模，需先求 $\varphi(m)$ |
| 扩展欧几里得 | $\gcd(a,m)=1$ | $O(\log m)$ | 合数模，或需同时求 GCD |

### 7.2 费马小定理与欧拉定理的关系

费马小定理是欧拉定理的**特例**：当 $m$ 为质数 $p$ 时 $\varphi(p) = p - 1$，欧拉定理 $a^{\varphi(p)} \equiv 1 \pmod{p}$ 即退化为 $a^{p-1} \equiv 1 \pmod{p}$。两者证明思路相同——对剩余系整体乘以 $a$，比较乘积后约去与模数互质的部分。

### 7.3 欧拉定理与扩展欧拉定理

欧拉定理只适用于 $\gcd(a, m) = 1$；扩展欧拉定理去掉了互质要求，代价是需要判断 $b$ 与 $\varphi(m)$ 的大小关系。在处理指数极长（如 $10^{100000}$）的降幂问题时，扩展欧拉定理是通用工具。

### 7.4 CRT 与 exCRT

| 维度 | CRT | exCRT |
|---|---|---|
| 前提 | 模数两两互质 | 无互质要求 |
| 方法 | 直接套公式（逆元 + 累加） | 两两合并，用 EXGCD 解线性同余 |
| 复杂度 | $O(k \log M)$ | $O(k \log m)$ |
| 无解 | 不存在（互质必有解） | 可能无解（合并时 $\gcd$ 不整除） |

### 7.5 BSGS 与原根的进一步扩展

- **扩展 BSGS**：当 $\gcd(a, p) \neq 1$ 时，先对两边约去公因子，转化为互质情形再套 BSGS。
- **原根与 NTT**：快速数论变换要求模数 $p$ 存在原根（常见如 $998244353$，原根为 $3$），利用原根的幂次可构造单位根替代复数域上的 FFT。
- **离散对数**：BSGS 是离散对数问题的分块解法，与"原根 / 阶"的概念一起，构成了有限域上对数运算的基础。

### 7.6 实现细节：解界、同余方程、BSGS 与原根求法

几个容易被忽略、却能避免爆 `long long` 或死循环的实现细节：

- **EXGCD 解界**：方程 $ax + by = \gcd(a, b)$ 由 EXGCD 直接返回的特解满足 $|x| \le b$、$|y| \le a$（对正数 $a, b$），量级不超过输入规模，中途一般不爆 `long long`；但解线性同余方程时若再乘上 $c/\gcd(a,b)$，量级会被放大，需注意。
- **线性同余方程 $ax \equiv b \pmod{m}$ 的模数缩放**：若 $g = \gcd(a, m) > 1$ 且 $g \mid b$，方程等价于 $(a/g)\,x \equiv (b/g) \pmod{m/g}$，共 $g$ 组模 $m$ 的解。不要拿原始模数 $m$ 直接套特解，否则会漏解或错解。
- **BSGS 归一化与特判**：先 `a %= p`（当 $a \equiv 0 \pmod{p}$ 时仅 $b \equiv 0$ 有解 $x = 1$，$b \not\equiv 0$ 无解）；且 $b \equiv 1 \pmod{p}$ 时最小解就是 $0$，应直接返回，避免由 `i*m - j` 反推出 $0$ 之外的伪解。
- **原根的求法**：先枚举求**最小原根** $g$，再由它生成全部原根——当 $g$ 为原根时，$g^k$ 是原根当且仅当 $\gcd(k, \varphi(p)) = 1$，故原根总数为 $\varphi(\varphi(p))$（$p$ 为质数时即 $\varphi(p-1)$）。

```
// 求最小原根 + 全部原根（p 为质数）
g = 使 is_primitive_root(g, p) 成立的最小正整数      // 用 3.7 的判断
for k = 1 .. p-1:
    若 gcd(k, p-1) == 1: 收集 g^k mod p 为一个原根   // 共 φ(p-1) 个
```

一句话总结：模运算是 OI 数论的公共语言——加减乘可自由变形，除法借逆元实现；逆元、EXGCD、费马 / 欧拉定理、CRT、BSGS 与原根分别解决了模意义下"做除法、解方程、解方程组、求对数"等核心问题。
