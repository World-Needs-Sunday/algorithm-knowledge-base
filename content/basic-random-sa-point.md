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
nx = curx + U(-32767,32767)\cdot T,\qquad ny = cury + S\cdot T
$$

其中 $U$ 是均匀分布，$S$ 为与 $U$ 同分布的新抽样。幅度随温度收缩。

**Metropolis 接受准则**：设 $\Delta E = E_{new}-E_{cur}$，接受概率

$$
P(\text{接受})=\begin{cases}1, & \Delta E\le 0\\[2pt] e^{-\Delta E/T}, & \Delta E>0\end{cases}
$$

**降温策略**：$T_0=4000$，每步 $T\gets T\cdot 0.9992$，直到 $T<10^{-12}$。降温次数约为

$$
\frac{\ln(T_0/T_{min})}{\ln(1/0.9992)}\approx\frac{\ln(4\times 10^{15})}{0.0008}\approx 2\times 10^4
$$

次；配合 `clock()` 时间卡点（`TL = 0.992` 秒）反复多轮退火，直到时限耗尽。

## 逐行代码解析

```cpp
#include<bits/stdc++.h>
using namespace std;
mt19937 rd;
vector<double> x, y, w;
const double TL = 0.992;
double curx, cury, curE, bestE, bestx, besty;
double cala(double mx, double my, int n)
{
	double res = 0;
	for (int i = 1; i <= n; i++)
	{
		double dx = mx - x[i];
		double dy = my - y[i];
		double len = sqrt(dx * dx + dy * dy);

		res += len * w[i];
	}
	return res;
}
void init(int n)
{
	x.assign(n + 1, 0);
	y.assign(n + 1, 0);
	w.assign(n + 1, 0);
	rd.seed(static_cast<unsigned int>(std::time(nullptr)));
}
void sa(int n)
{
	curx = bestx;
	cury = besty;
	curE = bestE;
	static uniform_real_distribution<double> dist(-32767.0, 32767.0);
	static uniform_real_distribution<double> prob(0.0, 1.0);
	for (double t = 4000; t > 1e-12; t *= 0.9992)
	{
		double nx = curx + dist(rd) * t;
		double ny = cury + dist(rd) * t;
		double newE = cala(nx, ny, n);
		double delta = newE - curE;
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

		if ((double)clock() / CLOCKS_PER_SEC >= TL) break;
	}
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);

	int n;
	cin >> n;
	init(n);
	double mx = 0, my = 0, mw = 0;
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
	while ((double)clock() / CLOCKS_PER_SEC < TL) sa(n);

	cout << fixed << setprecision(3) << bestx << ' ' << besty;
	
}
```

- `cala(mx, my, n)`：目标函数，$O(n)$ 求一点到所有点的加权距离和，是每次扰动后的评估核心。
- `rd.seed(static_cast<unsigned int>(std::time(nullptr)))`：用当前时间初始化 `mt19937` 引擎；每轮 `sa` 用的 `dist`、`prob` 是 `static` 分布对象，不会重复构造。
- `curx = bestx; ...`：每轮退火**从历史最优重新出发**，而不是从上一轮停下的位置继续，避免在劣解区域浪费迭代。
- `double nx = curx + dist(rd) * t;`：扰动幅度与温度 $t$ 成正比；`dist` 区间为 $[-32767,32767]$，高温时步长很大（见「常见陷阱」第 1 条）。
- `if (delta < 0 || exp(-delta / t) > prob(rd))`：Metropolis 准则。`delta<0` 表示更优，直接接受；否则按 $e^{-\Delta E/T}$ 概率接受劣解，`prob(rd)` 生成 $[0,1)$ 的均匀随机数作比较。
- `if (curE < bestE)`：全程维护全局最优，与「当前解是否接受劣解」解耦。
- `if ((double)clock() / CLOCKS_PER_SEC >= TL) break;`：超时立即退出一轮；外面 `while` 循环在时限内反复调用 `sa`，把评测时限用到极致。
- 起点是**加权质心** $(\sum x_iw_i/\sum w_i,\ \sum y_iw_i/\sum w_i)$：比算术平均更接近最优（$n=1$ 时它就是精确解）。
- 输出 `fixed << setprecision(3)`：保留 3 位小数，$10^{-12}$ 级的终止温度对 3 位精度绰绰有余。

## 复杂度分析

- **时间复杂度**：单轮退火的降温次数为 $O(\log(T_0/T_{min})/\ln(1/k))\approx 2\times 10^4$，每次扰动的评估 `cala` 是 $O(n)$，故单轮 $O(n\log(T_0/T_{min}))$。配合时间卡点，实际轮数由 `TL` 决定；总运行时间被钳制在约 1 秒内。
- **空间复杂度**：只存坐标与权重的三个 `vector`，$O(n)$。

## 适用场景

- **连续空间的多峰最优化**：带权费马点 / 平衡点（P1337）、覆盖圆、旅行商这类没有多项式精确解的近似最优化问题。
- **需要「够好就行」的数值解**：答案只要求若干位小数、允许误差的题（如几何平衡、连续函数求极值），模拟退火以极低实现成本换取稳定分数。
- **非精确解不可的场景不适用**：要求输出精确整数解、或解空间离散且约束强的题，退火可能永远踩不到可行解；这类题优先考虑 DP / 精确算法。
- 参数完全相同的两次运行结果可能不同（随机算法本质），对「必须复现」的评测要谨慎。

## 常见陷阱与注意事项

1. **扰动幅度没有归一化**：本代码 `dist(-32767, 32767) * t` 在 $t$ 还接近 $4000$ 时步长可达 $1.3\times 10^8$，远超坐标范围，高温阶段大量迭代被浪费。更标准的写法是把随机数归一化到 $[-1,1]$ 再乘 $t$（`dist(-1.0, 1.0)`），使「幅度 = 温度」名副其实。两种写法在小数据下都能过题，但归一化更稳健。
2. **只用当前解、丢掉历史最优**：随机游走会接受劣解，若最后输出「当前解」而不输出 `best`，可能丢掉了中途找到的最优值。务必单独维护 `bestE/bestx/besty`。
3. **降温太快**：$k$ 太小（如 $0.9$）会让算法快速「冻结」在局部最优；常取 $0.998\sim 0.999$。太慢又会超时，所以本代码把慢降温（$0.9992$）与时间卡点搭配使用。
4. **种子取秒级时间且只播一次**：同一个程序内 `time(nullptr)` 播一次没问题；但两个进程若在同一秒启动会得到相同序列，对拍多开进程时要注意（可混入进程 ID）。
5. **退化情形**：$n=1$ 时答案就是那一个点，算法应自然收敛于此（无需特判，但要知道会退化）；多个最优解（如两个等质量点）时答案不唯一，评测按误差判分，无需纠结。
6. **初温与坐标范围要匹配**：坐标范围 $\pm 10000$ 时 $T_0$ 取几百到几千都合理；若 $T_0$ 远小于整个搜索域，低温几乎探索不动全域，容易只找到局部最优。
7. **计时函数**：`clock()` 计量 CPU 时间，Windows 与 Linux 的 `CLOCKS_PER_SEC` 不同，阈值 `TL=0.992` 是按秒计的墙钟近似；提交到不同平台前用本地近似时限试跑一次。

## 对比与扩展

| 方法 | 接受劣解 | 局部最优 | 适用 |
|---|---|---|---|
| 爬山法 | 从不 | 容易卡住 | 凸函数、单峰 |
| 模拟退火 | 按 $e^{-\Delta E/T}$ 概率 | 慢降温下可逃离 | 多峰、需要近似全局最优 |
| 随机重启爬山 | 从不，但多次随机起点 | 靠重启覆盖 | 解空间小、重启成本低 |

- **前置/相关**：随机数引擎 `mt19937` 与分布对象见「C++ 随机数生成（mt19937 与分布对象）」（`basic-random-mt19937`），本代码的 `rd`、`dist`、`prob` 都是它的直接应用。
- **参数经验**（来自本题三套解法的对比）：降温系数 $0.9992$（慢、迭代约 2 万次）+ 时间卡点反复退火，鲁棒性最好；$0.993$（快、几千次一轮）配合跑 3 次也能过，但依赖题目数据较弱。慢降温 + 卡时间多跑几轮是最实用的稳分组合。
- **延伸**：遗传算法、粒子群、禁忌搜索同属随机优化；退火思想还可用于离散问题（如旅行商的 2-opt 邻域 + Metropolis）。