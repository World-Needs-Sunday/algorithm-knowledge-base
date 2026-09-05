---
id: graph-scc-condensation
title: 'SCC 缩点（强连通分量缩点）'
category: 图论
subcategory: 强连通分量
tags: ["图论", "强连通分量", "缩点", "Tarjan", "DAG"]
timeComplexity: 'O(n+m)'
spaceComplexity: 'O(n+m)'
codePath: '图论\强连通分量\缩点\源.cpp'
---

## 算法原理

**缩点（Condensation）**是强连通分量（SCC）最重要的应用之一：将每个强连通分量压缩成一个"超级点"，原图中跨 SCC 的边变成超级点之间的边。缩点后得到的图一定是**有向无环图（DAG）**，从而可以使用拓扑排序、DAG 上 DP 等高效算法。

### 为什么要缩点

在一般有向图中，由于存在环，很多问题难以高效求解（比如最长路是 NP-hard 的）。但如果把每个 SCC 缩成一个点，图就变成了 DAG，而 DAG 上的很多问题可以在 $O(n+m)$ 时间内解决。

### 核心思想

1. 先用 Tarjan 或 Kosaraju 算法求出所有 SCC，给每个节点标记所属分量编号 `col[u]`
2. 遍历原图的每条边 $u \to v$：
   - 若 `col[u] != col[v]`，说明这是一条跨 SCC 的边
   - 在缩点图中添加边 `col[u] → col[v]`
3. 为避免重复边（同一个 SCC 对之间可能有多条原边），需要去重

### 缩点图的性质

- 缩点图一定是 DAG（如果缩点图有环，那环上的超级点应该被合并成一个 SCC，矛盾）
- 缩点图的节点数 = SCC 的数量 = `cnt`
- 缩点图的边数 ≤ 原图边数 $m$

## 算法步骤

### 第一步：求 SCC

用 Tarjan 算法求出所有强连通分量，得到：
- `col[u]`：节点 $u$ 所属的 SCC 编号（1 ~ cnt）
- `ans[i]`：第 $i$ 个 SCC 包含的节点列表
- `cnt`：SCC 的总数

### 第二步：构建缩点图

```
初始化 scc_edges, scc_in, scc_out 大小为 cnt+1
对于每个 SCC u (1 ~ cnt):
    对于 u 中的每个节点 i:
        对于 i 的每条出边 j:
            v = col[j]   // 边的终点所在的 SCC
            如果 v == u: 跳过（自环）
            如果 u->v 这条边已经加过: 跳过（去重）
            在 scc_edges[u] 中加入 v
            scc_out[u]++   // u 的出度 +1
            scc_in[v]++    // v 的入度 +1
```

### 第三步：去重技巧

代码中用 `vis[v] = u` 的方式来去重：
- 用一个 `vis` 数组记录"当前处理的 SCC u 已经向 v 连过边了"
- 因为 u 从 1 到 cnt 依次处理，每次处理 u 时，只有 `vis[v] == u` 的才是本轮已加过的边
- 这样不需要每次清空 vis 数组，节省时间

## 逐行代码解析

### 新增变量

```cpp
vector<vector<int>> scc_edges;  // 缩点图的邻接表
vector<int> scc_in, scc_out;    // 每个超级点的入度、出度
```

### SCC 函数：求 SCC + 缩点一体化

```cpp
void SCC(int n)
{
    init(n);
    for (int i = 1; i <= n; i++)
    {
        if (!dns[i]) tarjan(i);  // 第一步：Tarjan 求所有 SCC
    }

    // 第二步：构建缩点图
    scc_edges.assign(cnt + 1, vector<int>());
    scc_in.assign(cnt + 1, 0);
    scc_out.assign(cnt + 1, 0);
    vector<int> vis(cnt + 1, false);  // 去重标记数组

    for (int u = 1; u <= cnt; u++)        // 枚举每个 SCC（超级点 u）
    {
        for (int i : ans[u])              // 枚举 u 中的每个原图节点
        {
            for (int j : edges[i])        // 枚举该节点的每条出边
            {
                int v = col[j];           // 边的终点所属的 SCC
                if (vis[v] == u || u == v) continue;  // 去重 + 跳过自环
                scc_edges[u].emplace_back(v);
                vis[v] = u;               // 标记 u 已经向 v 连过边
                scc_out[u]++;
                scc_in[v]++;
            }
        }
    }
}
```

**关键细节解读：**

| 代码行 | 含义 |
|--------|------|
| `int v = col[j]` | 将原边的终点映射到它所在的 SCC 编号 |
| `u == v` | 边在同一个 SCC 内部，是自环，不需要加到缩点图 |
| `vis[v] == u` | SCC u 已经向 SCC v 连过边了，避免重复加边 |
| `vis[v] = u` | 用当前 SCC 编号 u 作为标记值，不需要清空数组 |

### 主函数调用

```cpp
SCC(n);  // 一次性完成求 SCC 和缩点
cout << "SCC缩点和有 " << cnt << " 个强联通分量" << '\n';
for (int i = 1; i <= cnt; i++)
{
    // 输出每个超级点包含的原节点
    cout << "超级点" << i << "含有: ";
    for (int j : ans[i]) cout << j << ' ';
    cout << '\n';
    // 输出入度、出度
    cout << "超级点" << i << "有" << scc_in[i] << "个入度, " << scc_out[i] << "个出度" << '\n';
    // 输出缩点图的边
    for (int j : scc_edges[i])
    {
        cout << i << "->" << j << '\n';
    }
}
```

## 复杂度分析

- **时间复杂度**：$O(n+m)$
  - Tarjan 求 SCC：$O(n+m)$
  - 构建缩点图：遍历所有节点和边，仍是 $O(n+m)$
  - 去重操作每条边只处理一次，不增加复杂度量级
- **空间复杂度**：$O(n+m)$
  - 缩点图邻接表：$O(m)$（最坏情况每条边都跨 SCC）
  - 入度出度数组：$O(cnt) \approx O(n)$

## 去重方法对比

缩点图的构建中，去重是关键步骤。常见的去重方法有三种：

| 方法 | 思路 | 时间 | 空间 | 适用场景 |
|------|------|------|------|---------|
| **vis 数组法**（本代码） | 用 `vis[v] = u` 标记，无需清空 | $O(n+m)$ | $O(cnt)$ | 边数较多，追求效率 |
| **set 去重法** | 用 `set<int>` 存每个超级点的邻居 | $O(m \log cnt)$ | $O(m)$ | 代码简洁，边数不多 |
| **排序去重法** | 先全加进去，再 sort + unique | $O(m \log m)$ | $O(m)$ | 需要边按顺序排列 |

本代码用的 vis 数组法是最快的，也是竞赛中最常用的写法。

## 适用场景

1. **DAG 上的动态规划**：将一般图缩点成 DAG 后，可以在拓扑序上做 DP
   - 求经过节点权值最大的路径
   - 求方案数、期望值等
2. **入度/出度统计问题**：
   - 至少需要加多少条边才能让整个图强连通（答案 = max(入度为0的点数, 出度为0的点数)）
   - 求有多少个起点可以到达全图（入度为 0 的 SCC 数量）
3. **2-SAT 问题**：用 SCC 判断可行性后，缩点图的拓扑序决定变量取值
4. **图的简化**：将复杂有向图简化为 DAG，便于分析结构

## 常见陷阱与注意事项

1. **自环要跳过**：`u == v` 时不能加边，否则缩点图会有自环，不再是严格的 DAG
2. **边要去重**：同一个 SCC 对之间可能有多条原边，如果不去重，缩点图的边数会膨胀，影响后续算法效率
3. **vis 数组的含义**：`vis[v] == u` 表示"在处理 SCC u 时，已经加过 u→v 的边"，不是"v 被访问过"。这种写法利用了 u 递增的特性，无需每次清空数组
4. **分量编号从 1 开始**：代码中 `ans.assign(1, {})` 和 SCC 编号从 1 到 cnt，所以缩点图数组也要开 `cnt + 1`
5. **SCC 函数的参数**：传入的是节点数 n，不是边数 m

## 缩点 + 拓扑 DP 示例

缩点后最常见的操作就是在 DAG 上做拓扑 DP。以下是一个典型模板：

```cpp
// 伪代码：缩点后求每个超级点出发的最长路
vector<int> topo;          // 拓扑序
vector<int> deg(cnt + 1);  // 入度（已在 scc_in 中计算好）
queue<int> q;

for (int i = 1; i <= cnt; i++)
    if (scc_in[i] == 0) q.push(i);

while (!q.empty())
{
    int u = q.front(); q.pop();
    topo.push_back(u);
    for (int v : scc_edges[u])
    {
        scc_in[v]--;
        if (scc_in[v] == 0) q.push(v);
    }
}

// 在拓扑序上 DP
vector<int> dp(cnt + 1);
for (int u : topo)
    for (int v : scc_edges[u])
        dp[v] = max(dp[v], dp[u] + weight[v]);
```

## 与其他图算法的关系

| 算法 | 关系 |
|------|------|
| Tarjan / Kosaraju | 缩点的前置步骤，用于求出 SCC |
| 拓扑排序 | 缩点后常用的下一步操作，DAG 上 DP 的基础 |
| DAG 最短路 / 最长路 | 缩点后可以用拓扑序在 DAG 上求最短路/最长路 |
| 2-SAT | 用 SCC 求解后，缩点图的拓扑序决定变量真假 |
