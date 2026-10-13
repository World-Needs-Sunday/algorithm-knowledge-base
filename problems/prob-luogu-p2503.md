---
id: prob-luogu-p2503
title: 'P2503 [HAOI2006] 均分数据'
oj: '洛谷'
problemId: 'P2503'
difficulty: '困难'
category: 动态规划
tags: ["模拟退火", "动态规划", "均方差", "分组", "随机化"]
timeComplexity: 'O(n²m)（单次评估）'
spaceComplexity: 'O(nm)'
codePath: '题目专辑\动态规划\均分数据\代码.cpp'
level: A
---

## 题目描述

将 $n$ 个正整数 $a_1,a_2,\dots,a_n$ 分成 $m$ 组，使得各组**和的均方差**最小。设第 $i$ 组的和为 $x_i$，$\bar{x}=\frac{1}{m}\sum x_i$ 是各组和的平均值，均方差定义为

$$
\sigma=\sqrt{\frac{1}{m}\sum_{i=1}^{m}(\bar{x}-x_i)^2}
$$

求最小的 $\sigma$。题目来源：洛谷 P2503（[HAOI2006] 均分数据，河南省选）。

### 输入格式

第一行两个整数 $n,m$（$n$ 是整数个数，$m$ 是要分成的组数）。
第二行 $n$ 个整数 $a_1,\dots,a_n$（均在 $[1,50]$ 内），同一行的整数用空格隔开。

### 输出格式

输出一行一个实数，表示最小均方差的值，保留小数点后两位。

### 数据范围

- 对于 $40\%$ 的数据，$m\le n\le 10$，$2\le m\le 6$；
- 对于 $100\%$ 的数据，$m\le n\le 20$，$2\le m\le 6$。

---

## 样例

### 样例 1

**输入：**

```
6 3
1 2 3 4 5 6
```

**输出（本代码实测，与官方一致）：**

```
0.00
```

**解释：** 分成 $(1,6),(2,5),(3,4)$ 三组，每组和都是 $7$，与平均值 $7$ 偏差为 $0$，均方差为 $0.00$。

---

## 解题思路

看到「$n\le 20$、$m\le 6$」，第一反应是状态空间极小，启发式搜索有充分的空间发挥；而「最小化各组和与平均值的偏差平方和」是一个与**顺序无关的量**——同一个分组方案，组内元素怎么排都不影响每组和。

先解决一半问题：**如果元素顺序固定，最优分组是经典的区间 DP**。设前缀和为 $s_i$，令 $dp[k][i]$ 表示「前 $i$ 个数分成 $k$ 组的最小偏差平方和」，转移枚举第 $k$ 组的起点 $j$：

$$
dp[k][i]=\min_{j=k}^{i}\Big\{dp[k-1][j-1]+(\bar{x}-(s_i-s_{j-1}))^2\Big\}
$$

其中 $\bar{x}=s_n/m$ 是固定的全局平均值。这个 DP 是 $O(n^2m)$，在 $n\le 20$ 下几乎零成本。

但分组的边界是画在**具体排列**上的：同一个集合，排列不同，DP 能切出的最优方案就不同。因此外层需要搜索「排列」本身——用**排列退火**：随机交换两个元素产生邻域，内层用上面的 DP 求该排列下的最优划分作为能量，按 Metropolis 准则接受或拒绝，温度只负责「是否接受劣解」，与交换选择无关。

初始排列取「升序排序」：排序后的相邻元素相近，DP 容易切出接近平均值的组，是一个相当好的起点（绝不是随便一个恒等排列）。退火卡时 0.9 秒反复多轮，逐步逼近全局最优，最后输出 $\sqrt{bestE/m}$ 保留两位。

### 关键洞察

把问题拆成两层：**外层退火搜索排列，内层 DP 求该排列的最优分组**。固定顺序后「最优划分」有 $O(n^2m)$ 的多项式解法，于是随机搜索只需要在最难的部分——顺序空间——里找答案。

---

## 逐行代码解析

### 模块一：全局变量与 `init`

```cpp
#include<bits/stdc++.h>
using namespace std;
const double TL = 0.9;
vector<int> cur_arr, best_arr;
long double curE = DBL_MAX, bestE = DBL_MAX;
mt19937 gen;
void init(int n)
{
	cur_arr.assign(n + 1, 0);
	best_arr.assign(n + 1, 0);
	curE = DBL_MAX, bestE = DBL_MAX;
	gen.seed(time(nullptr));
}
```

- `cur_arr` 存当前排列，`best_arr` 存历史最优排列；能量用 `long double`，初值 `DBL_MAX` 表示「尚无解」，保证第一次评估一定会更新最优。
- `gen.seed(time(nullptr))` 只在 `init` 播一次种，全程序共用一个 `mt19937` 引擎。
- `TL = 0.9` 是退火的时间卡点（秒），比评测时限（1000ms）留了余量。

### 模块二：`cala` 用区间 DP 求给定排列的最优分组

```cpp
long double cala(int n, int m, vector<int>& arr)
{
	for (int i = 1; i <= n; i++) arr[i] += arr[i - 1];
	vector<vector<long double>> dp(m + 1, vector<long double>(n + 1,DBL_MAX));
	long double avg = (long double)arr[n] / m;
	for (int i = 1; i <= n; i++)
	{
		long double t = avg - (long double)arr[i];
		dp[1][i] = t * t;
	}
	for (int k = 2; k <= m; k++)
	{
		for (int i = k; i <= n; i++)//处理到的元素
		{
			for (int j = k; j <= i; j++)//新的一层的起点
			{
				long double t = avg - (long double)(arr[i] - arr[j - 1]);
				dp[k][i] = min(dp[k][i], dp[k - 1][j - 1] + t * t);
			}
		}
	}
	for (int i = n; i >= 1; i--) arr[i] -= arr[i - 1];
	return dp[m][n];
}
```

- 开头把 `arr` **原地改写成前缀和**，结尾再改回来：函数不改变调用方的排列内容，但省掉了新开前缀数组。
- `avg = arr[n] / m`：全局平均值 $\bar{x}$，对所有划分固定不变。
- `dp[1][i] = (avg - s_i)^2` 是只有一组的边界情况；$k\ge 2$ 时枚举第 $k$ 组起点 $j$，用 `dp[k-1][j-1]` 加最后一组的偏差平方转移，初始 `DBL_MAX` 配合 `min` 完成「未算过即无穷大」的语义。
- 返回 `dp[m][n]`，即**该排列下**分 $m$ 组的最小偏差平方和（不是均方差，开方交给 main）。

### 模块三：`sa` 退火主体

```cpp
void sa(int n , int m)
{
	cur_arr = best_arr;
	curE = bestE;
	static uniform_int_distribution<int> dist(1, n);
	static uniform_real_distribution<long double> prob(0.0, 1.0);
	for (long double t = 60000; t >= 1e-8; t *= 0.99)
	{
		int x = dist(gen);
		int y = dist(gen);
		while (x == y) y = dist(gen);
		swap(cur_arr[x], cur_arr[y]);
		long double tmpE = cala(n, m, cur_arr);
		long double delta = tmpE - curE;
		if (delta < 0)
		{
			curE = tmpE;
			if (curE < bestE)
			{
				bestE = curE;
				best_arr = cur_arr;
			}
		}
		else if (exp(-delta / t) > prob(gen)) curE = tmpE;
		else swap(cur_arr[x], cur_arr[y]);
		if ((double)clock() / CLOCKS_PER_SEC > TL) break;
	}
	
}
```

- 每轮**从历史最优重新出发**（`cur_arr = best_arr`），不在劣解区域浪费迭代。
- 扰动是「随机交换两个不同元素」，与温度无关；温度 $t$ 只出现在 `exp(-delta/t)` 的接受概率里——排列退火与点退火的本质区别。
- `delta < 0`（更优）直接接受并顺手更新 `bestE/best_arr`；更劣时按 $e^{-\Delta E/t}$ 概率接受；**不接受就把两个元素换回去**，保证「排列」与「能量」永远一致。
- 每轮主循环末尾不额外写 `best`，因为更优解只在 `delta<0` 分支出现，那里已经同步更新过历史最优。
- `if (clock() ... > TL) break;`：单轮超时即停，总轮数由 main 的外层循环控制。

### 模块四：`main` 主流程

```cpp
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);

	int n, m;
	cin >> n >> m;
	init(n);
	for (int i = 1; i <= n; i++) cin >> best_arr[i];
	sort(best_arr.begin() + 1, best_arr.end());
	bestE = cala(n, m, best_arr);
	while ((double)clock() / CLOCKS_PER_SEC <= TL) sa(n, m);
	cout << fixed << setprecision(2) << sqrt(bestE / m);
	return 0;
}
```

- 读入后先 `sort`：升序排列是精心选的初始解，相邻元素相近时 DP 容易切出接近 $\bar{x}$ 的组，退火起点质量高。
- `bestE = cala(n, m, best_arr);` 先算一次初始能量，`best_arr` 同步是排序后的排列。
- `while (clock() ... <= TL) sa(n, m);`：0.9 秒内反复整轮退火，多轮冗余稀释「单轮没转准」的风险。
- `sqrt(bestE / m)`：`cala` 返回的是偏差平方和 $\sum(\bar{x}-x_i)^2$，除以 $m$ 再开方才是均方差 $\sigma$；`fixed << setprecision(2)` 保留两位输出。

---

## 复杂度分析

- **时间复杂度**：单次评估 `cala` 是 $O(n^2m)$（三层循环，$n\le 20$、$m\le 6$，常数约 2400 次运算）；单轮退火的降温次数约 $\ln(60000/10^{-8})/\ln(1/0.99)\approx 2.8\times 10^3$。总轮数与总迭代由 `TL = 0.9` 秒的 `clock()` 卡点硬性封顶。
- **空间复杂度**：DP 表 $O(nm)$，排列数组 $O(n)$，合计 $O(nm)$。

---

## 常见陷阱与注意事项

1. **DP 的"未计算"初值**：`dp` 必须初始化成足够大的值（本代码 `DBL_MAX`）；若用 `0` 或者忘初始化，`min` 会直接取到脏值，答案偏小甚至全错。
2. **`cala` 原地改写前缀和必须还原**：函数开头 `arr[i] += arr[i-1]`、结尾 `arr[i] -= arr[i-1]` 成对出现；只改不改回，上层 `sa` 里的排列会被破坏，后续评估全部错乱。
3. **不接受的试探要回退**：代码先 `swap` 再评估，若 Metropolis 拒绝，必须再 `swap` 一次把两个元素换回原位；漏掉回退会让「能量没变但排列变了」，两者从此不一致。
4. **开方放最后、输出两位小数**：`cala` 的返回值是**偏差平方和**，要先 `sqrt(bestE / m)` 才是均方差；`fixed << setprecision(2)` 保证精度格式，直接输出原始值必然错误。
5. **初始解要用心选**：升序排序的初始排列让 DP 起点接近最优；若换成随便的初始顺序，退火要多花很多轮才能爬到同样位置，0.9 秒内可能不够。
6. **卡时留余量**：`TL = 0.9` 秒略小于 1000ms 评测时限，为输入输出与输出保留空间；注意 `clock()` 计量 CPU 时间，不同平台的 `CLOCKS_PER_SEC` 不同。
7. **平均值的类型**：`avg`、`t` 都用 `long double` 计算再平方相加；若用 `int` 计算 `avg`，小数部分被截断，偏差完全失真。

---

## 对比与扩展

- **同类「排列退火 + 多项式评估」结构**：本专辑外的基础算法知识点「模拟退火 · 区间退火（排列退火）」里，P2210 Haywire 用增量计算 $O(1)$ 评估一次交换；本题因为分组依赖全局结构，改用 $O(n^2m)$ 的 DP 评估——评估器按题目换，退火骨架不变，这正是排列退火的可迁移套路。
- **固定顺序后的分组 DP**：$dp[k][i]=\min_{j}(dp[k-1][j-1]+(\bar{x}-(s_i-s_{j-1}))^2)$ 与「划分序列使各段和尽量平均」问题的转移一致；该 DP 单独也可用于任何"给定顺序分组"的场景。
- **随机化与精确解的分工**：$n\le 20$ 时退火足够稳；若 $n$ 更大且要求精确，可考虑状压 DP 等精确算法（状态 $2^n$ 在本题范围同样可行），模拟退火则是省心且稳定的近似选择。