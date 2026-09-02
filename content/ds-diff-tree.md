---
id: ds-diff-tree
title: '树上点差分（路径点权修改）'
category: 数据结构
subcategory: 前缀和与差分
tags: ["数据结构", "差分", "树上", "LCA", "倍增", "路径修改", "子树和"]
timeComplexity: '修改O(log n)，还原O(n)'
spaceComplexity: 'O(n log n)'
codePath: '数据结构\前缀和与差分\树上点差分\Untitled1.cpp'
---

## 算法原理

树上点差分是一维差分在树结构上的推广。给定一棵以 $S$ 为根的有根树，每个节点有初始权值 $A[i]$。需要支持**路径修改**：对路径 $a \to b$ 上所有节点的权值加 $k$，最后统一查询每个节点的最终权值。

### 核心思想

一维差分通过"加左减右"实现区间修改，树上差分类似地利用 LCA 实现"路径修改"到"单点修改"的转化。关键公式：

对于路径 $a \to b$ 上所有节点加 $k$，设 $\text{lca} = \text{LCA}(a, b)$：

$$\text{diff}[a] \mathrel{+}= k, \qquad \text{diff}[b] \mathrel{+}= k, \qquad \text{diff}[\text{lca}] \mathrel{-}= k, \qquad \text{diff}[\text{fa}[\text{lca}]] \mathrel{-}= k$$

当 $\text{lca} = S$（即 LCA 为根节点）时，$\text{fa}[\text{lca}]$ 不存在，只需三处修改：

$$\text{diff}[a] \mathrel{+}= k, \qquad \text{diff}[b] \mathrel{+}= k, \qquad \text{diff}[\text{lca}] \mathrel{-}= k$$

### 为什么这样修改？

树上差分的还原方式是**DFS 求子树和**：每个节点的差分值向父节点累加。理解公式的关键在于分析每个节点被累加的次数：

- 节点 $a$ 和 $b$ 各自 $+k$：它们的祖先路径上的节点都会通过子树和获得 $+k$
- 但路径 $a \to b$ 经过 LCA，LCA 的子树同时包含 $a$ 和 $b$，因此 LCA 会被累加两次（$+2k$），需要减去一次（$-k$）
- LCA 的父节点同样通过两条子路径累加了两次，也需要减去一次（$-k$）
- 当 LCA 是根时，根没有父节点，只有根被多算了一次，减一次即可

### 还原操作

所有修改完成后，对差分数组做一次 DFS 子树和累加：

$$\text{diff}[u] \mathrel{+}= \sum_{v \in \text{children}(u)} \text{diff}[v]$$

最终每个节点的权值为 $\text{diff}[i] + A[i]$（差分累加值 + 初始权值）。

### 复杂度分析

| 操作 | 时间 | 说明 |
|---|---|---|
| DFS 预处理 | $O(n)$ | 求深度、父节点 |
| 倍增跳表初始化 | $O(n \log n)$ | $n$ 个节点，$\log n$ 层 |
| 单次路径修改 | $O(\log n)$ | LCA 查询 + $O(1)$ 差分 |
| DFS 还原子树和 | $O(n)$ | 每个节点访问一次 |
| 总体（$M$ 次修改） | $O(n \log n + M \log n)$ | 预处理 + $M$ 次修改 + 还原 |

## 数据结构图示

### 树结构示例

```
树结构 (根 S=1, 括号内为初始点权 A):
            1(A=10)
           /       \
        2(A=20)    3(A=30)
       /      \
    4(A=40)  5(A=50)

节点编号:  1    2    3    4    5
初始权值A: 10   20   30   40   50
```

### 差分修改过程

**操作 1：路径 4→5 上所有节点加 $k=100$**

```
路径 4→5: 4 → 2 → 5 (LCA(4,5) = 2, lca != S)

差分操作:
  diff[4] += 100      (端点 a)
  diff[5] += 100      (端点 b)
  diff[2] -= 100      (LCA 减一次)
  diff[fa[2]=1] -= 100 (LCA 父节点减一次)

diff 数组初始全 0:
  下标:   1     2     3     4     5
  diff:  -100  -100    0   +100  +100

图解 (虚线 = 路径 4→5):
            1(-100)
           /       \
        2(-100)    3(0)
       /      \
    4(+100)  5(+100)
```

### DFS 子树和还原

```
DFS 从根开始，子节点先递归，回溯时累加:

dfs_arr(4): 无子节点 → diff[4] = 100
dfs_arr(5): 无子节点 → diff[5] = 100
dfs_arr(2): diff[2] += diff[4] + diff[5]
           → diff[2] = -100 + 100 + 100 = 100
dfs_arr(3): 无子节点 → diff[3] = 0
dfs_arr(1): diff[1] += diff[2] + diff[3]
           → diff[1] = -100 + 100 + 0 = 0

还原后 diff (即每个节点的修改量):
  下标:    1    2    3    4    5
  diff:    0   100   0  100  100

最终权值 = A[i] + diff[i]:
  节点:   1      2       3      4       5
  最终权值: 10+0  20+100  30+0  40+100  50+100
         = 10   120    30    140     150

验证: 路径 4→2→5 的权值确实各加了 100 ✓
      节点 1, 3 不在路径上，权值不变 ✓
```

### LCA 为根的情况

**操作 2：路径 3→4 上所有节点加 $k=50$**

```
路径 3→4: 3 → 1 → 2 → 4 (LCA(3,4) = 1 = S)

差分操作 (lca = S, 只减一次):
  diff[3] += 50       (端点 a)
  diff[4] += 50       (端点 b)
  diff[1] -= 50       (LCA = 根，只减一次，不减 fa)

图解 (虚线 = 路径 3→1→2→4):
            1(-50)
           /       \
        2(0)     3(+50)
       /      \
    4(+50)    5(0)
```

### 倍增跳表结构

```
ST[i][k] 表示从节点 i 向上跳 2^k 步到达的祖先

ST[4][0] = fa[4] = 2     (跳 1 步)
ST[4][1] = ST[ST[4][0]][0] = ST[2][0] = 1  (跳 2 步)
ST[4][2] = ST[ST[4][1]][1] = ST[1][1] = S  (跳 4 步，超出根回到 S)

跳表:
节点  ST[][0]  ST[][1]  ST[][2]
 1      1        1        1    (根指向自身)
 2      1        1        1
 3      1        1        1
 4      2        1        1
 5      2        1        1
```

## 核心操作详解

### DFS 预处理（深度与父节点）

```cpp
int dfs(int rt, int rh)
{
    int ans = rh;
    h[rt] = rh;
    for (int i : edges[rt])
    {
        if (fa[i]) continue;
        fa[i] = rt;
        ans = max(dfs(i, rh + 1), ans);
    }
    return ans;
}
```

DFS 遍历树，计算每个节点的深度 `h[rt]` 和父节点 `fa[rt]`。返回最大深度用于确定倍增跳表的大小。与树上前缀和不同，此处的 DFS 不计算前缀和，只建立树结构信息。

### 倍增跳表初始化

```cpp
ST[i][0] = fa[i];                    // 跳 1 步 = 父节点
ST[i][k] = ST[ST[i][k-1]][k-1];      // 跳 2^k 步 = 跳 2^(k-1) 后再跳 2^(k-1)
```

倍增预处理：$ST[i][k]$ 表示从节点 $i$ 向上跳 $2^k$ 步到达的节点。根节点 `fa[S] = S`，跳表指向自身作为哨兵。

### LCA 查询

```cpp
int find_fa(int a, int b)
{
    if (h[a] < h[b]) swap(a, b);       // 保证 a 更深
    int tmp = h[a] - h[b];
    for (int i = 0; i <= sz; i++)      // 将 a 跳到与 b 同一深度
    {
        if (tmp & 1) a = ST[a][i];
        tmp >>= 1;
    }
    if (a == b) return a;              // b 是 a 的祖先

    for (int i = sz; i >= 0; i--)      // 两人同时向上跳
    {
        if (ST[a][i] != ST[b][i])
        {
            a = ST[a][i];
            b = ST[b][i];
        }
    }
    return fa[a];                      // 最终 a, b 的父节点就是 LCA
}
```

三步法：深度对齐（位运算逐位跳）→ 判断祖先关系 → 二分跳跃找 LCA。

### 路径差分修改

```cpp
void find_arr(int a, int b, long long k)
{
    int t_fa = find_fa(a, b);
    arr[a] += k;
    arr[b] += k;
    arr[t_fa] -= k;
    if (t_fa != S) arr[fa[t_fa]] -= k;
}
```

核心差分操作：端点 $a, b$ 各加 $k$，LCA 减 $k$，LCA 父节点减 $k$（当 LCA 不是根时）。当 LCA 是根时，`fa[S] = S`，如果也减去 `arr[fa[S]] = arr[S]` 会导致根被多减一次，因此必须特判。

### DFS 子树和还原

```cpp
void dfs_arr(int rt, int f)
{
    for (int i : edges[rt])
    {
        if (i == f) continue;
        dfs_arr(i, rt);
        arr[rt] += arr[i];
    }
}
```

后序 DFS：先递归子节点，回溯时将子节点的差分值累加到当前节点。这等价于对每个节点求子树和，将差分操作的效果传播到正确的路径上。

## 逐行代码解析

```cpp
#include<bits/stdc++.h>
using namespace std;
int N, M, S, sz;
vector<long long> A, arr;
vector<vector<int>> edges, ST;
vector<int> fa, h;
```

- `N`：节点数，`M`：修改次数，`S`：根节点，`sz`：倍增跳表最大层数
- `A`：初始权值，`arr`：差分数组
- `edges`：邻接表，`ST`：倍增跳表，`fa`：父节点数组，`h`：深度数组

```cpp
int dfs(int rt, int rh)
{
    int ans = rh;
    h[rt] = rh;
    for (int i : edges[rt])
    {
        if (fa[i]) continue;       // 已访问（fa[i] != 0）
        fa[i] = rt;
        ans = max(dfs(i, rh + 1), ans);
    }
    return ans;
}
```

DFS 遍历：用 `fa[i] != 0` 判断已访问，记录父节点和深度。返回最大深度用于计算跳表层数。

```cpp
void init()
{
    fa.resize(N + 1, 0);
    h.resize(N + 1, 0);
    arr.resize(N + 1, 0);
    fa[S] = S;                     // 根的父节点指向自身（哨兵）
    int q = dfs(S, 1);
    sz = 1;
    while (q > ((long long)1 << sz)) ++sz;   // 计算跳表层数
    ST.resize(N + 1, vector<int>(sz + 1, S));
    for (int i = 1; i <= N; i++) ST[i][0] = fa[i];
    for (int k = 1; k <= sz; k++)
        for (int i = 1; i <= N; i++)
            ST[i][k] = ST[ST[i][k - 1]][k - 1];
}
```

初始化：根节点 `fa[S] = S` 作为跳表终止哨兵，差分数组 `arr` 初始全 0。根据最大深度确定跳表层数 `sz`，然后倍增递推。

```cpp
void find_arr(int a, int b, long long k)
{
    int t_fa = find_fa(a, b);
    arr[a] += k;
    arr[b] += k;
    arr[t_fa] -= k;
    if (t_fa != S) arr[fa[t_fa]] -= k;   // LCA 不是根时才减父节点
}
```

差分修改：先求 LCA，再按公式修改四个位置。`if (t_fa != S)` 是关键特判——当 LCA 是根时，`fa[S] = S`，再减 `arr[fa[S]]` 会多减一次根。

```cpp
void dfs_arr(int rt, int f)
{
    for (int i : edges[rt])
    {
        if (i == f) continue;       // 跳过父节点（避免回退）
        dfs_arr(i, rt);
        arr[rt] += arr[i];          // 子树和累加
    }
}
```

注意此处用 `i == f` 而非 `fa[i]` 判断访问，因为 `dfs_arr` 需要支持双向遍历，传入父节点 `f` 更清晰。

```cpp
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    cin >> N >> M >> S;
    A.resize(N + 1);
    edges.resize(N + 1);
    for (int i = 1; i <= N; i++) cin >> A[i];
    for (int i = 1; i < N; i++)
    {
        int a, b;
        cin >> a >> b;
        edges[a].emplace_back(b);
        edges[b].emplace_back(a);
    }
    init();
    while (M--)
    {
        int a, b;
        long long k;
        cin >> a >> b >> k;
        find_arr(a, b, k);
    }
    dfs_arr(S, S);
    for (int i = 1; i <= N; i++) cout << arr[i] + A[i] << ' ';
    return 0;
}
```

读入树结构和初始权值，预处理后执行 $M$ 次路径修改，最后 DFS 还原子树和，输出 `arr[i] + A[i]`（差分累加值 + 初始权值）。

## 复杂度分析

### 时间复杂度

| 操作 | 复杂度 | 说明 |
|---|---|---|
| DFS 预处理 | $O(n)$ | 每个节点访问一次 |
| 倍增跳表初始化 | $O(n \log n)$ | $n$ 个节点，$\log n$ 层 |
| 单次路径修改 | $O(\log n)$ | LCA 查询 $O(\log n)$ + 差分 $O(1)$ |
| DFS 还原子树和 | $O(n)$ | 每个节点访问一次 |
| 总体（$M$ 次修改） | $O(n \log n + M \log n)$ | 预处理 + $M$ 次修改 + 还原 |

### 空间复杂度

| 数据 | 空间 | 说明 |
|---|---|---|
| 倍增跳表 `ST` | $O(n \log n)$ | $n \times (\log n + 1)$ 的二维数组 |
| 邻接表 `edges` | $O(n)$ | $n-1$ 条边 |
| 其他数组 | $O(n)$ | `A`, `arr`, `fa`, `h` |
| **总计** | $O(n \log n)$ | 倍增跳表占主导 |

## 适用场景

1. **树上路径批量修改**：对树上多条路径的所有节点加值，最后统一查询。例如统计每条边/点被多少条路径覆盖
2. **访问次数统计**：多次路径查询后统计每个节点被覆盖的次数（如运输路线经过的城市次数）
3. **树上染色问题**：对路径上所有节点染色，最后统计颜色分布
4. **结合树上前缀和**：树上差分修改 + 树上前缀和查询，实现"先修改后查询"的完整流程

**不适用场景**：
- 需要在线查询（边修改边查询）→ 应使用树链剖分 + 线段树/树状数组
- 仅需单次路径修改 → 直接 DFS 遍历路径即可，$O(n)$
- 需要路径修改 + 路径查询 → 树链剖分 + 线段树，$O(\log^2 n)$ 每次操作

## 常见陷阱与注意事项

1. **LCA 为根时的特判**
   - `fa[S] = S`（指向自身）作为倍增跳表的终止哨兵
   - 差分公式中必须特判 `lca == S`，否则 `arr[fa[S]] = arr[S]` 会导致根被多减一次 $k$
   - 正确做法：LCA 为根时只减 `arr[lca]` 一次，不减 `arr[fa[lca]]`

2. **差分数组类型用 `long long`**
   - 多次路径修改累加可能超过 `int` 范围
   - `arr` 数组和修改值 `k` 都必须声明为 `long long`

3. **DFS 还原时区分访问方式**
   - 预处理 DFS 用 `fa[i] != 0` 判断访问（因为 `fa` 数组在 init 中初始化）
   - 还原 DFS 用 `i == f` 判断父节点（避免与 `fa` 数组混淆）
   - 两种 DFS 的访问判断方式不同，不可混用

4. **倍增跳表层数计算**
   - `sz` 必须满足 $2^{\text{sz}} \ge \text{maxDepth}$，否则跳跃不足
   - 代码中 `while (q > (1LL << sz)) ++sz` 正确处理了这一点

5. **最终输出需加初始权值**
   - 差分数组只记录修改量，最终权值 = `arr[i] + A[i]`
   - 容易忘记加初始权值 `A[i]`，导致输出错误

6. **无向边需双向建图**
   - `edges[a].push_back(b)` 和 `edges[b].push_back(a)` 都要写
   - 还原 DFS 从根开始，必须确保树结构正确

## 对比与扩展

| 方法 | 修改 | 还原 | 查询 | 特点 |
|---|---|---|---|---|
| **树上点差分（本算法）** | $O(\log n)$ | $O(n)$ | 不支持在线 | 离线批量修改，实现简洁 |
| 树链剖分 + 线段树 | $O(\log^2 n)$ | — | $O(\log^2 n)$ | 在线修改 + 在线查询 |
| 树上边差分 | $O(\log n)$ | $O(n)$ | 不支持在线 | 差分对象为边权，还原方式不同 |

### 树上点差分 vs 树上边差分

| 对比项 | 点差分 | 边差分 |
|---|---|---|
| 修改对象 | 路径上所有**节点**权值 | 路径上所有**边**权值 |
| 差分公式 | $+a, +b, -\text{lca}, -\text{fa}[\text{lca}]$ | $+a, +b, -2 \cdot \text{lca}$ |
| 还原方式 | DFS 子树和，`diff[u] += diff[children]` | DFS 子树和，`diff[u]` 代表 $u \to \text{fa}[u]$ 的边 |
| LCA 处理 | LCA 点权需修改，需特判根 | LCA 不在路径边中，无需特判根 |

### 扩展

- **树上边差分**：将边权下放为子节点点权（每条边赋给深度较深的端点），路径修改公式变为 $+a, +b, -2 \cdot \text{lca}$，还原后 `diff[u]` 代表边 $u \to \text{fa}[u]$ 的修改量
- **树链剖分**：支持在线路径修改 + 路径查询，时间 $O(\log^2 n)$ 每次操作，适合需要实时查询的场景
- **LCT（Link-Cut Tree）**：支持动态加边删边的路径修改与查询，适合动态树问题
