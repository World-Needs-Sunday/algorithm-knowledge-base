---
id: graph-spfa
title: 'Bellman-Ford 批量松弛优化（类SPFA变体）'
category: 图论
subcategory: 最短路
tags: ["图论", "最短路", "Bellman-Ford", "批量松弛", "队列优化", "负权边", "类SPFA"]
timeComplexity: '均摊O(kE)，最坏O(VE)'
spaceComplexity: 'O(V+E)'
codePath: '图论\最短路\Bellman-Ford 批量松弛优化（类SPFA变体）\Untitled1.cpp'
---

## 算法原理

Bellman-Ford 每轮盲目遍历所有 $m$ 条边做松弛，但只有"上一轮被更新的节点"的出边才可能产生新的松弛。本代码借鉴了 SPFA 的核心思想——**只松弛上轮被更新节点的出边**，但实现方式与标准 SPFA 有本质区别，是一种**批量松弛优化的 Bellman-Ford 变体**。

标准 SPFA 的策略是"即时入队"：松弛成功后立刻将目标节点入队（若不在队列中），处理是流式的，节点一个接一个出队-松弛-入队，没有固定的轮次概念。而本代码采用**批量处理**（batch processing）策略：

- **阶段 1（处理）**：将当前队列中的所有节点处理完毕，松弛它们的出边，用 `vis[]` 标记被更新的节点。同轮内的松弛更新不会立即传播给其他节点。
- **阶段 2（收集）**：处理完后扫描所有节点，将 `vis[]` 为 `true` 的节点统一收集入队，进入下一轮。

这意味着本代码仍保留了 Bellman-Ford 的**轮次结构**（外层 $n-1$ 轮），每轮只处理"上轮被更新的节点"而非全量遍历。与标准 SPFA 相比：

| 特性 | 标准 SPFA | 本代码（批量变体） |
|---|---|---|
| 外层结构 | 无固定轮数，队列空即结束 | 固定 $n-1$ 轮外层循环 |
| 入队时机 | 松弛成功后即时入队 | 批量收集后下一轮入队 |
| 数据结构 | FIFO 队列 (`std::queue`) | 栈式访问 (`que[idx-1]` + `--idx`) |
| `vis` 语义 | "是否在队列中"（防重复入队） | "本轮是否被更新"（收集用） |
| 同轮内更新 | 即时传播给后续节点 | 不传播，下一轮才处理 |

批量策略的特点是：同一轮中一个节点可能被多次松弛更新，但只会在下一轮入队处理一次，避免了同轮内反复入队的冗余。代价是可能比标准 SPFA 多收敛几轮（因为同轮内的更新不会即时传播），但减少了重复入队次数。

算法的外层循环 $n-1$ 次保证正确性：与 Bellman-Ford 相同，最短路最多经过 $n-1$ 条边，每轮使最短路的边数上限 $+1$。提前终止优化：若某轮后队列为空（`!idx`），说明所有距离已收敛，提前退出。

## 状态定义与转移方程

距离数组定义：

$$A[v] = \text{从源点 } s \text{ 到节点 } v \text{ 的当前最短距离估计值}$$

初始化：

$$A[s] = 0, \quad A[v] = +\infty \quad (v \neq s)$$

松弛转移方程（对队列中节点 $u$ 的每条出边 $(u, v, w)$）：

$$A[v] = \min\big(A[v],\; A[u] + w\big)$$

批量收集条件：

$$\text{若 } A[v] \text{ 被更新} \;\Rightarrow\; vis[v] = \text{true}$$

下一轮入队：

$$\text{批量扫描 } vis[] \;\Rightarrow\; \text{将所有 } vis[v] = \text{true} \text{ 的 } v \text{ 入队，并重置 } vis[v] = \text{false}$$

收敛条件：

$$\text{若某轮后队列为空} \;\Rightarrow\; \forall v,\; A[v] \text{ 已收敛} \;\Rightarrow\; \text{break}$$

## 数据结构图示

```
输入示例：n=4, m=5, s=1（有向图）

  边: 1→2(2)  1→3(5)  2→3(1)  3→4(1)  2→4(4)

  邻接表：
  edges[1] = {(2,2), (3,5)}
  edges[2] = {(3,1), (4,4)}
  edges[3] = {(4,1)}
  edges[4] = {}

  执行过程（que 用 idx 指针模拟栈式处理）：

  初始:  A = [INF, 0, INF, INF, INF]
         que = [1], idx = 1

  === 第 1 轮 (t=1) ===
  while(idx): 处理 que[idx-1]=1
    1→2: A[2] = min(INF, 0+2) = 2    vis[2] = true
    1→3: A[3] = min(INF, 0+5) = 5    vis[3] = true
    --idx → idx=0, while 结束

  批量收集: vis[2]=true → que[0]=2, vis[2]=false
            vis[3]=true → que[1]=3, vis[3]=false
            idx = 2

  === 第 2 轮 (t=2) ===
  while(idx): 先处理 que[1]=3
    3→4: A[4] = min(INF, 5+1) = 6    vis[4] = true
    --idx → idx=1
  处理 que[0]=2
    2→3: A[3] = min(5, 2+1) = 3      vis[3] = true  ← 更新！
    2→4: A[4] = min(6, 2+4) = 6      无变化
    --idx → idx=0

  批量收集: vis[3]=true → que[0]=3, vis[3]=false
            vis[4]=true → que[1]=4, vis[4]=false
            idx = 2

  === 第 3 轮 (t=3) ===
  while(idx): 先处理 que[1]=4
    4 无出边
    --idx → idx=1
  处理 que[0]=3
    3→4: A[4] = min(6, 3+1) = 4      vis[4] = true  ← 更新！
    --idx → idx=0

  批量收集: vis[4]=true → que[0]=4, vis[4]=false
            idx = 1

  === 第 4 轮 (t=4, 但 t < n=4 不执行) ===
  实际: t 循环条件 t < n 即 t=1,2,3 共 3 轮
  第 3 轮后 idx=1，但循环已结束

  最终:  A = [INF, 0, 2, 3, 4]
         输出: 0 2 3 4
```

## 核心操作详解

### 建图：邻接表

- **数据结构**：`vector<vector<pair<int,int>>> edges(n+1)`，`edges[u]` 存储 $u$ 的所有出边 `(v, w)`。
- **关键细节**：与 Bellman-Ford 和 Dijkstra 相同的邻接表结构，支持有向图。若为无向图需双向建边。

### 队列：手写数组 + idx 指针

- **数据结构**：`vector<int> que(n+1)` 作为节点队列，`int idx` 作为尾指针。
- **入队**：`que[idx++] = v`；**出队/读取**：`que[idx-1]`（栈式 LIFO 访问）。
- **关键细节**：本代码用 `idx-1` 访问队尾并 `--idx` 弹出，实际是**栈式处理**（LIFO）而非 FIFO 队列。同一批节点按入队逆序处理。标准 SPFA 用 `std::queue` 的 FIFO 顺序。两种顺序不影响正确性，但影响收敛速度。

### 批量处理策略（即时收集优化版）

- **处理阶段**：`while(idx)` 循环弹出并处理当前队列中的所有节点，松弛它们的出边。松弛成功时，若目标节点本轮尚未被标记（`!vis[v]`），则标记 `vis[v] = true` 并立即写入临时数组 `tmp[id++]`。
- **收集阶段**：处理完后检查 `if(!id) break`，若 `id > 0` 则用 `while(id)` 将 `tmp` 中的节点转移到 `que`，同时重置 `vis` 为 `false`。
- **关键细节**：与旧版（扫描 $1 \sim n$ 所有节点收集 `vis[]` 为 `true` 的）相比，优化版在松弛时即时收集到 `tmp` 数组，避免了每轮 $O(n)$ 的全量扫描。`vis` 起去重作用：同一节点同轮内被多次松弛只入 `tmp` 一次，后续松弛跳过（`!vis[i.first]` 为 `false`）。收集阶段复杂度从 $O(n)$ 降为 $O(k)$（$k$ 为本轮更新节点数）。

### 提前终止优化

- **条件**：`if(!id) break`，即处理完当前队列后检查 `tmp` 是否为空。
- **效果**：若本轮松弛中没有任何节点被更新（`id == 0`），说明距离已收敛，提前退出外层循环。检查时机在收集阶段之前，避免无意义的 `while(id)` 转移。

### vis 数组与 tmp 数组的配合

- **处理阶段**：松弛成功时 `if(!vis[v])` 检查，若未被标记则 `vis[v] = true; tmp[id++] = v`，即时收集。
- **收集阶段**：`while(id)` 逐个从 `tmp` 取出节点，重置 `vis[tmp[id-1]] = false` 并放入 `que`。
- **关键细节**：`vis` 的语义是"本轮是否已加入 `tmp`"（去重用），而非旧版的"本轮是否被更新"。区别在于：旧版每次松弛都设 `vis = true`（即使已为 `true`），收集阶段扫描全部 $n$ 个节点；新版用 `!vis[v]` 守卫，只在首次松弛时加入 `tmp`，收集阶段只遍历 `tmp` 中的 $k$ 个节点。与标准 SPFA 中 `vis`/`inQueue` 表示"是否在队列中"也不同。

## 逐行代码解析

```cpp
#include<bits/stdc++.h>
using namespace std;
const long long INF = INT_MAX;
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);cout.tie(nullptr);
    int n,m,s;
    cin >> n >> m >> s;                   // n 个节点，m 条边，源点 s

    vector<vector<pair<int,int>>> edges(n + 1);
    for(int i = 1;i <= m;i++)
    {
        int u,v,w;
        cin >> u >> v >> w;
        edges[u].emplace_back(v,w);       // 有向边 u→v
    }

    vector<long long> A(n + 1,INF);        // 距离数组
    vector<int> que(n + 1,0),tmp(n + 1,0); // que: 主队列, tmp: 本轮收集
    vector<bool> vis(n + 1,false);        // 去重标记（本轮是否已入 tmp）
    int idx = 0,id = 0;                   // idx: que 尾指针, id: tmp 尾指针

    A[s] = 0;
    que[idx++] = s;                        // 源点入队

    for(int t = 1;t < n;++t)              // 最多 n-1 轮
    {
        // === 阶段1：处理当前队列中的所有节点 ===
        while(idx)
        {
            for(pair<int,int> i : edges[que[idx - 1]])  // 遍历栈顶节点的出边
            {
                if(A[i.first] > A[que[idx - 1]] + i.second)  // 松弛条件
                {
                    A[i.first] = A[que[idx - 1]] + i.second;  // 更新距离
                    if(!vis[i.first])        // 本轮尚未收集该节点
                    {
                        vis[i.first] = true;
                        tmp[id++] = i.first;  // 即时收集到 tmp
                    }
                }
            }
            --idx;                        // 弹出（栈式）
        }

        // === 提前终止 ===
        if(!id) break;                     // 本轮无任何更新 → 已收敛

        // === 阶段2：将 tmp 中的节点转移到 que ===
        while(id)
        {
            vis[tmp[id - 1]] = false;      // 重置标记
            que[idx++] = tmp[id - 1];     // 转入主队列
            --id;
        }
    }

    for(int i = 1;i <= n;i++) cout << A[i] << ' ';
    return 0;
}
```

**关键解析：**

- `vector<int> que(n+1,0), tmp(n+1,0)`：双数组设计。`que` 是主队列（存当前轮待处理节点），`tmp` 是收集数组（存本轮被更新的节点）。`idx` 和 `id` 分别是两个数组的尾指针。
- `if(!vis[i.first]) { vis[i.first] = true; tmp[id++] = i.first; }`：**核心优化点**。松弛成功后，若该节点本轮未被收集过（`!vis`），才标记并加入 `tmp`。这避免了同轮内同一节点多次入 `tmp` 的冗余，也使收集阶段无需扫描全部 $n$ 个节点。
- `if(!id) break`：提前终止检查移到收集阶段之前。若 `id == 0` 说明本轮无节点被更新，直接退出，跳过无意义的 `while(id)` 转移。
- `while(id) { vis[tmp[id-1]] = false; que[idx++] = tmp[id-1]; --id; }`：将 `tmp` 中的节点转移到 `que`，同时重置 `vis`。只遍历 $k$ 个本轮更新的节点（而非旧版的 $n$ 个全扫描），复杂度 $O(k)$。
- `que[idx-1]` + `--idx`：栈式（LIFO）弹出，同一批节点按入队逆序处理。标准 SPFA 用 FIFO 队列。
- `INF = INT_MAX` + `long long A`：用 `long long` 存距离，但 `INF` 取 `INT_MAX`。当 `A[i] = INF` 且边权为负时，`A[i] + w` 可能溢出。更安全的做法见"常见陷阱"。

## 复杂度分析

- **时间复杂度**：外层最多 $n-1$ 轮，每轮处理队列中的节点并松弛其出边。处理阶段每轮总松弛次数 $\leq m$（所有边最多被遍历一次），收集阶段遍历 `tmp` 中 $k$ 个节点为 $O(k)$（优化后不再全量扫描 $n$ 个节点）。最坏复杂度为 $O(VE)$（与 Bellman-Ford 相同）。均摊情况下，每个节点入队 $k$ 次（$k$ 通常为 $2\sim3$），总时间为 $O(kE)$。但构造数据可使 $k$ 接近 $V$，退化到 $O(VE)$。
- **空间复杂度**：邻接表 $O(V+E)$，距离数组 $O(V)$，`que` 和 `tmp` 各 $O(V)$，`vis` 数组 $O(V)$。总空间复杂度为 $O(V+E)$。

与 Bellman-Ford（$O(VE)$ 确定性，每轮全量遍历所有边）相比，本代码通过选择性松弛（只处理上轮更新节点的出边）在均摊下更快，但最坏复杂度相同。与标准 SPFA（均摊 $O(kE)$，即时入队 + FIFO）相比，批量策略的均摊常数可能略大（多收敛几轮），但减少了重复入队次数。与堆优化 Dijkstra（$O((V+E)\log V)$）相比，本代码支持负权边但复杂度不稳定。

## 适用场景

含负权边的单源最短路问题、图规模适中且不易被构造数据针对的场景、需要求解差分约束系统。在竞赛中，当题目明确存在负权边时，标准 SPFA 或 Bellman-Ford 是常见选择；若图无非负权边，应优先使用堆优化 Dijkstra 以保证稳定的复杂度。常见于洛谷 P3385 等负权最短路/负环检测题。

注意：本代码为**类 SPFA 的批量松弛变体**，不是标准 SPFA 实现。若需在竞赛中使用标准 SPFA，建议参考下方"对比与扩展"中的标准实现。

## 常见陷阱与注意事项

1. **最坏退化**：本代码和标准 SPFA 一样可被构造数据卡到 $O(VE)$，性能远不如堆优化 Dijkstra 稳定。常见卡法包括：网格图 + 特定边权分布、链式图 + 负权边等。在不确定数据性质时，若无非负权边需求，优先用 Dijkstra。
2. **INF 溢出**：`INF = INT_MAX` 时，当 `A[u] = INF` 且边权 $w$ 为负数，`A[u] + w` 溢出为正大值，可能导致错误的松弛（把不可达节点松弛为 `INT_MAX - 1` 等）。解决方案：① `INF = 1e18`（`long long` 安全范围）；② 松弛前加 `if(A[u] == INF) continue` 跳过未到达节点。
3. **负环检测**：本代码外层循环 $n-1$ 次，不检测负环。若存在从源点可达的负环，输出无意义。扩展方法：记录每个节点的入队次数，若某节点入队 $\geq n$ 次，则存在负环。
4. **栈式 vs 队列式处理**：本代码用 `que[idx-1]` + `--idx` 是栈式（LIFO）弹出，标准 SPFA 用 `std::queue` 的 FIFO。两者都正确，但 LIFO 可能改变收敛轮数。若追求标准 SPFA 行为，应改用 FIFO 队列。
5. **批量 vs 即时入队**：本代码的批量策略使同一轮内更新不即时传播，可能多收敛 $1\sim2$ 轮。标准 SPFA 即时入队，收敛通常更快但同轮内可能反复入队。两种策略的最坏复杂度相同。优化版用 `tmp` 数组即时收集避免了旧版每轮 $O(n)$ 的全量扫描，使收集阶段降至 $O(k)$。
6. **vis 语义**：本代码 `vis` 表示"本轮是否已加入 `tmp`"（去重用，防止同轮多次入 `tmp`），而标准 SPFA 的 `vis`/`inQueue` 表示"是否在队列中"（防止重复入队）。混用语义会导致逻辑错误。注意优化版的 `vis` 在 `tmp` 转移到 `que` 时立即重置为 `false`。
7. **节点编号从 1 开始**：代码中节点 $1 \sim n$，数组大小 $n+1$。若题目编号从 $0$ 开始需调整。

## 对比与扩展

与 Bellman-Ford 相比，本代码用选择性松弛（只处理上轮更新节点的出边）避免了全量边遍历，均摊更快但最坏复杂度相同。与标准 SPFA 相比，本代码的批量策略和栈式处理使均摊常数略大，但减少了重复入队。与堆优化 Dijkstra 相比，本代码支持负权边但复杂度不稳定（Dijkstra 稳定 $O((V+E)\log V)$ 但不支持负权）。与 Floyd（$O(V^3)$ 全源最短路）相比，规模互补。

本代码与标准 SPFA 的关键区别已在前文详述（批量处理 + 栈式弹出 + `vis` 语义不同 + 固定 $n-1$ 轮外层循环）。标准 SPFA 的典型实现如下供参考：

```cpp
// 标准 SPFA（FIFO 队列 + 即时入队）
void spfa(int s, int n, vector<vector<pair<int,int>>>& edges, vector<long long>& A) {
    const long long INF = 1e18;
    A.assign(n + 1, INF);
    vector<bool> inQueue(n + 1, false);
    queue<int> q;
    A[s] = 0;
    q.push(s);
    inQueue[s] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();      // FIFO 出队
        inQueue[u] = false;
        for (auto& [v, w] : edges[u]) {
            if (A[v] > A[u] + w) {       // 松弛
                A[v] = A[u] + w;
                if (!inQueue[v]) {        // 不在队列中才入队
                    q.push(v);
                    inQueue[v] = true;
                }
            }
        }
    }
}
```

标准 SPFA 没有 $n-1$ 轮的外层循环，仅靠队列驱动；松弛成功后即时入队（若不在队列中）；`inQueue` 表示"是否在队列中"而非"本轮是否被更新"。标准 SPFA 在多数场景下收敛更快。

扩展方向包括：① **SLF 优化**（Small Label First）：用双端队列，入队时若新距离 $\leq$ 队首距离则从队首入队，否则从队尾入队，加速收敛；② **LLL 优化**（Large Label Last）：出队时若队首距离 $>$ 队列平均距离，将队首移到队尾，减少大距离节点的无效松弛；③ **负环检测**：记录每个节点的入队次数，超过 $n$ 次则存在负环；④ **差分约束系统**：将不等式约束转化为图的最短路问题，用 SPFA 求解并检测无解（负环）。⑤ **多源最短路**：多源点同时入队，求各点到最近源点的最短距离。
