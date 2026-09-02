---
id: ds-block-decomposition
title: '分块（区间加·区间求和·区间≤k求和）'
category: 数据结构
subcategory: 分块
tags: ["数据结构", "分块", "Sqrt Decomposition", "区间修改", "区间查询", "lazy标记", "排序二分"]
timeComplexity: 'O(n√n log n)'
spaceComplexity: 'O(n√n)'
codePath: '数据结构\分块\Untitled1.cpp'
---

## 算法原理

分块（Sqrt Decomposition）是一种基于"大块整做、小段暴力"思想的数据结构。将长度为 $n$ 的数组划分为 $\lceil\sqrt{n}\rceil$ 个块，每块大小约 $\sqrt{n}$。对于区间操作：

- **整块**：用预处理的辅助数组 + lazy 标记 $O(1)$ 或 $O(\log n)$ 完成
- **散块**（区间两端不满一整块的部分）：直接暴力遍历 $O(\sqrt{n})$

本代码实现了三种操作：

| 操作 | 含义 | 整块复杂度 | 散块复杂度 |
|---|---|---|---|
| op 1 | 区间 $[l,r]$ 求和 | $O(1)$（预处理的块和 + lazy） | $O(\sqrt{n})$ |
| op 2 | 区间 $[l,r]$ 内 $\le k$ 的元素之和 | $O(\log\sqrt{n})$（排序 + 二分 + 前缀和） | $O(\sqrt{n})$ |
| op 3 | 区间 $[l,r]$ 加 $w$ | $O(1)$（lazy 标记） | $O(\sqrt{n}\log\sqrt{n})$（暴力改 + 重建块） |

### 核心思想

分块的本质是**降低常数、简化实现**。与线段树相比：
- 线段树维护的是树形递归区间，修改/查询都是 $O(\log n)$
- 分块维护的是连续大块，整块 $O(1)$，散块 $O(\sqrt{n})$，均摊 $O(\sqrt{n})$
- 分块最大的优势：**能维护线段树难以处理的操作**（如区间 ≤k 求和，需要可持久化或复杂合并）

### 本代码的特殊之处

op 2（区间 ≤k 的元素之和）是分块的**杀手级应用**——线段树做这个需要复杂的高阶操作（如线段树套平衡树），而分块只需每块维护一个**排序数组 + 前缀和**，二分即可。这正是分块存在的重要理由。

## 数据结构图示

### 分块布局

以 $n = 10$ 为例，$\lceil\sqrt{10}\rceil = 4$，分 3 个块：

```
下标:  1  2  3  4 | 5  6  7  8 | 9  10
       └─── 块1 ─┘ └── 块2 ──┘ └─块3─┘
       bk_sz = 4      bk_sz = 4    bk_sz = 2

块信息:
  块1: L[1]=1, R[1]=4, 大小=4
  块2: L[2]=5, R[2]=8, 大小=4
  块3: L[3]=9, R[3]=10, 大小=2

辅助数组:
  A[]:    原始数组值（含 lazy 下放后的真实值）
  arr[]:  每块的排序副本（用于 op 2 的二分）
  sum[][]: 每块排序数组的前缀和
  lazy[]: 每块的延迟标记（区间加未下放的部分）
```

### 整块与散块划分

查询区间 $[2, 9]$ 时：

```
下标:  1  2  3  4 | 5  6  7  8 | 9  10
         └─散块─┘ └──整块─────┘ └散┘
         块1右部     块2(整块)    块3左部

id_l = (2-1)/4+1 = 1 (块1)    ← 左散块在块1
id_r = (9-1)/4+1 = 3 (块3)    ← 右散块在块3

操作流程:
  1. 左散块: 遍历 [2, R[1]] = [2, 4]         O(√n)
  2. 整块:   遍历 id_l+1 ~ id_r-1 = 块2       O(1) 或 O(√n log√n)
  3. 右散块: 遍历 [L[3], 9] = [9, 9]         O(√n)
```

### init_bk 重建块的过程

当散块元素被修改后（op 3 的散块），需要重建该块的辅助数组：

```
块1: A = [3, 1, 4, 2], lazy[1] = 0

步骤1: 下放 lazy
  A[i] += lazy[id]   →  A 不变（lazy=0）

步骤2: 复制到 arr
  arr = [3, 1, 4, 2]

步骤3: 排序
  arr = [1, 2, 3, 4]

步骤4: 求前缀和
  sum[id][0] = 0
  sum[id][1] = 0 + 1 = 1
  sum[id][2] = 1 + 2 = 3
  sum[id][3] = 3 + 3 = 6
  sum[id][4] = 6 + 4 = 10

步骤5: 清零 lazy
  lazy[id] = 0

结果:
  A   = [3, 1, 4, 2]  （原始值，含已下放的增量）
  arr = [1, 2, 3, 4]  （排序后）
  sum = [0, 1, 3, 6, 10]  （前缀和）
  lazy = 0
```

### op 2 二分查询示意

查询块1中 $\le k = 3$ 的元素之和：

```
块1: arr = [1, 2, 3, 4], sum = [0, 1, 3, 6, 10], lazy = 0

upper_bound(arr, k - lazy) = upper_bound([1,2,3,4], 3)
  → 指向 arr[3] = 4（第一个 > 3 的位置）
  → 返回下标 i = 3（前3个元素 ≤ 3）

答案 = sum[1][3] + i * lazy[1] = 6 + 3*0 = 6
     = 1 + 2 + 3 = 6 ✓

如果 lazy[1] = 5（块整体加了5）:
  查找 ≤ k = 8 的元素:
  upper_bound([1,2,3,4], 8-5=3) → i = 3
  答案 = sum[1][3] + 3 * 5 = 6 + 15 = 21
       = (1+5)+(2+5)+(3+5) = 6+7+8 = 21 ✓
```

## 核心操作详解

### 块的划分（init 函数）

- **块大小**：`bk_sz = ceil(sqrt(N))`
- **块数量**：`bk_cnt = (N + bk_sz - 1) / bk_sz`（向上取整）
- **块边界**：`L[i] = (i-1)*bk_sz + 1`，`R[i] = min(i*bk_sz, N)`
- **块编号查询**：给定下标 $j$，其所属块号为 `(j-1)/bk_sz + 1`

### 重建块（init_bk 函数）

当散块元素被修改后，需要重建该块的 `arr` 和 `sum`：

1. **下放 lazy**：`A[i] += lazy[id]`，将延迟标记合并到真实值
2. **复制**：`arr[i] = A[i]`，复制到排序数组
3. **排序**：`sort(arr.begin()+L[id], arr.begin()+R[id]+1)`
4. **前缀和**：`sum[id][j] = sum[id][j-1] + arr[L[id]+j-1]`
5. **清零 lazy**：`lazy[id] = 0`

### op 1：区间求和

```
区间 [l, r] 求和:
  若 id_l == id_r: 暴力遍历 [l, r]，每个 A[i] + lazy[id]
  否则:
    左散块: 遍历 [l, R[id_l]]            → A[i] + lazy[id_l]
    整块:   遍历 id_l+1 ~ id_r-1          → sum[id][块大小] + lazy[id] * 块大小
    右散块: 遍历 [L[id_r], r]            → A[i] + lazy[id_r]
```

整块求和公式：`块内元素和 = sum[id][sz] + lazy[id] * sz`，其中 `sum[id][sz]` 是排序数组的总和（等于原始和），`lazy[id] * sz` 是整块加的增量。

### op 2：区间 ≤k 的元素之和

**散块版** `get_2(id, l, r, k)`：暴力遍历 `[l, r]`，对每个元素检查 `A[i] + lazy[id] <= k`，满足则累加。

**整块版** `get_2(id, k)`：
1. 二分查找 `upper_bound(arr, k - lazy[id])`，找到第一个 > k-lazy 的位置 $i$
2. 答案 = `sum[id][i] + i * lazy[id]`
3. 原理：排序数组中前 $i$ 个元素都 $\le k - \text{lazy}$（即 $\text{arr}[j] + \text{lazy} \le k$），它们的和加上 lazy 增量即为答案

### op 3：区间加

```
区间 [l, r] 加 w:
  若 id_l == id_r: 暴力 A[i] += w，然后重建块 id_l
  否则:
    左散块: A[i] += w for [l, R[id_l]]，重建块 id_l
    整块:   lazy[i] += w for id_l+1 ~ id_r-1     ← O(1) per block
    右散块: A[i] += w for [L[id_r], r]，重建块 id_r
```

整块加只需更新 `lazy` 标记，$O(1)$。散块需要逐个修改并重建排序数组，$O(\sqrt{n}\log\sqrt{n})$。

## 逐行代码解析

### 全局变量

```cpp
vector<long long> A, arr, lazy;       // A: 原始值, arr: 排序副本, lazy: 延迟标记
vector<vector<long long>> sum;         // sum[id][j]: 第id块排序数组前j个元素的和
vector<int> L, R;                     // L[id], R[id]: 第id块的左右边界
int N, T, bk_sz, bk_cnt;              // N: 数组长度, T: 操作数, bk_sz: 块大小, bk_cnt: 块数
```

### init_bk：重建块

```cpp
void init_bk(int id) {
    for (int i = L[id]; i <= R[id]; i++) {
        A[i] += lazy[id];            // 下放 lazy 到真实值
        arr[i] = A[i];               // 复制到排序数组
    }
    sort(arr.begin() + L[id], arr.begin() + R[id] + 1);  // 块内排序
    for (int i = L[id]; i <= R[id]; i++)
        sum[id][i - L[id] + 1] = sum[id][i - L[id]] + arr[i];  // 前缀和
    lazy[id] = 0;                    // 清零 lazy
}
```

- 下放 lazy 后 A 存的是真实值，arr 是 A 的排序副本。
- 前缀和 `sum[id][j]` 表示该块排序后前 $j$ 个元素之和。

### init：初始化

```cpp
void init() {
    bk_sz = ceil(sqrt(N));                        // 块大小 = √n 向上取整
    bk_cnt = (N + bk_sz - 1) / bk_sz;            // 块数 = ⌈n/√n⌉
    sum.resize(bk_cnt + 1, vector<long long>(bk_sz + 1, 0));
    arr.resize(N + 1);
    lazy.resize(bk_cnt + 1, 0);                   // lazy 按块数分配（修复了原 bk_sz 的问题）
    L.resize(bk_cnt + 1);
    R.resize(bk_cnt + 1);
    for (int i = 1; i <= bk_cnt; i++) {
        L[i] = (i - 1) * bk_sz + 1;              // 块左边界
        R[i] = min(i * bk_sz, N);                // 块右边界（最后一块可能不满）
        init_bk(i);                               // 重建每个块
    }
}
```

- `sum` 的第二维大小为 `bk_sz + 1`，因为前缀和需要从 0 开始。
- `lazy` 的大小为 `bk_sz + 1`（应为 `bk_cnt + 1`，但当 $n$ 是完全平方数时两者相等；一般情况应修正为 `bk_cnt + 1`）。

### get_2（两个重载）

**散块版**：

```cpp
inline long long get_2(int id, int l, int r, long long k) {
    long long ans = 0;
    for (int i = l; i <= r; i++) {
        if (A[i] + lazy[id] <= k) ans += A[i] + lazy[id];  // 暴力逐个检查
    }
    return ans;
}
```

**整块版**：

```cpp
inline long long get_2(int id, long long k) {
    int i = upper_bound(arr.begin() + L[id], arr.begin() + R[id] + 1, k - lazy[id])
            - (arr.begin() + L[id]);    // 二分找第一个 > k-lazy 的位置
    return sum[id][i] + (i * lazy[id]); // 排序前缀和 + lazy 增量
}
```

- `k - lazy[id]`：因为 `arr` 存的是不含 lazy 的值，查询 `A[i] + lazy <= k` 等价于 `arr[i] <= k - lazy`。
- `sum[id][i]`：排序数组前 $i$ 个元素之和（不含 lazy）。
- `i * lazy[id]`：这 $i$ 个元素每个都加了 lazy，所以增量是 $i \times \text{lazy}$。

### 主函数

```cpp
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    cin >> N >> T;
    A.resize(N + 1);
    for (int i = 1; i <= N; i++) cin >> A[i];
    init();                         // 分块 + 建排序数组 + 前缀和

    while (T--) {
        int op; cin >> op;
        if (op == 1) { /* 区间求和 */ }
        else if (op == 2) { /* 区间≤k求和 */ }
        else if (op == 3) { /* 区间加 */ }
    }
}
```

**op 1（区间求和）**关键行：

```cpp
// 整块求和: sum[id][sz] + lazy[id] * sz
for (int i = id_l + 1; i < id_r; i++)
    ans += sum[i][R[i] - L[i] + 1] + (lazy[i] * (R[i] - L[i] + 1));
```

`sum[i][R[i]-L[i]+1]` 是该块所有元素排序后的总和（等于原始和），加上 `lazy * 块大小` 得到当前真实和。

**op 2（区间 ≤k 求和）**关键行：

```cpp
if (id_l == id_r) ans += get_2(id_l, l, r, k);         // 同块暴力
else {
    ans += get_2(id_l, l, R[id_l], k);                  // 左散块暴力
    for (int i = id_l + 1; i < id_r; i++) ans += get_2(i, k);  // 整块二分
    ans += get_2(id_r, L[id_r], r, k);                  // 右散块暴力
}
```

**op 3（区间加）**关键行：

```cpp
// 整块加: 只更新 lazy
for (int i = id_l + 1; i < id_r; i++) lazy[i] += w;
// 散块加: 逐个修改 + 重建
for (int i = l; i <= R[id_l]; i++) A[i] += w;
init_bk(id_l);
```

## 复杂度分析

### 时间复杂度

| 操作 | 整块 | 散块 | 总复杂度 |
|---|---|---|---|
| op 1（区间求和） | $O(1)$ per block | $O(\sqrt{n})$ | $O(\sqrt{n})$ |
| op 2（≤k 求和） | $O(\log\sqrt{n})$ per block | $O(\sqrt{n})$ | $O(\sqrt{n}\log\sqrt{n})$ |
| op 3（区间加） | $O(1)$ per block | $O(\sqrt{n}\log\sqrt{n})$ | $O(\sqrt{n}\log\sqrt{n})$ |

散块重建（`init_bk`）需要排序，$O(\sqrt{n}\log\sqrt{n})$。这是 op 3 的瓶颈。

$T$ 次操作总时间：$O(T\sqrt{n}\log\sqrt{n})$。

### 空间复杂度

- `A[]`：$O(n)$
- `arr[]`：$O(n)$
- `sum[][]`：$O(\sqrt{n} \times \sqrt{n}) = O(n)$
- `lazy[]`、`L[]`、`R[]`：$O(\sqrt{n})$
- **总空间**：$O(n)$

## 适用场景

### 适用

- **区间加 + 区间求和**：线段树也能做，但分块更直观、更易调试
- **区间 ≤k 的元素求和**：分块的杀手级应用，线段树难以高效实现
- **区间加 + 区间第 k 大**：分块二分答案 + 整块二分判定
- **不满足可合并性的操作**：线段树 push_up 难以维护时，分块是替代方案
- **编写时间有限**：分块代码短、思路直，竞赛中比线段树更快写完

### 不适用

- **需要 $O(\log n)$ 的操作**：线段树/树状数组更优
- **$n$ 很大（$> 10^6$）且操作多**：$O(n\sqrt{n}\log\sqrt{n})$ 可能超时
- **需要可持久化**：分块难以实现可持久化版本

## 常见陷阱与注意事项

1. **lazy 数组大小问题（已修复）**
   - 早期版本中 `lazy.resize(bk_sz + 1, 0)`，当 $n$ 不是完全平方数时 `bk_cnt > bk_sz`，会导致 `lazy` 数组越界。
   - 正确写法：`lazy.resize(bk_cnt + 1, 0)`，当前代码已修复此问题。

2. **下放 lazy 的时机**
   - `init_bk` 会下放 lazy 到 `A[]` 并清零。散块操作（op 1/2 的散块）不会下放，而是直接用 `A[i] + lazy[id]` 计算真实值。
   - 如果散块操作后又调用了 `init_bk`（如 op 3），则 lazy 被下放。否则 lazy 仍保留在标记中。

3. **整块求和公式不要忘 lazy**
   - `sum[id][sz]` 是不含 lazy 的原始和，真实和 = `sum[id][sz] + lazy[id] * sz`。
   - 散块求和也要加 lazy：`A[i] + lazy[id]`。

4. **二分查找的坐标计算**
   - `upper_bound` 返回的是迭代器，需减去 `arr.begin() + L[id]` 得到块内偏移量。
   - 偏移量 $i$ 直接用于 `sum[id][i]`（前缀和从 0 开始）。

5. **init_bk 中的排序范围**
   - `sort(arr.begin() + L[id], arr.begin() + R[id] + 1)`，注意右端点 +1（STL 左闭右开）。

6. **块大小取 √n 还是 √(n log n)**
   - 标准 $\sqrt{n}$ 是经典选择，但若操作复杂度含 $\log$（如 op 2），块大小可调为 $\sqrt{n\log n}$ 优化常数。

## 对比与扩展

### 分块 vs 线段树

| 特性 | 分块 | 线段树 |
|---|---|---|
| 区间加 + 区间求和 | $O(\sqrt{n})$ | $O(\log n)$ |
| 区间 ≤k 求和 | $O(\sqrt{n}\log\sqrt{n})$ | $O(\log^2 n)$（线段树套平衡树） |
| 区间第 k 大 | $O(\sqrt{n}\log n)$ | $O(\log^3 n)$（主席树 + 线段树二分） |
| 代码复杂度 | 低 | 中~高 |
| 可持久化 | 困难 | 可行（主席树） |
| 操作可合并性 | 不需要 | 需要（push_up） |
| 调试难度 | 低 | 高 |

分块的核心优势：**不要求操作可合并**。只要单块能暴力处理，任何操作都能套分块框架。

### 莫队算法

莫队（Mo's Algorithm）是分块的变种，用于离线区间查询。它利用分块对查询进行排序，使得相邻查询只差 $O(\sqrt{n})$ 个元素，用"移动指针"的方式高效处理。详见 `ds-mo-team` 知识点。

### 值域分块

将值域分块（而非下标分块），可用于 $O(1)$ 修改 + $O(\sqrt{V})$ 查询的平衡，其中 $V$ 是值域大小。常与下标分块结合使用。

### 块大小优化

当操作包含 $\log$ 因子时，块大小取 $\sqrt{n\log n}$ 可以优化总复杂度至 $O(n\sqrt{n\log n})$，比 $\sqrt{n}$ 的 $O(n\sqrt{n}\log n)$ 更优。
