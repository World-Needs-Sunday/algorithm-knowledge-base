---
id: graph-tarjan-scc
title: 'Tarjan 强连通分量'
category: 图论
subcategory: 连通分量
subSubcategory: 强连通分量
tags: ["图论", "强连通分量", "Tarjan", "DFS", "缩点"]
timeComplexity: 'O(n+m)'
spaceComplexity: 'O(n+m)'
codePath: '图论\连通分量\Tarjan强连通分量\Untitled1--fenfeng.cpp'
level: A
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 算法原理

Tarjan 算法用于求有向图中所有的**强连通分量（Strongly Connected Components, SCC）**，它的核心是**一次深度优先搜索（DFS）**，复杂度为线性，实际运行效率很高，是竞赛中最常用的 SCC 算法。

### 什么是强连通分量

对于一个有向图 $G$，一个点集 $S$ 称为强连通分量，当且仅当 $S$ 中任意两个节点互相可以到达（默认节点可以到达自身）。当一个强连通分量无法在保持强连通性质的同时再扩张时，称其为**极大强连通分量**，通常简称为 SCC。

极大 SCC 有两个关键性质：

- 每一个点只属于一个 SCC，即不同的极大 SCC **没有交集**。
- 将所有极大 SCC 缩成一个点后，所得的图是 **DAG**（有向无环图）。

这两条性质是「缩点」类题目成立的基础。

### DFS 搜索树与四种边

任意一个有向图都可以从任意一点开始 DFS，得到一棵搜索树。Tarjan 在 DFS 过程中把每条边归为四类：

1. **树边**：搜索树上的边（指向未访问节点的边）。
2. **返祖边**：指向祖先节点的边。
3. **横叉边**：从一棵子树指向另一棵子树的边。
4. **前向边**：指向子树中（后代）节点的边。

其中前向边不携带任何有用信息；返祖边与树边一定构成环，横叉边与树边可能构成环。DFS 的性质保证有向图中只会有这四种边。

### 核心思想：`dfn` 与 `low`

Tarjan 只需在原图上进行一次 DFS 就能求出所有 SCC，它维护两个数组：

- `dfn[u]`：节点 $u$ 第一次被 DFS 访问时的时间戳。
- `low[u]`：从 $u$ 出发，经过若干条 DFS 树边以及**至多一条返祖边**，能够到达的最小 `dfn`。

当 DFS 处理节点 $u$ 时，先把它标记入栈；对每个邻点 $v$ 分三种情况处理（详见「核心公式」一节）。处理完所有邻点后，若 $dfn[u] = low[u]$，说明 $u$ 是当前 SCC 的根，栈中从栈顶到 $u$ 的所有节点恰好构成一个 SCC，直接统计并全部出栈即可。

由于 DFS 前序遍历的特点，在处理完一个 SCC 之前不会退出，Tarjan 正是利用这一性质构造出线性解法。

## 核心公式 / 状态定义与转移方程

Tarjan 没有数值型的递推式，它的「状态」是两个时间戳数组，转移规则体现在 `low` 的更新上。

### 状态定义

对每个节点 $u$ 维护：

- $dfn[u]$：DFS 首次访问 $u$ 的时间戳，按访问顺序递增编号。
- $low[u]$：$u$ 能回溯到的最小时间戳，定义为「从 $u$ 出发，经过若干条树边以及至多一条返祖边能到达的最小 $dfn$」。

### 转移：三种情况

DFS 到 $u$ 时，先令 $dfn[u] = low[u] = \text{++}idx$ 并压栈。随后对每条出边 $u \to v$：

1. **$v$ 未访问**（树边）：搜索树中 $u$ 是 $v$ 的父节点，$v$ 能到达的节点 $u$ 一定能到达，故先递归处理 $v$，再用 $low[v]$ 更新 $low[u]$：

$$low[u] = \min(low[u],\, low[v])$$

2. **$v$ 已访问且在栈中**（返祖边或横叉边）：用 $v$ 的时间戳更新 $low[u]$：

$$low[u] = \min(low[u],\, dfn[v])$$

3. **$v$ 已访问但不在栈中**：说明 $v$ 已在另一个 SCC 中且处理完毕，直接跳过。

### 判定 SCC 根

处理完 $u$ 的所有邻点后，若满足：

$$dfn[u] = low[u]$$

则 $u$ 是当前 SCC 的根，此时栈中从栈顶到 $u$ 的所有节点（含 $u$）恰好构成一个 SCC，把它们全部弹出并标记为同一个分量编号。

## 逐行代码解析

下面把 Tarjan 模板按职责拆成四个模块，各模块代码按顺序拼接即为 `codePath` 指向的 `.cpp`。

### 模块一：头文件与全局数据结构

```cpp
#include<bits/stdc++.h>
#define endl '\n'
#define fi first
#define se second
#define lowbit(x) x&(-x)
#define int long long
using namespace std;
typedef pair<int,int> PII;
typedef long long ll;
//const int N=1e5+10;
//int cnt=0;
//int a[N];
inline int read();
inline void write(int x);
const int MAXN = 10005;
vector<int> G1[MAXN];  // 邻接表存储有向图
int dfn[MAXN], low[MAXN], scc[MAXN];  // scc[u]表示u所属的SCC编号
stack<int> st;
int idx = 0, tot = 0;  // 时间戳、SCC数量
bool vis[MAXN];  // 标记是否在栈中
```

- `G1` 是邻接表，存储有向图；`dfn`、`low` 是时间戳数组，`scc[u]` 记录 $u$ 所属的 SCC 编号。
- `st` 是辅助栈，`idx` 是全局时间戳，`tot` 是已求出的 SCC 数量，`vis[u]` 标记 $u$ 是否在栈中。
- `#define int long long` 把 `int` 全部替换成 `long long` 防溢出，因此入口函数写作 `signed main`。
- `read` / `write` 是快读快写函数，声明在前、实现放在文件末尾。

### 模块二：Tarjan 主体

```cpp
void tarjan(int u) {
    dfn[u] = low[u] = ++idx;
    st.push(u);
    vis[u] = true;
    for (int v : G1[u]) {
        if (!dfn[v]) {  // v未访问
            tarjan(v);
            low[u] = min(low[u], low[v]);
        } else if (vis[v]) {  // v已访问且在栈中（回边）
            low[u] = min(low[u], dfn[v]);
        }
    }
    // 找到SCC的根
    if (dfn[u] == low[u]) {
        tot++;
        int v;
        do {
            v = st.top();
            st.pop();
            vis[v] = false;
            scc[v] = tot;
        } while (v != u);
    }
}
```

- 进入时给 $u$ 打上时间戳并压栈，`dfn[u]` 与 `low[u]` 初始都为当前时间戳。
- 遍历每条出边：`!dfn[v]` 表示 $v$ 未访问，递归后用 `low[v]` 更新 `low[u]`；`vis[v]` 表示 $v$ 在栈中（返祖边或横叉边），用 `dfn[v]` 更新 `low[u]`；两者都不满足说明 $v$ 已出栈（属于别的 SCC），跳过。
- 所有邻点处理完，若 `dfn[u] == low[u]`，则 $u$ 是 SCC 的根：`tot` 自增，用 `do...while` 把栈顶到 $u$ 的节点依次弹出并染成 `tot`。

### 模块三：主函数

```cpp
signed main(){
//    freopen(".in","r",stdin);
//    freopen(".out","w",stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);cout.tie(nullptr);
	//int n;cin>>n;
    return 0;
}
```

- 关闭同步流并解除绑定，加快输入输出；文件读写两行默认注释掉，按需打开。
- 主函数为模板占位：实际使用时在 `main` 中读入 $n$、$m$ 并建图，然后对每个未访问的节点调用一次 `tarjan`，最后按需输出 `tot` 或 `scc` 数组。

### 模块四：快读快写

```cpp
inline int read(){
    int x=0,f=1;
    char ch=getchar();
    while(ch< '0'||ch>'9'){
        if(ch=='-') f=-1;
        ch=getchar();
    }
    while(ch>='0'&&ch<='9'){
        x=(x<<1)+(x<<3)+(ch^48);
        ch=getchar();
    }
    return x*f;
}
inline void write(int x){
    if(x<0){
        putchar('-');
        x=-x;
    }
    if(x>9) write(x/10);
    putchar(x%10+'0');
}
```

- `read`：快读，逐字符读入并处理负号，`ch^48` 把数字字符转成数值，`x = (x<<1) + (x<<3) + ...` 等价于 `x = x*10 + ...`。
- `write`：快写，负数先输出负号，然后按位递归输出，`putchar(x%10+'0')` 把个位数字转成字符。

## 复杂度分析

- **时间复杂度**：$O(n+m)$。每个节点恰好入栈一次、出栈一次，每条边恰好被访问一次，一次 DFS 的总代价与图的规模线性相关。Tarjan 只做一次 DFS，常数很小，实际运行效率很高。
- **空间复杂度**：$O(n+m)$。邻接表 `G1` 占 $O(n+m)$；`dfn`、`low`、`scc`、`vis` 与栈各占 $O(n)$。整体为线性。

## 适用场景

1. **强连通分量检测**：求出有向图中所有 SCC，分析图的结构。
2. **缩点**：把每个 SCC 缩成一个点，将一般有向图转化为 DAG，之后可做拓扑排序、最长路、DP 等。
3. **2-SAT 问题**：用 Tarjan 求 SCC 后，根据每个变量的两个状态是否落在同一 SCC 中判断可行性。
4. **有向图连通性判断**：判断两个节点是否互相可达。

## 常见陷阱与注意事项

1. **「已访问但不在栈中」要跳过**：第三种情况（$v$ 已访问且已出栈）表示 $v$ 属于另一个已处理完的 SCC，不能再更新 `low[u]`，否则会把不同 SCC 错误地合并。
2. **`low` 的更新要分情况**：树边（$v$ 未访问）用 `low[v]` 更新，返祖边（$v$ 在栈中）用 `dfn[v]` 更新。两者不能混用：返祖边只能「跳一步」到祖先，不能用后代的 `low`。
3. **前向边无需处理**：前向边指向已访问的后代，不携带「回到更早节点」的信息，DFS 中会被「已访问」的判定自然跳过，不影响结果。
4. **横叉边与返祖边都落在「在栈中」分支**：有向图中只要终点仍在栈中，无论是返祖边还是横叉边，统一用 `dfn[v]` 更新 `low[u]`，实现上用 `vis[v]` 判断即可。
5. **不连通图要遍历所有节点**：图可能不连通，需要对每个未访问的节点都启动一次 `tarjan`，不能只从一个起点出发。
6. **链状图可能爆栈**：Tarjan 用递归实现，当图是一条很长的链时，DFS 深度可达 $O(n)$；$n$ 很大（如 $10^5$、$10^6$ 量级）时系统栈可能溢出，应改用显式栈的迭代写法或增大栈空间。

## 对比与扩展

### 与 Kosaraju 的对比

求 SCC 最常用的两个算法是 Tarjan 与 Kosaraju：

| 算法 | 思路 | DFS 次数 | 常数 | 直观程度 |
|------|------|---------|------|---------|
| Tarjan | 一次 DFS + 栈 + `dfn`/`low` | 1 次 | 小 | 较难理解 |
| Kosaraju | 正向 DFS 求出栈序 + 反图 DFS | 2 次 | 稍大 | 直观易理解 |

两者时间复杂度相同，都是 $O(n+m)$。Tarjan 只需一次 DFS、常数更小、代码更短，竞赛中最常用；Kosaraju 需要额外存反图、多跑一遍 DFS，但思路更清晰，适合入门理解 SCC。相关内容见「Kosaraju（强连通分量）」。

### 缩点：SCC 的典型应用

求出每个点所属的 SCC（`scc[u]`）后，可以构建**缩点图**：每个 SCC 是一个新节点，原图中跨 SCC 的边变成缩点图中的边。由极大 SCC 的性质，缩点图一定是 DAG。

```cpp
// 伪代码：构建缩点图
vector<vector<int>> new_edges(tot + 1);
for (int u = 1; u <= n; u++)
    for (int v : G1[u])
        if (scc[u] != scc[v])
            new_edges[scc[u]].push_back(scc[v]);
```

缩点后可以做：拓扑排序上的 DP（如求经过节点权值最大的路径）、求 DAG 上的最长路/最短路、统计入度为 0 的缩点个数（至少需要多少个起点才能走遍全图）等。
