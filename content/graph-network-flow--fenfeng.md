---
id: graph-network-flow
title: '网络流（最大流 Dinic 与费用流）'
category: 图论
subcategory: 网络流
subSubcategory: ''
tags: ["图论", "网络流", "最大流", "Dinic", "最小割", "费用流"]
timeComplexity: 'O(n^2 m)'
spaceComplexity: 'O(n + m)'
codePath: '图论\网络流\Untitled1.cpp'
level: A
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 算法原理

网络流研究的是「在有容量限制的有向图中，如何最有效率地把某种"流"从源点运送到汇点」。一个**流网络**由有向图、每条边 $(u,v)$ 上的非负**容量** $c(u,v)$、**源点** $s$ 与**汇点** $t$ 组成；一个合法的**流** $f$ 必须同时满足容量限制、流量守恒与反对称性三条约束（详见下一节）。

网络流有三大经典问题：**最大流**（$s$ 到 $t$ 最多能输送多少流量）、**最小割**（至少删掉多少容量的边才能让 $s$ 与 $t$ 不连通）、**费用流**（在流量最大的前提下使总运输费用最小）。三者紧密相连，其中**最大流 = 最小割**是最核心的定理之一。

求最大流的直观思路是**增广路**：不断找一条从 $s$ 到 $t$ 的可行路径，把这条路径能承载的流量"塞"进去，直到塞不下为止。但直接贪心会出问题——一旦选错路径，可能永远达不到全局最大流。为此，实现上为每条边建立一条**反向边**，把"已经用掉的流量"记在反向边上，从而提供一种**撤销（反悔）**机制，让算法能修正之前错误的流量分配。

竞赛中最常用、效率最高的实现是 **Dinic 算法**。它建立在**分层图**与**阻塞流**两个概念之上：

- **阻塞流 (Blocking Flow)**：在构建好的分层图里不断寻找并塞满增广路，直到再也找不到任何 $s \to t$ 路径，此时在分层图里得到的流称为阻塞流。它有一个关键性质：**最大流一定是阻塞流，但阻塞流不一定是最大流**——阻塞流只是在"当前这一层"视角下的局部最优。
- **Dinic 的核心**：通过**反复构建分层图并寻找阻塞流**，一步步逼近全局最大流。

Dinic 的具体过程是四步循环：① 用 BFS 从 $s$ 出发计算每个点到 $s$ 的最短距离并分层，只保留第 $i$ 层指向第 $i+1$ 层的边，于是分层图中任意一条 $s \to t$ 路径都是**最短增广路**；② 在分层图上用 DFS 一次性找出所有增广路并增广，得到阻塞流；③ 用阻塞流更新残量网络；④ 当某次 BFS 无法从 $s$ 到达 $t$ 时终止，累积的流量即为最大流。

Dinic 之所以快，靠两个优化：**分层图**保证每次增广的都是最短路径，且每次重构分层图后 $s$ 到 $t$ 的最短距离**至少增加 1**，故 BFS 次数最多为 $n$；**当前弧优化**用 `cur[u]` 记下每个节点下一条"还没试过"的出边，跳过已经饱和或已确定无用的边，避免重复扫描。

## 核心公式 / 状态定义与转移方程

流网络的三个基本约束：

1. **容量限制**：每条边上的流量不超过容量。

$$0 \le f(u,v) \le c(u,v)$$

2. **流量守恒**：除源点、汇点外，每个节点流入量等于流出量。

$$\sum_{(v,u) \in E} f(v,u) = \sum_{(u,v) \in E} f(u,v)$$

3. **反对称性**：$f(u,v) = -f(v,u)$，这是处理反向流量的数学约定。

增广与反悔的核心是**残量网络**：正向边 $(u,v)$ 的残量容量 $c_f(u,v) = c(u,v) - f(u,v)$ 表示还能再增广多少；反向边 $(v,u)$ 的容量等于已用流量 $f(u,v)$，用于撤销此前的分配。

**分层图**由 BFS 得到，层号 $\text{level}[u]$ 即 $u$ 到源点按边数计的最短距离：

$$\text{level}[s] = 0, \qquad \text{level}[v] = \text{level}[u] + 1 \quad (\text{沿残量边 } (u,v))$$

DFS 只沿 $\text{level}[v] = \text{level}[u] + 1$ 的边推进，单条增广路能通过的流量等于路径上残量容量的最小值，逐边取 $\min$ 向下传递。

最大流最小割定理：

$$\text{最大流} = \text{最小割}$$

## 逐行代码解析

下面把最大流 Dinic 模板按职责拆成四个模块，各模块代码按顺序拼接即为 `codePath` 指向的 `.cpp`。

### 模块一：头文件与全局数据结构

```cpp
#include <bits/stdc++.h>
using namespace std;
#define int long long
const int N=210;
const int INF=LLONG_MAX;
int n,m,s,t,mxf=0;
#define fi first
#define se second
typedef pair<int,int> PII;
struct ege{
	int to,w;
	int rev;
};
vector<ege> wYG[N],wRG[N];//原图和残量网络
int wLG[N],cur[N];//记录层数
```

- `N=210` 是顶点上限；`#define int long long` 把 `int` 全部替换成 `long long` 防流量溢出，因此入口用 `signed main`。
- 边结构体 `ege` 存 `to`（终点）、`w`（残量容量）、`rev`（反向边在 `wRG[to]` 中的下标）。
- `wYG` 存原图、`wRG` 存残量网络（含反向边）；`wLG` 存分层号、`cur` 存当前弧。
- `INF` 取 `LLONG_MAX` 表示"无穷大流量"，初始流量 `mxf=0`。

### 模块二：bfs 构建分层图

```cpp
bool bfs(){
	for(int i=1;i<=n;i++) wLG[i]=-1;
	queue<int> q;
	q.push(s);
	wLG[s]=0;
	while(q.size()){
		int tmp=q.front();q.pop();
		for(auto i:wRG[tmp]){
			if(wLG[i.to]!=-1) continue;
			if(i.w==0) continue;
			q.push(i.to);
			wLG[i.to]=wLG[tmp]+1;
		}
	}
	if(wLG[t]==-1) return false;
	else return true;
}
```

- 先把所有层号置为 $-1$（未访问），从源点 $s$ 开始 BFS，层号 = 父节点层号 $+1$。
- 只沿"残量容量大于 0"的边走（`i.w==0` 跳过），保证在残量网络上分层。
- 返回 $t$ 是否可达：`wLG[t] == -1` 说明 $s$ 已无法到达 $t$，增广结束。

### 模块三：dfs 阻塞流增广

```cpp
int dfs(int u,int f){
	if(u==t) return f;
	for(int &i=cur[u];i<(int)wRG[u].size();i++){
		ege &e=wRG[u][i];
		if(e.w>0&&wLG[e.to]==wLG[u]+1){
			int ret=dfs(e.to,min(f,e.w));
			if(ret>0){
				e.w-=ret;
				wRG[e.to][e.rev].w+=ret;
				return ret;
			}
		}
	}
	return 0;
}
```

- 到达汇点直接返回可增广流量 `f`；否则只沿"下一层"的边（`wLG[e.to]==wLG[u]+1`）递归。
- `int &i=cur[u]` 是引用，实现**当前弧优化**：已饱和或无用的边不会在下一次 DFS 到 $u$ 时重试。
- 增广后正向边 `e.w` 减去流量、反向边 `wRG[e.to][e.rev].w` 加上流量，即"反悔"机制。
- 一次 DFS 只返回一条增广路的流量，主循环反复调用直到返回 $0$。

### 模块四：main 建图与主循环

```cpp
signed main(){
	cin>>n>>m>>s>>t;
	for(int i=1;i<=m;i++){
		int u,v,w;cin>>u>>v>>w;
		wYG[u].push_back({v,w,0});
		wRG[u].push_back({v,w,(int)wRG[v].size()});
		wRG[v].push_back({u,0,(int)wRG[u].size()-1});
	}
	while(bfs()){
		for(int i=1;i<=n;i++) cur[i]=0;
		while(true){
			int pus=dfs(s,INF);
			if(pus==0) break;
			mxf+=pus;
		}
	}
	cout<<mxf<<'\n';
	return 0;
}
```

- 每条原边同时建正向边与反向边：正向边 `rev` 指向反向边的位置，反向边 `rev` 指向正向边的位置（`wRG[u].size()-1`）。
- 每轮 BFS 成功后把当前弧 `cur` 重置为 $0$，再反复 `dfs(s, INF)` 增广，把返回流量累加进 `mxf`。
- 当 `dfs` 返回 $0$（当前分层图已阻塞）就结束本轮，重新 BFS；最终输出最大流 `mxf`。

## 复杂度分析

- **时间复杂度**：$O(n^2 m)$（$n$ 为顶点数，$m$ 为边数）。原因：每次 BFS 构建分层图花费 $O(m)$；每轮增广后 $s$ 到 $t$ 的最短距离至少增加 1，因此 BFS 至多进行 $O(n)$ 轮；每一轮在分层图上用当前弧优化求阻塞流的 DFS 为 $O(nm)$。整体为 $O(n^2 m)$。实际运行中 Dinic 远达不到这个上界，效率很高。
- **空间复杂度**：$O(n+m)$。残量网络 `wRG` 为每条原边同时存正向边与反向边，共 $2m$ 条有向边；原图 `wYG` 存 $m$ 条；分层数组 `wLG` 与当前弧数组 `cur` 各 $O(n)$。

作为对比，「对比与扩展」中的**费用流**模板用 SPFA 求费用最短路，总复杂度与增广轮数相关，工程上通常慢于最大流。

## 适用场景

- **最大流 / 二分图匹配**：二分图最大匹配可转化为最大流（源点连左部、右部连汇点），因此网络流是比匈牙利算法更通用的匹配工具。
- **分配与调度**：资源分配、任务指派、运输调度等问题，凡能抽象成"有容量上限的流动"都可建模为网络流。
- **最小割 / 割问题**：借助「最大流 = 最小割」，把"删边使图不连通的最小代价"转化为最大流求解。
- **费用流**：在运输、指派中每条边有单位费用，要求"在流量最大的前提下费用最小"时使用。
- **建模类问题**：点容量限制（拆点）、分层图、闭合子图等技巧常与网络流结合。

不适合直接用网络流的情况：普通的单源最短路、最小生成树、拓扑排序等有专属算法的问题，用对应算法更高效、更简单。

## 常见陷阱与注意事项

1. **反向边的 `rev` 必须配对**：建边时正向边的 `rev` 指向反向边在 `wRG[to]` 中的下标，反向边的 `rev` 指向正向边在 `wRG[u]` 中的下标。写错会导致增广时改错边、残量网络错乱。
2. **当前弧 `cur` 每轮 BFS 后要重置为 0**：`cur` 记录的是"当前分层图里已废的边"，换新分层图后必须清空，否则会沿用上一轮的断点而漏掉增广路。
3. **DFS 只能走 `level[to] == level[u] + 1` 的边**：这条判定保证始终沿分层图"前进一层"，否则会退回同层或上一层，破坏分层图结构。
4. **容量为 0 的边要跳过**：`if (i.w == 0) continue;` 若不写，会沿已饱和的边反复递归，造成死循环或错误增广。
5. **`#define int long long` 与 `signed main`**：把 `int` 宏替换成 `long long` 后，`main` 的返回类型也变成 `long long`，必须写成 `signed main()`，否则编译报错。
6. **多组数据要清空**：多次建图时需清空 `wYG` / `wRG` 并把 `mxf` 归零，避免上一组数据残留。
7. **费用流不能用 Dijkstra 求最短路**：反向边的费用取负，图中存在负权边，必须用 SPFA；同时用 `vis` 防止在环上无限递归。

## 对比与扩展

- **最大流 vs 最小割**：二者互为对偶，`最大流 = 最小割` 是网络流最核心的定理，很多"删边代价"问题由此转化为最大流。
- **Dinic vs EK / ISAP**：EK 每次 BFS 只找一条增广路，实现简单但较慢；ISAP 是 Dinic 的改进（省去反复 BFS 的开销）；Dinic 是竞赛中最常用、综合效率最高的实现。
- **反向边 = 反悔贪心**：网络流中反向边的设计，本质上是给贪心增广加上了"撤销"能力，与反悔贪心的思想一脉相承。
- **费用流（SPFA + 增广路）**：在最大流的基础上给每条边增加一个**费用**属性，目标是"保证流量最大的前提下最小化总费用"。常用做法是每轮用 SPFA 在残量网络上找一条费用最短路并沿其增广，重复到找不到增广路为止；因为每次都在当前残量网络上找费用最短路，最终得到的最大流费用最小。

费用流参考模板如下（结构同 Dinic，把 BFS 分层换成 SPFA 求费用最短路，增广时按单位费用累加总费用）：

```cpp
#include <bits/stdc++.h>
using namespace std;
#define int long long
const int N=5e3+10;
const int INF=LLONG_MAX;
int n,m,s,t,mxf=0,mic=0;
#define fi first
#define se second
typedef pair<int,int> PII;
struct ege{
	int to,w;
	int rev;
	int cost;
};
vector<ege> wYG[N],wRG[N];//原图和残量网络
int wLG[N],cur[N];//记录层数
bool inq[N],vis[N];
bool spfa(){
	for(int i=1;i<=n;i++) wLG[i]=INF;
	for(int i=1;i<=n;i++) inq[i]=false;
	queue<int> q;
	q.push(s);
	wLG[s]=0;
	inq[s]=true;
	while(q.size()){
		int tmp=q.front();q.pop();
		for(auto i:wRG[tmp]){
			if(i.w==0) continue;
			if(wLG[i.to]<=wLG[tmp]+i.cost) continue;
			wLG[i.to]=wLG[tmp]+i.cost;
			if(inq[i.to]==false){
				q.push(i.to);
				inq[i.to]=true;
			}
		}
	}
	return inq[t];
}
int dfs(int u,int f){
	if(u==t) return f;
	vis[u]=true;
	int nf=0;
	for(int &i=cur[u];i<(int)wRG[u].size();i++){
		ege &e=wRG[u][i];
		if(e.w>0&&wLG[e.to]==wLG[u]+e.cost&&!vis[e.to]){
			int ret=dfs(e.to,min(f-nf,e.w));
			if(ret>0){
				e.w-=ret;
				wRG[e.to][e.rev].w+=ret;
				mic+=e.cost*ret;
				nf+=ret;
				if(nf==f) break;
			}
		}
	}
	vis[u]=false;
	return nf;
}
signed main(){
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	cin>>n>>m>>s>>t;
	for(int i=1;i<=m;i++){
		int u,v,w,c;cin>>u>>v>>w>>c;
		wYG[u].push_back({v,w,0,c});
		wRG[u].push_back({v,w,(int)wRG[v].size(),c});
		wRG[v].push_back({u,0,(int)wRG[u].size()-1,-c});
	}
	while(spfa()){
		for(int i=1;i<=n;i++) cur[i]=0;
		while(true){
			int pus=dfs(s,INF);
			if(pus==0) break;
			mxf+=pus;
		}
	}
	cout<<mxf<<" "<<mic<<'\n';
	return 0;
}
```
