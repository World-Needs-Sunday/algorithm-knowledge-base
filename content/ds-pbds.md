---
id: ds-pbds
title: 'pb_ds（基于策略的数据结构）'
category: 数据结构
subcategory: STL容器
subSubcategory: ''
tags: ['数据结构', '平衡树', '配对堆', '红黑树', '优先队列']
timeComplexity: '视具体实现'
spaceComplexity: '视具体实现'
level: A
noCode: true
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 算法原理

pb_ds（Policy-Based Data Structures，基于策略的数据结构）是 GCC 自带的扩展库，封装了平衡树、堆、字典树（Trie）与哈希表四类数据结构，功能强大且实现现成，省去手写的麻烦。竞赛中最常用的两个部分是：

1. **平衡树 `tree`**：一个"免写平衡树"，把几十甚至上百行的手写平衡树压缩到十几行。
2. **配对堆 `priority_queue`**：补齐了 `std::priority_queue` 的短板——可以修改堆内已有元素、删除任意元素，并且支持 $O(1)$ 合并两个堆。

"基于策略"的含义是：底层行为由模板参数决定——`Tag` 参数选择底层实现（如红黑树、配对堆），`Node_Update` 参数选择节点更新策略（如是否启用排名统计）。同一套接口，换一个模板参数就能切换实现，这正是它灵活的原因。

关于 NOI 系列竞赛的合规性：2021 年 9 月 1 日发布的《关于 NOI 系列活动中编程语言使用限制的补充说明》允许使用以下划线开头的库函数或宏（明确禁止操作的除外），这让 pb_ds 库的使用有了文件依据，在 NOI 系列竞赛中属于**完全允许**的范围。

需要强调的是，pb_ds 只在以 libstdc++ 为标准库的编译器（如 GCC / MinGW）下可用；在部分竞赛使用的 Dev-C++ 等环境中往往无法通过本地编译，导致代码难以本地调试，使用前需确认评测环境与本地环境一致。

## 核心公式 / 状态定义与转移方程

### 模板参数定义

平衡树 `tree` 的完整定义：

```cpp
tree<Key, Mapped, Cmp_Fn, Tag, Node_Update, Allocator>
```

- `Key`：存储的元素类型（自动去重）。
- `Mapped`：映射值类型；作 set 用时填 `null_type`，作 map 用时填具体值类型。
- `Cmp_Fn`：键的比较函子，决定元素在树中的排序规则，如 `less<Key>` 或 `greater<Key>`。
- `Tag`：底层数据结构，平衡树一般选用红黑树 `rb_tree_tag`。
- `Node_Update`：节点更新策略，填 `tree_order_statistics_node_update` 可启用排名统计，默认不启用。
- `Allocator`：空间配置器，使用默认值即可。

配对堆 `priority_queue` 的完整定义：

```cpp
__gnu_pbds::priority_queue<T, Compare, Tag, Allocator>
```

- `T`：存储的元素类型（支持自定义类型）。
- `Compare`：提供严格弱序的比较类型。
- `Tag`：堆的实现类型，默认为 `pairing_heap_tag`（配对堆）。
- `Allocator`：空间配置器，一般用不到。

### 排名与顺序统计

设树中已含 $n$ 个元素，$x$ 为待查询的键：

- `order_of_key(x)` 返回树中**严格小于** $x$ 的元素个数，记为 $k$。因此 $x$ 在树中的排名（从 $1$ 计）为 $k+1$。
- `find_by_order(k)` 返回从 $0$ 开始计的第 $k$ 个元素（即第 $k+1$ 小）的迭代器；当 $k \ge n$ 时返回 `end()`。

这两个操作互为逆操作：若 `find_by_order(k)` 指向元素 $x$，则 `order_of_key(x)` 返回 $k$。

### 复杂度关系

- 平衡树（红黑树）：所有查询与修改操作均为 $O(\log n)$。
- 配对堆：`push`、`top`、`join` 为 $O(1)$；`pop`、`modify`、`erase` 最坏 $O(n)$，均摊 $O(\log n)$。

## 逐行代码解析

### 头文件与命名空间

```cpp
#include <bits/extc++.h>    // 同时包含 STL、pb_ds 与 <bits/stdc++.h> 的全部内容
using namespace __gnu_pbds; // pb_ds 的通用命名空间
```

pb_ds 主要位于 `ext/pb_ds/assoc_container.hpp` 与 `ext/pb_ds/tree_policy.hpp` 两个头文件，`<bits/extc++.h>` 一次把它们连同 STL 全部引入。需要注意的是，`__gnu_pbds::priority_queue` 与 `std::priority_queue` 同名冲突，不要同时打开 `using namespace std;` 与 `using namespace __gnu_pbds;`。

### 配对堆用法

```cpp
__gnu_pbds::priority_queue<int> pq;   // 默认配对堆，大根堆
pq.push(x);                            // O(1) 压入，返回指向该元素的迭代器
int t = pq.top();                      // O(1) 取堆顶
pq.pop();                              // 弹出堆顶，均摊 O(log n)
pq.modify(it, new_key);                // 修改迭代器 it 处键值并重新调整
pq.erase(it);                          // 删除迭代器 it 处的元素
pq.join(other);                        // O(1) 合并 other，并把 other 清空
```

配对堆不仅支持普通堆的操作，还多了修改、删除任意元素与 $O(1)$ 合并，因此它同时是一个**可并堆**，很多场景下不必再手写左偏树。

### 平衡树用法

```cpp
tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update> tr;
tr.insert(x);                          // O(log n) 插入（自动去重）
auto it = tr.find(x);                  // 查找 x，找不到返回 end()
int k = tr.order_of_key(x);            // 严格小于 x 的元素个数
auto it2 = tr.find_by_order(k);        // 第 k（0 基）个元素的迭代器
tr.erase(x);                           // 删除所有等于 x 的元素
```

`tree` 的 `Key` 会自动去重；若需要保留重复元素（即模拟 multiset），常用 `pair<int, int>` 作为键，用第二维编号区分相同元素，例如 `tree<pair<int,int>, null_type, less<pair<int,int>>, rb_tree_tag, tree_order_statistics_node_update>`。

## 复杂度分析

### 配对堆

| 操作 | 说明 | 复杂度 |
|---|---|---|
| `push(x)` | 压入元素 | $O(1)$ |
| `top()` | 取堆顶 | $O(1)$ |
| `pop()` | 弹出堆顶 | 最坏 $O(n)$，均摊 $O(\log n)$ |
| `modify(it, k)` | 修改迭代器处键值并调整 | 最坏 $O(n)$，均摊 $O(\log n)$ |
| `erase(it)` | 删除迭代器处元素 | 最坏 $O(n)$，均摊 $O(\log n)$ |
| `join(other)` | 合并另一堆并清空 | $O(1)$ |

### 平衡树

| 操作 | 说明 | 复杂度 |
|---|---|---|
| `insert(x)` | 插入元素 | $O(\log n)$ |
| `erase(x)` / `erase(it)` | 删除元素 | $O(\log n)$ |
| `find(x)` | 查找元素 | $O(\log n)$ |
| `lower_bound(x)` / `upper_bound(x)` | 二分定位 | $O(\log n)$ |
| `order_of_key(x)` | 严格小于 x 的元素个数 | $O(\log n)$ |
| `find_by_order(k)` | 第 k（0 基）个元素 | $O(\log n)$ |
| `join(other)` | 合并两树（值域不相交） | 理论 $O(\log n)$，实测常数较大 |
| `split(k, other)` | 分裂出大于 k 的部分 | 理论 $O(\log^2 n)$，可能退化 $O(n)$ |

空间复杂度：两者均随元素个数线性增长，为 $O(n)$。红黑树每个节点需携带颜色与若干指针，常数略大于手写结构，但在竞赛中通常可接受。

## 适用场景

**适用**：

- 需要平衡树、但不想手写 Splay / Treap 的场景（NOI 系列竞赛已允许使用）。
- 需要第 $k$ 小查询与排名统计（`order_of_key` / `find_by_order`）时，`std::set` 不支持，pb_ds 原生支持。
- 需要删除任意元素、修改堆内元素、$O(1)$ 合并堆的场景（配对堆）。
- 元素可去重、且评测环境确定为 libstdc++ 的竞赛代码。

**不适用**：

- 评测机或本地环境不是 libstdc++（如 MSVC、部分旧 Dev-C++）时，代码无法编译。
- 需要严格本地调试、而本地编译器不支持 pb_ds 时。
- 需要自定义维护（如子树和、懒标记）时，标准 `tree` 只提供排名统计，需手写平衡树。
- 对常数敏感且频繁使用 `split` 时，其实际表现可能退化。

## 常见陷阱与注意事项

1. **命名空间冲突**
   - `__gnu_pbds::priority_queue` 与 `std::priority_queue` 同名，同时 `using namespace std;` 与 `using namespace __gnu_pbds;` 会产生二义性。
   - 建议只在需要的范围内打开 `__gnu_pbds`，或全程使用 `__gnu_pbds::` 前缀。

2. **本地编译失败**
   - pb_ds 依赖 libstdc++，非 GCC 系编译器或部分旧版 Dev-C++ 上无法通过编译。
   - 写题前先确认本地与评测环境一致；本地无法调试时改用等价手写结构。

3. **配对堆默认只用配对堆标签**
   - pb_ds 提供五种堆实现，除配对堆外的四种（如 `binary_heap_tag` 等）在竞赛中常因常数过大或内存占用过高而表现不佳，默认一律使用配对堆。

4. **`order_of_key` 的排名语义**
   - 返回的是**严格小于** key 的个数，等于"排名减 $1$"；要用 $1$ 基排名需再加 $1$。
   - 未启用 `tree_order_statistics_node_update` 时，`order_of_key` 与 `find_by_order` 无法使用（编译错误）。

5. **去重与 multiset 模拟**
   - `tree` 的 `Key` 自动去重；需要保留重复元素时，用 `pair<值, 编号>` 作为键。

6. **`join` / `split` 的前置条件**
   - `join(other)` 要求两棵树的值域不相交，合并后 `other` 被清空。
   - `split(k, other)` 要求 `other` 调用前为空；实测常数较大，极端情况下可能退化。

7. **头文件与命名空间的选择**
   - 一般用 `#include <bits/extc++.h>` 一次引入全部内容；也可仅包含 `ext/pb_ds/assoc_container.hpp` 与 `ext/pb_ds/tree_policy.hpp`。

## 对比与扩展

### 平衡树对比

| 对比维度 | pb_ds `tree` | `std::set` / `std::map` | 手写 Splay / Treap |
|---|---|---|---|
| 第 k 小 / 排名 | 原生支持 $O(\log n)$ | 不支持（需另写） | 需自行维护 size |
| 插入 / 删除 / 查找 | $O(\log n)$ | $O(\log n)$ | $O(\log n)$（均摊） |
| 代码量 | 极低（十几行） | 低 | 高（几十到上百行） |
| 自定义维护 | 弱（仅排名统计） | 不支持 | 强（子树和、懒标记等） |
| 可移植性 | 仅 libstdc++ | 标准库，全平台 | 全平台 |

### 堆对比

| 对比维度 | pb_ds 配对堆 | `std::priority_queue` | 左偏树 |
|---|---|---|---|
| $O(1)$ 合并 | 支持 | 不支持 | 支持 |
| 删除任意元素 | 支持 | 不支持 | 支持 |
| 修改堆内元素 | 支持 | 不支持 | 一般需删除后重插 |
| 实现成本 | 低 | 低 | 中 |
| 可移植性 | 仅 libstdc++ | 标准库 | 全平台 |

**扩展**：

- pb_ds 还提供哈希表（`gp_hash_table` 与 `cc_hash_table`，前者更常用且更快）与字典树（`trie`），在需要高性能哈希或前缀匹配时可选用。
- 需要"子树和 / 懒标记"等自定义维护时，pb_ds `tree` 无法胜任，应改用 FHQ-Treap / Splay 等手写结构。
- 配对堆的 $O(1)$ 合并使其成为可并堆的现成替代，适用于需要动态合并集合的堆合并类问题。
