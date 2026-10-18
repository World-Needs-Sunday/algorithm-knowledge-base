---
id: ds-stl-tips
title: 'STL 小妙招'
category: 数据结构
subcategory: STL容器
subSubcategory: ''
tags: ["数据结构", "STL", "vector", "set", "map"]
timeComplexity: '视具体实现'
spaceComplexity: '视具体实现'
level: A
noCode: true
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 算法原理

STL（Standard Template Library，标准模板库）的「小妙招」，本质是**利用容器的内部实现细节与成员函数的返回值**，写出更短、更快、更不易出错的代码。核心是三条认知：

1. **容器内部结构决定复杂度**：`vector` 是连续内存的动态数组（随机访问 $O(1)$、末尾插入均摊 $O(1)$）；`set` / `map` 内部是红黑树（查找、插入、删除均为 $O(\log n)$ 且元素有序）；`unordered_set` / `unordered_map` 内部是哈希表（平均 $O(1)$、元素无序）。
2. **接口返回值是「免费的信息」**：许多成员函数（如 `set::insert`、`map::insert`）其实有返回值，善用它们可以省掉一次额外的查找。
3. **「均摊」不等于「每次」**：`vector` 的 `push_back` 均摊 $O(1)$，但单次扩容时是 $O(n)$；理解扩容机制，才能在需要稳定性能时用 `reserve` 提前避免。

掌握这些「小妙招」不需要死背 API，而是**把容器的内部模型想清楚**：数据存在哪、插入删除会动到哪、查找靠什么。

## 核心公式 / 状态定义与转移方程

> 本节是「语言/工具类」知识点，没有状态转移方程，取而代之的是几条最常用的「口诀/事实」，是理解并套用下列妙招的关键。

**关键事实（口诀）：**

- `vector` 扩容：容量不足时按约 $2$ 倍增长，因此 `push_back` 均摊 $O(1)$，单次最坏 $O(n)$。
- `set` / `map` 的 `insert` 返回 `pair<iterator, bool>`：`second == true` 表示插入成功（原容器中没有等值元素），`false` 表示已存在、插入失败。
- 有序容器（`set` / `map`）的查找、插入、删除为 $O(\log n)$；无序容器（`unordered_*`）平均 $O(1)$、最坏 $O(n)$。

**常用复杂度速查（标准库典型实现）：**

| 操作 | `vector` | `set` / `map` | `unordered_set` / `unordered_map` |
|---|---|---|---|
| 随机访问 / 按下标 | $O(1)$ | 不支持 | 不支持 |
| 查找 `find` / `count` | $O(n)$ | $O(\log n)$ | 平均 $O(1)$ |
| 插入 / 删除 | 末尾均摊 $O(1)$ | $O(\log n)$ | 平均 $O(1)$ |
| 是否有序 | 保持插入顺序 | 有序 | 无序 |

## 逐行代码解析

> 下面均为「小妙招」的关键用法片段，用于表达技巧本身；正式使用请按需补充头文件与具体类型。

```cpp
// 妙招 1：vector 提前 reserve，避免反复扩容带来的整块拷贝
vector<int> v;
v.reserve(1000000);          // 预分配 100 万个元素的容量，之后插入不再扩容
for (int i = 0; i < 1000000; ++i)
    v.push_back(i);          // 均摊 O(1)，全程只发生一次内存分配

// 妙招 2：用一个空 vector 交换，立即回收多余容量
vector<int>().swap(v);       // 传统写法：交换空 vector，立刻释放容量
// v.shrink_to_fit();        // C++11 起可用，语义更直白

// 妙招 3：set/map 的 insert 返回值 —— 判断是否真的插入了
set<int> s;
auto res = s.insert(5);      // res.first 是指向元素 5 的迭代器
if (res.second) {            // res.second == true 表示插入成功
    // 之前没有 5，已插入
} else {
    // 之前已有 5，插入失败，res.first 指向原有的 5
}

// 妙招 4：判断键是否存在，用 find 而不是 count
if (s.find(x) != s.end()) { /* 存在 */ }
// 对 multiset/multimap，find 只找一个，count 却要数完全部等价元素，故 find 更快

// 妙招 5：有序容器用成员函数 lower_bound/upper_bound，比全局函数更快
auto it = s.lower_bound(x);  // 首个 >= x 的元素，利用红黑树结构 O(log n)

// 妙招 6：优先队列实现小根堆 / 自定义比较
priority_queue<int, vector<int>, greater<int>> q;   // 小根堆
```

- 妙招 1、2：`vector` 扩容要把旧元素整体搬到新内存，元素多或元素是自定义类时开销大；`reserve` 一次到位、`shrink_to_fit` 回收冗余。
- 妙招 3：`insert` 的返回值把「查找 + 插入」合并成一次操作，避免先 `find` 再 `insert` 的两遍遍历。
- 妙招 4：`count` 的语义是「数有几个等价元素」，对 `multiset` 可能退化为 $O(n)$；判断「是否存在」一律用 `find`。
- 妙招 5：`std::lower_bound` 依赖随机访问迭代器，对 `set` / `map` 反而要线性遍历；成员函数版本利用内部结构做到 $O(\log n)$。
- 妙招 6：`greater<int>` 是标准库比较器，注意写全第二个模板参数 `vector<int>`，否则无法变成小根堆。

## 复杂度分析

STL 各容器的复杂度由「标准库典型实现」决定，此处给出常见值（不同实现可能略有差异，故整体标注「视具体实现」）：

- `vector`：随机访问、末尾 `push_back` / `pop_back` 均摊 $O(1)$；中间插入 / 删除 $O(n)$；`reserve` 提前分配后，批量插入可稳定 $O(1)$。
- `set` / `map`：基于红黑树，查找、插入、删除均为 $O(\log n)$，且保持有序。
- `multiset` / `multimap`：同 `set` / `map`，但 `count` 为 $O(\log n + k)$（$k$ 为等价元素个数），`find` 仍为 $O(\log n)$。
- `unordered_set` / `unordered_map`：哈希表，查找 / 插入 / 删除平均 $O(1)$，最坏 $O(n)$（哈希冲突严重时）。
- `priority_queue`：`push` / `pop` 为 $O(\log n)$，`top` 为 $O(1)$。

记忆重点：**「有序」与「$O(\log n)$」相伴（红黑树），「无序」与「平均 $O(1)$」相伴（哈希）**；判断存在用 `find`，需要数个数才用 `count`。

## 适用场景

- **`vector`**：需要随机访问、尾部增删、内存紧密排列的场景（如邻接表、DP 滚动数组、结果缓存）；配合 `reserve` 用于已知规模的批量插入。
- **`set` / `map`**：需要「有序 + 去重」或「按 key 有序遍历」的场景（如需要 `lower_bound` 查询、排名、前驱后继）。
- **`unordered_set` / `unordered_map`**：只关心「是否存在 / 快速映射」、不关心顺序，且 key 可哈希的场景；平均更快，但最坏有退化风险。
- **`priority_queue`**：需要动态维护最值（堆、Dijkstra、贪心）的场景。
- **`insert` 返回值、`find` / `count` 选择、成员函数二分**：任何用到关联容器的代码里都可以套用，属于「顺手就能变快 / 变稳」的通用技巧。

## 常见陷阱与注意事项

1. **忽略 `insert` 返回值**：`set` / `map` 的 `insert` 不会覆盖已有元素，若想「插入或更新」，应先判断 `res.second`，否则可能误以为更新成功。
2. **用 `count` 判断存在**：`multiset` / `multimap` 中 `count` 会数遍所有等价元素，最坏 $O(n)$；判断存在应使用 `find`。
3. **对有序容器用全局二分**：`std::lower_bound` / `std::upper_bound` 依赖随机访问，对 `set` / `map` 会退化为 $O(n)$；应调用容器的成员函数版本。
4. **`unordered_*` 的最坏退化**：哈希冲突严重（或遭遇刻意的碰撞构造）时单次操作退化为 $O(n)$；需要严格复杂度保证时选 `map` / `set`。
5. **`vector` 扩容与迭代器失效**：`push_back` 触发扩容会使所有迭代器失效；`erase` / `insert` 会使其后的迭代器失效，遍历中删除元素应使用返回的迭代器 `it = v.erase(it)`。
6. **`reserve` 不会改变 `size`**：`reserve(n)` 只增加容量、不增加元素个数，只有 `i < size()` 的下标才安全；误把 `reserve` 当 `resize` 会越界。
7. **容器语义混淆**：`unordered_*` 无序遍历、`set` 去重而 `multiset` 不去重，选错容器会引入隐蔽 bug。

## 对比与扩展

| 容器 | 内部结构 | 查找 | 插入 / 删除 | 有序 | 允许重复 |
|---|---|---|---|---|---|
| `vector` | 动态数组 | $O(n)$ | 尾部均摊 $O(1)$ | 插入顺序 | 是 |
| `set` | 红黑树 | $O(\log n)$ | $O(\log n)$ | 是 | 否 |
| `multiset` | 红黑树 | $O(\log n)$ | $O(\log n)$ | 是 | 是 |
| `unordered_set` | 哈希表 | 平均 $O(1)$ | 平均 $O(1)$ | 否 | 否 |

扩展方向：

- **自定义 key 的比较与哈希**：为自定义结构体写 `operator<`（用于 `set` / `map`），或特化 `hash` 并写 `operator==`（用于 `unordered_*`）。
- **`multiset` / `multimap`**：需要「有序且可重复」时使用，配合 `equal_range` 一次拿到所有等价元素。
- **平板电视（`__gnu_pbds`）**：提供 `tree`、`hash_map` 等扩展容器，支持 `order_of_key`、`find_by_order` 等按排名查询。
- **结构化绑定**：遍历 `map` 时用 `for (auto &[k, v] : mp)`（C++17），可读性更好。
