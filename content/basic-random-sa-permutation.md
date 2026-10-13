---
id: basic-random-sa-permutation
title: '模拟退火 · 区间退火（排列退火）'
category: 基础算法
subcategory: 随机化
tags: ["模拟退火", "随机化", "排列退火", "增量计算", "邻域搜索"]
timeComplexity: 'O(deg·log(T0/Tmin))'
spaceComplexity: 'O(n+m)'
codePath: '基础算法\随机化\排列退火\Untitled1.cpp'
level: A
---

## 算法原理

解决的问题：**排列 / 顺序的最优化**。典型应用是洛谷 P2210「Haywire」：$n$ 头奶牛，每头恰好有 3 个好友，把它们排成一列（位置 $1\sim n$），两头好友之间的电线长度等于它们**位置差的绝对值**，求总电线长度的最小值：

$$
f(p)=\sum_{(i,j)\in E}\left|\mathrm{pos}(i)-\mathrm{pos}(j)\right|
$$

决策对象是一个 $1\sim n$ 的排列 $p$。上一篇「点退火」直接在连续坐标上挪 $(x,y)$，在这类问题里用不上；要在**排列空间**里做邻域搜索，也就是区间退火（排列退火）。

与点退火的核心区别，记三句话：

1. 点退火里，温度 $t$ 既管**扰动步长**、又管**接受劣解的概率**，两件事耦合，参数很敏感。
2. 区间退火里，扰动是**独立性的事件**——随机交换两个元素（或翻转一段），与 $t$ 完全无关。
3. 温度 $t$ 只出现在接受概率 $\exp(-\Delta E/t)$ 里，**只管「要不要接受一个更差的解」**。

于是「选哪个新状态」与「接不接受」解耦：$t$ 初值取大一点也没关系，降温系数粗一点（0.99）也能收敛，参数远不如点退火那么挑。这正是区间退火好用、好调的原因。

抽象模板：

```
初始化排列 cur（例如 恒等排列）与全局最优 best
for (t = t0; t >= t_min; t *= k):
    x, y = 随机选两个不同位置
    Δ = 交换 x、y 后的能量 - cur 的能量（增量计算）
    if Δ < 0 或 exp(-Δ/t) > random(): 真正执行交换
    若 cur 更优则更新 best
输出 best
```

## 核心公式 / 状态定义与转移方程

**状态**：排列 `cur_arr`，其中 `cur_arr[i]` 表示**奶牛 $i$ 所在的位置**（下标是奶牛、值是位置，天然是 $1\sim n$ 的排列）；当前能量 `curE`。另有全局最优 `best_arr / bestE`，退火途中接受劣解也不会丢掉已找到的最优解。

**目标函数**（P2210）：

$$
f(p)=\sum_{(i,j)\in E}\left|\mathrm{pos}(i)-\mathrm{pos}(j)\right|
$$

**扰动（转移）**：随机取两个不同下标 $x,y$，交换 `cur_arr[x]` 与 `cur_arr[y]`。两种常见扰动：

- **交换元素（swap）**：实现简单、增量计算方便、邻域粒度适中，排列退火的默认选择，本代码用的就是它。
- **区间翻转（reverse，2-opt 式）**：随机选 $[l,r]$ 整体反转，改动幅度大，适合 TSP 这类需要大范围重排的问题，但增量计算麻烦、跳变可能过大。

**增量计算（关键优化）**：交换 $a,b$ 后，只有与 $a$、$b$ 相连的边受影响。设交换前 $a$ 在位置 $pos_a$、$b$ 在位置 $pos_b$，则交换后的能量

$$
E' = E - \sum_{i\in adj(a),\,i\ne b}\Big(\left|pos_a-pos_i\right|-\left|pos_b-pos_i\right|\Big)
- \sum_{i\in adj(b),\,i\ne a}\Big(\left|pos_b-pos_i\right|-\left|pos_a-pos_i\right|\Big)
$$

$i\ne b$、$i\ne a$ 是为了跳过公共边 $a\!-\!b$：交换后 $|pos_a-pos_b|$ 不变，无需改动，也避免重复计算。单次评估复杂度从 $O(n)$ 降到 $O(\deg(a)+\deg(b))$。

**Metropolis 接受准则**：$\Delta E=E'-E$，接受概率

$$
P(\text{接受})=\begin{cases}1, & \Delta E\le 0\\[2pt] e^{-\Delta E/t}, & \Delta E>0\end{cases}
$$

**降温策略**：$t_0=20000$，每步 $t\gets t\cdot 0.99$，直到 $t<10^{-8}$；外层由 `clock()` 卡 $TL=0.9$ 秒反复整轮退火。

## 逐行代码解析

### 模块一：全局变量与 `init`（数据结构 + 初始化）

```cpp
#include<bits/stdc++.h>
using namespace std;
const double TL = 0.9;
vector<int> cur_arr, best_arr;//cur_arr[i]表示 奶牛i所在的位置
vector<vector<int>> edges;
long long curE = 0, bestE = 0;
mt19937 gen;
void init(int n)
{
	cur_arr.assign(n + 1, 0);
	best_arr.assign(n + 1, 0);
	for (int i = 1; i <= n; i++) best_arr[i] = i;
	edges.assign(n + 1, vector<int>());
	curE = 0, bestE = 0;
	gen.seed(time(nullptr));
}
```

- `cur_arr[i]` 存「奶牛 $i$ 的位置」，下标是奶牛、值是位置，天然就是一个排列；交换两个奶牛的位置就是 `swap(cur_arr[x], cur_arr[y])`。
- 初始排列取恒等排列（奶牛 $i$ 在位置 $i$）：模拟退火理论上与初值无关，随便一个合法排列即可，恒等排列最好写。
- `gen.seed(time(nullptr))` 只在 `init` 里播一次种；引擎是全局的，后续所有随机数都从它生成。

### 模块二：`cala` 增量计算交换后的能量

```cpp
long long cala(int a, int b, vector<int>& idx)
{
	long long ans = curE;
	for (int i : edges[a])
	{
		if (i != b)
		{
			ans -= abs(idx[a] - idx[i]);
			ans += abs(idx[b] - idx[i]);
		}
	}
	for (int i : edges[b])
	{
		if (i != a)
		{
			ans -= abs(idx[b] - idx[i]);
			ans += abs(idx[a] - idx[i]);
		}
	}
	return ans;
}
```

- 函数用**全局的 `curE` 作底**做增量：先减掉 $a$、$b$ 在旧位置对自己各条边的贡献，再加上换到对方位置后的新贡献，其余边不动，所以不用 $O(n)$ 全量重算。
- `if (i != b)` / `if (i != a)` 跳过公共边 $a\!-\!b$：交换前后 $|pos_a-pos_b|$ 不变，不需要也不能重复计算。
- 本题每头奶牛度数恰为 3，所以单次调用是 $O(1)$；若每次全量重算则是 $O(n)$，$n$ 大时差很多。

### 模块三：`sa` 退火主体

```cpp
void sa(int n)
{
	cur_arr = best_arr;
	curE = bestE;
	static uniform_int_distribution<int> dist(1, n);
	static uniform_real_distribution<long double> prob(0.0, 1.0);
	for (long double t = 20000; t >= 1e-8; t *= 0.99)
	{
		int x = dist(gen);
		int y = dist(gen);
		while(x == y) y = dist(gen);
		long long tmpE = cala(x, y, cur_arr);
		long long delta = tmpE - curE;
		if (delta < 0 || exp((long double)-delta / t) > prob(gen))
		{
			curE = tmpE;
			swap(cur_arr[x], cur_arr[y]);
		}
		if ((double)clock() / CLOCKS_PER_SEC > TL) break;
	}
	if (curE < bestE)
	{
		bestE = curE;
		best_arr = cur_arr;
	}
}
```

- `cur_arr = best_arr; curE = bestE;`：每轮从历史最优重新出发，不在劣解区域浪费迭代。
- `while(x == y) y = dist(gen);`：保证交换的是两个不同元素，否则这一步扰动无效。
- `if (delta < 0 || exp((long double)-delta / t) > prob(gen))`：Metropolis 准则。注意温度 $t$ **只出现在这里**，与「怎么选新状态」无关——这正是区间退火与点退火的本质区别。
- 接受才会真正 `swap(cur_arr[x], cur_arr[y])`；`curE` 同步更新为 `tmpE`，保证「能量」和「排列」始终一致。
- 每轮末尾只把**更优**的结果写进 `bestE/best_arr`，中途接受劣解不影响全局最优。

### 模块四：`main` 读入与主循环

```cpp
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);

	int n;
	cin >> n;
	init(n);
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= 3; j++)
		{
			int x;
			cin >> x;
			if (i < x) bestE += abs(i - x);
			edges[i].emplace_back(x);
		}
	}
	while ((double)clock() / CLOCKS_PER_SEC <= TL) sa(n);
	cout << bestE;

	return 0;

}
```

- 每条好友边在输入里会出现两次（好友关系是对称的），用 `if (i < x)` 保证每条边只累加一次，初始能量是恒等排列下的目标函数值。
- `while ((double)clock() / CLOCKS_PER_SEC <= TL) sa(n);`：在 `TL=0.9` 秒内反复整轮退火，多轮冗余稀释「单轮没转准」的风险。
- 最终输出 `bestE`（全局最优能量），而不是 `curE`（当前能量可能因接受劣解而变差）。

## 复杂度分析

- **时间复杂度**：单次扰动的增量评估为 $O(\deg(a)+\deg(b))$，本题度数恒为 3，即 $O(1)$；一**轮**退火的降温次数为 $\ln(T_0/T_{min})/\ln(1/0.99)\approx 2.8\times 10^3$。总轮数与总迭代由 `TL = 0.9` 秒的 `clock()` 卡点硬性封顶。
- **空间复杂度**：排列两个 `vector` 共 $O(n)$，邻接表 $O(m)$（$m$ 为边数），合计 $O(n+m)$。

## 适用场景

- **排列 / 顺序类最优化**：P2210 Haywire 这类「安排顺序、最小化与顺序相关的代价」，以及 TSP 等旅行商型问题。
- **解空间离散、无法用坐标退火**：状态是一个排列或序列，点退火的连续扰动失效时，改用「交换 / 翻转」等离散邻域。
- 注意：退火给出的是**近似最优**；对必须拿满分的精确解问题（$n$ 很大且时限紧张），优先考虑 DP 等精确算法，退火适合「够好就行」或数据有随机性的场合。

## 常见陷阱与注意事项

1. **忘了排除 `x == y`**：随机到的两个下标相同时交换无效，这一步会白白浪费迭代。务必用 `while(x == y)` 重新抽。
2. **增量计算漏掉公共边**：$a\!-\!b$ 这条边交换后长度不变，若不加 `i != a / i != b` 过滤，会把同一条边的贡献算两次，`delta` 直接算错。
3. **只输出 `curE` 不输出 `bestE`**：随机游走会接受劣解，最终停在的状态可能不是最优；`bestE/best_arr` 的独立维护和最终输出不能省。
4. **边的去重**：好友关系在输入中出现两次，`i < x` 只累加一次；漏了它 `bestE` 会变成真实值的 2 倍，输出直接错。
5. **初温与 $\Delta E$ 量级匹配**：$t_0$ 只需覆盖 $\Delta E$ 的量级（本题 20000）；$t_0$ 过小会让接受劣解概率趋近于 0，退化成爬山法卡在局部最优。
6. **排列的表示约定**：本代码是「下标 = 奶牛、值 = 位置」。若换成「下标 = 位置、值 = 奶牛」，`swap` 对象和 `cala` 里的 `abs(idx[a]-idx[i])` 都要跟着改，两种表示混用必然出错。
7. **时间卡点**：`TL=0.9` 按秒计的墙钟近似；`clock()` 在 Windows 与 Linux 的 `CLOCKS_PER_SEC` 不同，提交不同平台前先本地试跑一次确认不超时。

## 对比与扩展

| 维度 | 点退火（连续坐标） | 区间退火（排列） |
|---|---|---|
| 状态 | 坐标点，连续空间 | 排列，离散空间 |
| 扰动 | 高斯/均匀偏移，步长正比于温度 | 交换两元素或区间翻转，与温度无关 |
| 温度的作用 | 同时控制步长与接受概率，耦合 | 只控制接受概率，解耦 |
| 参数敏感度 | 高，需仔细调 | 低，初温取大点、降温粗略也能收敛 |
| 典型问题 | 费马点、平衡点（P1337） | Haywire（P2210）、TSP |

- **前置/相关**：随机引擎与分布对象见「C++ 随机数生成（mt19937 与分布对象）」；点退火细节与「为什么两者参数敏感度不同」见「模拟退火 · 点退火」。
- **扰动方式选择经验**：排列退火默认用「交换元素」；遇到回路、环等特殊结构（如 TSP 的 2-opt 邻域）再考虑区间翻转。
- **延伸**：增量计算是排列退火的常规优化（只重算受影响部分）；同一思想在「禁忌搜索 / 遗传算法」等启发式里同样适用。