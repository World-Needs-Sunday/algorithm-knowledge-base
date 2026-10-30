---
id: graph-2sat
title: '2-SAT'
category: 图论
subcategory: 连通分量
tags: [2-SAT, 强连通分量, 蕴含图, Tarjan, 拓扑序]
timeComplexity: 'O(n+m)'
spaceComplexity: 'O(n+m)'
level: B
verified: '2026-10-30'
codePath: '图论\连通分量\2-SAT\Untitled1.cpp'
---

## 算法原理

**问题模型（洛谷 P4782【模板】2-SAT，题号由用户确认）**：有 $n$ 个布尔变量 $x_1,\dots,x_n$（只能取 0/1），$m$ 个子句，每句形如 $(x_a=v_a)\lor(x_b=v_b)$。求一组使所有子句同时为真的赋值，或判定无解。「2」指**每个子句至多两个文字**；三个及以上文字就是一般 SAT，NP-hard。

**一句话核心**：把「或」翻成「如果……那么……」的强制关系 → 画成有向图（蕴含图）→ 用强连通分量判无解 → 缩点成 DAG 按拓扑序定赋值。七步：翻译、建图、可达判冲突、SCC 判无解、缩点、拓扑赋值、Tarjan 编号方向。

**第一步：或 ≡ 蕴含**。$a\lor b$ 只在「$a$ 假且 $b$ 假」时为假；$\lnot a\to b$ 也在同一情形为假（前提 $\lnot a$ 真 ⟺ $a$ 假），故 $a\lor b\equiv(\lnot a\to b)\equiv(\lnot b\to a)$。建图必须加**两条边**：只加一条会漏掉一个方向的强制。两条边互为逆否，这个对称性保证「$u$ 能到 $v$ ⟹ $\lnot v$ 能到 $\lnot u$」，后面判冲突、定赋值都要用它。

**点与边（本代码的编号约定）**：每个变量拆两个文字点；**点 $i\in[1,n]$ 表示「$x_i=0$」，点 $i+n$ 表示「$x_i=1$」**（与常见模板相反，见陷阱 1）。边 $u\to v$ 意为「$u$ 成立则 $v$ 必须成立」。对子句 $(x_a=v_a)\lor(x_b=v_b)$：令 $u$ 为「$x_a=v_a$」的点，$v$ 为「$x_b=v_b$」的点，加 $\text{neg}(u)\to v$ 与 $\text{neg}(v)\to u$。常见约束写法表：

| 约束 | 建边 |
|---|---|
| $a\lor b$ | `add(a,1,b,1)` |
| $\lnot a\lor b$（即 $a\to b$） | `add(a,0,b,1)` |
| $a\ne b$（异或） | `add(a,1,b,1); add(a,0,b,0)` |
| $a=b$ | `add(a,1,b,0); add(a,0,b,1)` |
| 强制 $a$ 为真 | `add(a,1,a,1)` |
| 强制 $a$ 为假 | `add(a,0,a,0)` |

**冲突判定**：路径 = 传递强制。若「$x_i=1$ 的点」能走到「$x_i=0$ 的点」且反向也能走到，两个方向都矛盾，变量无解。推广：**2-SAT 有解 ⟺ 不存在 $i$ 使两个文字点互相可达**，即它们不在同一强连通分量（SCC）里——充要条件，判一次即可，不需要搜索。

**SCC 为什么能一次算完传播**：同一 SCC 内文字同生共死（选一个得选全组），不同 SCC 之间只能单向可达；冲突被压缩成「真/假同 SCC」一条判定；缩点后必为 DAG（有环则互相可达应合并），传播单向、零回溯。这是 2-SAT 能做到 $O(n+m)$ 的根本原因。

**赋值（谁靠后选谁）**：单向可达时取可达链终点那一端（$i\rightsquigarrow i+n$ 则 $x_i$ 只能取假）；互相不可达也不能随便挑——不同变量的传播链可能在别的变量上打架，必须全局统一顺序。对边 $u\to v$ 要先定 $v$（终点）、后定 $u$（起点），即**按拓扑序从汇点往源点走，选拓扑序更靠后（靠近汇点）的文字为真**。

**Tarjan 编号方向**：SCC 在其所有出边处理完才编号出栈——汇点最先编号、编号最小 ⇒ **编号越小越靠近汇点、拓扑序越靠后**。本代码因此用 `vis[i] < vis[i+n]` 时选点 $i$（即 $x_i=0$）为真：点 $i$ 是「取 0」的文字，编号更小意味着「取 0」更靠后。换用 Kosaraju（编号正拓扑序）判断要写 `>`，「换实现不换判断，必错」。

**脆弱性**：2-SAT 容易不是因为搜索聪明，而是「每子句至多两个文字」让蕴含图足够简单——加优化目标立刻崩：Min-Ones 2-SAT（最小化真变量数）等价顶点覆盖，NP-hard。

**为什么不用匹配/搜索**：匈牙利靠增广路局部交换走向全局最优；2-SAT 的冲突在蕴含链末端、不是局部可修复的，翻决策点就退化成 DPLL（最坏 $O(2^n)$），浪费了特殊结构。SCC 把「所有传播结果」预处理时一次算完。

## 核心公式 / 状态定义与转移方程

**取反函数**（点 $i\in[1,n]$ 为 $x_i=0$，$i+n$ 为 $x_i=1$）：

$$\text{neg}(u)=\begin{cases}u+n & u\le n\\u-n & u>n\end{cases}$$

**子句建边**：$(x_a=v_a)\lor(x_b=v_b)$，$u=a+(v_a?n:0)$（$v_a=1$ 用 $a+n$）、$v=b+(v_b?n:0)$，加边 $\text{neg}(u)\to v$、$\text{neg}(v)\to u$。代码里用 `a ^ 1` 表达「不是 $v_a$」的乘数（$v_a$ 只会是 0/1）。

**无解判定**：

$$\exists i\in[1,n],\ \text{scc}[i]=\text{scc}[i+n]\ \Rightarrow\ \text{IMPOSSIBLE}$$

**赋值公式**（Tarjan 版，编号小=靠后）：

$$x_i=\begin{cases}0 & \text{scc}[i]<\text{scc}[i+n]\ \text{（“取0”的文字更靠后）}\\[2pt]1 & \text{否则}\end{cases}$$

**手算示例 1（强制为真）**：$n=1$，子句 $(x_1\lor x_1)$（输入 `1 1 1 1`）。$u=v=$ 点 $2$（$x_1=1$ 的点），两条边都是 $\text{neg}(2)=1\to 2$。Tarjan 从点 1 进：$1\to 2$，点 2 无出边先出栈成 SCC 编号 **1**；点 1 后出栈成编号 **2**。于是 $\text{scc}[1]=2,\ \text{scc}[2]=1$，`vis[1] < vis[2]` 不成立 → 输出 1 ✓（实测 POSSIBLE 1）。

**手算示例 2（矛盾）**：$n=1$，两个子句 $(x_1\lor x_1)$ 与 $(\lnot x_1\lor\lnot x_1)$（输入 `1 1 1 1`、`1 0 1 0`）。实得边 $1\to 2$（第一句）、$2\to 1$（第二句），两点互达同 SCC，`vis[1]==vis[2]` → IMPOSSIBLE ✓（实测）。

## 逐行代码解析

代码拆成 3 个模块，按顺序拼接即完整程序。

### 模块 1 全局数组与初始化

```cpp
#include<bits/stdc++.h>
using namespace std;
vector<unsigned int> dfn,low;
vector<int> sk,vis;
vector<bool> in_sk;
vector<vector<int>> edges;
unsigned int tim,top_idx,scc_cnt;
void init(int n)
{
	edges.assign(2 * n + 1,vector<int>());
	dfn.assign(2 * n + 1,0);
	low.assign(2 * n + 1,0);
	in_sk.assign(2 * n + 1,false);
	sk.assign(2 * n + 1,0);
	vis.assign(2 * n + 1,-1);
	tim = 0;
	top_idx = 0;
	scc_cnt = 0;
}


```

- 六个容器全部开 $2n+1$：变量拆成 $2n$ 个文字点，下标从 1 用起。
- `dfn/low` 无符号存时间戳；`vis` 存 **SCC 编号**（初值 $-1$）；`in_sk` 表示在栈内；各计数归零。
- 下标 0 全程不使用（点从 1 到 $2n$），所有数组多开一位。

### 模块 2 mark 与 Tarjan 缩点

```cpp
inline void mark(int idx)
{
	dfn[idx] = low[idx] = ++tim;
	sk[++top_idx] = idx;
	in_sk[idx] = true;
}

void tarjan(int idx)
{
	mark(idx);
	for(int to : edges[idx])
	{
		if(!dfn[to])
		{
			tarjan(to);
			low[idx] = min(low[idx],low[to]);
		}
		else if(in_sk[to]) low[idx] = min(low[idx],dfn[to]);
	}
	
	if(dfn[idx] == low[idx])
	{
		++scc_cnt;
		while(in_sk[idx])
		{
			int id = sk[top_idx--];
			in_sk[id] = false;
			vis[id] = scc_cnt;
		}
	}
}


```

- `mark` 是对每个首次访问的点的四件套初始化（时间戳、入栈、在栈标记）。
- 回边更新只对 `in_sk[to]` 生效：不在栈里的 `to` 属于已经定稿的 SCC，不能用于减小 `low`。
- `dfn[idx]==low[idx]` 时以 `idx` 为根出栈：本代码用 `while(in_sk[idx])` 出栈——循环直到弹出 `idx` 自己、`in_sk[idx]` 变假才停，恰好弹出本 SCC 全部节点；每弹一个点就记 `vis[id]=scc_cnt`（编号从 1 递增，汇点先编号、编号小）。
- 若把出栈条件写成 `while(x != idx)`，与 `in_sk[idx]` 写法在标准 Tarjan 中等价，但要与 `in_sk` 维护严格配套。

### 模块 3 主函数（建边、判无解、输出）

```cpp
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	
	int n,m;
	cin >> n >> m;
	init(n);
	//0在【1，n】 1在【n + 1,2n】;
	for(int d = 1;d <= m;d++)
	{
		int i,a,j,b;
		cin >> i >> a >> j >> b;
		// i != a -> j = b
		edges[i + (a ^ 1) * n].emplace_back(j + b * n);
		
		// j != b -> i = a;
		edges[j + (b ^ 1) * n].emplace_back(i + a * n);
	}
	for(int i = 1;i <= 2 * n;i++)
	{
		if(!dfn[i]) tarjan(i);
	}
	
	for(int i = 1;i <= n;i++)
	{
		if(vis[i] == vis[i + n])
		{
			cout << "IMPOSSIBLE\n";
			return 0;
		}
	}
	
	cout << "POSSIBLE\n";
	for(int i = 1;i <= n;i++)
	{
		if(vis[i] < vis[i + n]) cout << 0 << ' ';
		else cout << 1 << ' ';
	}
	return 0;
}
```

- `edges[i + (a ^ 1) * n]`：$a=1$ 时 $a\oplus1=0$ 乘 $n$ 得 0，落在点 $i$（$x_i=0$ 的点）——即「$x_i$ 不是 $a$ 就强制 $x_j=b$」；`j + b * n` 同理（$b=1$ 落在 $j+n$）。
- 建边两行即一条子句的两条逆否边（输入格式保证 $a,b\in\{0,1\}$，`a ^ 1` 才安全）。
- 对 $1\sim2n$ 全部跑 Tarjan（图不一定连通）。
- `vis[i]==vis[i+n]` 判无解（同 SCC）；有解时按 `vis[i]<vis[i+n]` 输出 0，否则 1（「取 0」的文字编号小 = 更靠后 = 选真）。

## 复杂度分析

- 建图 $O(m)$；Tarjan 对 $2n$ 个点、$2m$ 条边 $O(n+m)$；赋值 $O(n)$。
- 总时间 $O(n+m)$；空间：邻接表 $O(2n+2m)$、六个一维数组均 $O(n)$ → $O(n+m)$。

## 适用场景

**能用**：每个子句至多两个文字的布尔约束——或、蕴含（$\lnot a\lor b$）、异或、相等、强制取值（用两次真值表推导逐一验证是否确实只想选一个值）。「恰好一个为真」当 $k>2$ 时不能直接表示，需辅助变量或换模型。

**选择判定信号**：问题能表述为「每个变量二选一 + 每对约束最多涉及两个变量」且不含计数/优化目标 → 2-SAT；出现「最小化真变量个数」「恰好 k 个」等目标 → 不是单纯 2-SAT（可能 NP-hard 或要加状态压缩）。

**边界意识**：$n$ 到 $10^6$ 级时递归 Tarjan 会爆栈（本代码递归实现），需手写迭代 Tarjan 或 Kosaraju，或调大栈空间（不通用）。

## 常见陷阱与注意事项

1. **编号约定**：本代码「点 $i$=$x_i$ 取 0、点 $i+n$=$x_i$ 取 1」；思路笔记模板是「$i$=真」。建边、`neg`、最终输出三处必须同一套约定，改一处不改另两处必错。换约定时赋值判断要跟着换。
2. **SCC 编号方向搞反**：Tarjan 编号小=靠后，判断 `<`；Kosaraju 编号大=靠后，判断 `>`。换实现不换判断必错；记不住就用「谁靠后选谁」现场推。
3. **只加一条边**：每条子句必须加两条逆否边。只加一条会漏强制方向，可能把有解判成无解或给出错误赋值。
4. **无解判定漏变量**：必须检查所有 $i\in[1,n]$ 的 `vis[i]==vis[i+n]`，不能只查一部分。
5. **递归爆栈**：$n$ 大时递归 Tarjan 可能栈溢出；改迭代 Tarjan 或 Kosaraju，勿依赖调大系统栈。
6. **`a ^ 1` 的前提**：输入保证 $a,b$ 只取 0/1 才安全；若输入可能带其他值，`a^1` 语义就错（取反不对位），要先归一化。
7. **0 下标未用**：点从 1 到 $2n$，数组要开 $2n+1$；把 0 当成点时 `neg` 与建边全乱。
8. **超级点不赋值**：SCC 只是绑定的文字集，没有真假可赋；真正做选择的是变量，看的是两文字所在 SCC 的拓扑先后。
9. **以为不可达就能随便挑**：互相不可达只说明单向不冲突，不代表全局安全，必须按拓扑序统一赋值。
10. **把「或」翻反**：$a\lor b$ 的两条边是 $\lnot a\to b$、$\lnot b\to a$，不是 $a\to b$；方向反过来含义变成「$a$ 真则 $b$ 假」类错误约束。

## 对比与扩展

**Tarjan vs Kosaraju**：两者都 $O(n+m)$；差异只在编号方向——Tarjan 编号逆拓扑（小=靠后，判 `<`），Kosaraju 编号正拓扑（大=靠后，判 `>`）。建议只背一种：Tarjan + `<` 或 Kosaraju + `>`。

**与二分图匹配**：匈牙利靠增广路局部调整（匹配的交换性质），冲突可局部挪动；2-SAT 约束是蕴含链，冲突在链末端、非局部可修复，硬套回溯就是 DPLL 搜索（最坏 $O(2^n)$）。两者的可解来源不同：匹配靠交换性，2-SAT 靠「每子句两文字」的图结构。

**与一般 SAT**：子句长度升到 3 即 NP-hard；Min-Ones 2-SAT 等价顶点覆盖同属 NP-hard——所以 2-SAT 的线性性只对应「存在一个解」这一件事，别往带优化目标的场景套。

**模板题**：洛谷 P4782【模板】2-SAT（本代码即其通过版本，题号由用户确认）。