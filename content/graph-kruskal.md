---
id: graph-kruskal
title: 'Kruskal 最小生成树（并查集+贪心）'
category: 图论
subcategory: 最小生成树
tags: ["图论", "最小生成树", "Kruskal", "并查集", "贪心", "MST", "边集排序", "路径压缩"]
timeComplexity: 'O(m log m)'
spaceComplexity: 'O(n + m)'
codePath: '图论\最小生成树\Kruskal\Untitled1.cpp'
---

## 算法原理

Kruskal 算法解决**最小生成树**（Minimum Spanning Tree, MST）问题：给定一个含 $n$ 个节点、$m$ 条边的**连通无向带权图**，求一棵边权和最小的生成树。生成树是原图的一个极小连通子图，包含所有 $n$ 个节点和恰好 $n - 1$ 条边，且无环。

### 贪心核心思想

Kruskal 采用**边贪心**策略：将所有边按权值从小到大排序，依次尝试加入每条边。若加入该边不会形成环（即边的两端点不在同一连通分量），则选中该边；否则跳过。当恰好选中 $n - 1$ 条边时，最小生成树构造完毕。

正确性基于**贪心选择性质**和**最优子结构性质**的证明：

- **贪心选择性质**：设 $T$ 是一棵 MST，$e$ 是图中权值最小的边。若 $e \notin T$，则 $T$ 中存在一条路径连接 $e$ 的两端点，该路径上必有一条边 $e'$ 满足 $w(e') \ge w(e)$（否则 $e$ 不是最小的）。将 $e'$ 替换为 $e$ 得到的新树 $T'$ 仍然是一棵生成树且权值不增，故存在一棵包含 $e$ 的 MST。
- **最优子结构**：选中边 $e$ 后，将 $e$ 的两端点合并为一个超级节点，问题规模缩小为 $n-1$ 个节点的 MST 问题，递归求解即可。

### 算法流程

1. 将所有 $m$ 条边按权值升序排序
2. 初始化并查集：每个节点自成一个集合
3. 依次遍历排序后的边 $(u, v, w)$：
   - 若 `find(u) != find(v)`（$u$ 和 $v$ 不在同一集合），选中该边：执行 `unity(u, v)`，累加权值
   - 否则跳过（该边会形成环）
4. 当选中边数 `cnt == n - 1` 时提前终止
5. 若最终 `cnt < n - 1`，说明图不连通，不存在生成树

### 与 Prim 算法的关系

Kruskal 和 Prim 都是经典 MST 算法，但策略不同：

| 特性 | Kruskal | Prim |
|---|---|---|
| 贪心对象 | 边（全局最小边优先） | 点（从已选集合向外扩展最近点） |
| 数据结构 | 并查集 | 优先队列（堆） |
| 适合图类型 | 稀疏图（$m$ 较小） | 稠密图（$m$ 接近 $n^2$） |
| 时间复杂度 | $O(m \log m)$ | $O(m \log n)$（堆优化） |
| 实现方式 | 边集数组排序 | 类 Dijkstra 扩展 |

## 核心公式/状态定义与转移方程

### 并查集状态定义

$$\text{fa}[i] = \text{节点 } i \text{ 的父节点（路径压缩后指向根节点）}$$

$$\text{find}(i) = \begin{cases} i & \text{若 } \text{fa}[i] = i \\ \text{find}(\text{fa}[i]) & \text{否则} \end{cases}$$

路径压缩：在 `find` 过程中将路径上所有节点直接指向根：

$$\text{fa}[i] \leftarrow \text{find}(\text{fa}[i])$$

### 合并操作

$$\text{unity}(a, b): \quad \text{fa}[\text{find}(b)] \leftarrow \text{find}(a)$$

将 $b$ 所在树的根挂到 $a$ 所在树的根下。本代码**未使用按秩合并**，仅靠路径压缩保证性能。

### MST 选边条件

对排序后的边 $(u_i, v_i, w_i)$，选中条件：

$$\text{find}(u_i) \neq \text{find}(v_i) \implies \text{选中，执行 } \text{unity}(u_i, v_i),\; \text{ans} \leftarrow \text{ans} + w_i,\; \text{cnt} \leftarrow \text{cnt} + 1$$

### 终止与判定

$$\text{cnt} = n - 1 \implies \text{MST 构造完成，总权值 = ans}$$

$$\text{遍历完所有边后 cnt} < n - 1 \implies \text{图不连通，无生成树}$$

### 路径压缩的均摊复杂度

仅路径压缩（无按秩合并）的并查集，单次 `find` 的均摊时间复杂度为 $O(\log n)$。结合按秩合并可降至 $O(\alpha(n))$（阿克曼反函数，对于所有实际 $n$ 不超过 5）。但实践中路径压缩已足够快，且实现更简单。

## 逐行代码解析

### 第 1 段：边结构体与比较器

```cpp
struct edge
{
    int u, v;
    long long w;
    edge(){}
};
struct cmp
{
    bool operator() (const edge& a, const edge& b)
    {
        return a.w < b.w;
    }
};
```

- **`struct edge`**：存储边的起点 $u$、终点 $v$、权值 $w$。`edge(){}` 是默认构造函数，用于 `vector<edge>` 初始化。
- **`struct cmp`**：函数对象（functor），重载 `operator()` 实现按 `w` 升序排列。也可用 lambda 替代：`sort(edges.begin()+1, edges.end(), [](const edge& a, const edge& b){ return a.w < b.w; });`。
- **`long long w`**：权值用 `long long` 防止溢出。若权值范围在 `int` 内也可用 `int`。

### 第 2 段：并查集初始化

```cpp
int N, M;
vector<int> A;
void init()
{
    A.resize(N + 1, 0);
    for(int i = 1; i <= N; i++) A[i] = i;
}
```

- **`A`**：并查集父节点数组，`A[i]` 表示节点 $i$ 的父节点。
- **`init()`**：每个节点的父初始化为自身，表示各自独立成集合。
- **下标从 1 开始**：节点编号 $1 \sim N$，与题目输入一致。

### 第 3 段：查找（带路径压缩）

```cpp
int find(int i)
{
    if(A[i] != i) A[i] = find(A[i]);
    return A[i];
}
```

- **递归路径压缩**：若 $i$ 不是根（`A[i] != i`），递归查找根并将 $A[i]$ 直接指向根。
- **递归 vs 迭代**：递归写法简洁，但深层链可能导致栈溢出（$n$ 很大时）。迭代写法更安全：
  ```cpp
  int find(int i) {
      int root = i;
      while(A[root] != root) root = A[root];
      while(A[i] != root) { int next = A[i]; A[i] = root; i = next; }
      return root;
  }
  ```

### 第 4 段：合并

```cpp
void unity(int a, int b)
{
    A[find(b)] = find(a);
}
```

- **合并方向**：将 $b$ 的根挂到 $a$ 的根下。方向是任意的（不影响正确性），但**按秩合并**时方向影响树高。
- **无按秩合并**：本代码不做秩优化，仅靠路径压缩。最坏情况下（构造特殊数据），单次 `find` 可能退化为 $O(n)$，但实际中均摊表现良好。

### 第 5 段：主函数——输入与排序

```cpp
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    cin >> N >> M;
    init();
    vector<edge> edges(M + 1);
    for(int i = 1; i <= M; i++) cin >> edges[i].u >> edges[i].v >> edges[i].w;
    sort(edges.begin() + 1, edges.end(), cmp());
```

- **快读**：`ios::sync_with_stdio(false)` + `cin.tie(nullptr)` 加速输入输出。
- **边集数组**：`edges` 下标从 1 开始，`edges[0]` 是默认构造的空边（被排序跳过）。
- **`sort`**：$O(m \log m)$ 排序，使用 `cmp()` 仿函数对象（注意 `cmp()` 带括号创建临时对象）。

### 第 6 段：贪心选边（核心）

```cpp
    int cnt = 0;
    long long ans = 0;
    for(int i = 1; i <= M; i++)
    {
        int u = edges[i].u;
        int v = edges[i].v;

        if(find(u) != find(v))
        {
            unity(u, v);
            ans += edges[i].w;
            ++cnt;
        }

        if(cnt == N - 1) break;
    }
```

- **`cnt`**：已选中的边数，目标为 $n - 1$。
- **`ans`**：MST 总权值，用 `long long` 防止溢出。
- **选边判断**：`find(u) != find(v)` 检查两端点是否属于不同连通分量。若不同则安全加入（不会成环）。
- **提前终止**：`cnt == N - 1` 时 break，避免遍历剩余边。

### 第 7 段：输出结果

```cpp
    if(cnt == N - 1) cout << ans;
    else cout << "orz";
    return 0;
}
```

- **连通判定**：若选够 $n - 1$ 条边，输出总权值；否则图不连通，输出 `"orz"`（题目约定，表示无解）。
- **正确性**：$n$ 个节点的连通无向图的生成树恰好有 $n - 1$ 条边。若选不够说明图不连通。

## 复杂度分析

### 时间复杂度

| 步骤 | 复杂度 |
|---|---|
| 输入读入 | $O(m)$ |
| 并查集初始化 | $O(n)$ |
| 边排序 | $O(m \log m)$ |
| 贪心选边（$m$ 次 find + 最多 $n-1$ 次 unity） | $O(m \cdot \alpha(n)) \approx O(m)$ |
| **总计** | $O(m \log m)$ |

瓶颈在排序。由于 $m \le n^2$，$m \log m \le n^2 \log n$，对稀疏图（$m \approx n$）为 $O(n \log n)$。

### 空间复杂度

| 数据结构 | 空间 |
|---|---|
| 并查集数组 `A` | $O(n)$ |
| 边集数组 `edges` | $O(m)$ |
| **总计** | $O(n + m)$ |

### 路径压缩的性能分析

仅路径压缩（无按秩合并）的并查集，单次操作的均摊复杂度为 $O(\log n)$。虽然理论最坏复杂度不如按秩合并的 $O(\alpha(n))$，但实际差距很小，且实现更简洁。竞赛中通常足够。

## 适用场景

### 适合使用 Kruskal

- **稀疏图的最小生成树**：$m$ 较小时排序开销小，$O(m \log m)$ 优于 Prim 的 $O(n^2)$
- **需要按边权处理**：Kruskal 的边集排序后可复用于其他贪心/分治问题
- **判断图连通性**：若最终 `cnt < n - 1` 则图不连通
- **最小生成森林**：图不连通时，Kruskal 自然生成最小生成森林（每个连通分量一棵树）
- **次小生成树**：基于 Kruskal 的 MST，枚举非树边替换树边
- **动态加边问题**：并查集天然支持动态合并

### 不适合使用 Kruskal

- **稠密图**（$m \approx n^2$）：排序 $O(n^2 \log n)$ 逊于 Prim 的 $O(n^2)$
- **需要动态维护 MST**：边权变化时，Prim 的堆结构更易维护
- **需要最大生成树**：将排序改为降序即可，但需要明确需求

## 常见陷阱与注意事项

1. **重边处理**
   - 本代码**未做重边去重**，但 Kruskal 天然处理重边：排序后权值相同的多条边按序尝试，只有第一条能连接两个不同分量的边被选中，后续重边因 `find(u) == find(v)` 被跳过。
   - 若需去重可额外排序+unique，但通常无必要。

2. **自环处理**
   - 自环（$u = v$）会被 `find(u) == find(v)` 自动跳过，不影响结果。

3. **无向图 vs 有向图**
   - MST 定义在无向图上。本代码的 `edge` 只存 `(u, v, w)` 不区分方向，因为并查集只关心连通性。
   - 若题目给有向图求"最小树形图"，需要用**朱刘算法**（Edmonds 算法），而非 Kruskal。

4. **并查集合并方向**
   - `unity(a, b)` 将 $b$ 的根挂到 $a$ 的根下。方向任意不影响正确性，但若配合按秩合并，应将小树挂到大树下。

5. **未用按秩合并的退化风险**
   - 仅路径压缩在最坏情况下可能退化为链状，单次 `find` 为 $O(n)$。构造性攻击数据可使性能下降。
   - 竞赛中一般无妨，但若题目故意构造数据，建议加入按秩合并：
     ```cpp
     int rank_[N+1]; // 初始化为0
     void unity(int a, int b) {
         int ra = find(a), rb = find(b);
         if(ra == rb) return;
         if(rank_[ra] < rank_[rb]) swap(ra, rb);
         A[rb] = ra;
         if(rank_[ra] == rank_[rb]) rank_[ra]++;
     }
     ```

6. **INF 与溢出**
   - `ans` 用 `long long` 防止 $n - 1$ 条边权值累加溢出。若边权为 `int` 且 $n$ 较小，用 `int` 也可，但 `long long` 更安全。

7. **边集下标从 1 开始**
   - `edges` 从下标 1 开始存储，`edges[0]` 为默认空边。排序时 `sort(edges.begin() + 1, edges.end(), ...)` 跳过下标 0。
   - 若改为从 0 开始，需调整排序范围和循环边界。

8. **提前终止优化**
   - `if(cnt == N - 1) break;` 在选够 $n-1$ 条边后立即终止，避免遍历剩余边。这对 $m$ 远大于 $n$ 的图有显著优化。

## 对比与扩展

### Kruskal vs Prim

| 对比维度 | Kruskal | Prim |
|---|---|---|
| 策略 | 边贪心（全局最小边优先） | 点贪心（从已选集合向外扩展） |
| 数据结构 | 并查集 + 边集排序 | 优先队列 / 邻接矩阵 |
| 时间复杂度 | $O(m \log m)$ | $O(m \log n)$（堆）/ $O(n^2)$（矩阵） |
| 稀疏图 | $O(n \log n)$ ✅ | $O(n \log n)$ |
| 稠密图 | $O(n^2 \log n)$ | $O(n^2)$ ✅ |
| 实现复杂度 | 低（排序 + 并查集） | 中（堆 + lazy 删除） |
| 支持最大生成树 | 改排序方向即可 | 改比较方向即可 |
| 动态维护 | 难（需重排序） | 较易（堆更新） |

### 扩展：次小生成树

基于 Kruskal 求出的 MST，枚举每条非树边 $e$，加入 $e$ 后会在树中形成一个环，删除环上权值最大的边 $e'$，得到新树。若 $w(e) - w(e')$ 最小，则为次小生成树。可用 LCA + 倍增预处理每条路径上的最大边权，总复杂度 $O(m \log n)$。

### 扩展：最小生成森林

当图不连通时，Kruskal 自然产生最小生成森林（每个连通分量一棵最小生成树）。只需将终止条件从 `cnt == N - 1` 改为遍历完所有边，最终 `cnt` 即为森林中的总边数。

### 扩展：Kruskal 重构树

Kruskal 重构树是一种将 Kruskal 算法过程转化为树形结构的技术。按边权升序处理，每次合并两个连通分量时新建一个虚拟节点，权值为当前边权，将两个分量的根作为虚拟节点的左右子。重构树有以下性质：

- 是一棵二叉树，叶节点为原图节点，内部节点对应 Kruskal 选中的边
- 原图中两点间路径上最大边权 = 重构树上 LCA 的权值
- 可用于解决"路径上最大边权最小"等问题
