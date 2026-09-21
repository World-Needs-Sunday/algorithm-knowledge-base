---
id: graph-scc-dag-dp
title: 'SCC缩点 + DAG上拓扑DP（最大权值路径）'
category: 图论
subcategory: 连通分量
subSubcategory: 强连通分量
tags: ["图论", "强连通分量", "缩点", "Tarjan", "拓扑排序", "DAG DP", "最长路"]
timeComplexity: 'O(n+m)'
spaceComplexity: 'O(n+m)'
codePath: '图论\连通分量\SCC缩点DAG DP\源.cpp'
---

## 算法原理

### 核心问题

给定一个有向图，每个节点有权值 $W_i$。求一条路径，使得路径上经过的所有节点权值之和最大。**路径可以经过环上的所有节点**（因为一旦进入一个环，就可以走完整个环再出来）。

### 为什么不能直接 DP

在一般有向图中，环的存在导致无法定义拓扑序，直接 DP 会产生状态依赖环（无限递归）。但如果把每个强连通分量（SCC）缩成一个"超级点"，环就消失了——因为同一个 SCC 内的所有节点互相可达，一旦进入就可以走遍整个 SCC。

### 解决方案：SCC缩点 + DAG上DP

分三步：

1. **Tarjan 求 SCC**：找出所有强连通分量，每个 SCC 的权值 = 内部所有节点权值之和
2. **构建缩点图**：跨 SCC 的边变成超级点之间的边，缩点图是 DAG
3. **拓扑排序 + DP**：在 DAG 上按拓扑序做最大权值路径 DP

### 状态定义

$$A[u] = \text{以超级点 } u \text{ 为终点的最大权值路径和}$$

### 状态转移方程

对于超级点 $u$，其所有前驱 $v$（即缩点图中 $v \to u$ 的 $v$）：

$$A[u] = \max_{v \to u} \left( A[v] + SCC\_w[u] \right)$$

入度为 0 的超级点（没有前驱）初始化为：

$$A[u] = SCC\_w[u]$$

最终答案：

$$\text{ans} = \max_{u=1}^{cnt} A[u]$$

## 逐行代码解析

### 全局变量

```cpp
int tim, col_cnt, sk_top;        // DFS时间戳、SCC数量、栈顶指针
vector<bool> in_sk;               // 是否在栈中
vector<int> dfn, low, sk, col;    // 时间戳、low值、栈、SCC编号
vector<int> in_rt, W;            // 入度、节点权值
vector<int> SCC_w;               // 每个SCC的权值之和
vector<vector<int>> edges;        // 原图邻接表
vector<vector<int>> col_arr;     // 每个SCC包含的节点列表
vector<vector<int>> SCC_edges;   // 缩点图邻接表
```

| 变量 | 含义 |
|------|------|
| `tim` | DFS 时间戳计数器，从 1 开始递增 |
| `col_cnt` | SCC 总数 |
| `sk` / `sk_top` | Tarjan 算法中的栈及栈顶指针 |
| `col[u]` | 节点 u 所属的 SCC 编号 |
| `SCC_w[i]` | 第 i 个 SCC 的权值（内部所有节点权值之和） |
| `in_rt[u]` | 超级点 u 在缩点图中的入度 |
| `col_arr[i]` | 第 i 个 SCC 包含的所有原图节点 |

### init 函数：初始化

```cpp
void init(int n)
{
    W.assign(n + 1, 0);
    SCC_w.assign(1, 0);          // SCC_w[0] 占位，从1开始
    dfn.assign(n + 1, 0);
    low.assign(n + 1, 0);
    edges.assign(n + 1, vector<int>());
    sk.assign(n + 5, 0);
    in_sk.assign(n + 1, false);
    col_arr.emplace_back();       // col_arr[0] 占位
    col.assign(n + 1, 0);
    tim = 1;
    col_cnt = 0;
    sk_top = 0;
}
```

**关键点**：
- `SCC_w` 和 `col_arr` 从下标 1 开始存（下标 0 占位），后续每发现一个 SCC 就 `emplace_back` 追加
- `sk` 开 $n+5$ 防越界（最坏情况所有节点入栈）
- `tim` 从 1 开始，这样 `dfn[u] == 0` 可以表示"未访问"

### mark 函数：标记节点入栈

```cpp
void mark(int rt)
{
    dfn[rt] = low[rt] = tim++;    // 分配时间戳
    sk[sk_top++] = rt;            // 压栈
    in_sk[rt] = true;             // 标记在栈中
}
```

与标准 Tarjan 一致：新访问的节点初始化 `dfn = low`，压入栈中。

### tarjan 函数：求SCC + 计算SCC权值

```cpp
void tarjan(int rt)
{
    mark(rt);
    for (int i : edges[rt])
    {
        if (!dfn[i])                      // 未访问：继续DFS
        {
            tarjan(i);
            low[rt] = min(low[rt], low[i]);
        }
        else if (in_sk[i])                // 在栈中：回边
            low[rt] = min(low[rt], dfn[i]);
    }
    if (dfn[rt] == low[rt])               // rt 是 SCC 的根
    {
        col_arr.emplace_back();           // 新建 SCC 节点列表
        SCC_w.emplace_back();             // 新建 SCC 权值
        ++col_cnt;
        while (in_sk[rt])                 // 弹栈直到 rt 自己弹出
        {
            int tmp = sk[sk_top - 1];
            col_arr[col_cnt].emplace_back(tmp);
            col[tmp] = col_cnt;           // 标记 SCC 编号
            in_sk[tmp] = false;
            SCC_w[col_cnt] += W[tmp];     // 累加节点权值
            sk_top--;
        }
    }
}
```

**与标准 Tarjan 的区别**：在弹栈时多了一行 `SCC_w[col_cnt] += W[tmp]`，将同一 SCC 内所有节点的权值求和，作为超级点的权值。

| 步骤 | 代码 | 含义 |
|------|------|------|
| 分配时间戳 | `dfn[rt] = low[rt] = tim++` | 标记访问顺序 |
| 树边递归 | `tarjan(i)` 后 `low[rt] = min(low[rt], low[i])` | 用子节点 low 更新自己 |
| 回边更新 | `in_sk[i]` 为真时 `low[rt] = min(low[rt], dfn[i])` | 用栈中祖先的 dfn 更新 |
| 判定 SCC 根 | `dfn[rt] == low[rt]` | rt 无法到达更早的节点 |
| 弹栈建分量 | `while (in_sk[rt])` | 弹出 rt 及其上方所有节点 |

### SCC 函数：构建缩点图

```cpp
void SCC()
{
    in_rt.assign(col_cnt + 1, 0);
    SCC_edges.assign(col_cnt + 1, vector<int>());
    vector<int> vis(col_cnt + 1, 0);      // 去重标记
    for (int u = 1; u <= col_cnt; u++)     // 枚举每个 SCC
    {
        for (int i : col_arr[u])           // 枚举 SCC 中的原图节点
        {
            for (int j : edges[i])         // 枚举该节点的出边
            {
                int v = col[j];            // 终点所属的 SCC
                if (vis[v] == u || u == v) continue;  // 去重 + 跳过自环
                vis[v] = u;
                SCC_edges[u].emplace_back(v);
                in_rt[v]++;               // 终点 SCC 入度+1
            }
        }
    }
}
```

**去重技巧**：`vis[v] = u` 表示"当前处理 SCC u 时已向 v 连过边"。因为 u 递增，无需清空 vis 数组。

### fn 函数：拓扑排序 + DP

```cpp
int fn(int n)
{
    // 第一步：对每个未访问节点跑 Tarjan
    int head = 0, tail = 0;
    for (int i = 1; i <= n; i++)
    {
        if (!dfn[i]) tarjan(i);
    }
    vector<int> deq(col_cnt + 2, 0);   // 拓扑排序队列（手写双指针队列）
    vector<int> A(col_cnt + 2, 0);     // DP 数组
    SCC();                             // 构建缩点图

    // 第二步：入度为0的超级点入队，初始化 DP
    int ans = 0;
    for (int i = 1; i <= col_cnt; i++)
    {
        if (!in_rt[i])                 // 入度为0：DAG的起点
        {
            deq[tail++] = i;
            A[i] = SCC_w[i];           // 初始化：起点路径权值 = 自身权值
            ans = max(ans, SCC_w[i]);
        }
    }

    // 第三步：拓扑序 DP
    while (head < tail)
    {
        int i = deq[head++];           // 取出队首
        for (int j : SCC_edges[i])    // 枚举缩点图出边 i→j
        {
            in_rt[j]--;                // 终点入度-1
            A[j] = max(A[i] + SCC_w[j], A[j]);  // 转移：前驱+自身 vs 当前最大
            if (!in_rt[j])             // 入度归零：所有前驱已处理
            {
                deq[tail++] = j;
                ans = max(ans, A[j]);  // 更新全局答案
            }
        }
    }
    return ans;
}
```

| 阶段 | 代码 | 作用 |
|------|------|------|
| 初始化 | `A[i] = SCC_w[i]` | 入度为 0 的超级点，路径权值 = 自身权值 |
| 转移 | `A[j] = max(A[i] + SCC_w[j], A[j])` | 从前驱 i 走到 j，路径和 = A[i] + SCC_w[j] |
| 答案 | `ans = max(ans, A[j])` | 每个超级点入队时更新全局最大值 |

> **注意**：DP 转移在拓扑序上进行，保证处理 j 时其所有前驱 i 的 A[i] 已经是最终值。

### 手写队列 vs std::queue

代码用 `deq` 数组 + `head/tail` 指针模拟队列，而不是 `queue<int>`：

| 方式 | 优点 | 缺点 |
|------|------|------|
| 手写数组队列 | 无动态分配开销，常数小 | 需手动管理大小 |
| `std::queue` | 安全，无需关心容量 | 内部用 deque，常数略大 |

在竞赛中两者均可，手写队列是追求极限常数时的选择。

## 复杂度分析

- **时间复杂度**：$O(n + m)$
  - Tarjan 求 SCC：每个节点和边各访问一次，$O(n + m)$
  - 构建缩点图：遍历所有节点和边，$O(n + m)$
  - 拓扑排序 + DP：每个超级点和缩点图边各处理一次，$O(cnt + m') \leq O(n + m)$
- **空间复杂度**：$O(n + m)$
  - 原图邻接表 $O(n + m)$，缩点图邻接表 $O(cnt + m') \leq O(n + m)$
  - 各种辅助数组合计 $O(n)$

## 适用场景

1. **最大权值路径**：有向图上节点带权，求权值和最大的路径（可经过环）
2. **最长路问题**：有向图上的最长路（边权为正时，先缩点消除环，再 DAG 上 DP）
3. **连通性 + 优化结合**：需要同时利用连通性（缩点）和最优性（DP）的问题
4. **游戏/博弈问题**：有向图上的博弈，环代表循环状态，缩点后在 DAG 上做博弈 DP

## 常见陷阱与注意事项

1. **tarjan 调用位置**：代码在 `fn()` 中调用 tarjan，而不是在 `init` 后立即调用。确保 `SCC()` 在 tarjan 之后调用（依赖 `col` 和 `col_arr`）
2. **`in_rt` 数组**：`in_rt` 不在 `init` 中初始化，而是在 `SCC()` 中才 `assign` 为缩点图入度。不要在 tarjan 阶段使用 `in_rt`
3. **手写队列大小**：`deq` 开 `col_cnt + 2`，确保不会越界
4. **未连通图**：代码在 `fn` 中对所有未访问节点调用 tarjan，正确处理了森林（多个不连通分量）
5. **自环处理**：`SCC()` 中 `u == v` 跳过自环，确保缩点图是严格 DAG
6. **负权值**：代码用 `ans` 初始化为 0，如果所有权值都为负，答案会错误地返回 0。应初始化为 `INT_MIN` 或第一个 `SCC_w` 值

## 对比与扩展

### 与 SCC 缩点模板的关系

| 知识点 | 区别 |
|--------|------|
| `graph-tarjan-scc` | 仅求 SCC，不做缩点 |
| `graph-scc-condensation` | 求 SCC + 构建缩点图，不做 DP |
| **本知识点** | 求 SCC + 构建缩点图 + 拓扑 DP，完整流水线 |

本知识点是前两者的完整应用：将缩点技术应用到具体的优化问题上。

### DAG 上 DP 的其他变体

| 变体 | 转移方程 | 区别 |
|------|---------|------|
| 最大权值路径 | $A[u] = \max(A[v] + w[u])$ | 求最大和 |
| 最长路（边权） | $A[u] = \max(A[v] + w_{v \to u})$ | 边带权，不是点带权 |
| 方案数 | $C[u] = \sum C[v]$ | 统计路径数量，注意取模 |
| 最短路（DAG） | $D[u] = \min(D[v] + w_{v \to u})$ | 拓扑序松弛 |

### 进阶：传递闭包

缩点后还可在 DAG 上求传递闭包（可达性）。用 bitset 优化可达 $O(n^2 / 64)$，适合 $n \leq 2000$ 的场景。
