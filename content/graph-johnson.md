---
id: graph-johnson
title: 'Johnson 全源最短路（负权图）'
category: 图论
subcategory: 最短路
subSubcategory: 全源最短路
tags: ["图论", "最短路", "Johnson", "全源最短路", "负权边", "势能函数", "重赋权", "Dijkstra"]
timeComplexity: 'O(nm log n)'
spaceComplexity: 'O(n^2 + n + m)'
codePath: '图论\最短路\Johnson\Untitled1.cpp'
---

## 算法原理

Johnson 算法解决**全源最短路**（All-Pairs Shortest Path, APSP）问题：给定一个含 $n$ 个节点、$m$ 条边的**有向带权图**（边权可为负），求所有节点对 $(i, j)$ 之间的最短距离。其核心困难在于：图中有负权边时，Dijkstra 不能直接使用（贪心策略失效），而对每条源点各跑一次 Bellman-Ford 的总复杂度为 $O(n \cdot nm) = O(n^2 m)$，太慢。Johnson 的精妙之处在于通过**重赋权**（reweighting）将所有边权变为非负，然后对每个源点跑 Dijkstra，总复杂度降为 $O(nm + n \cdot m \log n) = O(nm \log n)$。

### 三步走框架

**第 1 步：添加虚拟节点 + 求势能函数**。新建一个虚拟节点 $0$，从 $0$ 向每个真实节点 $v$ 连一条权为 $0$ 的边。然后用 Bellman-Ford（或 SPFA 变体）求出从 $0$ 到每个节点 $v$ 的最短距离 $h[v]$，这个 $h$ 就是**势能函数**（potential function）。若此过程中检测到负环，则原图存在从源可达的负环，全源最短路无定义，直接输出 $-1$。

**第 2 步：重赋权**。对每条原图边 $(u, v, w)$，定义新权：

$$w'(u, v) = w(u, v) + h[u] - h[v]$$

由最短路的三角不等式 $h[v] \leq h[u] + w(u, v)$（因为 $h[v]$ 是从 $0$ 到 $v$ 的最短距离，而 $0 \to \cdots \to u \to v$ 是一条候选路径），可得 $w'(u, v) \geq 0$。即**重赋权后所有边权非负**，且对任意路径 $i \to j$，重赋权后的路径长度恰好比原长度多了 $h[i] - h[j]$（一个与路径无关的常数偏移），因此**最短路径的拓扑结构不变**。

**第 3 步：逐源 Dijkstra + 逆变换**。对每个真实节点 $i$，在重赋权图上跑一次 Dijkstra，得到 $d'(i, j)$。再用逆变换恢复原始距离：

$$d(i, j) = d'(i, j) - h[i] + h[j]$$

### 为什么正确

关键在于重赋权**不改变最短路的路径选择**。对任意路径 $P = i \to v_1 \to v_2 \to \cdots \to j$，重赋权后的总长为：

$$\sum_{e \in P} w'(e) = \sum_{e \in P} \big(w(e) + h[u_e] - h[v_e]\big) = \sum_{e \in P} w(e) + h[i] - h[j]$$

中间节点的 $h$ 值在求和时全部抵消（路径上每条边的 $h[v_e]$ 与下一条边的 $h[u_{e+1}]$ 相消，因为 $v_e = u_{e+1}$），只剩起点 $h[i]$ 和终点 $h[j]$ 的差。因此重赋权后每条 $i \to j$ 路径的长度都增加了同一个常数 $h[i] - h[j]$，最短路的路径选择不受影响，只需在最后减去这个偏移即可还原。

## 状态定义与转移方程

**势能函数定义**（从虚拟节点 $0$ 出发的单源最短路）：

$$h[v] = \text{dist}(0, v), \quad h[0] = 0$$

**Bellman-Ford 松弛方程**（对每条边 $(u, v, w)$）：

$$h[v] = \min\big(h[v],\; h[u] + w\big)$$

**重赋权公式**（对原图边 $(u, v, w)$）：

$$w'(u, v) = w(u, v) + h[u] - h[v] \geq 0$$

**非负性证明**：由三角不等式 $h[v] \leq h[u] + w(u, v)$，移项得 $w(u, v) + h[u] - h[v] \geq 0$。

**Dijkstra 松弛方程**（重赋权图上，对源点 $i$）：

$$d'(i, v) = \min\big(d'(i, v),\; d'(i, u) + w'(u, v)\big)$$

**逆变换公式**（恢复原始距离）：

$$d(i, j) = d'(i, j) - h[i] + h[j]$$

**负环检测条件**：Bellman-Ford 执行 $n+1$ 轮（共 $n+1$ 个节点），若第 $n+1$ 轮仍有节点被松弛，则存在负环。

## 数据结构图示

```
输入示例：n=4, m=5（有向带负权图）

  原图边：
    1→2 (1)    1→3 (4)
    2→3 (-3)   3→4 (2)    4→2 (1)

  原图结构：
         1 --(1)--> 2
         |          ^ \
        (4)       (1)  (-3)
         |          |   \
         v          |    v
         3 --(2)--> 4<---

  添加虚拟节点 0：
    0→1 (0)  0→2 (0)  0→3 (0)  0→4 (0)

  === Step 1: Bellman-Ford 求 h[] ===

  初始: h = [0, INF, INF, INF, INF]   (h[0]=0)

  第 1 轮 (处理 0 的出边):
    0→1: h[1] = min(INF, 0+0) = 0
    0→2: h[2] = min(INF, 0+0) = 0
    0→3: h[3] = min(INF, 0+0) = 0
    0→4: h[4] = min(INF, 0+0) = 0
    → stack2 = [1, 2, 3, 4]

  第 2 轮 (处理 1,2,3,4 的出边):
    1→2: h[2] = min(0, 0+1) = 0     无变化
    1→3: h[3] = min(0, 0+4) = 0     无变化
    2→3: h[3] = min(0, 0+(-3)) = -3  更新!
    3→4: h[4] = min(0, 0+2) = 0     无变化
    4→2: h[2] = min(0, 0+1) = 0     无变化
    → stack2 = [3]

  第 3 轮 (处理 3 的出边):
    3→4: h[4] = min(0, -3+2) = -1   更新!
    → stack2 = [4]

  第 4 轮 (处理 4 的出边):
    4→2: h[2] = min(0, -1+1) = 0    无变化
    → stack2 为空 → 收敛! return false (无负环)

  最终: h = [0, 0, 0, -3, -1]

  === Step 2: 重赋权 ===

  w'(u,v) = w(u,v) + h[u] - h[v]

  原边          →  重赋权后
  1→2 (1)       →  1 + 0 - 0 = 1
  1→3 (4)       →  4 + 0 - (-3) = 7
  2→3 (-3)      →  -3 + 0 - (-3) = 0    ← 负权边变为 0!
  3→4 (2)       →  2 + (-3) - (-1) = 0
  4→2 (1)       →  1 + (-1) - 0 = 0

  所有重赋权后边权 >= 0 ✓ → 可以用 Dijkstra

  === Step 3: 逐源 Dijkstra ===

  以 i=1 为例 (重赋权图上):
    d'(1,1) = 0
    d'(1,2) = 1  (1→2)
    d'(1,3) = 1  (1→2→3, 1+0=1)
    d'(1,4) = 1  (1→2→3→4, 1+0+0=1)

  逆变换: d(1,j) = d'(1,j) - h[1] + h[j]
    d(1,1) = 0 - 0 + 0 = 0
    d(1,2) = 1 - 0 + 0 = 1
    d(1,3) = 1 - 0 + (-3) = -2  ← 正确! 原图 1→2→3 = 1+(-3) = -2
    d(1,4) = 1 - 0 + (-1) = 0   ← 正确! 原图 1→2→3→4 = 1+(-3)+2 = 0
```

## 核心操作详解

### 虚拟节点与势能函数 `init_h()`

- **虚拟节点 0**：`for (int i = 1; i <= n; i++) edges[0].emplace_back(i, 0);` 向每个真实节点连一条权 0 的边。虚拟节点的存在保证了从 $0$ 到所有真实节点都可达（至少有一条 0 权边），从而 $h[v]$ 对所有 $v$ 有定义。
- **批量松弛策略**：使用两个栈 `stack1`（当前轮处理）和 `stack2`（下一轮收集），与 Bellman-Ford 批量松弛变体（见 graph-spfa 知识点）完全相同。`vis[]` 防止同一节点在同一轮内被重复收集到 `stack2`。
- **外层 $n+1$ 轮**：`for (int i = 0; i <= n; ++i)`。由于图有 $n+1$ 个节点（含虚拟节点），最短路最多经 $n$ 条边，故 $n+1$ 轮足够：前 $n$ 轮完成最短路计算，第 $n+1$ 轮用于负环检测。若第 $n+1$ 轮后 `id1 > 0`（仍有待处理节点），说明存在负环。
- **提前终止**：`if (!id2) return false;` 若某轮无任何节点被更新，说明距离已收敛，无负环，提前返回。
- **返回值**：`return id1;` 转为 `bool`：`id1 > 0`（真）表示负环，`id1 == 0`（假）表示无负环。

### 重赋权 `get_w(u, v, w)`

- **公式**：`return w + h[u] - h[v];` 即 $w'(u,v) = w + h[u] - h[v]$。
- **调用时机**：在 `dij()` 的松弛过程中调用，将原图边权转换为非负权后用于 Dijkstra。
- **关键细节**：`h[u]` 和 `h[v]` 已由 `init_h()` 预处理完毕，此处只是简单的 $O(1)$ 加减法。

### 逐源 Dijkstra `dij(i)`

- **数据结构**：`priority_queue` 配 `greater` 实现小根堆，存 `(distance, node)` 对。
- **`vis[]` 数组**：标记节点是否已确定最短路（已出堆并处理过）。这是 Dijkstra 的标准"惰性删除"实现——同一节点可能因多次松弛而多次入堆，但只处理第一次出堆（`if (vis[u]) continue;`）。
- **松弛过程**：对节点 $u$ 的每条出边 $(u, v, w)$，用 `get_w()` 取得重赋权后的边权 $w'$，若 $d'(i, v) > d'(i, u) + w'$ 则更新。
- **初始化**：`ans[i][i] = 0;` 自身距离为 0，其余节点保持 `INF`。

### 逆变换输出

- **公式**：`ans[i][j] - h[i] + h[j]` 即 $d(i,j) = d'(i,j) - h[i] + h[j]$。
- **不可达判断**：`if (ans[i][j] >= INF) cout << "NO"` 说明在重赋权图中 $j$ 不可从 $i$ 到达（由于重赋权不改拓扑，原图也不可达）。

## 逐行代码解析

```cpp
#include<bits/stdc++.h>
using namespace std;
const long long INF = 1e17;              // 充分大的值，避免溢出
int n, m;
vector<vector<pair<int, long long>>> edges;  // 邻接表 edges[u]={(v,w),...}
vector<vector<long long>> ans;               // ans[i][j]=重赋权图上 i→j 最短路
vector<long long> h;                         // 势能函数 h[v]=dist(0,v)

inline long long get_w(int u,int v,long long w)
{
    return w + h[u] - h[v];     // 重赋权: w'(u,v) = w + h[u] - h[v] >= 0
}

bool init_h()                   // 求 h[] + 负环检测，返回 true 表示有负环
{
    h.resize(n + 1, INF);
    vector<int> stack1(n + 1), stack2(n + 1);  // 双栈: stack1当前轮, stack2下一轮
    vector<bool> vis(n + 1, false);            // 本轮去重标记
    int id1 = 0, id2 = 0;                     // 两栈的尾指针
    stack1[id1++] = 0;                        // 虚拟节点 0 入栈
    h[0] = 0;
    for (int i = 0; i <= n; ++i)              // n+1 轮 (n+1 个节点)
    {
        // === 阶段1: 处理 stack1 中所有节点 ===
        while (id1)
        {
            int u = stack1[--id1];            // 栈式弹出 (LIFO)
            for (int j = 0; j < edges[u].size(); ++j)
            {
                int v = edges[u][j].first;
                long long w = edges[u][j].second;
                if (h[v] > h[u] + w)          // 松弛条件
                {
                    h[v] = h[u] + w;          // 更新势能
                    if (!vis[v])             // 本轮未收集过
                    {
                        vis[v] = true;
                        stack2[id2++] = v;   // 收集到下一轮
                    }
                }
            }
        }
        if (!id2) return false;              // 收敛 → 无负环
        // === 阶段2: 将 stack2 转移到 stack1 ===
        while (id2)
        {
            vis[stack2[id2 - 1]] = false;    // 重置标记
            stack1[id1++] = stack2[--id2];   // 转入当前栈
        }
    }
    return id1;                              // n+1 轮后仍有节点 → 负环
}

void dij(int i)                              // 重赋权图上从 i 跑 Dijkstra
{
    priority_queue<pair<long long, int>,
        vector<pair<long long,int>>,
        greater<pair<long long,int>>> que;   // 小根堆
    vector<bool> vis(n + 1, false);          // 是否已确定最短路
    ans[i][i] = 0;
    que.emplace(0, i);                       // (距离=0, 节点=i)
    while (!que.empty())
    {
        long long wu = que.top().first;      // 当前最短距离
        int u = que.top().second;
        que.pop();
        if (vis[u]) continue;                // 惰性删除: 已处理过
        vis[u] = true;
        for (int j = 0; j < edges[u].size(); ++j)
        {
            int v = edges[u][j].first;
            if (vis[v]) continue;            // 已确定, 跳过
            long long w_uv = get_w(u,v,edges[u][j].second);  // 重赋权边权
            if (ans[i][v] > wu + w_uv)       // 松弛
            {
                ans[i][v] = wu + w_uv;
                que.emplace(ans[i][v], v);   // 入堆
            }
        }
    }
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    cin >> n >> m;
    edges.resize(n + 1, vector<pair<int,long long>>());
    // 虚拟节点 0 向所有真实节点连 0 权边
    for (int i = 1; i <= n; i++) edges[0].emplace_back(i, 0);
    // 读入真实边 (有向)
    for (int i = 1; i <= m; i++)
    {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        edges[u].emplace_back(v, w);
    }
    ans.resize(n + 1, vector<long long>(n + 1, INF));  // 初始化距离矩阵

    if (init_h()) cout << -1;                // 负环 → 无解
    else
    {
        for (int i = 1; i <= n; ++i)          // 逐源 Dijkstra
        {
            dij(i);
            for (int j = 1; j <= n; ++j)
            {
                if (ans[i][j] >= INF) cout << "NO" << ' ';  // 不可达
                else cout << ans[i][j] - h[i] + h[j] << ' '; // 逆变换
            }
            cout << '\n';
        }
    }
    return 0;
}
```

**关键解析：**

- `edges[0].emplace_back(i, 0)`：虚拟节点的建边是 Johnson 算法的核心前提。没有它，`h[v]` 可能保持 `INF`（不可达），重赋权公式失去意义。
- `for (int i = 0; i <= n; ++i)`：外层 $n+1$ 轮而非 $n-1$ 轮，因为总节点数为 $n+1$（含虚拟节点）。$n+1$ 轮 = $n$ 轮求最短路 + $1$ 轮负环检测。
- `stack1`/`stack2` 双栈设计：`stack1` 存当前轮待处理节点，`stack2` 存本轮被更新节点（即下一轮待处理）。`vis[]` 防止同一节点同轮内重复入 `stack2`，收集后立即重置。
- `return id1`：函数返回类型为 `bool`，`id1` 作为 `int` 隐式转换——非零为 `true`（有负环），零为 `false`（无负环）。
- `get_w(u,v,edges[u][j].second)`：在 Dijkstra 松弛时调用，将原图边权转为重赋权后的非负权。这一步是 $O(1)$ 的简单加减。
- `ans[i][j] - h[i] + h[j]`：逆变换公式。重赋权使路径长度增加 $h[i]-h[j]$，故原距离 = 重赋权距离 $- h[i] + h[j]$。
- `INF = 1e17`：用 `long long` 且 `INF` 取 $10^{17}$，避免 `INT_MAX` 加负权时溢出。

## 复杂度分析

- **时间复杂度**：`init_h()` 为批量松弛 Bellman-Ford，复杂度 $O(nm)$（$n+1$ 个节点，每轮最多处理 $m$ 条边）。`dij(i)` 为堆优化 Dijkstra，单次 $O(m \log n)$。共 $n$ 次 Dijkstra，总复杂度 $O(nm + n \cdot m \log n) = O(nm \log n)$。当图较稠密（$m \approx n^2$）时为 $O(n^3 \log n)$，略劣于 Floyd 的 $O(n^3)$。
- **空间复杂度**：邻接表 $O(n + m)$，距离矩阵 `ans` 为 $O(n^2)$，势能函数 `h` 为 $O(n)$，`init_h` 的辅助数组 $O(n)$。总空间 $O(n^2 + n + m)$。当 $n$ 较大时，$O(n^2)$ 的距离矩阵是主要开销。

| 全源最短路算法 | 时间复杂度 | 支持负权 | 适用规模 |
|---|---|---|---|
| Floyd | $O(n^3)$ | 支持（无负环） | $n \leq 500$ |
| $n \times$ Bellman-Ford | $O(n^2 m)$ | 支持 | 理论价值，不实用 |
| $n \times$ Dijkstra（堆） | $O(nm \log n)$ | 不支持 | 仅非负权图 |
| **Johnson** | $O(nm \log n)$ | 支持（无负环） | $n \leq 3000$（稀疏图优势大） |

## 适用场景

- **含负权边的全源最短路**：Johnson 是首选算法。当图有负权边且需要求所有点对最短路时，Johnson 是理论最优的高效选择。
- **稀疏图的全源最短路**：当 $m \ll n^2$ 时，Johnson 的 $O(nm \log n)$ 远优于 Floyd 的 $O(n^3)$。例如 $n = 1000, m = 5000$ 时，Johnson 约 $10^8$ 级别，Floyd 需 $10^9$。
- **需要检测负环的全源问题**：`init_h()` 兼做负环检测，输出 $-1$ 即可判断无解。
- **竞赛中**：常见于洛谷 P5905 等全源最短路模板题。Johnson 是带负权全源最短路的标答算法。

## 常见陷阱与注意事项

1. **虚拟节点必须向所有真实节点连边**：若虚拟节点 $0$ 不向某个节点 $v$ 连 $0$ 权边，则 $h[v]$ 可能为 `INF`，重赋权公式 $w + h[u] - h[v]$ 产生溢出或无意义值。代码中 `for (int i = 1; i <= n; i++) edges[0].emplace_back(i, 0);` 保证了这一前提。
2. **INF 取值**：`INF = 1e17` 配合 `long long` 是安全的。若用 `INT_MAX`，当 $h[u] = 0$、$w = -10^9$ 时 `h[u] + w` 可能为负大值，导致逻辑错误。此外，`ans[i][j]` 比较时 `>= INF` 而非 `== INF`，防止多次松弛后 `INF` 值被改变。
3. **负环判定**：`init_h()` 执行 $n+1$ 轮，若第 $n+1$ 轮后仍有待处理节点（`id1 > 0`）则存在负环。注意这里的 $n+1$ 轮对应 $n+1$ 个节点（含虚拟节点），不是 $n$ 个真实节点。若误写为 $n-1$ 轮会导致漏检负环。
4. **重赋权不改变路径选择**：重赋权使每条 $i \to j$ 路径长度增加 $h[i] - h[j]$（常数偏移），因此重赋权图上的最短路径与原图的最短路径完全相同。不要误以为重赋权改变了最短路径本身。
5. **逆变换方向**：输出时是 `ans[i][j] - h[i] + h[j]`（减起点 $h$、加终点 $h$），不要写反为 `+ h[i] - h[j]`。记忆方法：重赋权使距离偏大了 $h[i] - h[j]$，还原时要减去这个偏移。
6. **有向图 vs 无向图**：本代码处理有向图。若为无向图，每条边需建双向边，但要注意无向负权边等价于负环（可来回走无限降低），因此无向图中不应有负权边。
7. **`vis[]` 在 `init_h` 和 `dij` 中语义不同**：`init_h` 的 `vis` 是"本轮是否已入 `stack2`"（批量收集去重）；`dij` 的 `vis` 是"是否已确定最短路"（Dijkstra 标准标记）。两者完全独立，不要混淆。
8. **节点编号从 1 开始**：代码中真实节点编号 $1 \sim n$，虚拟节点为 $0$，数组大小 $n+1$。若题目编号从 $0$ 开始需调整偏移。

## 对比与扩展

### Johnson vs Floyd

| 特性 | Johnson | Floyd |
|---|---|---|
| 时间复杂度 | $O(nm \log n)$ | $O(n^3)$ |
| 稀疏图效率 | 优（$m$ 小时远快于 Floyd） | 与稠密图相同 |
| 稠密图效率 | 劣（$m \approx n^2$ 时 $O(n^3 \log n)$） | 优（常数小，代码极短） |
| 负环检测 | 前置 `init_h` 兼做 | $d[i][i] < 0$ 即负环 |
| 代码量 | 较长 | 极短（三重循环） |
| 支持负权 | 支持 | 支持 |

**选择原则**：$n \leq 500$ 或稠密图用 Floyd；$n > 500$ 且稀疏图用 Johnson。

### Johnson vs $n \times$ Dijkstra

当图中无负权边时，不需要重赋权，直接对每个源点跑 Dijkstra 即可，复杂度相同 $O(nm \log n)$ 但省去了 `init_h()` 的 $O(nm)$ 开销。因此非负权图不应使用 Johnson，直接用 $n \times$ Dijkstra 更简洁。Johnson 的价值正是处理负权边时的重赋权技巧。

### Johnson vs $n \times$ Bellman-Ford

$n \times$ Bellman-Ford 的复杂度为 $O(n^2 m)$，远慢于 Johnson。Johnson 通过一次 Bellman-Ford + $n$ 次 Dijkstra 实现了从 $O(n^2 m)$ 到 $O(nm \log n)$ 的加速。

### 扩展方向

1. **不输出路径**：本代码只输出距离矩阵。若需还原路径，在 `dij()` 中维护 `prev[i][v]` 前驱数组即可。注意路径还原在重赋权图上进行，但前驱关系与原图一致（重赋权不改路径选择）。
2. **稀疏图优化**：Johnson 的瓶颈在 $n$ 次 Dijkstra。可用配对堆（Pairing Heap）或 Fibonacci 堆将 Dijkstra 降至 $O(m + n \log n)$，总复杂度 $O(nm + n^2 \log n)$。但实践中 `priority_queue` 的常数已经很好。
3. **与差分约束的关系**：势能函数 $h$ 本质上满足 $h[v] - h[u] \leq w(u,v)$ 对所有边成立，这正是差分约束系统的形式。Johnson 的 `init_h` 等价于求解一个差分约束系统。
4. **多源 Johnson**：若只需求从若干源点到所有点的最短路，可用多源 Bellman-Ford 一次求出势能，然后对每个源点跑一次 Dijkstra，无需跑满 $n$ 次。
