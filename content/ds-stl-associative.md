---
id: ds-stl-associative
title: '关联式容器（set / map）'
category: 数据结构
subcategory: STL容器
subSubcategory: ''
tags: ["数据结构", "STL", "关联式容器", "集合", "映射"]
timeComplexity: '视具体实现'
spaceComplexity: '视具体实现'
level: A
noCode: true
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 算法原理

关联式容器是 C++ 标准模板库（STL）中的一类容器，其内部通常以**红黑树**（一种自平衡二叉搜索树）实现，因此元素会**按键自动排序**，查找、插入、删除等操作的时间复杂度均为 $O(\log n)$。与之相对的是**无序关联式容器**（如 `unordered_set`、`unordered_map`），它们内部通过**哈希表**实现，元素不以任何特定顺序排列。

本节介绍两种最常用的关联式容器：

- **`set`**：元素集合，元素**不重复**（等价于数学意义上的集合），插入时会自动排序。若需要保留相同元素，可使用 **`multiset`**。
- **`map`**：键值对容器，每个键**唯一**，按键排序，用于建立「键 → 值」的映射。若需要允许多个元素拥有同一键，可使用 **`multimap`**。

两者共同的性质：

1. 内部通常通过红黑树实现。
2. 元素按键值自动排序（从小到大）。
3. 插入、删除、查找的时间复杂度均为 $O(\log n)$。

## 核心公式 / 状态定义与转移方程

关联式容器属于「语言工具」而非「算法」，没有状态转移方程；它的核心刻画是各类容器的复杂度差异。设 $n$ 为容器内元素个数：

- `set` / `map`：插入、删除、查找均为 $O(\log n)$。
- `multiset` / `multimap`：与 `set` / `map` 相同，但允许重复键。
- `unordered_set` / `unordered_map`：查找与插入均摊 $O(1)$，最坏 $O(n)$，且元素无序。

由于同一知识点覆盖多种容器、复杂度各不相同，frontmatter 中的复杂度记作「视具体实现」，实际使用时按具体容器取值。

## 逐行代码解析

本节给出 `set` 与 `map` 的关键用法片段，说明接口形态，不要求与任何完整程序一一对应。

### set 的声明与常用操作

```cpp
set<int> s;             // 有序、不重复的整数集合
multiset<int> ms;       // 有序、可重复的整数集合
unordered_set<int> us;  // 无序、不重复的整数集合（哈希实现，不支持二分查找）

// 插入与删除
s.insert(x);               // 若不存在等价元素，则将 x 插入容器
s.erase(x);                // 删除值为 x 的所有元素，返回删除元素的个数
s.erase(pos);              // 删除迭代器 pos 指向的元素，要求迭代器合法
s.erase(first, last);      // 删除迭代器 [first, last) 范围内的所有元素
s.clear();                 // 清空容器

// 查找
s.count(x);                // 返回容器内键为 x 的元素数量（set 中为 0 或 1）
s.find(x);                 // 存在键为 x 的元素时返回其迭代器，否则返回 s.end()
s.lower_bound(x);          // 返回指向首个不小于 x 的元素的迭代器，不存在则返回 s.end()
s.upper_bound(x);          // 返回指向首个大于 x 的元素的迭代器，不存在则返回 s.end()

// 容量
s.empty();                 // 返回容器是否为空
s.size();                  // 返回容器内元素个数
```

要点：

- `multiset` 与 `set` 用法相同；`unordered_set` 内部无序，不支持 `lower_bound` / `upper_bound` 这类二分查找操作。
- `insert` 有返回值，类型为 `pair<iterator, bool>`：迭代器指向被插入的元素（或原本就存在的等值元素），布尔值表示是否插入成功。由于 `set` 元素唯一，插入已存在的等值元素会失败并返回 `false`；`map` 的 `insert` 同理。

### map 的声明与常用操作

```cpp
map<string, int> mp;              // 键为 string、值为 int 的有序映射
multimap<string, int> mmp;        // 允许同一键对应多个值
unordered_map<string, int> ump;   // 无序映射（哈希实现）

// 插入与删除
mp["Alan"] = 100;                               // 下标访问：查询或插入键 "Alan" 并赋值为 100
mp.insert(pair<string, int>("Alan", 100));      // 插入一个 pair<Key, T> 类型的键值对
mp.erase(key);                                  // 删除键为 key 的所有元素，返回删除元素的数量
mp.erase(pos);                                  // 删除迭代器 pos 指向的元素，要求迭代器合法
mp.erase(first, last);                          // 删除迭代器 [first, last) 范围内的所有元素
mp.clear();                                     // 清空整个容器

// 查询
mp.count(x);                // 返回容器内键为 x 的元素数量
mp.find(x);                 // 存在键为 x 的元素时返回其迭代器，否则返回 end()
mp.lower_bound(x);          // 返回指向首个键不小于 x 的元素的迭代器
mp.upper_bound(x);          // 返回指向首个键大于 x 的元素的迭代器

// 容量
mp.empty();                 // 返回容器是否为空
mp.size();                  // 返回容器内元素个数
```

要点：

- 下标访问 `mp[key]` 在键不存在时会**自动插入**一个默认值；若只想查询而不插入，应改用 `mp.find(key)` 或 `mp.count(key)`。
- `count(x)` 返回键为 $x$ 的元素个数，复杂度为 $O(\log n)$ 加上匹配到的元素个数。
- `map` 的元素类型为 `pair<const key, type>`，其中键为常量不可修改，值可以修改。

## 复杂度分析

设 $n$ 为容器内元素个数，各类容器的复杂度对比如下：

| 操作 | `set` / `map` | `multiset` / `multimap` | `unordered_set` / `unordered_map` |
|---|---|---|---|
| 插入 | $O(\log n)$ | $O(\log n)$ | 均摊 $O(1)$，最坏 $O(n)$ |
| 删除 | $O(\log n)$ | $O(\log n)$ | 均摊 $O(1)$，最坏 $O(n)$ |
| 查找（`find`） | $O(\log n)$ | $O(\log n)$ | 均摊 $O(1)$，最坏 $O(n)$ |
| 二分查找（`lower_bound`） | $O(\log n)$ | $O(\log n)$ | 不支持 |
| 空间 | $O(n)$ | $O(n)$ | $O(n)$ |

- `set` / `map` 基于红黑树实现，查找、插入、删除均为 $O(\log n)$，且元素始终有序、支持二分查找。
- `unordered_*` 基于哈希表实现，均摊 $O(1)$，但在哈希冲突严重时可能退化到 $O(n)$，且元素无序、不支持二分查找。

由于不同容器的复杂度各不相同，frontmatter 中的复杂度记作「视具体实现」，实际使用时按上表选取对应的复杂度。

## 适用场景

- **`set`**：需要**去重**且**有序**的元素集合；需要频繁判断「某个值是否存在」、二分查找前驱/后继的场景。
- **`multiset`**：需要保留重复元素且保持有序的场景，如维护可重复的有序序列。
- **`map`**：需要建立「键 → 值」映射且键有序、唯一，如统计出现次数、离散化后的映射、字符串到编号的映射。
- **`multimap`**：同一键对应多个值的映射。
- **`unordered_set` / `unordered_map`**：只关心「是否存在 / 快速取值」而不需要有序，且元素数量较大时，哈希表通常比红黑树更快。

选择建议：默认使用 `set` / `map`（有序、行为可预测）；确认不需要有序且追求更小常数时，改用 `unordered_set` / `unordered_map`；需要保留重复元素时，改用 `multiset` / `multimap`。

## 常见陷阱与注意事项

1. **下标访问会意外插入**：`map` 的 `mp[key]` 在键不存在时会插入一个默认值。若只想判断键是否存在，务必使用 `mp.find(key)` 或 `mp.count(key)`，否则会向容器中误插入元素。
2. **`count` 与 `find` 的语义**：`count(x)` 返回匹配个数，在 `set` / `map` 中只有 $0$ 或 $1$；判断存在性用 `count` 更简洁，但若要取得元素本身，应使用 `find` 返回的迭代器。
3. **`erase` 的重载语义不同**：`s.erase(x)` 删除值为 $x$ 的**所有**元素并返回删除个数；`s.erase(pos)` 删除迭代器指向的单个元素。二者返回值与行为不同，混用容易出错。
4. **`unordered_*` 不支持二分查找**：`unordered_set` / `unordered_map` 内部无序，调用 `lower_bound` / `upper_bound` 会编译失败。
5. **哈希表的最坏复杂度**：`unordered_*` 均摊 $O(1)$，但构造数据可能使其退化为 $O(n)$；在对抗性较强的题目中，使用红黑树实现的 `set` / `map` 往往更稳妥。
6. **`map` 的键不可修改**：`map` 中元素的键类型是 `const key`，不能直接修改；需要改键时应先 `erase` 旧元素，再 `insert` 新键值对。
7. **迭代器失效**：删除元素会使指向被删元素的迭代器失效；遍历过程中如需删除元素，应先保存下一个迭代器再删除当前元素。

## 对比与扩展

- **`set` vs `unordered_set`**：`set` 基于红黑树，有序、支持二分查找，复杂度 $O(\log n)$；`unordered_set` 基于哈希表，无序、均摊 $O(1)$。需要有序或二分查找时用 `set`，只关心存在性且追求常数时用 `unordered_set`。
- **`set` vs `multiset`**：`set` 元素唯一，`multiset` 允许重复；需要「有序可重复集合」时用 `multiset`。
- **`map` vs `unordered_map`**：与 `set` 对比同理，`map` 有序，`unordered_map` 无序但均摊更快。
- **关联式容器 vs 序列式容器**：序列式容器（如 `vector`）按下标访问、元素按插入顺序排列；关联式容器按键自动排序、通过键查找，二者适用场景不同。
- **扩展方向**：`set` 的「有序 + 可二分查找」性质使其常被用于维护动态有序集合，进而衍生出如珂朵莉树（基于 `set` 维护区间）等数据结构；`map` 可用于离散化、统计、建立各类映射关系。无序关联式容器的底层是哈希表，其细节可参考哈希表相关知识。
