---
id: graph-prim
title: 'Prim 最小生成树（堆优化·Lazy 版）'
category: 图论
subcategory: 最小生成树
tags: ["图论", "最小生成树", "Prim", "堆优化", "优先队列", "贪心", "邻接表"]
timeComplexity: 'O((n + m) log n)'
spaceComplexity: 'O(n + m)'
codePath: '图论\最小生成树\Prim\Untitled1--fenfeng.cpp'
level: A
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 算法原理

Prim 算法用于求解**最小生成树**（Minimum Spanning Tree, MST）：给定一张含 $n$ 个节点、$m$ 条边的**带权无向连通图**，选择其中若干条边，使所有节点仍然连通，且所选边的权值之和最小。所选出的这棵边权和最小的树就是最小生成树。

与 Kruskal 的"边贪心"（全局最小边优先）不同，Prim 采用**点贪心**策略，思路与 Dijkstra 高度相似：维护一个已选点集合 $S$，每一步从 $S$ 之外的节点中选一个**离 $S$ 最近**的节点加入 $S$，同时把连接它的那条最小边计入生成树。重复这一过程直到所有节点都进入 $S$，就得到了最小生成树。

Prim 的正确性基于**切定理**：把节点划分为已选集合 $S$ 与未选集合 $V \setminus S$，横跨这两个集合的边中权值最小的一条，一定属于某棵最小生成树。Prim 每一步都在维护这样一个切，并选取横跨切的最小边，因此最终得到的树必为最小生成树。

本实现直接沿用了 Dijkstra 的堆优化框架：用小根堆维护"候选节点及其到 $S$ 的当前最短连接边权"，并用一个 `d` 数组记录每个节点当前到 $S$ 的最短边权。扩展时**只有在发现更优边权时才入堆**（即 `d` 数组剪枝），这相当于 Lazy Prim 与 Eager Prim 的折中——既无需 decrease-key，又能显著减少堆中过期元素的堆积。

## 核心公式 / 状态定义与转移方程

### 状态定义

$$\text{vis}[v] = \text{节点 } v \text{ 是否已加入生成树 } S$$

$$d[v] = \min_{u \in S} w(u, v) \quad (v \notin S \text{ 到已选集合 } S \text{ 的最短连接边权})$$

### 初始化

$$d[v] = +\infty \ (v \neq S),\qquad d[S] = 0,\qquad \text{vis 全部为 } 0$$

起始将 $(S, 0)$ 入堆，表示起点 $S$ 以边权 $0$ 连向一个"虚拟根"，这保证起点能被率先选中且不会给答案多加权值。

### 选点与累加

每轮从堆中弹出边权最小的候选 $(id, x)$：

- 若 $\text{vis}[id] = 1$，说明该节点已被加入生成树，这是过期候选，直接跳过（惰性删除）；
- 否则置 $\text{vis}[id] = 1$，把 $x$ 累加进答案，并把已选节点数加一。

### 剪枝式扩展

对 $id$ 的每条出边 $(id, v, w)$：

$$\text{若 } w < d[v] \implies d[v] = w;\quad \text{且当 } \text{vis}[v] = 0 \text{ 时，将 } (v, d[v]) \text{ 入堆}$$

这里"仅当 $w < d[v]$ 才入堆"就是本实现相对纯 Lazy Prim 的剪枝：每个节点只在找到更短连接边时被重新入堆。

### 终止条件

$$\text{已选节点数} = n \implies \text{生成树完成}$$

$$\text{堆已空且已选节点数} < n \implies \text{图不连通，无生成树}$$

## 逐行代码解析

下面把代码按职责拆成四个模块，各模块代码按顺序拼接即为 `codePath` 指向的 `.cpp`。

### 模块一：头文件与全局数据结构

```cpp
#include <bits/stdc++.h>
using namespace std;
const int INF = 1e9;
const int MaxN = 1e6 + 10;
int n, m, cnt = 0, ans = 0;
vector<pair<int, int>> G1[MaxN];
vector<int> d, vis;
struct Point {
    int id, x;
    bool operator<(const Point &aa) const { return x > aa.x; }
};
```

- `G1`：邻接表，`G1[u]` 存 $u$ 的所有出边 `(邻居, 边权)`。
- `d[v]`：节点 $v$ 到已选集合 $S$ 的当前最短连接边权；`vis[v]`：$v$ 是否已加入生成树。
- `Point` 是堆中的元素，`id` 为节点编号、`x` 为其到 $S$ 的最短边权。重载 `operator<` 为 `x > aa.x`，使得 `priority_queue` 成为按 `x` 取最小的小根堆。
- `cnt` 与 `ans` 的命名与常见写法相反：本代码中 `cnt` 累加边权和、`ans` 计数已选节点数，阅读时需注意。

### 模块二：solve 初始化

```cpp
void solve(int S) {
    d.assign(1e6, INF);
    vis.assign(1e6, 0);
    d[S] = 0;
    priority_queue<Point> q;
    q.push({S, 0});
```

- `d` 全部置为 `INF`、`vis` 全部置为 `0`，再把起点 `S` 的 `d[S]` 置为 `0`。
- 建立小根堆 `q`，将 $(S, 0)$ 入堆作为最初的候选。

### 模块三：主循环——选点与剪枝扩展

```cpp
    while (!q.empty()) {
        auto it = q.top(); q.pop();
        if (vis[it.id]) continue;
        vis[it.id] = 1;
        cnt += it.x;
        //cout<<cnt<<endl;
        if (ans >= n) break;
        ans++;
        for (auto t : G1[it.id]) {
            if (t.second < d[t.first]) {
                d[t.first] = t.second;
                if (!vis[t.first]) { q.push({t.first, d[t.first]}); }
            }
        }
    }
}
```

- `q.top(); q.pop();` 取出当前边权最小的候选，随后弹出。
- `if (vis[it.id]) continue;` 是**惰性删除**：同一节点可能被多次入堆，只有第一次弹出时才真正选中，后续过期副本一律跳过。
- `cnt += it.x` 累加选中边的权值；起点 `x = 0`，因此不会给答案多加权值。
- `//cout<<cnt<<endl;` 是一行被注释掉的调试输出，不影响结果。
- `if (ans >= n) break;` 在已选满 $n$ 个节点后提前退出；`ans++` 在选中每个节点后使已选节点数加一。
- 扩展循环遍历 `it.id` 的所有出边：只有当 `t.second < d[t.first]`（发现更短连接边）时才更新 `d` 并入堆，这就是 `d` 数组剪枝，等价于"每个节点只保留比已记录更优的候选"。

### 模块四：main 建图与输出

```cpp
int main() {
    cin >> n >> m;
    for (int i = 1; i <= m; i++) {
        int a, b; cin >> a >> b;
        int c; cin >> c;
        G1[a].push_back({b, c});
        G1[b].push_back({a, c});
    }
    solve(1);
    if (ans != n) cout << "impossible" << endl;
    else cout << cnt << endl;
    return 0;
}
```

- 无向图：每条边 `(a, b, c)` 都同时 `G1[a].push_back({b, c})` 与 `G1[b].push_back({a, c})` 双向建边。
- 以节点 $1$ 为起点调用 `solve(1)`。
- 连通判定：`ans == n` 表示所有 $n$ 个节点都已加入生成树，输出边权和 `cnt`；否则输出 `"impossible"`（图不连通）。

## 复杂度分析

### 时间复杂度

| 阶段 | 复杂度 |
|---|---|
| 建图（邻接表） | $O(m)$ |
| 初始化 `d` / `vis` | $O(n)$ |
| 堆操作与松弛 | $O((n + m) \log n)$ |
| **总计** | $O((n + m) \log n)$ |

每个节点首次被弹出时（`vis` 由 0 变 1）会遍历一次它的邻接表，所有节点的出边合计被访问 $O(m)$ 次；每次对 `d` 的改进最多引发一次入堆，堆中元素最多 $O(m)$，单次入堆/出堆为 $O(\log n)$。因此总复杂度为 $O((n + m) \log n)$。

### 空间复杂度

| 数据结构 | 空间 |
|---|---|
| 邻接表 `G1` | $O(n + m)$ |
| `d` / `vis` 数组 | $O(n)$ |
| 小根堆 `q` | $O(m)$（最坏情况） |
| **总计** | $O(n + m)$ |

## 适用场景

- **稠密图**：$m$ 接近 $n^2$ 时，Prim 表现不错；若改用邻接矩阵 + 线性扫描最小值的写法可进一步降到 $O(n^2)$。
- **需要从指定起点扩展**：Prim 天然从某个起始节点出发，适合有起点要求的问题。
- **边可在线生成、流式处理**：Prim 可以边读入边扩展，不必像 Kruskal 那样预先收集全部边再排序。
- **与 Dijkstra 同构**：如果已经熟练 Dijkstra 的堆优化写法，Prim 几乎可以直接套用同一框架，学习成本低。

不适合直接用 Prim 的情况：图可能不连通且需要**最小生成森林**时，Prim 只生成一棵树，需要对每个连通分量分别运行，而 Kruskal 天然支持森林；一般 OI 中 Kruskal 因实现更简单、对稀疏图更友好而更常被选用。

## 常见陷阱与注意事项

1. **`d` 数组必须初始化为 INF、起点置 0**
   - `d.assign(1e6, INF)` 后必须 `d[S] = 0`，否则起点无法被率先选中，算法无法启动。

2. **惰性删除不能省略**
   - 同一节点可能被多次入堆，弹出时必须 `if (vis[it.id]) continue;`，否则会重复累加边权、破坏生成树。

3. **`cnt` 与 `ans` 命名相反**
   - 本代码中 `cnt` 累加边权和、`ans` 计数已选节点数，与站主版（`ans` 为边权和、`cnt` 为节点数）恰好相反，改写或对比时务必看清语义。

4. **无向图必须双向建边**
   - `G1[a]` 与 `G1[b]` 都要 push，否则部分节点不可达，会被误判为不连通而输出 `impossible`。

5. **剪枝条件要用严格小于**
   - 只有 `t.second < d[t.first]` 才更新并入堆；若写成 `<=`，等权边会反复入堆，虽不改变答案却会堆积大量冗余候选。

6. **不连通判定**
   - 循环结束后用 `if (ans != n)` 判断是否所有节点都被选中；堆空但 `ans < n` 说明图不连通，应输出无解。

7. **数组容量与初始化长度一致**
   - `G1` 开到 `MaxN = 1e6 + 10`，而 `d.assign(1e6, INF)` 只分配了 $10^6$ 个元素，节点编号需落在 $[1, 10^6]$ 内，否则越界。

8. **重边与自环**
   - 重边全部入堆后，堆保证最小权值的边先弹出，较大边随后被 `vis` 惰性跳过，无需额外去重；自环在扩展时因 `vis[it.id]` 已置 1 而天然被跳过。

## 对比与扩展

### Prim vs Kruskal

| 对比维度 | Prim（堆优化） | Kruskal |
|---|---|---|
| 策略 | 点贪心：从已选集合向外扩展最近点 | 边贪心：全局最小边优先 |
| 核心数据结构 | 优先队列 + 邻接表 | 并查集 + 边集排序 |
| 时间复杂度 | $O((n + m) \log n)$ | $O(m \log m)$ |
| 空间复杂度 | $O(n + m)$ | $O(n + m)$ |
| 稠密图 | 友好（矩阵版 $O(n^2)$） | 排序开销大 |
| 稀疏图 | 一般 | 更友好 |
| 不连通图 | 只生成一棵树，需逐分量重跑 | 天然生成最小生成森林 |
| 实现难度 | 中（堆 + 惰性删除） | 低（排序 + 并查集） |

### Prim 与 Dijkstra 的关系

Prim 与 Dijkstra 的堆优化框架几乎一致，都是"从起点出发、用优先队列维护当前最近的未访问节点"，区别仅在于距离的含义与更新方式：Dijkstra 优化源点到各点的最短路，按 $d[v] = \min(d[v], d[u] + w)$ 松弛；Prim 优化节点到已选集合的最短连接边，只需比较边的原始权值 $w$ 与 $d[v]$。因此熟悉 Dijkstra 即可快速掌握 Prim。

### `d` 数组剪枝：Lazy 与 Eager 的折中

纯 Lazy Prim 每次扩展把所有未访问邻居都入堆，堆中过期元素可达 $O(m)$；Eager Prim 维护每个节点唯一的最短边并需要 decrease-key，而 C++ STL 的 `priority_queue` 不支持。本实现额外维护 `d[v]`，入堆前先判 `w < d[v]`，使每个节点只在发现更短边时入堆，明显减少过期元素，复杂度仍为 $O((n + m) \log n)$，且无需手写索引堆。

### 扩展：邻接矩阵版（$O(n^2)$）

稠密图（$m \approx n^2$）下可用邻接矩阵 + 线性扫描最小值，把复杂度降到 $O(n^2)$，优于堆优化版：

```cpp
for (int i = 1; i <= n; i++) { key[i] = INF; vis[i] = 0; }
key[1] = 0;
for (int i = 1; i <= n; i++) {
    int u = -1, mn = INF;
    for (int j = 1; j <= n; j++)
        if (!vis[j] && key[j] < mn) { mn = key[j]; u = j; }
    vis[u] = 1; ans += key[u];
    for (int v = 1; v <= n; v++)
        if (!vis[v] && G[u][v] < key[v]) key[v] = G[u][v];
}
```

### 扩展：最大生成树

把堆的比较方向反转（由小根堆改为大根堆），每次选连接边权最大的点，即可求最大生成树；Kruskal 中对应地把边按权值降序排序。

### 扩展：完全图与 Borůvka

对于完全图或边权由规则动态生成、无法显式存边的 MST 问题，Borůvka 算法的"每轮为每个连通块求最小出边"框架往往能配合数据结构优化到远低于枚举所有边，此时比 Prim、Kruskal 更合适。
