---
id: ds-odt
title: '珂朵莉树（ODT）'
category: 数据结构
subcategory: 其他数据结构
subSubcategory: ''
tags: ["数据结构", "珂朵莉树", "ODT", "区间推平", "随机化", "暴力"]
timeComplexity: '单次操作期望 O(log n)（随机数据），最坏 O(n)'
spaceComplexity: 'O(n)'
codePath: '数据结构\珂朵莉树\Untitled1.cpp'
level: A
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 算法原理

珂朵莉树（Old Driver Tree，ODT）得名于 Codeforces 896C 题面中的人物珂朵莉。它常被称为"骗分神器"：本质是一种基于 `set` 的暴力区间维护结构，核心思想非常质朴——把值相同的一段连续下标合并成一个三元组 $(l, r, v)$，统一放进 `set` 中维护。

这种结构之所以成立，关键在于题目需要同时满足两个前提：**存在"区间赋值 / 区间推平"（assign）操作**，且**数据具有一定随机性**。

- **为什么快**：每次 assign 都会把 $[l,r]$ 内大量零碎的区间删除、合并成一个大区间，使 `set` 中的区间段数急剧下降。在操作随机分布的前提下，`set` 内的平均区间数量可以维持在 $O(\log n)$ 级别。
- **为什么好用**：每个节点是一个具象的区间 $[l, r, v]$，面对"区间第 $k$ 小""区间 $k$ 次方和"这类线段树极难维护的操作，珂朵莉树只需要暴力遍历对应的区间段即可，几乎零思考成本。
- **风险所在**：如果出题人构造特殊数据（例如完全没有推平操作，或反复在单点位置交替修改使区间碎裂），区间数量会退化到 $O(n)$，单次操作退化为 $O(n)$，整体退化为 $O(n^2)$，极易被卡常或 TLE。

## 核心公式 / 状态定义与转移方程

**状态定义**：珂朵莉树把下标区间 $[1, n]$ 划分成若干互不相交的连续段，每个段用一个三元组 $(l, r, v)$ 表示，含义是"下标 $l$ 到 $r$ 的所有元素值都等于 $v$"。整棵树即一个 `set<Node>`，所有段按左端点 $l$ 严格递增排列，并始终覆盖 $[1, n]$。任意两个相邻段的值都不相同，否则它们本应被合并成一个段——这是"按值合并"这一核心思想的不变量。

**split 的分裂转移**（把包含 $pos$ 的段切开，返回以 $pos$ 为左端点的段）：

$$[l,\ r,\ v]\ \xrightarrow{\ \mathrm{split}(pos)\ }\ [l,\ pos-1,\ v]\ \cup\ [pos,\ r,\ v]$$

**assign 的推平转移**（把 $[l, r]$ 内所有段替换为单一区间）：

$$\mathrm{assign}(l,\ r,\ x):\quad [l,\ r]\ \text{内所有段}\ \longrightarrow\ [l,\ r,\ x]$$

**区间 $k$ 次方和（模 $M$）的统计**：利用"同段同值"把逐点累加折叠成逐段累加，每段只需算一次幂再乘段长：

$$S = \sum_{[L,R,v]} (R-L+1)\cdot v^{ex},\qquad \text{答案为}\ S \bmod M$$

## 逐行代码解析

### 节点定义与 set 容器（含初始化）

```cpp
struct Node {
    int l, r;
    mutable long long v;

    bool operator<(const Node &other) const {
        return l < other.l;
    }
};

set<Node> odt;

// 最简单的单点装入初始化
for (int i = 1; i <= n; ++i) {
    odt.insert({i, i, a[i]});
}
```

- `Node` 记录区间左端点 `l`、右端点 `r` 与公共权值 `v`；重载 `operator<` 只按 `l` 比较，使 `set` 按左端点有序。
- `v` 声明为 `mutable long long`：`set` 的元素是只读的，直接通过迭代器改 `v` 会编译报错，`mutable` 允许在只读上下文中修改该成员。
- 初始化把每个位置 $i$ 单独作为一个区间 `{i, i, a[i]}` 插入，形成 $n$ 个单点区间；单次插入 $O(\log n)$，共 $O(n\log n)$。

### split：区间分裂

```cpp
auto split(int pos) {
    if (pos > n) return odt.end();

    // 找到第一个左端点 >= pos 的区间
    auto it = odt.lower_bound({pos, -1, 0});
    if (it != odt.end() && it->l == pos) {
        return it;
    }

    // 真正的目标区间在 it 的前一个
    --it;
    int l = it->l, r = it->r;
    long long v = it->v;

    // 先删除原区间，再插入切分后的两个区间
    odt.erase(it);
    odt.insert({l, pos - 1, v});
    return odt.insert({pos, r, v}).first;
}
```

- `pos > n` 时越界，直接返回 `end()` 作为哨兵，避免对 `end()` 做 `--`。
- `lower_bound({pos, -1, 0})` 找到第一个 `l >= pos` 的区间；若其 `l` 恰好等于 `pos`，说明该区间已以 `pos` 为左端点，直接返回。
- 否则包含 `pos` 的区间在 `it` 前一个，`--it` 后取出原 `[l, r, v]`；先 `erase` 原区间，再插入 `[l, pos-1, v]` 与 `[pos, r, v]`。
- `insert` 返回 `pair<iterator, bool>`，`.first` 是插入元素的迭代器，正是以 `pos` 为左端点的新区间。

### assign：区间推平

```cpp
void assign(int l, int r, long long x) {
    auto itr = split(r + 1);
    auto itl = split(l);

    // 删除 [itl, itr) 内的所有旧区间
    odt.erase(itl, itr);

    // 插入合并后的大区间
    odt.insert({l, r, x});
}
```

- 必须"先 `split(r + 1)` 再 `split(l)`"：若先切 $l$，返回的 `itl` 可能在后续 `split(r + 1)` 时因所属区间被删除、重构而失效，导致 RE。
- `split(r + 1)` 保证 $r$ 恰好是某区间的右端点，`split(l)` 保证 $l$ 恰好是某区间的左端点，于是 `[itl, itr)` 精确覆盖 $[l, r]$。
- `erase(itl, itr)` 删除左闭右开区间内的所有旧段，再插入合并后的大区间 `[l, r, x]`。

### add：区间加

```cpp
void add(int l, int r, long long val) {
    auto itr = split(r + 1);
    auto itl = split(l);

    for (auto it = itl; it != itr; ++it) {
        it->v += val; // 由于使用了 mutable，可以直接修改
    }
}
```

- 同样先切出 `[itl, itr)`，再暴力遍历其中每个区间段，把公共权值整体加 `val`。
- 直接 `it->v += val` 依赖 `mutable`；没有 `mutable` 时需先取出值、`erase` 旧段再 `insert` 新值。

### query_kth：区间第 k 小

```cpp
long long query_kth(int l, int r, int k) {
    auto itr = split(r + 1);
    auto itl = split(l);

    vector<pair<long long, int>> elements;
    for (auto it = itl; it != itr; ++it) {
        elements.push_back({it->v, it->r - it->l + 1});
    }

    sort(elements.begin(), elements.end());

    for (auto &[val, cnt] : elements) {
        k -= cnt;
        if (k <= 0) return val;
    }
    return -1;
}
```

- 把 $[l, r]$ 内的每个段 `(值, 长度)` 收集进 `vector`，按值排序后即为升序。
- 逐个段累加长度，当 `k` 减到 $\le 0$ 时，当前段的值就是第 $k$ 小。
- `return -1` 是 `k` 超过区间长度时的哨兵，正常 $k$ 不会走到。

### query_power_sum：区间 k 次方和

```cpp
long long query_power_sum(int l, int r, long long ex, long long mod) {
    auto itr = split(r + 1);
    auto itl = split(l);

    long long sum = 0;
    for (auto it = itl; it != itr; ++it) {
        long long len = it->r - it->l + 1;
        sum = (sum + len * qpow(it->v % mod, ex, mod)) % mod;
    }
    return sum;
}
```

- 每个段的贡献是"段长 × 段值 $ex$ 次方"，因为段内所有元素值相同，只需算一次幂再乘长度。
- `qpow` 是外部快速幂函数，负责算 `base^ex mod mod`；`it->v` 先 `% mod` 再求幂，防止溢出。
- 累加时随时 `% mod`，保持 `sum` 不溢出。

## 复杂度分析

**时间复杂度**：

- 初始化：把 $n$ 个单点区间逐个插入 `set`，每次 $O(\log n)$，共 $O(n\log n)$。
- 单次 `split`：两次 `lower_bound` / `insert`，均为 $O(\log n)$，故 $O(\log n)$。
- 单次 `assign`：两次 `split` 加一次 `erase(itl, itr)` 与一次 `insert`，为 $O(\log n + \text{被删段数})$。
- 随机数据下：每次 assign 会把大量小段合并，`set` 内段数始终维持在期望 $O(\log n)$（源资料亦给出 $O(\log\log n)$ 的提法），因此单次操作期望 $O(\log n)$。
- 最坏情况：构造数据使段数退化到 $O(n)$，单次操作 $O(n)$，$q$ 次操作整体 $O(nq)$；当 $q$ 与 $n$ 同阶时为 $O(n^2)$。

**空间复杂度**：`set` 中最多同时存在 $O(n)$ 个区间段，每个段存一个 `Node`，故为 $O(n)$。

## 适用场景

珂朵莉树适合两类题目：一类是明确包含"区间赋值 / 区间推平"操作、且数据随机或部分分不卡的题（典型代表是 CF896C 原题）；另一类是需要在短时间内"骗"到部分分的场合。

- **适合**：频繁区间推平 + 随机数据；需要维护"区间第 $k$ 小""区间 $k$ 次方和""区间加"等组合操作但数据不强；追求实现简单、十几行搞定。
- **不适合**：完全没有 assign 操作（段数无法被压缩）；出题人构造了反复在单点位置交替修改的数据；正赛强数据且要求严格复杂度保证。此时应改用线段树 / 平衡树 / 分块等有确定复杂度的结构。

## 常见陷阱与注意事项

1. **split 的调用顺序**：assign、add、查询中必须**先 `split(r + 1)` 再 `split(l)`**。若先切 $l$，返回的 `itl` 会在后续 `split(r + 1)` 时因所属区间被删除、重构而失效，引发未定义行为（常见 RE）。
2. **`mutable` 不能省**：`set` 的迭代器解引用得到的是 `const Node&`，直接写 `it->v = ...` 会编译失败；`mutable long long v` 正是为了允许在只读上下文中修改 $v$。
3. **`operator<` 只按 `l` 比较**：若同时比较 $r$ 或 $v$，`set` 的去重语义会被破坏，导致区间合并出错。
4. **`lower_bound({pos, -1, 0})` 的临时对象**：用 `-1`、`0` 占位 $r$、$v$ 不影响"按 $l$ 查找"，因为比较只看 `l`。
5. **退化风险**：没有推平保证或遇到构造数据时会退化到 $O(n^2)$，不要把珂朵莉树当作唯一正解提交。
6. **溢出与外部依赖**：`add` 累加后 $v$ 可能超出 `int`，故 $v$ 用 `long long`；`query_power_sum` 依赖外部快速幂 `qpow(base, ex, mod)`，需自行定义，且先 `it->v % mod` 再求幂防止溢出。

## 对比与扩展

- **与线段树对比**：线段树维护的是树形递归区间，修改/查询均为严格的 $O(\log n)$，不依赖随机性；珂朵莉树依赖"随机 + 推平"，复杂度是期望意义下的，最坏 $O(n)$，但代码量少得多，且能轻松维护线段树很难维护的"区间第 $k$ 小""区间 $k$ 次方和"。
- **与分块对比**：分块单次 $O(\sqrt n)$ 是确定的、无退化；珂朵莉树在随机数据下往往更快（期望 $O(\log n)$），但没有保证。
- **与平衡树维护区间对比**：珂朵莉树本质是"按值合并的区间集合"，与 `set` / 平衡树维护的"区间"是同一类思想，只是珂朵莉树把合并后的同值段作为一个整体节点，省去了维护平衡的负担。
- **扩展方向**：CF896C 是珂朵莉树的源头例题；珂朵莉树也常与线段树、树状数组配合处理更复杂的区间信息，另有"可持久化珂朵莉树"的讨论（不常见，主要停留在概念层面）。
