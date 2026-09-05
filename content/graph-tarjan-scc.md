---
id: graph-tarjan-scc
title: 'Tarjan 强连通分量'
category: 图论
subcategory: 强连通分量
tags: ["图论", "强连通分量", "Tarjan", "DFS", "缩点"]
timeComplexity: 'O(n+m)'
spaceComplexity: 'O(n+m)'
codePath: '图论\强连通分量\Untitled1.cpp'
---

## 算法原理

Tarjan 算法由 Robert Tarjan 于 1972 年提出，基于**一次深度优先搜索（DFS）**找出有向图中所有的强连通分量（Strongly Connected Components, SCC），时间复杂度为 $O(n+m)$。

### 什么是强连通分量

在有向图中，若两个节点 $u$ 和 $v$ 互相可达（即存在 $u \to v$ 的路径，也存在 $v \to u$ 的路径），则称它们强连通。图中**极大的强连通子图**称为强连通分量。

强连通分量的意义在于：**将每个 SCC 缩成一个点后，原图变成 DAG**，从而可以用拓扑排序等 DAG 专属算法高效解决问题。

### 核心思想

在 DFS 过程中，用两个关键数组标记每个节点：

| 数组 | 含义 |
|------|------|
| `dns[u]`（dfn） | 节点 $u$ 被首次访问的时间戳（DFS 序） |
| `low[u]` | 从 $u$ 出发，通过**一条返祖边**或通过**后代的返祖边**能回到的最早节点的时间戳 |

核心结论：**当 `dfs[u] == low[u]` 时，以 $u$ 为根的子树中所有仍在栈里的节点恰好构成一个强连通分量。**

### 为什么正确

- 如果 `low[u] < dns[u]`，说明 $u$ 能通过某条路径回到更早的祖先，因此 $u$ 一定和那个祖先在同一个 SCC 中
- 如果 `low[u] == dns[u]`，说明 $u$ 无法回到任何更早的节点，它就是当前 SCC 的"最高点"（根），此时栈中从 $u$ 到栈顶的所有节点都属于这个 SCC

## 算法步骤

1. **初始化**：`dns`、`low` 全为 0（未访问），栈为空，时间戳 `tim = 1`，分量计数 `cnt = 0`
2. **遍历所有节点**：对每个未访问的节点启动 DFS
3. **DFS 过程**：
   - 标记当前节点的 `dns` 和 `low`，将节点压入栈并标记在栈中
   - 遍历每条出边：
     - 若后继未访问：递归 DFS，回溯时更新 `low[u] = min(low[u], low[v])`
     - 若后继已访问且仍在栈中（返祖边）：更新 `low[u] = min(low[u], dns[v])`
   - 遍历结束后，若 `dns[u] == low[u]`：弹出栈中元素直到包含 $u$，这些节点构成一个 SCC

## 逐行代码解析

### 变量与数组

```cpp
vector<vector<int>> edges, ans;  // 邻接表 / 各分量的节点列表
vector<int> sk, dns, low, col;   // 栈 / 时间戳 / 能回退的最早时间 / 所属分量
vector<bool> in_sk;              // 节点是否在栈中
int idx, tim, cnt;               // 栈顶指针 / 时间戳 / 分量计数
```

### 初始化

```cpp
inline void init(int n)
{
    idx = 0, tim = 1, cnt = 0;
    sk.assign(n + 1, 0);
    in_sk.assign(n + 1, false);
    dns.assign(n + 1, 0);
    low.assign(n + 1, 0);
    col.assign(n + 1, 0);
    ans.assign(1, {});  // ans[0] 占位，分量从 1 开始
}
```

### 标记节点首次访问

```cpp
inline void mark(int rt)
{
    dns[rt] = tim;
    low[rt] = tim;   // 初始假设只能回到自己
    sk[idx++] = rt;  // 入栈
    in_sk[rt] = true;
    tim++;
}
```

### Tarjan DFS 主体

```cpp
void tarjan(int rt)
{
    mark(rt);
    for (int i : edges[rt])
    {
        if (!dns[i])          // 树边：后继未访问
        {
            tarjan(i);
            low[rt] = min(low[rt], low[i]);  // 用儿子的low更新自己
        }
        else if (in_sk[i])    // 返祖边：后继已访问且在栈中
            low[rt] = min(low[rt], dns[i]);  // 用后继的dfn更新自己
    }
    if (dns[rt] == low[rt])   // rt是SCC的根
    {
        ++cnt;
        ans.emplace_back();
        while (in_sk[rt])     // 弹出到rt为止的所有节点
        {
            col[sk[idx - 1]] = cnt;       // 标记所属分量
            ans[cnt].emplace_back(sk[idx - 1]);
            in_sk[sk[idx - 1]] = false;
            --idx;
        }
    }
}
```

### 主函数：处理不连通图

```cpp
for (int i = 1; i <= n; i++)
{
    if (!dns[i]) tarjan(i);  // 每个未访问的节点都要启动一次DFS
}
```

图可能不连通，需要对每个连通分量分别运行 Tarjan。

## 复杂度分析

- **时间复杂度**：$O(n+m)$
  - 每个节点恰好入栈一次、出栈一次
  - 每条边恰好被访问一次
  - DFS 总代价线性
- **空间复杂度**：$O(n+m)$
  - 邻接表：$O(m)$
  - dns、low、col、in_sk、栈：$O(n)$

## 适用场景

1. **强连通分量检测**：找出图中所有 SCC，常用于图的结构分析
2. **缩点**：将每个 SCC 缩成一个点，把一般有向图转化为 DAG，然后做拓扑 DP、最长路等
3. **2-SAT 问题**：利用 Tarjan 求 SCC 后，根据每个变量的两个状态是否在同一 SCC 中判断可行性
4. **有向图的连通性问题**：判断两个节点是否互相可达

## 常见陷阱与注意事项

1. **in_sk 的判断**：返祖边的条件是 `dns[i] != 0 && in_sk[i] == true`，不能只判断 `dns[i] != 0`。因为如果 `i` 已经被弹出栈（属于其他 SCC），就不是返祖边而是横叉边
2. **low 的更新方式**：树边用 `low[v]` 更新，返祖边用 `dns[v]` 更新（不是 `low[v]`）。返祖边只能跳一步，不能用后代的 low 值
3. **栈的弹出条件**：当 `dns[rt] == low[rt]` 时，弹出**从栈顶到 rt** 的所有节点，不是只弹 rt
4. **多连通分量**：图可能不连通，必须遍历所有节点，对未访问的都跑一次 Tarjan
5. **ans 的索引**：代码中 `ans.assign(1, {})` 让 ans 从下标 1 开始，分量编号从 1 到 cnt
6. **变量名 dns**：部分教材写作 dfn（depth-first number），含义相同

## 缩点：SCC 的典型应用

求出 SCC 后，可以构建**缩点图**：每个 SCC 是一个新节点，原图中跨 SCC 的边变成缩点图中的边。缩点图一定是 DAG。

```cpp
// 伪代码：构建缩点图
vector<vector<int>> new_edges(cnt + 1);
for (int u = 1; u <= n; u++)
    for (int v : edges[u])
        if (col[u] != col[v])
            new_edges[col[u]].push_back(col[v]);
```

缩点后可以做：
- 拓扑排序上的 DP（比如求经过节点权值最大的路径）
- 求 DAG 上的最长路/最短路
- 统计入度为 0 的缩点个数（至少需要多少个起点才能走遍全图）

## 与 Kosaraju 算法对比

| 算法 | 思路 | DFS 次数 | 常数 | 直观程度 |
|------|------|---------|------|---------|
| Tarjan | 一次DFS + 栈 + dfn/low | 1 次 | 小 | 较难理解 |
| Kosaraju | 正向DFS出栈序 + 反向图DFS | 2 次 | 稍大 | 直观易理解 |

两者时间复杂度相同（都是 $O(n+m)$），Tarjan 只需要一次 DFS 所以常数更小；Kosaraju 思路更清晰但代码稍长。竞赛中 Tarjan 更常用。
