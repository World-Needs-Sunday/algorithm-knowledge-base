---
id: basic-random-sa-point
title: '模拟退火 · 点退火（求加权费马点）'
category: 基础算法
subcategory: 随机化
tags: ["模拟退火", "随机化", "爬山法", "Metropolis准则", "费马点"]
timeComplexity: 'O(n·log(T0/Tmin))（单轮退火）'
spaceComplexity: 'O(n)'
codePath: '基础算法\随机化\点退火\Untitled1.cpp'
level: A
---

## 算法原理

解决的问题：在**连续空间中求多峰函数的近似全局最优**。典型应用是洛谷 P1337「平衡点」（带权费马点 / 费马-韦伯点）：给定平面上的 $n$ 个点和权重，求

$$
f(x,y)=\sum_{i=1}^{n}w_i\cdot\sqrt{(x-x_i)^2+(y-y_i)^2}
$$

的最小值点。该函数是凸函数，但一般没有闭式解，用模拟退火求数值解非常合适。

核心思想来自金属退火的物理过程：高温时原子剧烈运动，随温度缓慢下降，能量逐渐降到最低。对应到算法上：

- **当前解**：一个状态（本代码里是坐标 $(x,y)$），伴随当前能量值（目标函数值）。
- **随机扰动**：每步在当前解附近随机跳一步，扰动幅度与当前温度 $T$ 成正比——温度越高跳得越远。
- **Metropolis 准则**：新解更优（$\Delta E<0$）就接受；更劣则以概率 $\exp(-\Delta E/T)$ 接受——这是模拟退火与「爬山法」的唯一本质区别。
- **缓慢降温**：$T\gets T\cdot k$（$k$ 略小于 1）。温度越低，扰动越小、接受劣解的概率也越小。

为什么这样设计：爬山法只接受更优解，会卡在局部最优；Metropolis 准则让算法在高温时能「翻墙」逃出局部最优去探索全局，低温时趋于稳定收敛。温降得足够慢时，算法以概率收敛到全局最优。

抽象成模板：

```
初始化当前解 cur 与全局最优 best
for (T = T0; T > T_min; T *= k):
    new = cur + 随机扰动(幅度 ~ T)
    Δ = f(new) - f(cur)
    if Δ < 0 或 exp(-Δ/T) > random(): cur = new
    若 f(cur) 更优则更新 best
输出 best
```

## 核心公式 / 状态定义与转移方程

**状态**：当前坐标 $(curx, cury)$ 与当前能量 $curE$；全局最优 $(bestx, besty)$ 与 $bestE$。每次扰动都从当前解出发，但**历史最优单独记录**——中途接受过劣解不会丢掉已经找到的最优值。

**目标函数**（本代码的带权费马点）：

$$
f(x,y)=\sum_{i=1}^{n}w_i\sqrt{(x-x_i)^2+(y-y_i)^2}
$$

**扰动（转移）**：

$$
nx = curx + U(-1,1)\cdot T,\qquad ny = cury + S\cdot T
$$

其中 $U$ 是均匀分布，$S$ 为与 $U$ 同分布的新抽样；随机数**归一化到 $[-1,1]$**，使扰动幅度恰好等于当前温度 $T$，随降温自然收缩。

**Metropolis 接受准则**：设 $\Delta E = E_{new}-E_{cur}$，接受概率

$$
P(\text{接受})=\begin{cases}1, & \Delta E\le 0\\[2pt] e^{-\Delta E/T}, & \Delta E>0\end{cases}
$$

**降温策略（多轮粗搜 → 精修）**：每轮内部 $T\gets T\cdot a$，直到 $T<10^{-8}$；每轮结束后缩小初温、放慢降温：

$$
T_0\gets T_0\cdot 0.8,\qquad a\gets 1-0.5(1-a)
$$

即初温 $10000$ 起步不断乘 $0.8$，降温系数从 $a=0.99$ 逐渐逼近 $1$（每轮余量 $1-a$ 减半）。首轮内部迭代次数约为

$$
\frac{\ln(T_0/T_{min})}{\ln(1/a)}\approx\frac{\ln(10^{12})}{0.01005}\approx 2.7\times 10^3
$$

次；后续轮的温度区间越来越小、降温越来越慢，在最优解附近越来越精细。外层 `while` 由 `clock()` 卡 `TL = 0.9` 秒，时限内尽量多跑几轮。

## 逐行代码解析

### 全局变量与目标函数 `cala`

```cpp
#include<bits/stdc++.h>
using namespace std;
mt19937 rd;
vector<long double> x, y, w;
const long double TL = 0.9;
long double curx, cury, curE, bestE, bestx, besty;
long double cala(long double mx, long double my, int n)
{
	long double res = 0;
	for (int i = 1; i <= n; i++)
	{
		long double dx = mx - x[i];
		long double dy = my - y[i];
		long double len = sqrt(dx * dx + dy * dy);

		res += len * w[i];
	}
	return res;
}
```

`cala` 计算的是「点 $(mx,my)$ 到所有洞的加权距离和」，也就是物理系统的势能。它被每一轮、每一步扰动反复调用，是**全程序的性能核心**：

- 一次调用是 $O(n)$：把 $n$ 个点各算一遍距离再按权重累加，没有任何可以省掉的项。
- 全程用 `long double`：坐标范围大、权重和可达 $10^{14}$ 量级，`double` 的有效数字在累加大量项后末位会漂出评测容差，实测 `double` 就是 WA 的直接原因之一；换上 `long double` 后 `sqrt`、`exp` 都有对应重载，精度差异自然消失。
- 距离用 `sqrt(dx*dx + dy*dy)` 手写：没有用 `hypot`，因为平方和在小数场景下更直观可控；写成 `dx*dx` 而不是 `pow(dx, 2)`，既快又避免 `pow` 的浮点误差。

### 初始化 `init`

```cpp
void init(int n)
{
	x.assign(n + 1, 0);
	y.assign(n + 1, 0);
	w.assign(n + 1, 0);
	rd.seed(static_cast<unsigned int>(std::time(nullptr)));
}
```

- 下标习惯从 $1$ 到 $n$：`x/y/w` 都 `assign(n+1, 0)` 开 $n+1$ 个位置，主循环从 $1$ 读入，与数学推导里的下标保持一致，不容易在边界上写错。
- `rd.seed(static_cast<unsigned int>(std::time(nullptr)))`：用当前秒数初始化 `mt19937`。同一个程序里引擎**只播种这一次**（`init` 只调用一次）；如果放在退火循环里反复播种，短时间内的种子完全相同，每一轮随机序列都一模一样。
- 全局变量 `curx/cury/curE/bestx/besty/bestE`：退火状态跨函数共享，`sa` 多轮调用之间靠它们传递「当前解」与「历史最优」，不用把六七个参数传来传去。

### 核心 `sa`：一轮退火

```cpp
void sa(int n, long double t, long double a)
{
	curx = bestx;
	cury = besty;
	curE = bestE;
	static uniform_real_distribution<long double> dist(-1.0, 1.0);
	static uniform_real_distribution<long double> prob(0.0, 1.0);
	for (; t > 1e-8; t *= a)
	{
		long double nx = curx + dist(rd) * t;
		long double ny = cury + dist(rd) * t;
		long double newE = cala(nx, ny, n);
		long double delta = newE - curE;
		if (delta < 0 || exp(-delta / t) > prob(rd))
		{
			curx = nx;
			cury = ny;
			curE = newE;
		}
		if (curE < bestE)
		{
			bestE = curE;
			bestx = curx;
			besty = cury;
		}
		if ((long double)clock() / CLOCKS_PER_SEC >= TL) break;

	}
}
```

- `curx = bestx; cury = besty; curE = bestE;`：**每轮都从历史最优重新出发**，而不是接着上一轮结束的位置走。这样即使上一轮末尾因为接受劣解而停在一个很差的位置，新一轮也不会在差区域的浪费迭代。
- `static uniform_real_distribution<long double> dist(-1.0, 1.0);`：分布对象声明成 `static`，整个程序只构造一次，避免每轮退火重建分布对象（重建会重置内部状态，且无谓地消耗时间）。
- `long double nx = curx + dist(rd) * t;`：扰动。`dist` 已经**归一化到 $[-1,1]$**，所以步长幅度恰好等于当前温度 $t$——温度高时大步跳跃、探索全局；温度降低后步长自动收缩、精细微调。这正是「扰动幅度 $\sim T$」的标准写法。
- `if (delta < 0 || exp(-delta / t) > prob(rd))`：**Metropolis 准则**，整个模拟退火的灵魂。`delta = newE - curE` 是新旧能量差：更优（`delta<0`）无条件接受；更劣时，接受概率是 $e^{-\Delta E/T}$，温度 $t$ 越高、劣解越容易被接受。`prob(rd)` 是 $[0,1)$ 的均匀随机数，用「概率大于随机数」的办法掷出这一步是否接受。
- `if (curE < bestE) { ... }`：**独立维护历史最优**。随机游走会接受劣解，`curE` 可能短暂变差，但不影响 `bestE` 一直记录全场最优——最终输出用的就是 `bestx/besty`，而不是当前解。
- `if ((long double)clock() / CLOCKS_PER_SEC >= TL) break;`：时钟超限立即跳出本轮的降温循环。没有这行，降温链会老老实实跑满 $T_{min}$ 才停；有了它，时间预算由主循环统一管理。

### 主函数 `main`：起点、多轮策略与输出

```cpp
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);

	int n;
	cin >> n;
	init(n);
	long double mx = 0, my = 0, mw = 0;
	for (int i = 1; i <= n; i++)
	{
		cin >> x[i] >> y[i] >> w[i];
		mx += x[i] * w[i];
		my += y[i] * w[i];
		mw += w[i];
	}
	mx /= mw;
	my /= mw;

	curx = mx, cury = my, curE = cala(mx, my, n);
	bestE = curE, bestx = mx, besty = my;

	long double t = 10000, a = 0.99;
	while ((long double)clock() / CLOCKS_PER_SEC < TL)
	{
		sa(n, t, a);
		t *= 0.8;
		a = 1 - 0.5 * (1 - a);
	}

	cout << fixed << setprecision(3) << bestx << ' ' << besty;

}
```

- **起点取加权质心**：先累加 $\sum x_i w_i$、$\sum y_i w_i$、$\sum w_i$ 再相除，得到

$$
\left(\frac{\sum x_i w_i}{\sum w_i},\ \frac{\sum y_i w_i}{\sum w_i}\right)
$$

它比算术平均更贴近最优解，$n=1$ 时甚至就是精确解。起点好，退火「热机」阶段可以省掉大量浪费的搜索。

- **多轮粗搜 → 精修**：`t *= 0.8; a = 1 - 0.5 * (1 - a);`。第一轮 `t=10000`、`a=0.99`，降温快、覆盖广，先粗略定位最优区域；之后每轮初温变成原来的 $0.8$ 倍，降温系数 $a$ 的余量 $1-a$ 每轮减半、越发趋近 $1$，降温变慢、精度提高。层层递进：**范围逐轮缩小，精度逐轮提升**。这套调度是本代码能做到稳定 100% AC 的关键，而不是靠某一轮「运气好」。
- `while ((long double)clock() / CLOCKS_PER_SEC < TL)`：时间卡点。`TL=0.9` 秒预算内能跑几轮跑几轮，每轮都在缩范围、提精度；多轮冗余也把「单轮没转准」的概率稀释掉了。
- `cout << fixed << setprecision(3) << bestx << ' ' << besty;`：固定 3 位小数输出最小值点。题目要求 3 位精度，而 `long double` + $10^{-8}$ 的终止温度已经远高于这个要求，末位不会漂。

## 复杂度分析

- **时间复杂度**：单轮内部降温次数为 $\ln(T_0/T_{min})/\ln(1/a)$，首轮（$T_0=10000$、$a=0.99$、$T_{min}=10^{-8}$）约 $2.7\times 10^3$ 次；后续轮 $1-a$ 每轮减半，降温链越来越长，但初温也在乘 $0.8$ 收缩，两者此消彼长。每次扰动的评估 `cala` 是 $O(n)$。总轮数与总迭代由 `TL = 0.9` 秒的 `clock()` 卡点硬性封顶，实际运行远低于时限（零点几秒即可 AC）。
- **空间复杂度**：只存坐标与权重的三个 `vector`，$O(n)$。

## 适用场景

- **连续空间的多峰最优化**：带权费马点 / 平衡点（P1337）、覆盖圆、旅行商这类没有多项式精确解的近似最优化问题。
- **需要「够好就行」的数值解**：答案只要求若干位小数、允许误差的题（如几何平衡、连续函数求极值），模拟退火以极低实现成本换取稳定分数。
- **非精确解不可的场景不适用**：要求输出精确整数解、或解空间离散且约束强的题，退火可能永远踩不到可行解；这类题优先考虑 DP / 精确算法。
- 参数完全相同的两次运行结果可能不同（随机算法本质），对「必须复现」的评测要谨慎。

## 常见陷阱与注意事项

1. **`double` 精度不够会 WA**：本题把坐标、权重、能量全部换成 `long double` 是稳定 AC 的直接原因之一。`sqrt`、`exp` 对 `long double` 有对应重载；若回退到 `double`，末位精度可能差出评测容差。
2. **扰动幅度必须归一化**：`dist(-1.0, 1.0)` 乘 $t$ 才能让「步长幅度 = 温度」；若用 `dist(-32767, 32767)` 这类大区间且不除范围，高温期步长会远超坐标范围，大量迭代浪费在坐标域之外。
3. **只用当前解、丢掉历史最优**：随机游走会接受劣解，若最后输出「当前解」而不输出 `best`，可能丢掉了中途找到的最优值。务必单独维护 `bestE/bestx/besty`。
4. **单轮收敛不可靠，多轮粗精结合才稳**：单靠一轮快降温容易停在局部最优附近；本代码「第一轮粗搜 + 后续轮缩小初温（$\times 0.8$）并放慢降温（$a\to 1$）」逐步逼近，比固定一套参数的单调降温更稳。
5. **种子取秒级时间且只播一次**：同一个程序内 `time(nullptr)` 播一次没问题；但两个进程若在同一秒启动会得到相同序列，对拍多开进程时要注意（可混入进程 ID）。
6. **退化情形**：$n=1$ 时答案就是那一个点，算法应自然收敛于此（无需特判，但要知道会退化）；多个最优解（如两个等质量点）时答案不唯一，评测按误差判分，无需纠结。
7. **计时函数**：`clock()` 计量 CPU 时间，Windows 与 Linux 的 `CLOCKS_PER_SEC` 不同，阈值 `TL=0.9` 是按秒计的墙钟近似；`TL` 别贴满评测时限，留余量给输入输出。

## 对比与扩展

| 方法 | 接受劣解 | 局部最优 | 适用 |
|---|---|---|---|
| 爬山法 | 从不 | 容易卡住 | 凸函数、单峰 |
| 模拟退火 | 按 $e^{-\Delta E/T}$ 概率 | 慢降温下可逃离 | 多峰、需要近似全局最优 |
| 随机重启爬山 | 从不，但多次随机起点 | 靠重启覆盖 | 解空间小、重启成本低 |

- **前置/相关**：随机数引擎 `mt19937` 与分布对象见「C++ 随机数生成（mt19937 与分布对象）」（`basic-random-mt19937`），本代码的 `rd`、`dist`、`prob` 都是它的直接应用。
- **参数经验**：本代码采用「多轮粗搜 → 精修」策略——首轮 $T_0=10000$、$a=0.99$ 快速粗搜，之后每轮 $T_0\gets T_0\times0.8$、$a\gets 1-0.5(1-a)$ 缩小搜索范围并放慢降温，配合 $0.9$ 秒时间卡点，实际零点几秒即可稳定 AC。相比「单一慢降温系数（如 $0.9992$）跑一轮」，多轮缩放对精度和鲁棒性都更友好；其中的 $0.8$、$0.5$ 属于经验参数，可按题目的坐标范围与精度要求微调。
- **延伸**：遗传算法、粒子群、禁忌搜索同属随机优化；退火思想还可用于离散问题（如旅行商的 2-opt 邻域 + Metropolis）。