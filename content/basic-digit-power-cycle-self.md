---
id: basic-digit-power-cycle-self
title: '按数位分层扩张幂周期算法（自己写的 P1050循环）'
category: 基础算法
subcategory: 数位提升
tags: ["基础算法", "数位提升", "Hensel提升", "大整数", "幂周期", "P1050"]
timeComplexity: 'O(10k³)'
spaceComplexity: 'O(k)'
codePath: '基础算法(有价值的题目集)\按数位分层扩张幂周期算法\自己写的\Untitled1.cpp'
---

## 算法原理

本算法解决洛谷 P1050 问题：给定大整数 $A$（可达 $100$ 位）和 $k$，求最小的正整数 $t$，使得 $A^{t+1} \equiv A \pmod{10^k}$，即 $A$ 的幂次在末 $k$ 位上的循环长度。由于 $A$ 的位数远超基本数据类型，且 $t$ 本身可能是极大的数，无法用普通取模快速幂直接求解，需要采用逐位提升（Hensel lifting）策略。

算法的核心是按数位从低到高逐位确定循环节。维护一个不变量：`tg = A^ans`（在模 $10^k$ 意义下），且 $A \cdot \text{tg} \equiv A \pmod{10^i}$，即 `ans` 是前 $i$ 位的一个合法循环长度。初始时 `ans = 1`，`tg = A`，显然 $A \cdot A = A^2 \equiv A \pmod{10^0}$（模 $1$ 恒成立）。

提升到第 $i+1$ 位时，需要找到最小的 $j \in [1, 10]$ 使得 $A \cdot \text{tg}^j$ 在第 $i$ 位上与 $A$ 匹配。其数学依据是：由于 $A \cdot \text{tg} \equiv A \pmod{10^i}$，可设 $A \cdot \text{tg} = A + c \cdot 10^i$，则利用 $(1 + c \cdot 10^i)^j \equiv 1 + j \cdot c \cdot 10^i \pmod{10^{i+1}}$（二项式展开中高阶项被 $10^{i+1}$ 整除），$A \cdot \text{tg}^j$ 在第 $i$ 位的值随 $j$ 线性变化，$j \in [1, 10]$ 恰好覆盖该位的全部 $10$ 种可能。找到匹配的 $j$ 后，更新 $\text{ans} \leftarrow \text{ans} \times j$，$\text{tg} \leftarrow \text{tg}^j$，不变量提升到 $i+1$ 位。

实现上，`mul(a, b, k)` 是截断到末 $k$ 位的高精度乘法（相当于模 $10^k$），`add(j)` 实现大数 `ans` 乘以小整数 $j$ 的全精度运算（因为 `ans` 可能非常大，不能截断）。每位提升时先检查 $j=1$（即当前 `tg` 已使该位匹配），若匹配则直接 `continue` 不累乘；否则逐个尝试 $j=2, \ldots, 10$，用 `newtg` 从 `tg` 起反复乘 `tg` 递推得到 $\text{tg}^j$，找到匹配后更新。

## 核心公式

问题目标（求最小循环长度 $t$）：
$$A^{t+1} \equiv A \pmod{10^k}$$

不变量（提升到第 $i$ 位时成立）：
$$\text{tg} = A^{\text{ans}} \pmod{10^k}, \quad A \cdot \text{tg} \equiv A \pmod{10^i}$$

Hensel 提升的线性近似（设 $A \cdot \text{tg} = A + c \cdot 10^i$）：
$$(1 + c \cdot 10^i)^j \equiv 1 + j \cdot c \cdot 10^i \pmod{10^{i+1}}$$

逐位找最小 $j$：
$$j = \min \{ j \in [1, 10] : (A \cdot \text{tg}^j)_i = (A)_i \}$$

状态更新：
$$\text{ans} \leftarrow \text{ans} \times j, \quad \text{tg} \leftarrow \text{tg}^j \pmod{10^k}$$

## 逐行代码解析

```cpp
#include<bits/stdc++.h>
using namespace std;
struct num
{
	int v[105];                    // 低位在前存储十进制位
	num() { memset(v, 0, sizeof(v)); }
};

// 读入大整数，逆序存储（低位在前）
void read(num& tmp)
{
	char ch = getchar();
	int idx = 0;
	while (ch <= '9' && ch >= '0') {
		tmp.v[idx++] = ch - 48;
		ch = getchar();
	}
	reverse(tmp.v, tmp.v + idx);   // 逆序：低位在前
}

// 截断到末 k 位的高精度乘法（等价于模 10^k）
struct num mul(num& a, num& b, int k)
{
	num ans;
	for (int i = 0; i < k; i++)
		for (int j = 0; j < k; j++)
			if (i + j < k) ans.v[i + j] += a.v[i] * b.v[j];
	for (int i = 0; i < k; i++) {   // 统一进位
		ans.v[i + 1] += ans.v[i] / 10;
		ans.v[i] %= 10;
	}
	return ans;
}

int ans[200];  // 循环节长度（大整数，低位在前）
int n = 1;     // ans 的有效位数

// ans = ans * j（大数乘小整数，全精度不截断）
void add(int j)
{
	int tmp[200];
	memset(tmp, 0, sizeof(tmp));
	for (int i = 0; i < n; i++) tmp[i] = ans[i] * j;
	int i = 0;
	while (i < n || tmp[i] != 0) {  // 进位直到无残留
		tmp[i + 1] += tmp[i] / 10;
		tmp[i] %= 10;
		++i;
	}
	n = i;
	for (int i = 0; i < n; i++) ans[i] = tmp[i];
}

int main()
{
	int k;
	num A;
	read(A);
	scanf("%d", &k);

	num tg = A;        // tg = A^ans，初始 ans=1 时 tg=A
	ans[0] = 1;        // ans 初始为 1

	for (int i = 0; i < k; i++)      // 逐位提升
	{
		bool fg = false;
		num tmp = mul(A, tg, k);     // A * tg，检查 j=1 情形
		if (tmp.v[i] == A.v[i]) continue;  // 该位已匹配，无需累乘

		num newtg = tg;
		for (int j = 2; j <= 10; j++)
		{
			newtg = mul(newtg, tg, k);    // newtg = tg^j
			if (mul(A, newtg, k).v[i] == A.v[i])  // A * tg^j 第 i 位匹配
			{
				add(j);            // ans *= j
				fg = true;
				tg = newtg;        // tg = tg^j
				break;
			}
		}
		if (!fg) { printf("-1"); return 0; }  // 无解
	}

	// 输出 ans（逆序：高位在前）
	n -= 1;
	while (n >= 0) { putchar(ans[n] + 48); --n; }
	return 0;
}
```

**关键解析：**

- `struct num` 与 `v[105]`：低位在前的存储方式使竖式乘法的进位方向（从低位到高位）与数组下标递增方向一致，简化实现。`105` 位足以容纳 $k \le 100$ 的输入。
- `mul(a, b, k)` 的第三个参数 `k`：控制只保留末 $k$ 位（`if(i + j < k)`），等价于模 $10^k$ 运算。这是局部计算的核心——提升第 $i$ 位时只需关注末 $k$ 位，高位不影响结果。
- `if (tmp.v[i] == A.v[i]) continue`：先检查 $j=1$ 情形（即当前 `tg` 已使 $A \cdot \text{tg}$ 在第 $i$ 位匹配 $A$）。若匹配则该位无需扩展，`ans` 和 `tg` 不变，直接处理下一位。
- `newtg = mul(newtg, tg, k)`：从 `tg` 开始，每次乘以 `tg` 递推得到 $\text{tg}^2, \text{tg}^3, \ldots, \text{tg}^j$。这种逐次累乘避免了重复计算。
- `add(j)`：`ans` 是循环长度，可能极大（远超 $10^k$），必须用全精度大数存储。`add` 实现大数乘小整数并自动进位扩长，`while (i < n || tmp[i] != 0)` 确保所有进位都被处理。
- `n -= 1`：`add` 最后会使 `n` 多算一位（进位循环退出时 `i` 多走一步），输出前减 $1$ 修正。

## 复杂度分析

每位提升最多尝试 $10$ 个 $j$ 值，每次尝试需计算一次 `mul`（$O(k^2)$）和一次验证 `mul`（$O(k^2)$）。$k$ 位共需 $k$ 轮提升，因此总时间复杂度为 $O(10k \cdot k^2) = O(10k^3)$。对于 $k = 100$，约 $10^7$ 量级基本运算，可在时限内完成。`add` 的复杂度为 $O(|\text{ans}|)$（`ans` 的位数），远小于 $O(k^2)$，不构成瓶颈。空间上 `num` 结构体为 $O(k)$，`ans` 数组为 $O(200)$，总空间复杂度为 $O(k)$。

## 适用场景

超大整数（$100$ 位以上）的幂末位循环长度问题、同余周期问题、普通取模快速幂无法承载的大数场景。适用于需要逐位确定解的 Hensel 提升类问题。

## 常见陷阱与注意事项

1. **`j` 的范围必须是 $1 \sim 10$**：第 $i$ 位有 $10$ 种可能值（$0 \sim 9$），$j$ 从 $1$ 到 $10$ 恰好覆盖。若 $j$ 上界不足 $10$ 可能遗漏解，若超过 $10$ 则产生冗余计算（$j$ 和 $j+10$ 在该位等价）。
2. **$j=1$ 的特殊处理**：当该位已匹配时 `continue` 不累乘。若误将 $j=1$ 也执行 `add(1)`，虽然 `ans * 1 = ans` 不改变值，但 `tg = tg^1 = tg` 也不变，逻辑正确但多余。关键是不能遗漏 $j=1$ 的检查。
3. **`tg` 和 `newtg` 的引用安全**：`newtg = mul(newtg, tg, k)` 中 `newtg` 同时作为输入和输出。由于 `mul` 返回新对象（值传递），不会出现引用被修改后再读取的问题。但若改为引用传递则需注意。
4. **`add` 中 `n` 的修正**：`add` 的进位循环 `while (i < n || tmp[i] != 0)` 在最后一次进位为零时仍会执行一轮使 `i` 多走一步，导致 `n` 偏大 $1$。输出前 `n -= 1` 修正此偏差。
5. **无解情况**：若某位 $j=1 \sim 10$ 均无法匹配（当 $A$ 与 $10$ 不满足特定条件时），返回 $-1$。题目通常保证有解，但在推广场景中需处理。
6. **大整数读入的逆序**：`read` 先正序读入再 `reverse`，确保低位在前。若忘记逆序则乘法进位方向错误，结果完全不对。

## 对比与扩展

与参考模板版本相比，本实现将 $j=1$ 情形单独提取为 `continue`，逻辑更清晰但代码稍长。模板版本将 $j=1$ 纳入统一循环（`for(j=1;j<=10;j++)`），更简洁但语义略隐晦。两种写法在数学上完全等价。与暴力枚举循环节（枚举 $t = 1, 2, \ldots$ 直到 $A^{t+1} \equiv A$）相比，逐位提升将指数级搜索降为多项式级。扩展方向包括：处理 $A$ 与 $10$ 不互质的情形（需分离 $2$ 和 $5$ 的因子）、推广到任意进制基 $B$ 的幂周期问题、以及利用 Euler 定理和 Carmichael 函数分析循环节的理论上界。
