---
id: graph-lca-tarjan
title: '最近公共祖先 LCA（Tarjan 离线·并查集）'
category: 图论
subcategory: 最近公共祖先
tags: ["图论", "树", "LCA", "最近公共祖先", "Tarjan", "离线算法", "并查集", "DFS", "Union-Find"]
timeComplexity: 'O(n + m·α(n))'
spaceComplexity: 'O(n + m)'
codePath: '图论\最近公共祖先\Tarjan\Untitled1.cpp'
---

## 算法原理

Tarjan 算法解决**最近公共祖先**（Lowest Common Ancestor, LCA）问题：给定一棵有根树和 $m$ 个查询请求 $(u_i, v_i)$，求每对节点的 LCA。与倍增法（在线查询）不同，Tarjan 是一种**离线算法**——它需要提前知道所有查询，在一次 DFS 遍历中批量回答所有问题。

### 核心思想：DFS + 并查集

Tarjan 算法的精髓在于利用 DFS 的回溯时机和并查集的连通性来判断 LCA。算法基于以下关键观察：

> **当 DFS 访问到节点 $u$ 时，$u$ 的子树尚未被探索；当 DFS 从 $u$ 的子树回溯时，子树中所有节点已经访问完毕。此时若查询 $(u, v)$ 中的 $v$ 已经被访问过，则 $\text{LCA}(u, v)$ 就是 $v$ 所在并查集的根。**

具体来说，DFS 过程中维护一个并查集，其状态具有特殊语义：

- **已访问完毕的节点**（子树已回溯）：已合并到其父节点所在的集合中
- **正在访问中的节点**（DFS 栈中）：尚未合并，父节点指向自身
- **未访问的节点**：尚未触及

当 DFS 从节点 $u$ 的某棵子树回溯时，该子树根节点 $v$ 被合并到 $u$（`fa[v] = u`）。这意味着：**已回溯的子树中的所有节点，其并查集根指向它们当前最高的"已回溯祖先"**。因此，若查询 $(u, v)$ 中 $v$ 已访问完毕，`find(v)` 返回的就是 $v$ 当前最高的已回溯祖先——恰好是 $\text{LCA}(u, v)$。

### 算法流程

1. **预处理**：读取树结构，找到根节点（未被任何节点作为子节点的节点）
2. **存储查询**：每个查询 $(u, v)$ 存储在 `arr[u]` 和 `arr[v]` 两端（双向存储），附带查询编号
3. **初始化并查集**：`fa[i] = i`，每个节点自成一个集合
4. **DFS 遍历**（核心）：
   - 标记当前节点 `rt` 为已访问
   - 递归访问每个子节点 `i`：
     - `tarjan(i)` 递归处理子树
     - 回溯后执行 `fa[i] = rt`，将子树合并到当前节点
   - 处理与 `rt` 相关的所有查询 $(rt, v)$：
     - 若 $v$ 已访问（`vis[v] == true`），则 `ans[查询编号] = find(v)`
5. **输出**：按查询编号顺序输出所有答案

### 离线 vs 在线

| 特性 | Tarjan（离线） | 倍增法（在线） |
|---|---|---|
| 查询方式 | 必须提前知道所有查询 | 查询一个答一个 |
| 预处理 | $O(n)$ | $O(n \log n)$ |
| 单次查询 | 均摊 $O(\alpha(n))$ | $O(\log n)$ |
| 总复杂度 | $O(n + m \cdot \alpha(n))$ | $O(n \log n + m \log n)$ |
| 空间 | $O(n + m)$ | $O(n \log n)$ |
| 实现难度 | 中（DFS + 并查集） | 中（倍增表） |
| 适用场景 | 批量查询、离线 | 在线、逐个查询 |

当 $m$ 很大时，Tarjan 的 $O(m \cdot \alpha(n))$ 远优于倍增法的 $O(m \log n)$。

## 核心公式/状态定义与转移方程

### 状态定义

$$\text{fa}[i] = \text{节点 } i \text{ 在并查集中的父节点（路径压缩后指向根）}$$

$$\text{vis}[i] = \begin{cases} \text{true} & \text{节点 } i \text{ 已被 DFS 访问} \\ \text{false} & \text{尚未访问} \end{cases}$$

$$\text{ans}[k] = \text{第 } k \text{ 个查询的 LCA 结果}$$

### 并查集查找（带路径压缩）

$$\text{find}(i) = \begin{cases} i & \text{若 } \text{fa}[i] = i \\ \text{find}(\text{fa}[i]) & \text{否则（路径压缩：fa}[i] \leftarrow \text{find}(\text{fa}[i])\text{）} \end{cases}$$

### DFS 回溯合并

当 DFS 从子节点 $i$ 回溯到父节点 $rt$ 时：

$$\text{fa}[i] \leftarrow rt$$

此时子树 $i$ 中所有已访问节点的 `find` 结果都指向 $rt$（或更高的已回溯祖先）。

### LCA 查询判定

当 DFS 处理节点 $rt$ 的查询 $(rt, v)$ 时：

$$\text{若 } \text{vis}[v] = \text{true} \implies \text{ans}[k] = \text{find}(v)$$

**正确性证明**：若 $v$ 已被访问，说明 $v$ 在 DFS 栈的某个已回溯分支中。此时 $\text{find}(v)$ 返回 $v$ 所在并查集的根——即 $v$ 的最高已回溯祖先。这个祖先恰好是 $rt$ 和 $v$ 的 LCA，因为：

- 它是 $v$ 的祖先（$v$ 在它的子树中）
- 它是 $rt$ 的祖先（$rt$ 正在 DFS 中，$\text{find}(v)$ 的结果必然是 $rt$ 的祖先或 $rt$ 本身）
- 它是最深的同时是 $rt$ 和 $v$ 祖先的已回溯节点

### 时空复杂度

$$T_{\text{DFS}} = O(n), \quad T_{\text{查询}} = O(m \cdot \alpha(n)), \quad T_{\text{总}} = O(n + m \cdot \alpha(n))$$

$$S = O(n + m)$$

其中 $\alpha(n)$ 是阿克曼反函数，对于所有实际 $n$ 不超过 5，可视为 $O(1)$。

## 逐行代码解析

### 第 1 段：全局变量与并查集查找

```cpp
int N, M;
vector<int> fa, ans, vis;
vector<vector<int>> A;                    // 树结构：A[u] 存 u 的所有子节点
vector<vector<pair<int,int>>> arr;        // 查询：arr[u] 存 (对端节点, 查询编号)
int find(int i)
{
    if (fa[i] != i) fa[i] = find(fa[i]);  // 路径压缩
    return fa[i];
}
```

- **`fa`**：并查集父节点数组。初始时 `fa[i] = i`，DFS 回溯时 `fa[子] = 父`。
- **`ans`**：存储 $m$ 个查询的答案，`ans[k]` 是第 $k$ 个查询的 LCA。
- **`vis`**：DFS 访问标记。注意 `vis` 被复用了两次——先用于找根节点，后用于 Tarjan DFS。
- **`A`**：树结构。`A[u]` 存储 $u$ 的所有子节点列表。输入格式为每个节点先给子节点数，再给子节点编号。
- **`arr`**：查询的邻接表。每个查询 $(u, v)$ 同时存入 `arr[u]` 和 `arr[v]`，附带查询编号 $i$。
- **`find`**：标准路径压缩并查集查找。

### 第 2 段：Tarjan DFS（核心）

```cpp
void tarjan(int rt)
{
    vis[rt] = true;                       // 标记当前节点已访问
    for (int i : A[rt])                   // 遍历所有子节点
    {
        tarjan(i);                        // 递归处理子树
        fa[i] = rt;                       // 回溯：将子节点合并到当前节点
    }
    for (int i = 0; i < arr[rt].size(); i++)   // 处理与 rt 相关的所有查询
    {
        if (!vis[arr[rt][i].first]) continue;  // 对端节点未访问，跳过
        ans[arr[rt][i].second] = find(arr[rt][i].first);  // 对端已访问，LCA = find(对端)
    }
}
```

逐行分析：

- **`vis[rt] = true`**：标记当前节点正在被访问。此标记在回溯后**不重置**——Tarjan 算法中 `vis` 一旦设为 `true` 就保持，表示"已被 DFS 访问过"。
- **`for (int i : A[rt])`**：遍历 `rt` 的所有子节点。这是树形 DFS，不需要像图 DFS 那样跳过父节点（因为树中只有父→子方向）。
- **`tarjan(i)`**：递归处理子树 $i$。递归返回时，子树 $i$ 中的所有节点已访问完毕，且 $i$ 子树中的节点已在并查集中合并。
- **`fa[i] = rt`**：**关键操作**。递归回溯后，将子节点 $i$ 的并查集父设为 $rt$。这样 $i$ 子树中所有节点的 `find` 结果指向 $rt$（或更高的已回溯祖先）。
- **查询处理循环**：遍历所有与 $rt$ 相关的查询 $(rt, v, k)$：
  - `if (!vis[arr[rt][i].first]) continue`：若对端节点 $v$ 尚未被 DFS 访问，跳过（等 $v$ 那边处理时再回答）。
  - `ans[查询编号] = find(对端)`：$v$ 已访问，`find(v)` 返回 $v$ 的最高已回溯祖先，即 LCA。

### 第 3 段：主函数——建树与找根

```cpp
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    cin >> N >> M;
    A.resize(N + 1);
    vis.resize(N + 1, false);
    for (int i = 1; i <= N; i++)
    {
        int n;
        cin >> n;
        A[i].resize(n);
        for (int j = 0; j < n; j++)
        {
            cin >> A[i][j];
            vis[A[i][j]] = true;           // 标记所有"作为子节点"的节点
        }
    }
    int rt = 1;
    while (vis[rt]) rt++;                 // 找根：未被标记为子节点的节点
```

- **输入格式**：每个节点先输入子节点数 $n$，再输入 $n$ 个子节点编号。
- **`vis[A[i][j]] = true`**：复用 `vis` 数组标记所有"是某节点子节点"的节点。最终未被标记的就是根。
- **找根**：`while (vis[rt]) rt++` 从 1 开始找第一个未被标记的节点。注意此时 `vis` 的语义是"是否是子节点"，与 Tarjan DFS 中的语义不同。

### 第 4 段：存储查询与初始化

```cpp
    arr.resize(N + 1);
    for (int i = 1; i <= M; i++)
    {
        int u, v;
        cin >> u >> v;
        arr[u].emplace_back(v, i);        // 双向存储：u 的查询列表中加入 (v, i)
        arr[v].emplace_back(u, i);        // v 的查询列表中加入 (u, i)
    }

    fa.resize(N + 1);
    ans.resize(M + 1);
    for (int i = 1; i <= N; i++) fa[i] = i, vis[i] = false;  // 初始化并查集 + 重置 vis
    tarjan(rt);
```

- **双向存储**：每个查询 $(u, v)$ 同时加入 `arr[u]` 和 `arr[v]`，这样无论 DFS 先访问到 $u$ 还是 $v$，都能在另一端已访问时回答。
- **查询编号**：`emplace_back(对端, 查询编号)`，查询编号用于将答案写入 `ans[编号]`。
- **重置 vis**：`vis[i] = false` 将 `vis` 从"是否是子节点"重置为"是否被 DFS 访问"。
- **`tarjan(rt)`**：从根节点开始一次 DFS，回答所有查询。

### 第 5 段：输出结果

```cpp
    for (int i = 1; i < ans.size(); i++) cout << ans[i] << ' ';
    return 0;
}
```

- 按查询编号 $1 \sim M$ 顺序输出所有 LCA 结果。

## 复杂度分析

### 时间复杂度

| 步骤 | 复杂度 |
|---|---|
| 建树 + 找根 | $O(n)$ |
| 存储查询 | $O(m)$ |
| 并查集初始化 | $O(n)$ |
| Tarjan DFS（$n$ 个节点，每个访问一次） | $O(n)$ |
| 查询处理（$m$ 个查询，每个一次 `find`） | $O(m \cdot \alpha(n))$ |
| **总计** | $O(n + m \cdot \alpha(n))$ |

DFS 中每个节点访问一次，每条树边遍历一次（$n - 1$ 条），并查集合并 $n - 1$ 次。每个查询的 `find` 为 $O(\alpha(n))$。

### 空间复杂度

| 数据结构 | 空间 |
|---|---|
| 树结构 `A` | $O(n)$ |
| 查询存储 `arr` | $O(m)$（每查询存两份） |
| 并查集 `fa` + 答案 `ans` + 标记 `vis` | $O(n + m)$ |
| DFS 递归栈 | $O(n)$（最坏链状树） |
| **总计** | $O(n + m)$ |

### 与倍增法对比

| 维度 | Tarjan | 倍增法 |
|---|---|---|
| 预处理 | $O(n)$ | $O(n \log n)$ |
| 单次查询 | $O(\alpha(n)) \approx O(1)$ | $O(\log n)$ |
| $m$ 次查询总时间 | $O(n + m)$ | $O(n \log n + m \log n)$ |
| 空间 | $O(n + m)$ | $O(n \log n)$ |
| 在线/离线 | **离线** | **在线** |

当 $m$ 很大（如 $m = 10^6$）时，Tarjan 的 $O(m)$ 远优于倍增法的 $O(m \log n)$。但 Tarjan 要求所有查询提前已知。

## 适用场景

### 适合使用 Tarjan

- **批量 LCA 查询**：所有查询提前已知，$m$ 很大时效率极高
- **离线问题**：题目允许离线处理（如"给定 $m$ 个查询，输出所有 LCA"）
- **树上路径问题**：配合差分思想处理"路径上所有节点加/减权值"等问题
- **内存受限**：$O(n + m)$ 空间优于倍增法的 $O(n \log n)$
- **需要 $O(1)$ 查询**：均摊 $O(\alpha(n)) \approx O(1)$，是所有 LCA 算法中查询最快的（与欧拉序 + ST 表的 $O(1)$ 相当，但空间更小）

### 不适合使用 Tarjan

- **在线查询**：查询必须逐个回答，无法提前收集（倍增法或重链剖分更合适）
- **树动态变化**：树结构在查询间可能修改（所有 LCA 算法均不适用动态树，需用 Link-Cut Tree）
- **递归深度过大**：$n$ 很大（如 $10^6$）且树退化为链时，DFS 递归可能栈溢出（可用迭代 DFS 或手动扩栈解决）

## 常见陷阱与注意事项

1. **vis 数组的双重用途**
   - 本代码中 `vis` 先用于**找根**（标记"是某节点的子节点"），后用于 **Tarjan DFS**（标记"已被 DFS 访问"）。
   - 在进入 Tarjan 之前必须 `vis[i] = false` 重置，否则 DFS 中所有节点的 `vis` 都是 `true`，查询判定失效。

2. **查询双向存储**
   - 每个查询 $(u, v)$ 必须同时存入 `arr[u]` 和 `arr[v]`。若只存一端，当 DFS 先访问到 $u$ 时 $v$ 还未访问，该查询永远不会被回答。
   - 双向存储保证：无论 DFS 先到 $u$ 还是 $v$，当后到达的那端处理时，先到达的端已 `vis = true`，能正确回答。

3. **回溯合并的时机**
   - `fa[i] = rt` 必须在 `tarjan(i)` 递归返回**之后**执行。若在递归前设置，子树中节点的 `find` 会提前指向 $rt$，导致错误。
   - 正确顺序：先递归处理子树 → 回溯 → 合并并查集。

4. **查询处理在子树全部回溯之后**
   - 查询处理循环 `for (int i = 0; i < arr[rt].size(); i++)` 位于所有子树递归之后。这确保 $rt$ 的所有子树都已合并，此时 $rt$ 的并查集状态正确。
   - 若将查询处理放在子树递归之前，$rt$ 的子节点尚未回溯合并，`find` 结果不正确。

5. **并查集不带按秩合并**
   - 本代码仅用路径压缩，无按秩合并。单次 `find` 均摊 $O(\log n)$，但结合 DFS 的特殊结构（合并顺序与树结构一致），实际性能极佳。
   - 加入按秩合并可保证 $O(\alpha(n))$，但实现稍复杂。

6. **树结构为有根树**
   - 代码中 `A[u]` 存储 $u$ 的子节点（父→子方向）。DFS 从根出发，只往子节点方向递归。
   - 若题目给出无根树（边集形式），需先建无向邻接表，DFS 时跳过父节点：
     ```cpp
     void tarjan(int rt, int fa_node) {
         vis[rt] = true;
         for (int v : adj[rt]) {
             if (v == fa_node) continue;
             tarjan(v, rt);
             fa[v] = rt;
         }
         // ... 查询处理
     }
     ```

7. **DFS 栈溢出**
   - 当树退化为链（$n = 10^5 \sim 10^6$）时，递归深度等于 $n$，可能栈溢出。
   - 解决方法：`#pragma comment(linker, "/STACK:102400000,102400000")`（MSVC）或手动改为迭代 DFS。

8. **查询编号从 1 开始**
   - `ans` 数组下标从 1 开始（查询编号 $1 \sim M$），输出时 `for (int i = 1; i < ans.size(); i++)`。注意 `ans[0]` 未使用。

## 对比与扩展

### 三种 LCA 算法综合对比

| 算法 | 预处理 | 查询 | 空间 | 在线/离线 | 特点 |
|---|---|---|---|---|---|
| 倍增法 | $O(n \log n)$ | $O(\log n)$ | $O(n \log n)$ | 在线 | 通用、易扩展路径信息 |
| Tarjan | $O(n)$ | $O(\alpha(n))$ | $O(n + m)$ | **离线** | 查询最快、空间最小 |
| 欧拉序+ST 表 | $O(n \log n)$ | $O(1)$ | $O(n \log n)$ | 在线 | 在线 $O(1)$ 查询 |
| 重链剖分 | $O(n)$ | $O(\log n)$ | $O(n)$ | 在线 | 可维护树链信息 |

### 扩展：树上路径距离

与倍增法相同，利用 LCA 可求树上两点距离：

$$\text{dist}(u, v) = h[u] + h[v] - 2 \times h[\text{LCA}(u, v)]$$

Tarjan 在一次 DFS 中即可收集所有查询的 LCA，配合深度数组 $h$ 批量计算距离。

### 扩展：树上差分

Tarjan 天然适合树上差分问题。对于 $m$ 条路径 $(u_i, v_i)$，每条路径上所有节点加 $w_i$：

$$\text{diff}[u_i] \mathrel{+}= w_i, \quad \text{diff}[v_i] \mathrel{+}= w_i, \quad \text{diff}[\text{LCA}_i] \mathrel{-}= w_i, \quad \text{diff}[\text{fa}(\text{LCA}_i)] \mathrel{-}= w_i$$

用 Tarjan 一次性求出所有 LCA，再做一次 DFS 求子树和即可。总复杂度 $O(n + m)$。

### 扩展：无根树的处理

本代码假设树以"每个节点的子节点列表"形式给出。若题目给出无根树（边集），需要：

1. 建无向邻接表
2. 任选一个节点作为根（如节点 1）
3. DFS 时传入父节点参数，跳过父节点方向

```cpp
vector<vector<int>> adj;
void tarjan(int rt, int fa_node) {
    vis[rt] = true;
    for (int v : adj[rt]) {
        if (v == fa_node) continue;
        tarjan(v, rt);
        fa[v] = rt;
    }
    // 查询处理不变
}
```

### 扩展：与 Kruskal 的联系

Tarjan 的 LCA 算法和 Kruskal 算法都使用了并查集，但用途不同：
- **Kruskal**：并查集用于判断连通性（是否成环），合并方向无限制
- **Tarjan LCA**：并查集用于追踪"已回溯的最高祖先"，合并方向严格按树结构（子→父）

两者都依赖路径压缩保证性能，但 Tarjan 中并查集的合并顺序由 DFS 回溯顺序决定，具有特殊的树形结构。
