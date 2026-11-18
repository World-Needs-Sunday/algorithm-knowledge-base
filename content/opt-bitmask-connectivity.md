---
id: opt-bitmask-connectivity
title: '位掩码连通性判定'
category: 优化技巧
subcategory: 位运算技巧
tags: [位运算, 状压, 连通性, BFS, 子集]
timeComplexity: 'O(m+kn)'
spaceComplexity: 'O(n)'
level: B
verified: '2026-11-18'
codePath: '优化技巧\位运算技巧\位掩码连通性判定\Untitled1.cpp'
---

## 算法原理

**问题模型**：给定 $n$ 个点的图（$n$ 很小，位掩码可表示），用 32 位无符号整数的**每一位代表一个点**。给定若干查询点集，判断每个点集在图的**诱导子图**（只保留点集内部的边）中是否连通。

**核心思想**：把「邻接表」与「BFS 队列」全部压进一个 32 位整数，用位运算并行地完成一轮扩展：

- 邻接掩码：`edges[v]` 的第 $u$ 位为 1 ⟺ 存在边 $v-u$；
- 点集：`mask` 的每一位表示该点是否在查询集合里；
- BFS 一层扩展：把当前层 `bfs_que` 里每个点的邻接掩码**按位或**起来，就得到「下一层可能到达的全部点」，一次或运算等价于对整层所有点同时做「取出边」操作。

**判定流程**：

1. 特判：`!(mask & (mask-1))` 为真 ⟺ mask 中至多一个 1（空集或单点）——语义上视为连通，直接返回。
2. 取 `seed = mask & (~mask+1)`（即 `mask & (-mask)`，lowbit）：把点集中编号最小的点作为 BFS 起点。
3. 循环扩展：`comp` 记录已连通的点、`bfs_que` 记录当前层；每轮 `nxt = OR_{v∈bfs_que} edges[v]`，再做三步清洗——`& mask`（只留查询点集内的点）、`& ~comp`（去掉已访问）、`comp |= nxt`（并入），然后 `bfs_que = nxt` 进入下一层。
4. 当 `bfs_que == 0`（无可扩展点）循环结束，返回 `comp == mask`：能走到的点恰等于整个点集 ⟺ 连通。

**为什么位运算版快**：普通 BFS 每个点入队要出队、遍历邻接表、查 visited 数组；位运算版把「整层的邻接合并」变成一次 32 位或运算（单条指令），「去重/去已访」变成两次按位与，层内每个点只做一次 `__builtin_ctz` 取最低位。所有操作都在寄存器宽度内完成，常数比数组版 BFS 小一个量级。

## 核心公式 / 状态定义与转移方程

**状态定义**：

- $mask$：查询点集（第 $i$ 位 = 点 $i$ 是否在集合中，点编号 0-based）；
- $edges[v]$：点 $v$ 的邻接掩码；
- $comp$：BFS 已确定与起点连通的点集；
- $bfs\_que$：当前层的点集。

**初始化**：

$$seed = mask\ \&\ (-mask),\qquad comp = seed,\qquad bfs\_que = seed$$

**每轮转移**（对 `bfs_que` 中每个点 $v$，用 `__builtin_ctz` 依次取出）：

$$nxt = \bigvee_{v\in bfs\_que} edges[v]$$

$$nxt \leftarrow nxt\ \&\ mask\ \&\ \sim comp$$

$$comp \leftarrow comp\ |\ nxt,\qquad bfs\_que \leftarrow nxt$$

**判定**：

$$\text{连通}\ \iff\ comp = mask$$

**手算走读**（$n=4$，边 $0-1,1-2,2-3$ 的链，查询 $mask=1111_2=15$，本代码实测输出「是联通块(或为空)」）：

- $seed=1$（点 0），$comp=1$，$bfs\_que=1$；
- 第 1 轮：$nxt=edges[0]=0010_2=2$，清洗后 $comp=0011_2$，$bfs\_que=2$；
- 第 2 轮：$nxt=edges[1]=0101_2=5$，$nxt\ \&\ mask\ \&\ \sim comp$ 得 $0100_2=4$，$comp=0111_2$；
- 第 3 轮：$nxt=edges[2]=1000_2=8$，$comp=1111_2$；
- 第 4 轮：$nxt=edges[3]=0100_2=4$ 被清洗干净 → 0，循环结束，$comp=15=mask$ → 连通 ✓。

对照查询 $mask=1001_2=9$（点 0 和点 3，不相邻）：$seed=1$，第 1 轮 $nxt=edges[0]=2$，$nxt\ \&\ mask$ 后为 0 → 循环结束，$comp=1\ne9$ → 不是连通块 ✓（实测一致）。

## 逐行代码解析

代码拆成 2 个模块，按顺序拼接即完整程序。

### 模块 1 连通判定函数

```cpp
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
vector<unsigned int> edges;

//判断是否联通
bool is_cond(unsigned int mask)
{
	if (!(mask & (mask - 1))) return true; //没有点和一个点看为联通
	unsigned int seed = mask & (~mask + 1); //等价与mask & (-mask)
	unsigned int comp = seed; //已经确定相连的点

	unsigned int bfs_que = seed; //模拟bfs队列，即下一轮要处理的点

	while (bfs_que)
	{
		//处理bfs_que可以连接的边
		unsigned nxt = 0;
		for (unsigned int f = bfs_que; f; f &= f - 1) nxt |= edges[__builtin_ctz(f)];

		nxt &= mask; // 将不属于mask的点去除
		nxt &= ~comp; // 将已经在连通图的去掉
		comp |= nxt; // 将本轮可以加的点加入comp
		bfs_que = nxt; //下一轮要处理的点
	}

	return comp == mask;
}

```

- `if (!(mask & (mask - 1))) return true;`：$mask\&(mask-1)$ 消掉最低位的 1；结果为 0 ⟺ 至多一位（空集或单点），按注释语义视为连通。这是**唯一没有隐含在循环里的语义决定**。
- `seed = mask & (~mask + 1)`：`~mask+1` 即补码取负，等价 `mask & (-mask)`，取出编号最小的点作起点——起点选谁不影响连通性，选最低位只是为了确定。
- `while (bfs_que)` 的循环不变式：`comp` 始终是「从 seed 出发、只走 mask 内点、能到达的全部点」；`bfs_que` 是本轮新并入的点。
- 遍历当前层：`for (f = bfs_que; f; f &= f - 1)` 用「消最低位」枚举层内每个点，`__builtin_ctz(f)` 取该点编号（f 非零才进循环体，不会对 0 调用 ctz——那是 UB）。
- 三步清洗顺序不能乱：先 `& mask` 限定在查询点集内（否则会走出子图）、再 `& ~comp` 去已访问（否则死循环或重复计数）、最后才 `comp |= nxt`。
- `comp == mask`：BFS 自然结束后可达集与点集相等即为连通；不存在「提前返回」的分支，结构清晰。

### 模块 2 主函数

```cpp
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	//点的下标为0~base - 1    正常题目是1 ~ base 记得要映射成0~base - 1
	int n, m, k;
	cin >> n >> m >> k;
	edges.assign(n, 0);
	for (int i = 1; i <= m; i++)
	{
		int a, b;
		cin >> a >> b;
		a -= 1, b -= 1;
		edges[a] |= (unsigned int)1 << b;
		edges[b] |= (unsigned int)1 << a;
	}
	for (int i = 1; i <= k; i++)
	{
		unsigned int cost;
		cin >> cost;
		if (is_cond(cost)) cout << "是联通块(或为空)\n";
		else cout << "不是连通块\n";
	}
	return 0;	
}
```

- 点下标约定为 **0 ~ n-1**（注释写明：题目常用 1 ~ n，要映射成 0-based），读入后 `a -= 1, b -= 1`。
- 建图：无向边在 `edges[a]` 里置 $b$ 位、`edges[b]` 里置 $a$ 位，各一次或运算。
- 查询掩码以十进制整数读入（如 $15$ 表示点集 $\{0,1,2,3\}$），每个查询调用一次判定并输出中文结论。

## 复杂度分析

- 预处理：读边 $O(m)$，每条边两次位或（$O(1)$）。
- 每次判定：每个点至多进入 `bfs_que` 一次（`& ~comp` 保证），层内每个点做一次 `ctz`（$O(1)$）与一次或（32 位单指令）→ 总 $O(n)$。
- $k$ 次查询：$O(m+kn)$。
- 空间：`edges` 数组 $O(n)$（每元素一个 32 位字）。

## 适用场景

**适用**：$n\le30$ 的小图上「给定子集问是否连通」的高频查询——状压 DP 里枚举子集并判连通（生成树计数、连通块划分、旅行商类剪枝）；静态图上批量连通性查询（预处理一次、查 $k$ 次 $O(kn)$）。

**特别适合**：子集枚举 + 连通判定的内层循环——位运算版没有数组清空开销（每查一个 mask 从头算，不需要 memset），常数与代码量都极小。

**不适用**：$n>30$（32 位装不下，需换 `bitset`/`unsigned long long` 多字或普通 BFS）；动态加边删边的连通性（那是 DSU 的主场）；需要路径信息而不只是"是/否"的查询。

## 常见陷阱与注意事项

1. **点数上限（原代码未写明的约束）**：掩码是 32 位 `unsigned int`，$n\le30$ 稳妥（bit 31 虽在 unsigned 下合法，惯例不碰边界位）；$n$ 更大必须换 `unsigned long long` 或 `bitset` 多字版本。
2. **a、b 输入范围（原代码未写明）**：代码直接 `a -= 1`，若输入含 0 会下溢成 $2^{32}-1$ 级的位移量，`1 << 巨大数` 是未定义行为——题面必须保证 $1\le a,b\le n$。
3. **查询掩码格式（原代码未写明）**：`cost` 按十进制整数读入，要求 $0\le mask<2^n$；超出 $n$ 位的位会被当成"不存在的点"，判定结果无意义。
4. **空集/单点语义**：本代码把 0 个点与 1 个点视为连通（`!(mask&(mask-1))` 特判）；若题目要求"空集不算连通"或"单点不算"，特判与输出口径都要改。
5. **`__builtin_ctz(0)` 是 UB**：两个循环（`for(f=bfs_que; f; …)` 与 `while(bfs_que)`）都保证参数非零；自己改动时别把 ctz 挪到零值路径上。
6. **三步清洗顺序**：`& mask` → `& ~comp` → `comp |= nxt` 不能乱——先并入 comp 再去重会漏掉"本轮新点间互连"的扩展；忘 `& mask` 会走出子图。
7. **n、m、k 范围（原代码未写明）**：边读入与查询循环没有上限保护，复用时按题意补上；重边、自环在代码中无害（或运算幂等），但题面应说明。
8. **点编号 0-based**：`ctz` 返回的是 0-based 编号；题目若 1-based，读入与输出都要统一映射，别只映射一边。
9. **输出编码与评测**：字符串输出内容属程序行为，搬运时不要改动；本地测试若乱码，是文件编码与控制台编码不一致，不是程序逻辑问题。
10. **`comp` 初值即 `seed`**：起点必须在"已连通"集合里，若从 0 开始第一轮会把自己当新点反复入队。

## 对比与扩展

**与普通 BFS**：都是 $O(n)$ 判定，但位运算版把邻接合并、去重、去已访压缩成两三条寄存器指令，常数小、无 visited 数组清空开销；代价是 $n$ 受机器字长限制且代码依赖 GNU 内建函数。

**与 DSU（并查集）**：DSU 擅长"动态加边 + 任意两点连通查询"（近 $O(\alpha)$），但每次查询一个**子集**的诱导连通性时 DSU 要么重建要么做大量 find；位掩码版在"静态图 + 子集批量判定"场景下更贴合状压枚举。

**与 `opt-bitset`（bitset 优化）**：同一思想的两层形态——bitset 把**布尔矩阵**压成整字并行（如 Floyd 传递闭包除 64），本技巧把**集合运算**（并、交、补）压成整字并行；都属「机器字并行除常数」一族。$n$ 超过 32 时把 `unsigned int` 换成 `bitset<50>` 或 `unsigned long long` 数组，本框架照搬。

**可复用套路**：任何"对小集合做 BFS/DFS"的过程，只要集合大小在字长内，都可以用「邻接掩码 + 分层或 + 三步清洗」重写；同样的"mask 模拟队列"手法还可用于子集 DP 的转移加速（一次或出全部后继状态）。