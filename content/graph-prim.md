---
id: graph-prim
title: 'Prim 最小生成树（堆优化·Lazy 版）'
category: 图论
subcategory: 最小生成树
tags: ["图论", "最小生成树", "Prim", "堆优化", "优先队列", "贪心", "邻接表", "MST"]
timeComplexity: 'O(m log m)'
spaceComplexity: 'O(n + m)'
codePath: '图论\最小生成树\Prim\Untitled1.cpp'
---

## 算法原理

Prim 算法解决**最小生成树**（Minimum Spanning Tree, MST）问题：给定一个含 $n$ 个节点、$m$ 条边的**连通无向带权图**，求一棵边权和最小的生成树。与 Kruskal 的"边贪心"（全局最小边优先）不同，Prim 采用**点贪心**策略——从一个起始节点出发，逐步将最近的未访问节点加入生成树。

### 核心思想：切点定理

Prim 的正确性基于**切（Cut）定理**：将图中的节点划分为已选集合 $S$ 和未选集合 $V \setminus S$，则横跨这两个集合的边中，权值最小的边一定属于某棵 MST。

Prim 每一步维护一个切：$S$ = 已加入生成树的节点，$V \setminus S$ = 未加入的节点。每次从横跨切的边中选取权值最小的一条，将其连接的未选节点加入 $S$。重复 $n - 1$ 次即可得到 MST。

### 算法流程

1. 任选一个起始节点 $s$（本代码选 $s = 1$），将其加入"候选边"
2. 从优先队列中取出权值最小的边 $(w, v)$：
   - 若 $v$ 已访问，跳过（惰性删除）
   - 若 $v$ 未访问，将 $v$ 加入 MST：`vis[v] = true`，`ans += w`，`cnt++`
3. 将 $v$ 的所有出边 $(w', u)$ 中 $u$ 未访问的加入优先队列
4. 重复直到所有 $n$ 个节点都加入（`cnt == n`），或队列为空（图不连通）

### Lazy Prim vs Eager Prim

本代码实现的是 **Lazy Prim**（惰性 Prim）：

| 特性 | Lazy Prim | Eager Prim |
|---|---|---|
| 优先队列存储 | 所有候选边（含过期的） | 每个节点只保留当前最短边 |
| 过期处理 | 弹出时检查 `vis` 跳过 | 用 decrease-key 更新 |
| 堆中元素数 | $O(m)$ | $O(n)$ |
| 时间复杂度 | $O(m \log m)$ | $O(m \log n)$ |
| 实现难度 | 低（标准堆即可） | 高（需索引堆或 Fibonacci 堆） |

Lazy 版虽然堆中可能有大量过期元素（同一节点被多次入队），但 `greater<>` 的 `priority_queue` 实现简单，竞赛中通常足够。

### 与 Dijkstra 的相似性

Prim 与 Dijkstra 的算法结构几乎完全相同——都是从起点出发，用优先队列维护"当前最近的未访问节点"。区别在于：

| 特性 | Dijkstra | Prim |
|---|---|---|
| 优化目标 | 起点到各点的最短距离 | 生成树总权值最小 |
| 距离含义 | $d(s, u)$：源点到 $u$ 的距离 | $w(u)$：$u$ 到已选集合的最短边 |
| 距离更新 | $d[v] = \min(d[v], d[u] + w)$ | 只需 $w$（边的原始权值） |
| 起始边权 | $d[s] = 0$ | 起始边权 $= 0$（虚拟连接） |
| 累加方式 | 不累加 | 累加 `ans += w` |

## 核心公式/状态定义与转移方程

### 状态定义

$$\text{vis}[v] = \text{节点 } v \text{ 是否已加入生成树}$$

$$\text{key}(v) = \min_{u \in S} w(u, v) \quad \text{（未访问节点 } v \text{ 到已选集合 } S \text{ 的最短边权）}$$

### 初始化

$$\text{vis}[v] = \text{false} \quad \forall v, \qquad \text{起点 } s = 1, \quad \text{key}(s) = 0$$

起始时将 $(0, 1)$ 入队，表示节点 1 以权值 0 连接到"虚拟根"。

### 选点与扩展

每轮从优先队列弹出最小边 $(w, v)$：

$$\text{若 } \text{vis}[v] = \text{false} \implies \text{vis}[v] = \text{true},\; \text{ans} \leftarrow \text{ans} + w,\; \text{cnt} \leftarrow \text{cnt} + 1$$

扩展时对 $v$ 的每条出边 $(v, u, w')$：

$$\text{若 } \text{vis}[u] = \text{false} \implies \text{将 } (w', u) \text{ 入队}$$

### 终止条件

$$\text{cnt} = n \implies \text{所有节点已加入，MST 完成}$$

$$\text{队列为空且 cnt} < n \implies \text{图不连通，无生成树}$$

### 正确性证明（切定理）

设 $S$ 和 $V \setminus S$ 是一个切，$e = (u, v)$ 是横跨该切的最小边（$u \in S, v \notin S$）。假设 $e$ 不属于任何 MST，则存在一棵 MST $T$ 不包含 $e$。$T$ 中连接 $u$ 和 $v$ 的路径必有一条边 $e'$ 横跨该切。由于 $w(e) \le w(e')$，将 $e'$ 替换为 $e$ 得到 $T'$，$w(T') \le w(T)$，且 $T'$ 仍为生成树。故存在包含 $e$ 的 MST，矛盾。

## 逐行代码解析

### 第 1 段：邻接表建图

```cpp
vector<vector<pair<int,long long>>> A;
int N, M;
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    cin >> N >> M;
    A.resize(N + 1);
    for(int i = 1; i <= M; i++)
    {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        A[u].emplace_back(v, w);
        A[v].emplace_back(u, w);
    }
```

- **`A`**：邻接表，`A[u]` 存储 $u$ 的所有出边 `(邻居, 权值)`。
- **无向图**：`A[u].emplace_back(v, w)` 和 `A[v].emplace_back(u, w)` 双向建边。
- **`long long w`**：边权用 `long long` 防止累加溢出。
- **快读**：`ios::sync_with_stdio(false)` 解除 cin/cout 与 stdio 的同步。

### 第 2 段：优先队列初始化

```cpp
    vector<bool> vis(N + 1, false);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>,
                   greater<pair<long long, int>>> que;
    que.emplace(0, 1);
    long long ans = 0;
    int cnt = 0;
```

- **`vis`**：标记节点是否已加入 MST。
- **`priority_queue<..., greater<...>>`**：小根堆（最小堆），`pair` 的 first 是边权，second 是节点编号。堆按边权升序排列，每次弹出最小边。
- **`que.emplace(0, 1)`**：起点为节点 1，入队边权为 0（虚拟边，表示节点 1 到"空集"的距离为 0）。
- **`ans`**：MST 总权值。
- **`cnt`**：已加入 MST 的节点数，目标为 $n$。

### 第 3 段：主循环——选点与扩展（核心）

```cpp
    while(!que.empty())
    {
        int nd = que.top().second;
        long long w = que.top().first;
        que.pop();
        if(vis[nd]) continue;

        vis[nd] = true;
        ans += w, ++cnt;
        if(cnt == N) break;
        for(int i = 0; i < A[nd].size(); i++)
        {
            int nd_son = A[nd][i].first;
            if(vis[nd_son]) continue;
            que.emplace(A[nd][i].second, nd_son);
        }
    }
```

逐行分析：

- **`que.top()` → `que.pop()`**：取出堆顶（当前全局最小候选边），弹出。
- **`if(vis[nd]) continue`**：**惰性删除**。同一节点可能被多次入队（因为每次扩展都推入所有未访问邻居），但只有第一次弹出时才被选中。后续弹出的是过期数据，直接跳过。
- **`vis[nd] = true`**：将节点加入 MST。
- **`ans += w`**：累加选中边的权值。注意起点的 $w = 0$，不影响结果。
- **`cnt++` + `if(cnt == N) break`**：当所有 $n$ 个节点都加入时终止。注意这里是 `cnt == N`（节点数），不是 `N - 1`（边数），因为起点也是通过"边权 0 的虚拟边"加入的，所以 `cnt` 计的是节点而非边。
- **扩展循环**：遍历 `nd` 的所有邻居，将未访问的邻居 `(w', u)` 推入堆。这里**不做 decrease-key**——即同一个节点可能被多次入队，这就是 Lazy Prim 的特征。

### 第 4 段：输出结果

```cpp
    if(cnt == N) cout << ans;
    else cout << "orz";
    return 0;
}
```

- **连通判定**：`cnt == N` 表示所有节点都加入了 MST，图连通；否则图不连通。
- **"orz"**：题目约定的无解输出（对应 Luogu P3366）。

## 复杂度分析

### 时间复杂度

| 步骤 | 复杂度 |
|---|---|
| 建图（邻接表） | $O(m)$ |
| 堆操作 | $O(m \log m)$ |
| 扩展（遍历所有邻接边） | $O(m)$ |
| **总计** | $O(m \log m)$ |

堆操作分析：每条边最多被入队一次（从两个端点各一次），堆中最多 $O(m)$ 个元素，每次 push/pop 为 $O(\log m)$。总堆操作次数 $O(m)$，故 $O(m \log m)$。

### 空间复杂度

| 数据结构 | 空间 |
|---|---|
| 邻接表 `A` | $O(n + m)$ |
| 访问标记 `vis` | $O(n)$ |
| 优先队列 `que` | $O(m)$（最坏情况） |
| **总计** | $O(n + m)$ |

### 与 Kruskal 的复杂度对比

| 维度 | Prim (Lazy) | Kruskal |
|---|---|---|
| 时间 | $O(m \log m)$ | $O(m \log m)$ |
| 瓶颈 | 堆操作 $O(m \log m)$ | 排序 $O(m \log m)$ |
| 空间 | $O(n + m)$ | $O(n + m)$ |
| 稠密图 $m \approx n^2$ | $O(n^2 \log n)$ | $O(n^2 \log n)$ |
| 稀疏图 $m \approx n$ | $O(n \log n)$ | $O(n \log n)$ |

两者渐进复杂度相同，但常数因子不同：Prim 的堆操作通常比 Kruskal 的排序+并查集更快（因为堆可以利用局部性），但 Kruskal 的排序可以利用更高效的排序算法。

## 适用场景

### 适合使用 Prim

- **稠密图**：$m$ 接近 $n^2$ 时，Prim 的堆版本 $O(n^2 \log n)$ 与朴素 Prim $O(n^2)$ 接近，而实现更灵活
- **需要从特定起点扩展**：Prim 天然从指定节点出发，适合有起点要求的场景
- **边动态变化**：若图中边权可能动态更新，Prim 的堆结构比 Kruskal 的全量排序更易调整
- **在线算法需求**：Prim 可以边读入边处理（流式建图），而 Kruskal 需要预先读取所有边排序

### 不适合使用 Prim

- **图可能有重边且需要特殊处理**：Prim 自动取最小边（堆保证），但若需要保留所有重边信息则需额外处理
- **需要最小生成森林**（图不连通）：Prim 只能生成一棵树，需要对每个连通分量分别运行。Kruskal 天然支持森林。
- **$n$ 很大且图极度稀疏**：Eager Prim（索引堆）的 $O(m \log n)$ 更优

### Prim 邻接矩阵版（$O(n^2)$）

当图为稠密图（$m \approx n^2$）时，用邻接矩阵 + 线性扫描最小值的 Prim 可以达到 $O(n^2)$，优于堆优化版的 $O(n^2 \log n)$：

```cpp
for(int i = 1; i <= N; i++) { key[i] = INF; vis[i] = false; }
key[1] = 0;
for(int i = 1; i <= N; i++) {
    int u = -1, minw = INF;
    for(int j = 1; j <= N; j++)
        if(!vis[j] && key[j] < minw) { minw = key[j]; u = j; }
    vis[u] = true; ans += key[u];
    for(int v = 1; v <= N; v++)
        if(!vis[v] && G[u][v] < key[v]) key[v] = G[u][v];
}
```

## 常见陷阱与注意事项

1. **起点边权为 0**
   - `que.emplace(0, 1)` 将起点以权值 0 入队。第一次弹出时 `ans += 0`，不影响结果。
   - 若忘记设为 0 而设为 INF，起点无法被选中。
   - 若误设为非 0 值，`ans` 会多加这个值。

2. **cnt 计的是节点数而非边数**
   - 本代码 `cnt` 从 0 开始，每次选中节点 `cnt++`，终止条件是 `cnt == N`。
   - 与 Kruskal 的 `cnt == N - 1`（边数）不同，因为 Prim 的起点也是通过"虚拟边"加入的。
   - 混淆会导致少选/多选一条边。

3. **惰性删除（Lazy Deletion）**
   - 同一节点可能被多次入队。弹出时必须检查 `if(vis[nd]) continue`，否则会重复累加权值。
   - 这是 Lazy Prim 的核心特性，不能省略。

4. **重边处理**
   - 邻接表中可能存有多条 $u \to v$ 的边（不同权值）。全部入队后，堆自动保证最小权值的边先弹出。
   - 弹出最小边后 `vis[v] = true`，后续相同节点的更大边被惰性跳过。无需额外去重。

5. **自环处理**
   - 自环 $(u, u, w)$ 在扩展时会因为 `if(vis[nd_son]) continue`（此时 `nd_son == nd` 已被 `vis` 标记）被跳过。

6. **无向图建边**
   - 必须 `A[u].emplace_back(v, w)` 和 `A[v].emplace_back(u, w)` 双向建边。若只建单向，会导致部分节点无法到达，误判为不连通。

7. **INF 与 `long long`**
   - `ans` 用 `long long`，因为 $n - 1$ 条边的权值之和可能超过 `int` 范围。
   - 本代码无显式 INF，因为不维护 `key[]` 数组，直接用边的原始权值入队。

8. **堆中 pair 的排序**
   - `pair<long long, int>` 的 `greater` 比较：先比 first（边权），再比 second（节点编号）。
   - 边权相同时按节点编号排序，不影响正确性但影响选边的确定性。

## 对比与扩展

### Prim vs Kruskal 综合对比

| 对比维度 | Prim (堆优化 Lazy) | Kruskal |
|---|---|---|
| 策略 | 点贪心（从已选集合向外扩展最近点） | 边贪心（全局最小边优先） |
| 数据结构 | 优先队列（堆）+ 邻接表 | 并查集 + 边集数组 |
| 时间复杂度 | $O(m \log m)$ | $O(m \log m)$ |
| 稠密图 $m \approx n^2$ | $O(n^2 \log n)$，可用矩阵版降到 $O(n^2)$ | $O(n^2 \log n)$ |
| 稀疏图 $m \approx n$ | $O(n \log n)$ | $O(n \log n)$ |
| 空间复杂度 | $O(n + m)$（堆最大 $O(m)$） | $O(n + m)$ |
| 起点选择 | 任意起点（通常选 1） | 无起点概念 |
| 不连通图 | 只生成一棵树，需对每个分量重跑 | 天然生成最小生成森林 |
| 实现复杂度 | 中（堆 + 惰性删除） | 低（排序 + 并查集） |
| 适用场景 | 稠密图、有起点要求 | 稀疏图、需要森林 |

### 扩展：Eager Prim（索引堆优化）

Eager Prim 维护 `key[v]` 数组（每个未访问节点到已选集合的最短边权），用**索引堆**（Indexed Priority Queue）支持 decrease-key 操作。堆中始终只有 $O(n)$ 个元素，每次更新为 $O(\log n)$，总复杂度 $O(m \log n)$。但 C++ STL 的 `priority_queue` 不支持 decrease-key，需要手写索引堆或用 `std::set` 模拟。

### 扩展：最小生成森林

图不连通时，对每个未访问节点运行一次 Prim，可得到最小生成森林。等价于将终止条件从 `cnt == N` 改为"队列为空"，外层加一个遍历所有节点的循环。

### 扩展：Prim 求最大生成树

将小根堆改为大根堆（去掉 `greater<>` 或用 `less<>`），每次选最大边，即可求最大生成树。也可在 Kruskal 中将排序改为降序实现。

### 扩展：Boruvka 算法

Boruvka 是第三种经典 MST 算法，结合了 Kruskal 和 Prim 的特点：每轮每个连通分量同时选取各自的最小出边，然后合并。时间复杂度 $O(m \log n)$，适合并行化。可用于求解最小生成森林和某些离线问题。
