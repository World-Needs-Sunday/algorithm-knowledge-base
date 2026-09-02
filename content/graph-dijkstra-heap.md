---
id: graph-dijkstra-heap
title: 'Dijkstra 最短路（堆优化）'
category: 图论
subcategory: 最短路
tags: ["图论", "最短路", "Dijkstra", "堆优化", "优先队列", "单源最短路", "贪心"]
timeComplexity: 'O((V+E) log V)'
spaceComplexity: 'O(V+E)'
codePath: '图论\最短路\Dijkstra（堆优化）\Untitled1.cpp'
---

## 算法原理

Dijkstra 算法是求解**非负权图**中单源最短路径的经典算法，基于贪心思想：每一步从未确定最短路的节点中，选取距离源点最近的那个，将其标记为"已确定"，并用它去松弛（relax）相邻节点的距离估计值。

朴素 Dijkstra 的时间复杂度为 $O(V^2)$，在稠密图中表现尚可，但在边数 $E \ll V^2$ 的稀疏图中效率低下。堆优化的核心改进是：用最小堆（优先队列）替代线性扫描来寻找"当前距离最小的未确定节点"，将"取最小"操作从 $O(V)$ 降为 $O(\log V)$。

本代码使用 C++ STL 的 `priority_queue`（配 `greater<>` 比较器实现小根堆），堆中存储 `(distance, node)` 二元组。由于 STL 优先队列不支持 decrease-key 操作（修改堆中已有元素的优先级），代码采用**惰性删除**（lazy deletion）策略：当发现更短路径时直接将新的 `(dist, node)` 压入堆，不修改旧条目；弹出时通过 `vis[]` 数组检查该节点是否已被确定，若已确定则跳过。这样堆中可能存在同一节点的多个过期条目，但每个条目最多被弹出一次，不影响正确性。

松弛操作是 Dijkstra 的核心：对于当前确定的节点 $u$ 及其出边 $(u, v, w)$，若 $A[v] > A[u] + w$，则更新 $A[v] = A[u] + w$ 并将 $(A[v], v)$ 压入堆。这保证了每个节点被确定时，其距离值已经是全局最优（由 Dijkstra 的正确性证明保证：在非负权图中，已确定节点的距离值不会再被更新）。

## 状态定义与转移方程

距离数组定义：

$$A[v] = \text{从源点 } s \text{ 到节点 } v \text{ 的当前最短距离估计值}$$

初始化：

$$A[s] = 0, \quad A[v] = +\infty \quad (v \neq s)$$

松弛转移方程（对当前确定节点 $u$ 的每条出边 $(u, v, w)$）：

$$A[v] = \min\big(A[v],\; A[u] + w\big)$$

松弛条件（仅在距离能改善时执行更新）：

$$\text{if } A[v] > A[u] + w \;\Rightarrow\; A[v] \leftarrow A[u] + w, \;\; \text{push } (A[v], v) \text{ into heap}$$

贪心选择：每轮从堆顶取出 $A$ 值最小的未确定节点 $u$，标记 $vis[u] = \text{true}$，此时 $A[u]$ 即为 $s$ 到 $u$ 的最短路长度（最终答案）。在非负权图中此选择满足最优子结构：已确定节点的距离不会被后续松弛改变。

## 数据结构图示

```
输入示例：n=5, m=7, s=1（有向图）

  边: 1→2(4)  1→3(1)  3→2(2)  2→4(1)  3→4(5)  4→5(1)  2→5(3)

  邻接表：
  edges[1] = {(2,4), (3,1)}
  edges[2] = {(4,1), (5,3)}
  edges[3] = {(2,2), (4,5)}
  edges[4] = {(5,1)}
  edges[5] = {}

  执行过程（堆中 (dist, node)）：

  初始:  A=[INF, 0, INF, INF, INF, INF]
         堆: [(0,1)]

  弹出(0,1), 确定 u=1: 松弛 2→A[2]=4, 松弛 3→A[3]=1
         堆: [(1,3), (4,2)]

  弹出(1,3), 确定 u=3: 松弛 2→A[2]=min(4,1+2)=3, 松弛 4→A[4]=1+5=6
         堆: [(3,2), (4,2)*, (6,4)]      // (4,2) 是过期条目

  弹出(3,2), 确定 u=2: 松弛 4→A[4]=min(6,3+1)=4, 松弛 5→A[5]=3+3=6
         堆: [(4,4), (4,2)*, (6,4)*, (6,5)]

  弹出(4,4), 确定 u=4: 松弛 5→A[5]=min(6,4+1)=5
         堆: [(4,2)*, (5,5), (6,4)*, (6,5)*]

  弹出(4,2)*, vis[2]=true → 跳过（惰性删除）
  弹出(5,5), 确定 u=5: 无出边
  弹出剩余过期条目，全部跳过

  最终:  A=[INF, 0, 3, 1, 4, 5]
         输出: 0 3 1 4 5
```

## 核心操作详解

### 建图：邻接表

- **数据结构**：`vector<vector<pair<int,int>>> edges(n+1)`，`edges[u]` 存储 $u$ 的所有出边，每条边为 `(v, w)` 即 `(终点, 权重)`。
- **关键细节**：使用 `emplace_back(v, w)` 而非 `push_back({v, w})`，直接原地构造 `pair`，略快。图为有向图，仅添加 $u \to v$；若需无向图，应同时添加 `edges[v].emplace_back(u, w)`。

### 堆：小根优先队列

- **声明**：`priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> que`
- **作用**：存储 `(距离, 节点编号)` 对，按距离从小到大排序（`greater<>` 使最大堆行为反转为最小堆）。
- **关键细节**：`pair` 的比较先比较 `.first`（距离），再比较 `.second`（节点编号），因此堆顶始终是当前距离最小的条目。将距离放在 `.first` 是正确排序的关键。

### 惰性删除（Lazy Deletion）

- **策略**：不修改堆中旧条目，弹出时用 `vis[tmp.second]` 判断是否已确定。
- **代码**：`if(vis[tmp.second]) continue;`
- **关键细节**：STL `priority_queue` 不支持 decrease-key，惰性删除是替代方案。堆中同一节点可能有多个条目（旧大值 + 新小值），但每个过期条目弹出时 $O(1)$ 跳过，不影响总复杂度。

### 松弛（Relaxation）

- **条件**：`if(A[i.first] > tmp.first + i.second)`
- **执行**：`A[i.first] = tmp.first + i.second; que.emplace(A[i.first], i.first);`
- **关键细节**：`tmp.first` 是已确定节点 $u$ 的最短距离（非临时值），`i.second` 是边权 $w$。松弛成功后将新距离压入堆。注意松弛前额外检查 `if(vis[i.first]) continue` 跳过已确定节点，减少无效入堆。

## 逐行代码解析

```cpp
#include<bits/stdc++.h>
using namespace std;
const int INF = INT_MAX;
int main()
{
    ios::sync_with_stdio(false);          // 关闭 C/C++ 流同步，加速 I/O
    cin.tie(nullptr);cout.tie(nullptr);   // 解绑 cin/cout，进一步加速

    // 小根堆：(距离, 节点)，greater<> 使距离小的在堆顶
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> que;

    int n,m,s;
    cin >> n >> m >> s;                   // n 个节点，m 条边，源点 s

    vector<int> A(n + 1,INF);             // 距离数组，初始化为 INF
    vector<vector<pair<int,int>>> edges(n + 1);  // 邻接表

    for(int i = 1;i <= m;i++)
    {
        int u,v,w;
        cin >> u >> v >> w;
        edges[u].emplace_back(v,w);       // 有向边 u→v，权重 w
    }

    vector<bool> vis(n + 1,false);        // 标记节点是否已确定最短路
    A[s] = 0;                              // 源点距离为 0
    que.emplace(0,s);                     // 将源点入堆

    while(!que.empty())
    {
        pair<int,int> tmp = que.top();    // 取堆顶（距离最小的未处理节点）
        que.pop();

        if(vis[tmp.second]) continue;     // 惰性删除：已确定则跳过
        vis[tmp.second] = true;           // 标记为已确定

        for(pair<int,int> i : edges[tmp.second])  // 遍历 u 的所有出边
        {
            if(vis[i.first]) continue;    // 跳过已确定的邻居

            if(A[i.first] > tmp.first + i.second)  // 松弛条件
            {
                A[i.first] = tmp.first + i.second;  // 更新距离
                que.emplace(A[i.first],i.first);    // 新距离入堆
            }
        }
    }

    for(int i = 1;i <= n;i++) cout << A[i] << ' ';  // 输出各点最短距离
    return 0;
}
```

**关键解析：**

- `priority_queue<..., greater<...>>`：STL 优先队列默认是大根堆，加 `greater<>` 后变为小根堆。堆中元素是 `pair<int,int>`，比较时先比 `.first`（距离），确保距离最小的节点在堆顶。
- `edges[u].emplace_back(v, w)`：构建有向图的邻接表。若为无向图需在此行后加 `edges[v].emplace_back(u, w)`。
- `que.emplace(0, s)`：源点入堆，距离为 $0$。`emplace` 比 `push` 省去一次移动/拷贝。
- `if(vis[tmp.second]) continue`：**惰性删除**的核心。堆中可能有同一节点的多个条目（例如先入 $(4, 2)$ 后又入 $(3, 2)$），弹出 $(4, 2)$ 时 `vis[2]` 已为 `true`，直接跳过。
- `A[i.first] > tmp.first + i.second`：松弛判断。`tmp.first` 是节点 $u$ 的最短距离（已确定），`i.second` 是边权 $w$。若 $A[v] > A[u] + w$ 则更新。
- `que.emplace(A[i.first], i.first)`：将更新后的距离和新节点入堆。不删除旧条目（STL 优先队列不支持），靠惰性删除处理。
- 输出 `A[i]`：若某节点不可达，输出 `INF`（即 `INT_MAX`）。

## 复杂度分析

- **时间复杂度**：建图 $O(m)$。主循环中每个节点最多被确定一次（$V$ 次），每次确定后遍历其所有出边，总松弛次数为 $O(E)$。每次松弛/入堆为 $O(\log V)$，因此总时间为 $O\big((V + E) \log V\big)$。惰性删除使堆中最多有 $E$ 个条目（每条边最多贡献一次入堆），堆操作上限为 $O(E \log E) = O(E \log V)$（因为 $E \le V^2$）。整体为 $O\big((V + E) \log V\big)$。

- **空间复杂度**：邻接表 $O(V + E)$，距离数组 $O(V)$，`vis` 数组 $O(V)$，堆最多 $O(E)$ 个条目。总空间复杂度为 $O(V + E)$。

与朴素 Dijkstra（$O(V^2)$）相比，堆优化在稀疏图（$E \ll V^2$）中显著更优；在稠密图（$E \approx V^2$）中两者接近，朴素版常数更小。与 SPFA（最坏 $O(VE)$）相比，堆优化 Dijkstra 复杂度稳定，不受负权边/构造数据影响（但要求非负权）。

## 适用场景

非负权有向/无向图中求单源最短路。具体包括：无权图可视为所有边权为 $1$ 的特例（但 BFS 更优）；网络延迟问题（节点为服务器，边权为延迟）；最短路径计数（在松弛时额外记录方案数）；分层图最短路（将状态扩展为多维）。常见于竞赛中的最短路模板题（如洛谷 P4779），也是 A* 算法、K 短路等高级算法的基础组件。

## 常见陷阱与注意事项

1. **负权边**：Dijkstra 的正确性依赖于"已确定节点的距离不会被后续更新"。若存在负权边，这一假设被打破：一个已确定的节点可能通过负权边获得更短路径。存在负权时应使用 Bellman-Ford（$O(VE)$）或 SPFA（均摊 $O(kE)$，最坏 $O(VE)$）。
2. **INF 溢出**：`INF = INT_MAX` 时，松弛 `tmp.first + i.second` 可能溢出（`INT_MAX + w` 越界为负数）。若边权较大，应将 `INF` 设为 `1e9` 或 `0x3f3f3f3f`（约 $10^9$，加上边权不溢出），或使用 `long long`。
3. **无向图建边**：代码构建的是有向图（仅 `edges[u].emplace_back(v,w)`）。若为无向图，需同时添加 `edges[v].emplace_back(u,w)`，否则结果错误。
4. **堆的元素顺序**：`pair` 中距离必须在 `.first`（`emplace(dist, node)`），这样 `greater<>` 才能按距离排序。若误写为 `emplace(node, dist)`，堆会按节点编号排序，完全错误。
5. **重复入堆导致的空间**：惰性删除策略下堆可能膨胀到 $O(E)$ 大小。在极端图（如完全图）中堆占用内存较大，但不会超时。
6. **不可达节点**：未被松弛到的节点 `A[v]` 保持 `INF`。输出时若题目要求特殊处理（如输出 $-1$），需额外判断 `A[i] == INF`。
7. **多组数据**：若有多组测试数据，需在每组开始前清空 `edges`、`vis` 和堆，或将堆和 `vis` 声明在循环内部（如本代码，每组新建）。

## 对比与扩展

与朴素 Dijkstra（$O(V^2)$）相比，堆优化将"取最小"从线性扫描降为堆操作，在稀疏图中效率提升数个数量级；但在稠密图中朴素版常数更小，且可用 `bitset` 优化进一步降低常数。与 SPFA 相比，Dijkstra 复杂度稳定为 $O((V+E)\log V)$，而 SPFA 在构造数据下退化为 $O(VE)$，但 SPFA 支持负权边（Dijkstra 不支持）。与 Floyd（$O(V^3)$）相比，Dijkstra 求单源最短路，Floyd 求全源最短路，适用规模不同。

扩展方向包括：① **多源 Dijkstra**（多源点同时入堆，求各点到最近源点的最短路）；② **分层图最短路**（将节点状态扩展为 $(位置, 层级)$，处理带状态约束的最短路）；③ **K 短路**（在 Dijkstra 基础上用 A* 算法求第 $K$ 短路）；④ **最短路计数**（松弛时若 $A[u] + w = A[v]$ 则 $cnt[v] += cnt[u]$）；⑤ **反图 Dijkstra**（建反图后从终点跑 Dijkstra，用于求各点到终点的最短路，常与正图配合解决最短路相关问题）。若需支持负权边且不引入 SPFA 的不稳定性，可考虑 Johnson 全源最短路算法（用 Bellman-Ford 消除负权后对每个点跑 Dijkstra）。
