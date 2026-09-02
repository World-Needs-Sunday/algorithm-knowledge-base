---
id: basic-quickselect-kth
title: '快速选择（Quickselect — 找第 k 小）'
category: 基础算法
subcategory: 分治
tags: ["分治", "快速选择", "Quickselect", "Top-K", "三数取中", "划分", "无序数组"]
timeComplexity: '平均 O(n)，最坏 O(n^2)'
spaceComplexity: 'O(log n)（递归栈）'
codePath: '基础算法(有价值的题目集)\分治\快速选择\源.cpp'
---

## 算法原理

快速选择（Quickselect）是快速排序的"半边"版本：利用划分（partition）确定 pivot 的最终位置，然后**只递归包含第 $k$ 小的那一半**，丢弃另一半。这使得平均时间从排序的 $O(n \log n)$ 降到了 $O(n)$。

### 核心思路

对于数组 $A[l..r]$ 中找第 $k$ 小（$k$ 是下标，从 $l$ 开始计）：

1. **选 pivot**：从 $A[l]$、$A[r]$、$A[\lfloor (l+r)/2 \rfloor]$ 中取中位数（三数取中法），避免最坏情况
2. **划分**：双指针从两端向中间扫描，把 $\leq$ pivot 的放左边、$\geq$ pivot 的放右边
3. **分支**：划分结束后，左段为 $[l, \text{mr}]$，右段为 $[\text{ml}, r]$，中间 $[\text{mr}+1, \text{ml}-1]$ 全是 pivot 值
   - 若 $k \leq \text{mr}$：第 $k$ 小在左段，递归左段
   - 若 $k \geq \text{ml}$：第 $k$ 小在右段，递归右段
   - 否则：$k$ 落在中间的 pivot 区，直接返回 pivot 值

### 三数取中法

pivot 的选择决定算法效率：

| pivot 选择 | 平均时间 | 最坏时间 | 风险 |
|---|---|---|---|
| 取首元素 | $O(n)$ | $O(n^2)$ | 有序数组退化 |
| 随机取 | $O(n)$ | $O(n^2)$（概率极低） | 需要随机数生成器 |
| **三数取中** | $O(n)$ | $O(n^2)$（概率极低） | 简单且实用，本算法采用 |

三数取中法取 $A[l]$、$A[r]$、$A[\text{mid}]$ 的中位数作为 pivot，有效避免有序或逆序数组的退化。

## 核心公式/状态定义与转移方程

### 划分不变量

划分结束后，数组被分成三段：

$$\underbrace{A[l] \cdots A[\text{mr}]}_{\leq q} \quad \underbrace{A[\text{mr}+1] \cdots A[\text{ml}-1]}_{= q} \quad \underbrace{A[\text{ml}] \cdots A[r]}_{\geq q}$$

其中 $q$ 是 pivot 值，$\text{ml}$ 和 $\text{mr}$ 是划分后的边界。

### 递归转移

$$\text{fast\_k}(l, r, k) = \begin{cases} \text{fast\_k}(l, \text{mr}, k) & \text{若 } k \leq \text{mr} \\ \text{fast\_k}(\text{ml}, r, k) & \text{若 } k \geq \text{ml} \\ q & \text{否则（} k \text{ 落在 pivot 区）} \end{cases}$$

### 划分过程示意

以数组 $A = [0, 3, 1, 4, 2, 5, 0]$（下标从 1 开始）找第 3 小为例：

```
初始:  [3, 1, 4, 2, 5, 0]   (下标 1..6)
       l=1                r=6

三数取中: A[l]=3, A[r]=0, A[mid]=A[3]=4
  → 中位数是 3，pivot q = 3

划分（双指针 ml=1, mr=6）:
  ┌─ A[ml]=3, 不 < q=3, ml 停在 1
  │  A[mr]=0, 不 > q=3, mr 停在 6... 不对，0 < 3 所以 mr 继续走？
  └─ 实际: A[mr]=0, 0 < 3 → 0 不 > 3, mr 停在 6
     交换 A[1] 和 A[6]: [0, 1, 4, 2, 5, 3]
     ml=2, mr=5
     A[ml]=1 < 3, ml→3; A[ml]=4 不 < 3, ml 停在 3
     A[mr]=5 > 3, mr→4; A[mr]=2 不 > 3, mr 停在 4
     交换 A[3] 和 A[4]: [0, 1, 2, 4, 5, 3]
     ml=4, mr=3, ml > mr → 退出

结果:  [0, 1, 2] [4, 5, 3]
       ≤q (l..mr=3)  ≥q (ml=4..r)
       中间无 pivot 区

k=3, mr=3 → k <= mr, 递归左段 [0, 1, 2]
  l=1, r=3, q = getnum(1,3) = 中位数(1, 2, 1) = 1... 
  实际三数取中: A[1]=0, A[3]=2, A[2]=1 → 中位数 1
  划分后: [0] [1] [2], ml=2, mr=1
  k=3 > mr=1, k=3 >= ml=2 → 递归右段 [2]
  l=2, r=3, q = getnum(2,3) = 中位数(1, 2, 1)... 
  → 最终找到 A[3] = 2

第 3 小 = 2 ✓
```

## 逐行代码解析

### 快速输入输出

```cpp
inline int read()
{
    int x = 0, f = 1; char ch = getchar();
    while (ch < '0' || ch>'9') { if (ch == '-') f = -1; ch = getchar(); }
    while (ch >= '0' && ch <= '9') { x = x * 10 + ch - 48; ch = getchar(); }
    return x * f;
}
inline void write(long long x)
{
    static char buf[30];
    int p = 0;
    if (x < 0) { putchar('-'); x = -x; }
    if (x == 0) buf[p++] = '0';
    while (x) { buf[p++] = x % 10 + 48; x /= 10; }
    while (p--) putchar(buf[p]);
}
```

手写 `read`/`write` 比标准 `cin`/`cout` 快 5-10 倍，适合大数据量（$n \le 10^6$ 级别）。

### 三数取中

```cpp
int getnum(int l, int r)
{
    int a = A[l], b = A[r], c = A[l + (r - l) / 2];
    if (a >= b)
    {
        if (b >= c) return b;        // a >= b >= c → 中位数 b
        else
        {
            if (a >= c) return c;    // a >= c > b → 中位数 c
            else return a;           // c > a >= b → 中位数 a
        }
    }
    else
    {
        if (a >= c) return a;        // b > a >= c → 中位数 a
        else
        {
            if (b >= c) return c;    // b >= c > a → 中位数 c
            else return b;           // c > b > a → 中位数 b
        }
    }
}
```

通过 6 次比较找三个数的中位数。逻辑展开为分支判断，无额外函数调用开销。

### 划分 + 递归

```cpp
int fast_k(int l, int r, int k)
{
    if (l == r) return A[l];         // 递归基：只剩一个元素
    int ml = l, mr = r;
    int q = getnum(l, r);            // 三数取中选 pivot
    while (ml <= mr)
    {
        while (A[ml] < q) ++ml;      // 左指针找 >= q 的
        while (A[mr] > q) --mr;      // 右指针找 <= q 的
        if (ml <= mr) swap(A[ml++], A[mr--]);  // 交换并移动
    }
    // 划分结束: [l..mr] <= q, [mr+1..ml-1] == q, [ml..r] >= q
    if (k <= mr) return fast_k(l, mr, k);    // 第 k 小在左段
    if (k >= ml) return fast_k(ml, r, k);    // 第 k 小在右段
    return q;                                 // k 落在 pivot 区
}
```

关键点：
- `while (A[ml] < q)` 而非 `<=`：遇到等于 pivot 的元素会停下并交换，使得 pivot 值的元素均匀分布到两侧，中间可能形成 pivot 区
- `if (ml <= mr)` 而非 `<`：允许 `ml == mr` 时也交换（虽然交换自己，但两个指针会各自前进一步），确保循环正确推进
- 三分支递归：左段、右段、pivot 区，只递归其中一个，平均每次规模减半

### 主函数

```cpp
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    int n, k;
    n = read(), k = read();
    A.resize(n + 1);
    for (int i = 1; i <= n; i++) A[i] = read();
    write(fast_k(1, n, k));
    return 0;
}
```

- 下标从 1 开始，`A[0]` 不使用（`resize` 初始化为 0）
- `k` 是目标排名（1-based：$k=1$ 最小，$k=n$ 最大）

## 复杂度分析

### 时间复杂度

| 情况 | 时间 | 说明 |
|---|---|---|
| 平均 | $O(n)$ | 每次划分约减半，$n + n/2 + n/4 + \cdots \le 2n$ |
| 最坏 | $O(n^2)$ | 每次只减 1 个元素，三数取中使概率极低 |

平均情况推导：设每次划分后保留 $p$ 比例的元素，则 $T(n) = T(pn) + O(n)$。若 $p \approx 1/2$，则 $T(n) = n + n/2 + n/4 + \cdots \le 2n = O(n)$。

### 空间复杂度

| 数据 | 空间 | 说明 |
|---|---|---|
| 数组 A | $O(n)$ | 原地修改 |
| 递归栈 | $O(\log n)$ | 平均递归深度 |
| **总计** | $O(n)$ | 数组主导 |

## 适用场景

- **无序数组找第 $k$ 小/大**：经典 Top-K 问题，$O(n)$ 比排序后取 $O(n \log n)$ 更快
- **求中位数**：$k = \lceil n/2 \rceil$，比排序更高效
- **求前 $k$ 小的元素**：配合划分，可以把前 $k$ 小的元素放到数组前 $k$ 个位置（不一定有序）
- **数据流中位数**：配合堆或直接用快速选择，处理静态数据
- **不适合动态数据**：数组会被原地修改，若需要保持原序需要拷贝

## 常见陷阱与注意事项

1. **原地修改**
   - 快速选择会**打乱原数组**，元素位置会被交换
   - 若需要保持原数组，必须先拷贝一份

2. **$k$ 的语义是下标还是排名**
   - 代码中 $k$ 是**下标**（1-based），$k=1$ 表示最小，$k=n$ 表示最大
   - 有些实现中 $k$ 是 0-based，需要根据题目要求调整

3. **最坏情况退化**
   - 三数取中虽大幅降低最坏情况概率，但并非完全避免
   - 若对最坏情况有严格要求，应使用 BFPRT（Median of Medians），保证 $O(n)$

4. **等于 pivot 的元素处理**
   - `A[ml] < q` 和 `A[mr] > q` 使用严格不等号，等于 pivot 的元素会被交换到两侧
   - 若用 `<=` 和 `>=`，等于 pivot 的元素会集中在一侧，可能导致退化

5. **`ml <= mr` 而非 `ml < mr`**
   - 当 `ml == mr` 时仍需交换（虽然交换自己），目的是让两个指针各自前进一步，否则会死循环

6. **整数溢出**
   - `l + (r - l) / 2` 而非 `(l + r) / 2`：避免大数相加溢出
   - 虽然 32 位下 `l + r` 不会溢出，但这是好习惯

## 对比与扩展

| 方法 | 平均时间 | 最坏时间 | 空间 | 特点 |
|---|---|---|---|---|
| **快速选择（本算法）** | $O(n)$ | $O(n^2)$ | $O(n)$ | 简单高效，原地修改 |
| 排序后取第 $k$ | $O(n \log n)$ | $O(n \log n)$ | $O(n)$ | 简单但慢，不破坏原序（需拷贝） |
| BFPRT（Median of Medians） | $O(n)$ | $O(n)$ | $O(n)$ | 最坏线性，但常数大 |
| 堆（维护大小为 $k$ 的堆） | $O(n \log k)$ | $O(n \log k)$ | $O(k)$ | 适合动态数据流 |
| 桶排序（值域小时） | $O(n + V)$ | $O(n + V)$ | $O(V)$ | 值域 $V$ 小时最快 |

### 扩展

- **BFPRT 算法**：将数组每 5 个分一组，取每组中位数的中位数作为 pivot，保证最坏 $O(n)$
- **C++ `nth_element`**：标准库的快速选择实现，`std::nth_element(first, nth, last)` 将第 $n$ 小的元素放到第 $n$ 个位置
- **Top-K 问题**：快速选择的划分可以直接把前 $k$ 小的元素放到数组前 $k$ 个位置（不保证有序），配合一次排序即可得到有序的前 $k$ 小
- **快速排序**：快速选择是快速排序的"半递归"版本——快速排序递归两半，快速选择只递归一半
