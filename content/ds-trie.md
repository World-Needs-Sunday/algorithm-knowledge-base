---
id: ds-trie
title: '字典树（Trie）— 前缀检索的标配结构'
category: '数据结构'
subcategory: '字典树'
tags: ['数据结构', '字典树', 'Trie', '前缀树', '前缀匹配', '哈希表']
timeComplexity: 'O(|S|)'
spaceComplexity: 'O(N * |S|)'
codePath: '数据结构\字典树\源.cpp'
description: '字典树（Trie，又称前缀树）将若干字符串按公共前缀组织成一棵树，插入和查询均沿字符逐层下行，单次操作 O(|S|)。本实现用 vector<node> 数组存节点，每个节点的子边用 unordered_map<char,int> 映射，兼顾内存与灵活性；支持动态扩容和 O(|S|) 的前缀计数查询。'
---

## 一、算法原理

### 1.1 核心思想

如果有 $n$ 个字符串需要频繁做"查询某个前缀出现了多少次"的操作，朴素做法是每次线性扫描所有字符串——$O(n \times |S|)$。字典树把这 $n$ 个字符串的**公共前缀合并到同一条路径上**，使得插入和查询都只需遍历一次目标串的字符序列。

### 1.2 定义

字典树是一棵多叉树（通常字符集为 26 个字母或更大）：

- **根节点**（本实现编号为 1）代表"空前缀"
- 每条边上标一个字符
- 从根到某节点的路径上的字符序列 = 该节点代表的字符串前缀
- 每个节点维护一个 `cnt`：**有多少个插入的字符串以此节点为前缀终点**（即经过此节点的字符串数）

### 1.3 关键性质

1. **公共前缀共享路径**：`"apple"` 和 `"app"` 共享 `a → p → p` 三层节点
2. `cnt` 的语义是"经过此节点的字符串总数"，所以查询前缀 `"app"` 返回的就是 `app` 终点的 `cnt`
3. 插入一个字符串 $s$ 时，路径上每个节点的 `cnt` 都 +1

## 二、数据结构图示

### 2.1 插入 "ab", "abc", "ab", "ac" 后的 Trie

```
           [1] root
          /   \
        a/     \a
        /       \
      [2]       [3]
      cnt=3     cnt=1
      / \         |
    b/   \c      \c
    /     \       \
  [4]     [6]     [7]
  cnt=3   cnt=0   cnt=1
  / \
c/   \(end)
/
[5]
cnt=1
```

- `root [1]`：空串，不存 `cnt`（或视为 0）
- `a [2]`：3 个字符串以 `a` 开头 → `cnt=3`（"ab", "abc", "ab"）
- `ab [4]`：3 个字符串以 `ab` 开头 → `cnt=3`
- `abc [5]`：1 个字符串以 `abc` 开头 → `cnt=1`
- `ac [7]`：1 个字符串以 `ac` 开头 → `cnt=1`

### 2.2 查询过程示例

查询 `"ab"`：`root → a(2) → b(4)`，返回 `tree_arr[4].cnt = 3`。
查询 `"ad"`：`root → a(2)`，从节点 2 找不到 `d` 的边 → 返回 0。

## 三、核心操作详解

### 3.1 insert(str) — 插入字符串

```
cur = 1 (root)
for each ch in str:
    nxt = tree_arr[cur].find(ch)   // 在 cur 的子边中找 ch
    if nxt == -1:                  // 没找到 → 新建节点
        nxt = ++idx
        tree_arr[cur].add(ch, nxt)
    tree_arr[nxt].cnt++            // 经过此节点的字符串数 +1
    cur = nxt
```

每插入一个字符，要么沿着已有边走，要么新建一条边。`cnt` 在"经过"时递增，保证查前缀时终点 `cnt` = 以该前缀开头的字符串总数。

### 3.2 find(str) — 查询前缀出现次数

```
cur = 1 (root)
for each ch in str:
    cur = tree_arr[cur].find(ch)
    if cur == -1: return 0         // 路径断了 → 前缀不存在
return tree_arr[cur].cnt           // 返回终点节点的 cnt
```

逐字符下行，任一步找不到边就返回 0；全部走完返回终点 `cnt`。

### 3.3 clear() — 清空 Trie

```
for i = idx downto 0:
    tree_arr[i].son.clear()
    tree_arr[i].cnt = 0
idx = 1                            // 重置为只有 root
```

从最高编号节点往下清，只清用过的节点（idx 以内），不遍历整个数组。重置 `idx = 1` 使 root 复活。

### 3.4 动态扩容机制

```cpp
if (idx >= tree_arr.size() - 1)
    tree_arr.insert(tree_arr.end(), new_cap, node{});
```

当节点编号逼近数组容量上限时，一次性追加 `new_cap = 128` 个空节点。批量扩容比逐个 `push_back` 更高效（减少重分配次数）。

## 四、逐行代码解析

### 4.1 node 结构

```cpp
struct node
{
	int cnt;
	unordered_map<char, int> son;
	node() :cnt(0) {}
	int find(char ch)
	{
		auto it = son.find(ch);
		return it == son.end() ? -1 : it->second;
	}
	void add(char ch, int rt) { son.emplace(ch, rt); }
};
```

| 行 | 代码 | 解析 |
|----|------|------|
| 2 | `int cnt;` | 经过此节点的字符串数 |
| 3 | `unordered_map<char, int> son;` | 子边映射：字符 → 子节点编号。用 `unordered_map` 而非 `int[26]`，字符集不限于小写字母，支持任意字符 |
| 4 | `node() :cnt(0) {}` | 构造函数初始化 `cnt` 为 0，`son` 默认空 |
| 5-9 | `find(ch)` | 在子边中查找字符 `ch`，找到返回子节点编号，找不到返回 -1 |
| 10 | `add(ch, rt)` | 添加一条边 `ch → rt` |

### 4.2 strtree 结构

```cpp
struct strtree
{
	int idx;
	vector<node> tree_arr;
	strtree(int big) :idx(1) { tree_arr.resize(big); }
```

| 行 | 代码 | 解析 |
|----|------|------|
| 2 | `int idx;` | 当前最大已分配节点编号。root 是 1，`idx` 从 1 开始 |
| 3 | `vector<node> tree_arr;` | 节点数组，下标即节点编号 |
| 4 | `strtree(int big) :idx(1) { tree_arr.resize(big); }` | 预分配 `big` 个节点空间，`idx` 从 1 开始（0 不用） |

### 4.3 insert 函数

```cpp
void insert(const string& str)
{
	int cur = 1;
	for (char ch : str)
	{
		int nxt = tree_arr[cur].find(ch);
		if (nxt == -1)
		{
			if (idx >= tree_arr.size() - 1) tree_arr.insert(tree_arr.end(), new_cap, node{});
			nxt = ++idx;
			tree_arr[cur].add(ch, nxt);
		}
		++tree_arr[nxt].cnt;
		cur = nxt;
	}
}
```

| 行 | 代码 | 解析 |
|----|------|------|
| 2 | `int cur = 1;` | 从 root 开始 |
| 4 | `int nxt = tree_arr[cur].find(ch);` | 在当前节点的子边中找 `ch` |
| 5-10 | `if (nxt == -1)` | 找不到 → 扩容检查 + 新建节点 + 添加边 |
| 6 | `if (idx >= tree_arr.size() - 1)` | 容量检查：`idx` 离数组末尾只剩 1 的余量时触发扩容 |
| 7 | `tree_arr.insert(tree_arr.end(), new_cap, node{});` | 批量追加 128 个空节点 |
| 8 | `nxt = ++idx;` | 分配新节点编号（先自增再赋值） |
| 9 | `tree_arr[cur].add(ch, nxt);` | 在当前节点添加边 |
| 12 | `++tree_arr[nxt].cnt;` | 新节点或已有节点的 `cnt` 都 +1 |
| 13 | `cur = nxt;` | 下行到子节点 |

### 4.4 find 函数

```cpp
int find(const string& str)
{
	int cur = 1;
	for (char ch : str)
	{
		cur = tree_arr[cur].find(ch);
		if (cur == -1) return 0;
	}
	return tree_arr[cur].cnt;
}
```

| 行 | 代码 | 解析 |
|----|------|------|
| 2 | `int cur = 1;` | 从 root 开始 |
| 4 | `cur = tree_arr[cur].find(ch);` | 沿字符边下行 |
| 5 | `if (cur == -1) return 0;` | 任一字符找不到边 → 前缀不存在，返回 0 |
| 7 | `return tree_arr[cur].cnt;` | 走完全程 → 返回终点 `cnt` |

### 4.5 clear 函数

```cpp
void clear()
{
	for (int i = idx; i >= 0; i--)
	{
		tree_arr[i].son.clear();
		tree_arr[i].cnt = 0;
	}
	idx = 1;
}
```

| 行 | 代码 | 解析 |
|----|------|------|
| 3 | `for (int i = idx; ...)` | 从最高编号往下清，只遍历用过的节点 |
| 5 | `tree_arr[i].son.clear();` | 清空子边映射 |
| 6 | `tree_arr[i].cnt = 0;` | 重置计数 |
| 8 | `idx = 1;` | 重置为只有 root 可用 |

### 4.6 main 函数

```cpp
strtree tree(sz);
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int t;
	cin >> t;
	while (t--)
	{
		int n, q;
		cin >> n >> q;
		tree.clear();
		for (int i = 1; i <= n; i++)
		{
			string str;
			cin >> str;
			tree.insert(str);
		}
		for (int i = 1; i <= q; i++)
		{
			string str;
			cin >> str;
			cout << tree.find(str) << '\n';
		}
	}
	return 0;
}
```

| 行 | 代码 | 解析 |
|----|------|------|
| 1 | `strtree tree(sz);` | 全局 Trie 对象，预分配 $10^5 + 10$ 个节点 |
| 8 | `tree.clear();` | 每组测试前清空 |
| 10-14 | 插入 $n$ 个字符串 | |
| 16-20 | 查询 $q$ 个前缀 | 输出每个前缀的出现次数 |

## 五、复杂度分析

| 操作 | 时间复杂度 | 说明 |
|------|------------|------|
| `insert(s)` | $O(\|s\|)$ | 每字符一次 `unordered_map` 查找/插入，均摊 $O(1)$ |
| `find(s)` | $O(\|s\|)$ | 每字符一次 `unordered_map` 查找 |
| `clear()` | $O(\text{节点总数})$ | 只清用过的节点 |

**空间复杂度**：$O(N \times \bar{|s|})$，其中 $N$ 为字符串总数，$\bar{|s|}$ 为平均长度。每个节点有一个 `unordered_map`，常数因子比数组版大。

**与数组版 Trie 的对比**：

| 维度 | 数组版 `int son[maxN][26]` | 本版 `unordered_map` |
|------|--------------------------|---------------------|
| 单次操作 | $O(\|s\|)$（直接下标） | $O(\|s\|)$（哈希查找） |
| 空间 | $O(\text{maxN} \times 26 \times 4)$ 字节 | $O(\text{实际节点数})$ |
| 字符集 | 固定 26 | 任意字符 |
| 常数 | 小 | 较大（哈希开销） |
| 适用场景 | 字符集小且固定 | 字符集大或不确定 |

## 六、适用场景

| 场景 | 说明 |
|------|------|
| 前缀计数 | 统计以某前缀开头的字符串个数（本代码的直接功能） |
| 字符串检索 | 判断某字符串是否在集合中（终点 `cnt > 0`） |
| 自动补全 | 找到前缀节点后遍历子树，列出所有候选词 |
| 词频统计 | 重复插入同一字符串，`cnt` 自然累加 |
| 最长公共前缀 | 两字符串在 Trie 上的公共路径长度 |
| AC 自动机 | Trie + BFS 构造 fail 指针，多模式串匹配 |

## 七、常见陷阱与注意事项

### 1. `unordered_map` 的常数因子

`unordered_map` 虽然均摊 $O(1)$，但常数比数组大 3-5 倍。如果字符集确定且小（如 26 个小写字母），改用 `int son[26]` 数组版更快。

### 2. `cnt` 的语义：是"经过"不是"终点"

本实现中 `cnt` 记录的是**经过此节点的字符串数**，不是"在此节点结束的字符串数"。如果要统计"以某字符串为完整单词的次数"，需要额外维护一个 `end_cnt` 字段，在插入结束时 +1。

### 3. clear 只清用过的节点

`clear()` 从 `idx` 往下清，不遍历整个 `tree_arr`。如果忘记重置 `idx = 1`，旧节点的残留边会污染下一组数据。

### 4. 扩容条件用 `>=` 不是 `>`

`if (idx >= tree_arr.size() - 1)` 确保在 `idx` 到达最后一个可用位置之前就扩容。如果用 `>`，当 `idx == size - 1` 时再 `++idx` 会变成 `size`，越界。

### 5. `emplace` 不会覆盖已有键

`son.emplace(ch, rt)` 在键已存在时不会更新值。由于 `find` 先于 `add` 调用，已存在的边不会走到 `add` 分支，所以没有问题。但如果重构代码时改变了调用顺序，需注意这个语义。

### 6. 全局对象 vs 局部对象

本代码的 `tree` 是全局对象，构造函数在 `main` 之前执行，预分配 $10^5 + 10$ 个 `node`。如果放局部变量且 $n$ 很大，栈空间可能不够——全局或堆分配更安全。

### 7. `unordered_map` 的迭代器稳定性

`unordered_map` 在 rehash 时迭代器失效。本代码不跨调用持有迭代器，没有问题。但如果后续扩展需要遍历子节点做 DFS，应先收集到 `vector` 再递归。

## 八、对比与扩展

### 8.1 三种 Trie 实现对比

| 维度 | 数组版 `int[26]` | `unordered_map` 版（本代码） | `map` 版 |
|------|-----------------|---------------------------|---------|
| 单次操作 | $O(\|s\|)$ | $O(\|s\|)$ | $O(\|s\| \log |\Sigma|)$ |
| 空间 | $O(\text{maxN} \times 26)$ | $O(\text{实际节点})$ | $O(\text{实际节点})$ |
| 字符集 | 固定 26 | 任意 | 任意（有序） |
| 常数 | 最小 | 中等 | 较大 |
| 适用 | 竞赛首选 | 字符集不确定 | 需要有序遍历 |

### 8.2 扩展方向

| 扩展 | 说明 |
|------|------|
| AC 自动机 | Trie + BFS 构造 fail 指针，支持多模式串同时匹配 |
| 可持久化 Trie | 每次插入保存历史版本，支持按版本查询前缀 |
| 01-Trie | 字符集为 $\{0, 1\}$，用于异或最值等问题 |
| Trie + 贪心 | 01-Trie 上按位贪心求最大异或和 |
| 后缀自动机 (SAM) | 更强的字符串处理结构，支持子串计数等 |
