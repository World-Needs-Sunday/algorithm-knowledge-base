---
id: math-block-division
title: '整除分块（数论分块）'
category: 数学
subcategory: 数论基础
subSubcategory: '数论函数'
tags: [数学, 整除分块, 数论分块, 根号复杂度, 余数求和]
timeComplexity: 'O(√n)'
spaceComplexity: 'O(1)'
level: B
verified: '2026-10-21'
codePath: '数学\数论基础\整除分块\Untitled1.cpp'
---

## 算法原理

**一句话结论**：数论分块（整除分块）的核心思想只有一句——**按某个整除结果分段，段内批量处理，段与段之间用除法跳转**。它把 $\sum_{i=1}^{n}\left\lfloor\dfrac{n}{i}\right\rfloor$ 这类从 $O(n)$ 的逐项求和降到 $O(\sqrt{n})$。

**解决的问题**：计算

$$S(n)=\sum_{i=1}^{n}\left\lfloor\frac{n}{i}\right\rfloor$$

**关键观察：单调 + 大量重复值**。$\lfloor n/i\rfloor$ 随 $i$ 增大单调不增，并且大量相邻 $i$ 取同一个值。例如 $n=10$ 时十个取值是 $10,5,3,2,2,1,1,1,1,1$，只有 5 个不同的段：$[1,1]$、$[2,2]$、$[3,3]$、$[4,5]$、$[6,10]$。只要能把每段一次算掉，总工作量就从 10 降到 5；$n$ 很大时这个差距是 $O(n)$ 与 $O(\sqrt n)$ 的差距。

**核心推导：段右端点怎么求**。设当前段的左端点为 $L$，段内值为 $v=\lfloor n/L\rfloor$，右端点为 $R$。段内值恒为 $v$ 即 $v\le n/R<v+1$，乘 $R$ 得 $vR\le n<(v+1)R$，整理为 $n/(v+1)<R\le n/v$。取 $R$ 的最大整数值：

$$R=\left\lfloor\frac{n}{v}\right\rfloor$$

下一个不同值的段从 $R+1$ 开始。整个循环就是：`v = n / l` → `r = n / v` → 段内乘长度 → `l = r + 1`。

**为什么这样做是对的**：段内值恒定由上式保证（不漏）；$l$ 每轮跳到 $r+1$ 且 $r\ge l$ 严格递增，最终越过 $n$ 退出（不重、有限步）。所以每个 $i$ 恰被统计一次，结果与逐项求和完全一致。

**为什么段数只有 $O(\sqrt n)$**：当 $i\le\sqrt n$ 时这样的 $i$ 至多 $\sqrt n$ 个；当 $i>\sqrt n$ 时 $\lfloor n/i\rfloor<\sqrt n$，取值至多 $\sqrt n$ 种。两种情况段数都不超过 $O(\sqrt n)$。

## 核心公式 / 状态定义与转移方程

**连续型（fn1）——段内值是常数**：

- 段内值：$v=\left\lfloor\dfrac{n}{l}\right\rfloor$
- 段右端：$r=\left\lfloor\dfrac{n}{v}\right\rfloor$
- 段内贡献：$v\cdot(r-l+1)$，即「值 × 长度」

$$S(n)=\sum_{\text{每段}} v\cdot len$$

**余数型（fn2）——段内是 $i$ 的一次函数**。利用恒等式 $n\bmod i=n-i\lfloor n/i\rfloor$：

$$T(n)=\sum_{i=1}^{n}(n\bmod i)=\sum_{i=1}^{n}n-\sum_{i=1}^{n}i\left\lfloor\frac{n}{i}\right\rfloor=n^2-\sum_{i=1}^{n}i\left\lfloor\frac{n}{i}\right\rfloor$$

分块方式与 fn1 相同，但段内 $n\bmod i=n-iv$ 随 $i$ 线性变化，必须用等差数列求和：设 $len=r-l+1$、$sum_i=\dfrac{(l+r)\cdot len}{2}$，段内贡献为

$$n\cdot len-v\cdot sum_i$$

**倍数型（fn3）——沿 $k$ 的幂分层**。把 $1\sim n$ 中的数按「根 $a$ 不被 $k$ 整除」分链：根 $a$ 的链是 $a,\ ak,\ ak^2,\dots\le n$，链长 $L(a)=\max\{t:ak^t\le n\}+1$。设 $U_t=\left\lfloor n/k^t\right\rfloor$，由整除恒等式 $\lfloor\lfloor n/a\rfloor/b\rfloor=\lfloor n/(ab)\rfloor$ 得递推：

$$U_{t+1}=\left\lfloor\frac{U_t}{k}\right\rfloor$$

链长恰为 $t+1$ 的根满足 $U_{t+1}<a\le U_t$（连续候选区间），再从中扣掉 $k$ 的倍数：区间内 $k$ 的倍数个数 = $\lfloor U_t/k\rfloor-\lfloor U_{t+1}/k\rfloor=U_{t+1}-U_{t+2}$。于是链长为 $t+1$ 的根个数是**二阶差分**：

$$cnt_{t+1}=(U_t-U_{t+1})-(U_{t+1}-U_{t+2})=U_t-2U_{t+1}+U_{t+2}$$

代码变量替换 $U=U_t,\ D=U_{t+1},\ D/k=U_{t+2}$，得 `cnt = U - 2*D + D/k`；下一层 `U = D`、`++len`。层数只有 $O(\log_k n)$。

**手算示例 1（连续型，$n=10$）**：

| l | v | r | 段 | 贡献 |
|---|---|---|---|---|
| 1 | 10 | 1 | [1,1] | 10 |
| 2 | 5 | 2 | [2,2] | 5 |
| 3 | 3 | 3 | [3,3] | 3 |
| 4 | 2 | 5 | [4,5] | 2×2=4 |
| 6 | 1 | 10 | [6,10] | 1×5=5 |

合计 $10+5+3+4+5=27$，与逐项求和的 $10+5+3+2+2+1+1+1+1+1=27$ 一致。经本代码实测输出 27。

**手算示例 2（余数型，$n=10$）**：

| l | v | r | len | sum_i | 段内贡献 |
|---|---|---|---|---|---|
| 1 | 10 | 1 | 1 | 1 | 10−10=0 |
| 2 | 5 | 2 | 1 | 2 | 10−10=0 |
| 3 | 3 | 3 | 1 | 3 | 10−9=1 |
| 4 | 2 | 5 | 2 | 9 | 20−18=2 |
| 6 | 1 | 10 | 5 | 40 | 50−40=10 |

合计 13，与 $10\%1+\dots+10\%10=13$ 一致。经本代码实测输出 13。

**手算示例 3（倍数型，$n=10,k=2$）**：根（不被 2 整除的数）为 $1,3,5,7,9$，链分别为 `1,2,4,8`（长4）、`3,6`（长2）、`5,10`（长2）、`7`（长1）、`9`（长1）。代码逐层输出 `len cnt`：层 1 的 $cnt=10-10+2=2$、层 2 的 $5-4+1=2$、层 3 的 $2-2+0=0$、层 4 的 $1-0+0=1$，即链长 1 有 2 个、链长 2 有 2 个、链长 4 有 1 个，与手数一致；所有 cnt 之和 $5=n-\lfloor n/k\rfloor$（sanity check）。

## 逐行代码解析

代码拆成 4 个模块，按顺序拼接即完整程序。

### 模块 1 头文件与连续型 fn1

```cpp
#include<bits/stdc++.h>
using namespace std;
//求取sum(i = 1 ~ n) floor(n / i);   (连续型分块)
long long fn1(long long n)
{
	/*
	可以发现在求取的过程中有很多重复的数字。并且具有单调性
	假设一个相同值的区间是[L,R];则floor(n / L) == floor(n / R)
	设 k = floor(n / L)。则:  k <= n / R < k + 1 
	所以  k * R <= n < (k + 1) * R
	n / (k + 1) <  R <= n / k ,  R = floor(n / k)
	第一个起点L = 1,下一个不同值的区间为R + 1
	*/
	long long ans = 0;
	for(long long l = 1,r;l <= n;l = r + 1)
	{
		long long k = n / l;
		r = n / k;
		ans += k * (r - l + 1);
	}
	return ans;
}


```

- 注释块完整给出了推导链：段内值恒为 $k=\lfloor n/L\rfloor$，由 $kR\le n<(k+1)R$ 推出 $R=\lfloor n/k\rfloor$，下一段从 $R+1$ 开始。
- `for(long long l = 1, r; l <= n; l = r + 1)`：`l` 从 1 出发、每轮跳到段右端加一，保证全区间 $[1,n]$ 不重不漏。
- `k = n / l`（段内值）、`r = n / k`（段右端）：一次除法确定整段范围，这是 O(√n) 的关键。
- `ans += k * (r - l + 1)`：段内值 × 段长度，一次处理整段。

### 模块 2 余数型 fn2

```cpp
//求取sum(i = 1 ~ n) n % i;    (余数型分块)
long long fn2(long long n)
{
	/*
	sum(i = 1 ~ n) n % i 可以看成  sum(i = 1 ~ n) n - floor(n / i) * i;
	这一部分：sum(i = l ~ r) floor(n / i) * i 的求和可以写成 len * (l + r) / 2 * k;
	后面过程与前面差不多 
	*/
	long long ans = 0;
	for(long long l = 1,r;l <= n;l = r + 1)
	{
		long long k = n / l;
		r = n / k;
		long long len = r - l + 1;
		ans += len * n - len * (l + r) / 2 * k;  
	} 
	return ans;
}


```

- 把 $n\bmod i$ 拆成 $n-i\lfloor n/i\rfloor$：前半段每块贡献 $len\cdot n$，后半段是「等差数列和 × 段内值」。
- `len * (l + r) / 2 * k`：即 $sum_i \cdot k$，其中 $len(l+r)/2$ 是段内 $i$ 之和（连续整数和）。
- 注意此处不能写成 `(n % l) * len`——段内 $n\bmod i$ 随 $i$ 线性变化、不是常数，那样会少算（见陷阱）。
- 本写法的溢出边界：$len\cdot(l+r)$ 先乘，$n\ge10^{12}$ 时可能溢出；更稳的写法是「先除 2 再乘」（$len$ 与 $l+r$ 中恰有一个偶数），属于实现细节，本库不代改代码。

### 模块 3 倍数型 fn3

```cpp
//倍增型分块 不具体题目了，就把每个长度的个数打印一下吧。 
void fn3(long long n,long long k)
{
	/*
	不知道怎么写，给ai了 
	把 1..n 中的数按“根 a 不被 k 整除”分链：
		根 a 的链为 a, a*k, a*k^2, ...
		链长 L(a) = 满足 a*k^t <= n 的最大 t + 1

	设 U_t = floor(n / k^t)，则:  U_0 = n,  U_{t+1} = floor(U_t / k)
	（因为 floor(floor(n/A)/B) = floor(n/(A*B))，取 A=k^t, B=k 即得）

	链长恰好为 t+1 的根 a 满足:
		a*k^t <= n < a*k^{t+1}
	两边除以 k^t:
		a <= n/k^t 且 a > n/k^{t+1}
	对整数 a:
		U_{t+1} < a <= U_t
	所以候选根是连续区间 (U_{t+1}, U_t]

	但根必须 k ? a，要从该区间里排除 k 的倍数:
	区间长度 = U_t - U_{t+1}
	其中 k 的倍数个数 = floor(U_t/k) - floor(U_{t+1}/k)
					= U_{t+1} - U_{t+2}
	（因为 floor(U_t/k) = U_{t+1}，floor(U_{t+1}/k) = U_{t+2}）

	所以链长为 t+1 的根个数:
		cnt = (U_t - U_{t+1}) - (U_{t+1} - U_{t+2})
			= U_t - 2*U_{t+1} + U_{t+2}

	代码变量替换:  U = U_t,  D = U_{t+1},  D/k = U_{t+2}
		cnt = U - 2*D + D/k
	下一层:  U = D,  len++（链长递增）
	层数只有 O(log_k n)，非常快
	注意: k = 1 时 U 不降，会死循环，必须特判
	*/
	
	if (k == 1) 
	{
        cout << "k=1 无意义（所有数都能被1整除，根不存在）\n";
        return;
    }
	
	long long U = n;
	int len = 1;
	while(U > 0)
	{
		long long D = U / k;
		long long cnt = U - 2 * D + D / k;
		
		cout << len << ' ' << cnt << '\n'; 
		
		U = D;
		++len;
	}
}

```

- `if (k == 1)` 必须特判：$k=1$ 时 $D=U/1=U$ 恒不变，循环永不前进（死循环）；语义上所有数都能被 1 整除，「根」不存在。
- `D = U / k` 即 $U_{t+1}=\lfloor U_t/k\rfloor$：由嵌套整除恒等式保证精确，无需考虑余数。
- `cnt = U - 2 * D + D / k` 即二阶差分：整段长度 $U-D$ 减去「其中 $k$ 的倍数个数」$D-D/k$。
- `U = D; ++len`：进入下一层（链长加一），层数只有 $O(\log_k n)$。
- 打印每层「链长 个数」，可加 sanity check：所有 cnt 之和应等于 $n-\lfloor n/k\rfloor$。

### 模块 4 主函数

```cpp
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	long long n,k;
	cin >> n >> k;
	cout << fn1(n) << '\n';
	cout << fn2(n) << '\n';
	fn3(n,k); 
	return 0;
}
```

- 读入 $n,k$ 后依次输出 fn1、fn2 结果与 fn3 的逐层统计，方便三种模板一起验证。
- `ios::sync_with_stdio(false)` 关闭同步，大输入下显著提速。

## 复杂度分析

- **连续型 / 余数型**：段数 $O(\sqrt n)$（$i\le\sqrt n$ 与 $i>\sqrt n$ 两段论证），每段 $O(1)$，总时间 $O(\sqrt n)$；只用常数个变量，空间 $O(1)$。
- **倍数型**：层数 $O(\log_k n)$（$n=10^{18},k=2$ 也才 60 层），时间 $O(\log_k n)$，空间 $O(1)$。
- **数值量级**：$S(n)\approx n\ln n$（$n=10^{12}$ 时约 $2.8\times10^{13}$，`long long` 安全；$n\ge10^{18}$ 需 `__int128`）；$T(n)\approx n^2/2$（$n=10^9$ 时约 $5\times10^{17}$ 尚安全，$n\ge10^{10}$ 需 `__int128` 或取模）。fn2 中间量 $len\cdot(l+r)$ 在 $n\ge10^{12}$ 时先乘会溢出（见陷阱）。

## 适用场景

- **$\sum f(\lfloor n/i\rfloor)$ 型**：出现「对整除结果的函数求和」就用连续型，段内贡献换成 $f(v)\cdot len$。
- **$\sum (n\bmod i)$、$\sum i\lfloor n/i\rfloor$ 型**：余数型，段内按等差数列处理。
- **$k$ 的倍数 / 链 / 幂次分层**：「选 $x$ 不能选 $kx$」、按 $k$ 的幂分层计数的题用倍数型；这是「根 + 链」结构的通用模板。
- **双键与反演前置**：$\sum\lfloor n/i\rfloor\lfloor m/i\rfloor$ 右端点取 `min(n/(n/l), m/(m/l))`，是莫比乌斯反演、杜教筛的常见前置。
- **不要用 / 慎用**：$n$ 很小逐项求和更简单；多次不同 $n$ 的询问注意每种 $n$ 单独分块；需要上取整时先转成 $\lceil n/i\rceil=\lfloor(n-1)/i\rfloor+1$ 再套下取整模板。

## 常见陷阱与注意事项

1. **右端点写成 `n/(v+1)`**：正确是 `r = n/v`；`n/(v+1)` 会取到更小的右端，漏掉段尾元素。
2. **忘记 `l = r + 1`**：死循环。每轮必须更新左端点。
3. **`int` 不够用**：`l, r, v, ans` 全部 `long long`；$n\ge10^{18}$ 时答案还要 `__int128`。
4. **余数型段内误当常数**：`ans += (n % l) * len` 是错的——段内 $n\bmod i$ 是 $i$ 的一次函数，必须用等差数列和。
5. **`len*(l+r)/2*v` 先乘溢出**：先算 $len\times(l+r)$，$n\ge10^{12}$ 时乘积超 `long long`；改成先除 2 再乘（$len$ 与 $l+r$ 必有一个偶数）。
6. **取模除法**：模意义下除以 2 要用 `inv2 = (MOD + 1) / 2`（奇质数模），不能直接 `/2`。
7. **倍数型 `k == 1` 没特判**：$U$ 恒不变、死循环；语义上也没有根，应直接返回特值。
8. **倍数型用浮点**：`D/k` 若转成 `double` 大数会掉精度，必须整数除法；幂次计数用快速幂而不是 `pow`。
9. **倍数型方案数用加法**：求「方案数」时每层 $cnt$ 个根的贡献是乘上 $f(len)^{cnt}$（初值 1），不是加法；只求和时才用加法。
10. **变式里漏了整体量级**：$T(n)\approx n^2/2$ 增长极快，大 $n$ 必须取模或 `__int128`。

## 对比与扩展

**三种分块对比**：

| 维度 | 连续型 fn1 | 余数型 fn2 | 倍数型 fn3 |
|---|---|---|---|
| 分块键 | ⌊n/i⌋ | ⌊n/i⌋ | ⌊n/k^t⌋ |
| 段形状 | [l,r] | [l,r] | (U_{t+1}, U_t] |
| 段内性质 | 值恒定 | i 的一次函数 | 层恒定，需筛 k 倍数 |
| 段推进 | r = n/v，l = r+1 | 同左 | U = U/k |
| 段内处理 | v·len | n·len − v·sum_i | U−2D+D/k（二阶差分） |
| 复杂度 | O(√n) | O(√n) | O(log_k n) |

**一句话对比**：连续型段内是常数、乘长度；余数型段内是一次函数、等差数列；倍数型段内还有筛选、整段减掉排除项即二阶差分。

**识别信号三问**：出现 $\sum f(\lfloor n/i\rfloor)$ → 连续型；出现 $n\bmod i$ 或 $\sum i\lfloor n/i\rfloor$ → 余数型；出现 $k$ 的倍数、链、幂、$k\nmid a$ → 倍数型。

**扩展方向**：

- **双键分块**：$r=\min(\lfloor n/v_n\rfloor,\ \lfloor m/v_m\rfloor)$，莫比乌斯反演标配。
- **上取整分块**：$\lceil n/i\rceil=\lfloor(n-1)/i\rfloor+1$ 转下取整。
- **二次分块**：外层枚举 $\lfloor n/i\rfloor$，内层再分块求 $\sum i$、$\sum i^2$。
- **莫比乌斯反演 + 分块**：预处理 $\mu$ 前缀和 $M$，段内贡献 $(M(r)-M(l-1))\cdot\lfloor n/l\rfloor\cdot\lfloor m/l\rfloor$。
- **杜教筛 + 分块**：前缀和无法直接预处理时，杜教筛算 $\sum f(i)$，外层继续分块，复杂度 $O(n^{2/3})$。