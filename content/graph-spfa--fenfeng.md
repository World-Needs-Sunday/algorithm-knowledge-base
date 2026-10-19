---
id: graph-spfa
title: 'Bellman-Ford 批量松弛优化（类SPFA变体）'
category: 图论
subcategory: 最短路
subSubcategory: 单源最短路
tags: ["图论", "最短路", "SPFA", "Bellman-Ford", "负环检测", "队列优化", "负权边"]
timeComplexity: 'O(nm)'
spaceComplexity: 'O(n+m)'
level: A
codePath: '图论\最短路\Bellman-Ford 批量松弛优化（类SPFA变体）\Untitled1--fenfeng.cpp'
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 算法原理

当图中存在负权边时，Dijkstra 的贪心策略不再成立，需要改用能够处理负权边的算法。一种朴素的暴力做法是 Bellman-Ford：共进行 $n-1$ 轮松弛，每轮尝试所有 $m$ 条边，复杂度为 $O(nm)$。SPFA（Shortest Path Faster Algorithm）是对 Bellman-Ford 的优化，其核心观察是：**只有本轮被更新过的点，才有可能去更新别人**。因此不必每轮盲目遍历全部边，而是用队列维护这些「待处理」的点。

SPFA 的流程是：每次取出队首节点，松弛它的所有出边；如果某个节点被更新成功且当前不在队列中，就把它入队。如此反复，直到队列为空。

负环检测：如果某个点被反复松弛，说明可以无限绕圈使总权值不断减小，此时最短路不存在。常用的判定思路是「某个点的入队次数超过 $n$」，或更精确的「某点最短路径上的边数达到 $n$」。本代码采用后者——维护边数计数 `cnt[v] = cnt[u] + 1`，一旦 `cnt[v] >= n` 即判定存在负环。

## 核心公式 / 状态定义与转移方程

设 $dis[v]$ 为源点到节点 $v$ 的当前最短距离估计值。对于普通单源最短路，初始化为 $dis[s]=0$，其余为 $+\infty$。本代码面向**全图负环检测**，改用「超级源点」初始化：令所有 $dis[i]=0$ 并将所有节点入队，等价于新增一个到所有点连边权为 $0$ 的超级源点。

松弛转移方程（对队首 $u$ 的每条出边 $(u,v,w)$）：

$$dis[v] = \min\big(dis[v],\; dis[u] + w\big)$$

负环判定（边数计数判据）：

$$cnt[v] = cnt[u] + 1 \qquad \Rightarrow \qquad \text{若 } cnt[v] \ge n \text{，则存在负环}$$

## 逐行代码解析

以下按逻辑阶段把整份程序拆成 3 个模块，各模块代码块按顺序拼接即为完整 `.cpp` 源码（与 `codePath` 指向的文件一致）。

### 头文件、常量与全局数组

```cpp
#include <bits/stdc++.h>
using namespace std;
#define int long long
const int N = 2010;
vector<pair<int,int>> e[N];
int dis[N], cnt[N], vis[N];
queue<int> q;
```

- `#define int long long` 把代码中的 `int` 整体替换为 `long long`，避免距离累加溢出；因此入口函数必须写作 `signed main()`。
- `e[N]` 为邻接表，`e[u]` 存节点 $u$ 的所有出边 `(v, w)`；`dis` 存距离、`cnt` 存最短路径边数（判负环用）、`vis` 标记是否在队列中。

### spfa 函数

```cpp
bool spfa(int n) {
    for(int i=1;i<=n;i++){
        dis[i] = 0, vis[i] = 1;
          q.push(i);
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop(), vis[u] = 0;
        for (auto ed : e[u]) {
              int v = ed.first, w = ed.second;
              if (dis[v] > dis[u] + w) {
                dis[v] = dis[u] + w;
                cnt[v] = cnt[u] + 1;  
                if (cnt[v]>=n) return false;
                if (!vis[v]) q.push(v), vis[v] = 1;
            }
        }
    }
    return true;
}
```

- 初始化把所有点的距离置 $0$ 并全部入队，即「超级源点」写法，用于检测全图（不限定从某个源点可达）的负环。
- 松弛成功后执行 `cnt[v] = cnt[u] + 1` 记录当前最短路径的边数；一旦 `cnt[v] >= n` 说明最短路径经过了至少 $n$ 条边，必然存在负环，立即返回 `false`。
- `vis` 表示「是否在队列中」，用于防止同一节点重复入队；出队时置回 `false`。

### main 函数

```cpp
signed main(){
    int n,m;cin >> n >> m;
    memset(dis,0x3f,sizeof dis);
    for(int i = 1; i <= m; i ++) {
        int a, b; cin >> a >> b;
        int c; cin >> c;
        e[a].push_back({b, c});
    }

    if(spfa(n)) cout<<"No";
    else cout<<"Yes";    
    return 0;
}
```

- 读入 $n$ 个节点、$m$ 条边并建图（有向边 $a \to b$，边权 $c$）。
- `spfa(n)` 返回 `true` 表示图中无负环，输出 `No`；返回 `false` 表示存在负环，输出 `Yes`。
- `main` 中的 `memset(dis,0x3f,...)` 会被 `spfa` 内 `dis[i]=0` 的超级源点初始化覆盖，属于冗余语句，不影响结果。

## 复杂度分析

- **时间复杂度**：理论上最坏为 $O(nm)$，但实际运行通常很快（均摊约 $O(km)$，$k$ 为小常数）。在构造数据下可被卡回 $O(nm)$。
- **空间复杂度**：邻接表 $O(n+m)$，辅助数组 `dis`、`cnt`、`vis` 各 $O(n)$，总空间复杂度为 $O(n+m)$。

## 适用场景

- 含负权边的单源最短路（正权图同样适用）。
- 负环检测（判定图中是否存在负环），这是 SPFA 相对 Dijkstra / Bellman-Ford 的关键优势场景。
- 后续的部分图论算法（如费用流）也以 SPFA 为基础。

## 常见陷阱与注意事项

1. **构造数据下易退化**：SPFA 可被卡到 $O(nm)$，容易超时。非负权图上首选 Dijkstra；SPFA 主要用在负权边与负环检测场景。
2. **判负环的标准**：常见有两种等价判据——「入队次数超过 $n$」与「最短路径边数达到 $n$」。本代码用后者（`cnt[v] = cnt[u] + 1`），比入队次数判据更精确、误判更少。
3. **`#define int long long` 与 `signed main`**：宏会把 `int` 整体替换为 `long long`，因此 `main` 必须写成 `signed main()`，否则 `int main()` 会被展开成 `long long main()` 而编译报错。
4. **全图负环检测需要超级源点**：普通 SPFA 只能发现「从源点可达」的负环；若要检测任意位置的负环，需把所有点的距离置 $0$ 并全部入队（即本代码的写法）。
5. **`vis` 语义**：表示「是否在队列中」，用于防止重复入队，与「本轮是否被更新」含义不同，混用会导致逻辑错误。

## 对比与扩展

- **与 Bellman-Ford**：SPFA 是 Bellman-Ford 的队列优化，只处理被更新节点的出边，均摊更快，但最坏复杂度同为 $O(nm)$。
- **与 Dijkstra**：堆优化 Dijkstra 复杂度 $O((n+m)\log n)$ 且稳定，但不支持负权边；SPFA 支持负权但复杂度不稳定。非负权图应首选 Dijkstra。
- **与 Floyd**：Floyd 求全源最短路，复杂度 $O(n^3)$，可处理负权（但不能有负环），与 SPFA 的规模互补。
- **扩展方向**：SLF（Small Label First，用双端队列，距离更小者从队首入队）与 LLL（Large Label Last，队首距离大于平均时移到队尾）加速收敛；差分约束系统（把不等式约束转成最短路并判负环）；费用流（SPFA 找费用最短路 + 增广路）。
