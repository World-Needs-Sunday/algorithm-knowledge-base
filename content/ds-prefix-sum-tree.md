---
id: ds-prefix-sum-tree
title: '树上前缀和（路径点权和查询）'
category: 数据结构
subcategory: 前缀和与差分
subSubcategory: 树上
tags: ["数据结构", "前缀和", "树上", "LCA", "倍增", "路径查询"]
timeComplexity: '预处理O(n log n)，查询O(log n)'
spaceComplexity: 'O(n log n)'
codePath: '数据结构\前缀和与差分\树上前缀和\Untitled1.cpp'
---

## 算法原理

树上前缀和是一维前缀和在树结构上的推广。给定一棵以 $S$ 为根的有根树，每个节点有权值 $A[i]$，定义树上前缀和：

$$\text{sum}[u] = \sum_{v \in \text{path}(S \to u)} A[v]$$

即从根 $S$ 到节点 $u$ 的路径上所有节点权值之和。预处理后，可以在 $O(\log n)$ 时间内回答任意两点路径上的权值之和。

路径和查询的关键在于 LCA（最近公共祖先）。对于查询 $(a, b)$，设 $\text{lca} = \text{LCA}(a, b)$，则：

- **一般情况**（$\text{lca} \ne S$）：路径 $a \to b$ 的权值和为 $\text{sum}[a] + \text{sum}[b] - \text{sum}[\text{lca}] - \text{sum}[\text{fa}[\text{lca}]]$，因为两条根到节点的路径在 LCA 及其父节点处重叠。
- **LCA 为根**（$\text{lca} = S$）：路径 $a \to b$ 的权值和为 $\text{sum}[a] + \text{sum}[b] - \text{sum}[\text{lca}]$，因为根没有父节点，只需减去一次根的权值。

本代码同时实现了倍增 LCA（Binary Lifting），通过预处理 $O(n \log n)$ 的跳表实现 $O(\log n)$ 的 LCA 查询。

## 数据结构图示

### 树上前缀和的构造

```
        树结构 (根 S=1, 括号内为点权):
                    1(A=10)
                   /       \
                2(A=20)    3(A=30)
               /      \
            4(A=40)  5(A=50)

        DFS 从根开始，sum[i] = sum[fa] + A[i]:

                    1(sum=10)
                   /       \
                2(sum=30)  3(sum=40)
               /      \
            4(sum=70)  5(sum=80)

        前缀和数组:
        下标:   1    2    3    4    5
        sum:   10   30   40   70   80
```

### 路径和查询示意

```
查询 find_sum(4, 5):
  LCA(4, 5) = 2, lca != S(=1)

  sum[4] = 70  (1→2→4: 10+20+40)
  sum[5] = 80  (1→2→5: 10+20+50)
  sum[lca=2] = 30
  sum[fa[lca]=1] = 10

  路径 4→5 的权值和 = 70 + 80 - 30 - 10 = 110
  验证: 40+20+50 = 110 ✓

  图解:
  1(10) ─ 2(20) ─ 4(40)
           └──── 5(50)

  sum[4] 包含: 10+20+40     ──┐
  sum[5] 包含: 10+20+50     ──┤
  减去 sum[2]: 10+20        ──┤  被加了两次
  减去 sum[1]: 10           ──┘  也被加了两次
  剩余: 40 + 50 + 20 = 110 ✓
```

```
查询 find_sum(3, 4):
  LCA(3, 4) = 1 = S

  sum[3] = 40  (1→3: 10+30)
  sum[4] = 70  (1→2→4: 10+20+40)
  sum[lca=1] = 10

  路径 3→4 的权值和 = 40 + 70 - 10 = 100
  验证: 30+10+20+40 = 100 ✓

  图解:
  3(30) ─ 1(10) ─ 2(20) ─ 4(40)

  sum[3] 包含: 10+30          ──┐
  sum[4] 包含: 10+20+40       ──┤
  减去 sum[1]: 10             ──┘  被加了两次
  剩余: 30 + 20 + 40 + 10 = 100 ✓
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

### DFS 预处理前缀和

```cpp
int dfs(int rt, int rh)
{
    int ans = rh;
    h[rt] = rh;
    for (int i : edges[rt])
    {
        if (fa[i]) continue;
        fa[i] = rt;
        sum[i] = sum[rt] + A[i];
        ans = max(dfs(i, rh + 1), ans);
    }
    return ans;
}
```

DFS 遍历树，计算每个节点的深度 `h[rt]`、父节点 `fa[rt]` 和前缀和 `sum[rt]`。返回最大深度用于确定倍增跳表的大小。

### 倍增跳表初始化

```cpp
ST[i][0] = fa[i];                    // 跳 1 步 = 父节点
ST[i][k] = ST[ST[i][k-1]][k-1];      // 跳 2^k 步 = 跳 2^(k-1) 后再跳 2^(k-1)
```

倍增预处理：$ST[i][k]$ 表示从节点 $i$ 向上跳 $2^k$ 步到达的节点。

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

三步法：深度对齐 → 判断祖先关系 → 二分跳跃找 LCA。

### 路径和查询

```cpp
long long find_sum(int a, int b)
{
    int g_fa = find_fa(a, b);
    if (g_fa != S)
        return sum[a] + sum[b] - sum[g_fa] - sum[fa[g_fa]];
    return sum[a] + sum[b] - sum[g_fa];
}
```

当 LCA 不是根时，需要额外减去 `sum[fa[lca]]`，因为 `sum[a]` 和 `sum[b]` 都包含了从根到 LCA 的路径（含 LCA 及其所有祖先），减去 `sum[lca]` 消除 LCA 以下的重叠，再减去 `sum[fa[lca]]` 消除 LCA 以上根路径的重复。当 LCA 是根时，根的父节点指向自身（`fa[S] = S`），如果也减去 `sum[fa[S]] = sum[S]` 会多减一次根权值，因此需特判。

## 逐行代码解析

以下按代码逻辑阶段拆成 6 个模块：全局变量 → `dfs` 预处理 → `init` 跳表 → `find_fa`(LCA) → `find_sum`(路径和) → `main`(读入与查询)。各模块代码块按顺序拼接即完整 `.cpp`。

### 全局变量与头文件

```cpp
#include<bits/stdc++.h>
using namespace std;
int N, M, S, sz;                 // N:节点数 M:查询数 S:根节点 sz:跳表最大层数
vector<long long> A, sum;        // A:点权 sum:树上前缀和（long long 防溢出）
vector<vector<int>> edges, ST;   // edges:邻接表 ST:倍增跳表
vector<int> fa, h;               // fa:父节点 h:深度
```

- `N`：节点数，`M`：查询数，`S`：根节点，`sz`：倍增跳表最大层数。
- `A` 存点权、`sum` 存树上前缀和，两者都用 `long long`，路径点权和可能超过 `int` 上限。
- `ST[i][k]` 表示从节点 `i` 向上跳 $2^k$ 步到达的祖先；`fa`、`h` 分别存父节点与深度。

### dfs：预处理前缀和、深度与父节点

```cpp
int dfs(int rt, int rh)
{
    int ans = rh;
    h[rt] = rh;                        // 记录当前节点深度
    for (int i : edges[rt])
    {
        if (fa[i]) continue;           // 已访问（fa[i] != 0），跳过父节点回边
        fa[i] = rt;                    // 记录父节点
        sum[i] = sum[rt] + A[i];       // 前缀和 = 父节点前缀和 + 当前点权
        ans = max(dfs(i, rh + 1), ans); // 递归子树，取最大深度
    }
    return ans;
}
```

- 无向树用 `fa[i] != 0` 判断是否已访问，避免沿父边回走（隐含节点从 1 开始编号）。
- 递归时先记父节点再算 `sum[i] = sum[rt] + A[i]`，保证前缀和自上而下累加。
- 返回值 `ans` 是整棵树的最大深度，供 `init()` 计算跳表层数 `sz`。

### init：初始化数组与倍增跳表

```cpp
void init()
{
    fa.resize(N + 1, 0);
    h.resize(N + 1, 0);
    sum.resize(N + 1, 0);
    fa[S] = S, sum[S] = A[S];                  // 根父节点指向自身，根前缀和 = 自身点权
    int q = dfs(S, 1);                         // 从根 DFS，返回最大深度
    sz = 1;
    while (q > ((long long)1 << sz)) ++sz;     // 求最小 sz 使 2^sz >= q
    ST.resize(N + 1, vector<int>(sz + 1, S));  // 跳表先全部初始化为 S（越界回根）
    for (int i = 1; i <= N; i++) ST[i][0] = fa[i]; // 跳 1 步 = 父节点
    for (int k = 1; k <= sz; k++)
    {
        for (int i = 1; i <= N; i++)
        {
            ST[i][k] = ST[ST[i][k - 1]][k - 1]; // 跳 2^k = 跳两次 2^(k-1)
        }
    }
}
```

- `fa[S] = S` 是跳表的终止哨兵：根再往上跳仍停在根，避免越界。
- 先填 `ST[i][0] = fa[i]`，再用 `ST[i][k] = ST[ST[i][k-1]][k-1]` 递推整张跳表。
- `while (q > (1LL << sz)) ++sz` 保证 $2^{\text{sz}} \ge \text{maxDepth}$，深度对齐时跳跃步数才够用。

### find_fa：倍增求 LCA

```cpp
int find_fa(int a, int b)
{
    if (h[a] < h[b]) swap(a, b);   // 保证 a 更深
    int tmp = h[a] - h[b];         // 深度差
    for (int i = 0; i <= sz; i++)  // 按二进制拆深度差，把 a 抬到与 b 同层
    {
        if (tmp & 1) a = ST[a][i];
        tmp >>= 1;
    }
    if (a == b) return a;          // b 是 a 的祖先（或 a == b）

    for (int i = sz; i >= 0; i--)  // 从高到低同时上跳，避免越过 LCA
    {
        if (ST[a][i] != ST[b][i])
        {
            a = ST[a][i];
            b = ST[b][i];
        }
    }
    return fa[a];                  // 最终 a、b 的父节点就是 LCA
}
```

- 深度对齐循环 `i <= sz`：用 `tmp & 1` 判断第 $i$ 位是否为 1，每轮 `tmp >>= 1`，逐位跳跃。
- 第二个循环从 `sz` 递减：仅当 `ST[a][i] != ST[b][i]` 才一起跳，保证不会越过 LCA。
- 循环结束后 `a`、`b` 停在 LCA 的两个子节点处，返回 `fa[a]` 即 LCA。

### find_sum：路径点权和查询

```cpp
long long find_sum(int a, int b)
{
    int g_fa = find_fa(a, b);       // LCA
    if (g_fa != S) return sum[a] + sum[b] - sum[g_fa] - sum[fa[g_fa]];
    return sum[a] + sum[b] - sum[g_fa]; // LCA 为根：只减一次 sum[lca]
}
```

- LCA 非根：`sum[a] + sum[b] - sum[g_fa] - sum[fa[g_fa]]`，去掉两条根路径在 LCA 及其祖先处的重复累加。
- LCA 为根：只减 `sum[g_fa]`；因 `fa[S] = S`，若再减 `sum[fa[S]]` 会多减一次根权值。
- 返回类型与 `sum` 一致为 `long long`，避免中间结果溢出。

### main：读入、预处理与回答查询

```cpp
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    //N:点的数量,M:测试案例数量,S:根节点
    cin >> N >> M >> S;
    A.resize(N + 1);
    edges.resize(N + 1);
    for (int i = 1; i <= N; i++) cin >> A[i]; // 读入点权
    for (int i = 1; i < N; i++)               // 树有 N-1 条边
    {
        int a, b;
        cin >> a >> b;
        edges[a].emplace_back(b);             // 无向边，双向建图
        edges[b].emplace_back(a);
    }
    init();                                    // 预处理前缀和 + 跳表

    while (M--)                                // 回答 M 次查询
    {
        int a, b;
        cin >> a >> b;
        cout << find_sum(a, b) << '\n';
    }
    return 0;
}
```

- 读入点权后读 $N-1$ 条无向边，双向加入邻接表。
- `init()` 完成 DFS 与跳表预处理，随后循环回答 $M$ 次路径和查询。
- 关闭 C/C++ 输入输出同步并解除 `cin`/`cout` 绑定，加速大数据读写。

## 复杂度分析

### 时间复杂度

| 操作 | 复杂度 | 说明 |
|---|---|---|
| DFS 预处理 | $O(n)$ | 每个节点访问一次 |
| 倍增跳表初始化 | $O(n \log n)$ | $n$ 个节点，$\log n$ 层 |
| 单次 LCA 查询 | $O(\log n)$ | 深度对齐 + 二分跳跃 |
| 单次路径和查询 | $O(\log n)$ | LCA 查询 + $O(1)$ 计算 |
| 总体（$M$ 次查询） | $O(n \log n + M \log n)$ | 预处理 + $M$ 次查询 |

### 空间复杂度

| 数据 | 空间 | 说明 |
|---|---|---|
| 倍增跳表 `ST` | $O(n \log n)$ | $n \times (\log n + 1)$ 的二维数组 |
| 邻接表 `edges` | $O(n)$ | $n-1$ 条边 |
| 其他数组 | $O(n)$ | `A`, `sum`, `fa`, `h` |
| **总计** | $O(n \log n)$ | 倍增跳表占主导 |

## 适用场景

1. **树上路径权值和查询**：静态树，多次查询两点间路径上所有节点权值之和
2. **树上距离查询**：若边权代替点权，可查询两点间路径的边权之和
3. **子树和查询**：通过 DFS 序 + 前缀和可扩展到子树权值和
4. **结合差分做树上路径修改**：树上差分 + LCA 可实现路径加 + 单点查询

**不适用场景**：
- 需要动态修改点权 → 应使用树链剖分 + 线段树/树状数组
- 仅需查询子树和 → DFS 序 + 一维前缀和更简单，$O(1)$ 查询
- 树结构频繁变化（加边/删边）→ LCT（Link-Cut Tree）

## 常见陷阱与注意事项

1. **根节点的父节点处理**
   - `fa[S] = S`（指向自身）作为倍增跳表的终止哨兵
   - 路径和公式中必须特判 `lca == S`，否则 `sum[fa[S]] = sum[S]` 会多减一次根权值

2. **前缀和类型用 `long long`**
   - 树上路径可能很长，权值累加可能超过 `int` 范围
   - `sum` 数组必须声明为 `long long`

3. **无向边需双向建图**
   - `edges[a].push_back(b)` 和 `edges[b].push_back(a)` 都要写
   - DFS 用 `fa[i] != 0` 判断已访问，避免重复遍历

4. **倍增跳表层数计算**
   - `sz` 必须满足 $2^{\text{sz}} \ge \text{maxDepth}$，否则跳跃不足
   - 代码中 `while (q > (1LL << sz)) ++sz` 正确处理了这一点

5. **深度对齐时的位运算**
   - `for (int i = 0; i <= sz; i++)` 中用 `tmp & 1` 判断是否需要跳 $2^i$ 步
   - 每次检查后 `tmp >>= 1`，注意循环范围必须覆盖 `sz`

6. **LCA 查询的边界情况**
   - $a = b$ 时直接返回 $a$（深度对齐后 `a == b`）
   - $a$ 是 $b$ 的祖先时，深度对齐后 `a == b` 也正确返回

7. **链状树递归 DFS 易栈溢出**
   - 退化成一条链的树（深度 $n$）递归 DFS 可能爆栈，需改用迭代 DFS 或增大栈空间

## 对比与扩展

| 方法 | 预处理 | 查询 | 空间 | 特点 |
|---|---|---|---|---|
| **倍增 LCA（本算法）** | $O(n \log n)$ | $O(\log n)$ | $O(n \log n)$ | 在线查询，实现简洁 |
| Tarjan LCA | $O(n + M \cdot \alpha)$ | 离线 | $O(n)$ | 离线查询，理论更快 |
| 重链剖分 LCA | $O(n)$ | $O(\log n)$ | $O(n)$ | 可结合线段树做路径修改 |
| 欧拉序 + ST 表 | $O(n \log n)$ | $O(1)$ | $O(n \log n)$ | $O(1)$ 查询，常数较大 |

### 扩展

- **树上前缀和 + 树上差分**：路径加操作 $(a, b, w)$ 可转化为 `diff[a] += w, diff[b] += w, diff[lca] -= w, diff[fa[lca]] -= w`，最后 DFS 求子树和还原
- **边权版本**：将边权下放为子节点点权，路径和公式相同；注意 LCA 处的点权不属于路径
- **树链剖分**：支持动态修改点权的路径和查询，时间 $O(\log^2 n)$ 每次操作
