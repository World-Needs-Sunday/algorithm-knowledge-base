---
id: ds-stl-queue
title: '队列（queue）'
category: 数据结构
subcategory: STL容器
subSubcategory: ''
tags: ["数据结构", "STL", "队列", "双端队列", "优先队列"]
timeComplexity: '视具体实现'
spaceComplexity: '视具体实现'
level: A
noCode: true
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 算法原理

队列（queue）是一种「先进先出」（FIFO，First In First Out）的线性数据结构：元素只能从队尾（rear）入队，只能从队首（front）出队。它只关心元素到达的先后顺序，不关心元素之间的大小关系，因此出队顺序与入队顺序完全一致。

C++ STL 提供三种常用的队列相关容器：

- `queue<T>`：普通队列，只能从队尾入队、队首出队，底层默认由 `deque` 实现，需要包含 `<queue>`（使用万能头 `<bits/stdc++.h>` 时可省略）。
- `deque<T>`：双端队列（double-ended queue），两端都可插入和删除，还支持随机访问，需要包含 `<deque>`。
- `priority_queue<T>`：优先队列，底层是堆，每次取出的都是当前最值元素，需要包含 `<queue>`。

普通队列的入队、出队示意：

```
入队:  队尾 <-- 新元素         出队:  队首 --> 弹出
       [ a1 | a2 | a3 | x ]          [ a2 | a3 | x ]
        队首           队尾           队首        队尾
```

## 核心公式 / 状态定义与转移方程

队列的核心约束是「先入先出」。设元素按 $a_1, a_2, \dots, a_n$ 的顺序依次入队，则出队顺序恒为 $a_1, a_2, \dots, a_n$，即第 $i$ 次出队得到的元素是 $a_i$。若队首在前，把队列记为 $[a_1, a_2, \dots, a_n]$，则一次出队操作满足：

$$
\text{pop}\big([a_1, a_2, \dots, a_n]\big) = (a_1,\ [a_2, a_3, \dots, a_n])
$$

即弹出队首 $a_1$，剩余部分成为新的队列。

若用循环数组（环形缓冲）手写队列，设容量为 $N$、队首下标为 $head$、队尾下标为 $tail$（$tail$ 指向队尾元素的下一个空位），则入队与出队只移动指针、不搬动元素：

$$
head = (head + 1) \bmod N, \qquad tail = (tail + 1) \bmod N
$$

此时队列中元素个数可表示为 $(tail - head + N) \bmod N$。

## 逐行代码解析

下面是三种队列容器的关键用法（伪代码 / 关键片段，非完整可运行程序）。

### 普通队列 `queue`

```cpp
#include<queue>            // 使用万能头<bits/stdc++.h>时可省略
queue<int> q;

q.push(x);   // 在队尾插入元素 x
q.front();   // 返回队首元素（不删除）
q.back();    // 返回队尾元素（不删除）
q.pop();     // 弹出队首元素（无返回值）
q.empty();   // 判断队列是否为空
q.size();    // 返回队列中元素的数量

queue<int> q1, q2;
q1 = q2;     // 将 q2 赋值给 q1
```

- `front()` / `back()` 只访问不删除；`pop()` 只删除不返回，取值需先调用 `front()`。
- `queue` 默认底层是 `deque`，因此 `push` / `pop` 都是常数时间。

### 双端队列 `deque`

```cpp
#include<deque>
deque<int> dq;

dq.front(); dq.back();      // 访问队首 / 队尾元素
dq.push_back(x);            // 在队尾插入
dq.pop_back();              // 弹出队尾元素
dq.push_front(x);           // 在队首插入
dq.pop_front();             // 弹出队首元素
dq.insert(it, x);           // 在迭代器 it 指向位置之前插入 x
dq.erase(it);               // 删除迭代器 it 指向的元素
dq.empty(); dq.size();      // 判空 / 元素个数
```

- `deque` 两端操作都是 $O(1)$，并支持 `operator[]` 随机访问。
- `insert` / `erase` 在中间位置操作是 $O(n)$。

### 优先队列 `priority_queue`

```cpp
#include<queue>
priority_queue<int> q;                           // 默认大根堆
priority_queue<int, vector<int>, greater<int>> q; // 小根堆

q.top();    // 返回堆顶元素（不删除）
q.pop();    // 弹出堆顶元素
q.push(x);  // 插入元素 x
q.empty();  // 判断是否为空
```

- 优先队列没有 `front()` / `back()`，只能用 `top()` 访问堆顶。
- 默认是大根堆；第三个模板参数填 `greater<int>` 使其成为小根堆。

## 复杂度分析

各容器的单次操作复杂度如下：

| 容器 | 访问 | 插入 / 删除 | 说明 |
|---|---|---|---|
| `queue` | `front` / `back` $O(1)$ | `push` / `pop` $O(1)$ | 底层为 `deque` |
| `deque` | 两端 $O(1)$，随机访问 $O(1)$ | 两端 $O(1)$，中间 `insert` / `erase` $O(n)$ | 双端操作 |
| `priority_queue` | `top` $O(1)$ | `push` / `pop` $O(\log n)$ | 底层为堆 |

空间复杂度均为 $O(n)$，其中 $n$ 为容器中元素个数。

## 适用场景

- 需要按到达先后顺序处理元素：BFS 广度优先搜索的扩展队列、树的层次遍历。
- 任务排队、缓冲区、生产者与消费者模型等「先进先出」语义。
- 需要在两端插入或删除：单调队列（滑动窗口最值）、01BFS（用 `deque`）。
- 需要动态维护最值：`priority_queue` 用于 Dijkstra、Prim、哈夫曼编码、求第 $k$ 大等。

## 常见陷阱与注意事项

1. `pop()` 无返回值：先 `front()` 取值，再 `pop()`，否则会丢失队首元素。
2. 访问前先判空：对空队列调用 `front()` / `pop()` 是未定义行为。
3. `queue` 没有迭代器，不能遍历或随机访问；需要遍历时应改用 `deque` 或手写数组队列。
4. `priority_queue` 只有 `top()`，没有 `front()` / `back()`：访问堆顶必须用 `top()`，误用 `front()` 会编译报错。
5. 小根堆写法 `priority_queue<int, vector<int>, greater<int>>` 在 C++11 之前，两个 `>` 之间必须加空格写成 `> >`；C++11 起可省略。
6. `queue` 没有 `clear()`：清空可写成 `q = queue<int>();` 或循环 `pop()`。
7. `queue` / `deque` / `priority_queue` 都支持整体赋值（如 `q1 = q2`），本质是拷贝容器内容。

## 对比与扩展

| 容器 | 头文件 | 底层结构 | 访问方式 | 核心特点 | 典型应用 |
|---|---|---|---|---|---|
| `queue` | `<queue>` | `deque` | `front` / `back` | 队尾入队、队首出队（FIFO） | BFS、层次遍历 |
| `deque` | `<deque>` | 分块双端数组 | 两端 + 随机访问 | 两端均可插删 | 单调队列、01BFS |
| `priority_queue` | `<queue>` | 二叉堆 | `top` | 每次取最值 | Dijkstra、top-k |
| `stack` | `<stack>` | `deque` | `top` | 后进先出（LIFO） | DFS、括号匹配 |

扩展方向：

- **单调队列**：用 `deque` 维护滑动窗口内的最值，是滑动窗口与动态规划问题的常用优化（见 `ds-monoqueue-1d`）。
- **01BFS**：边权为 $0$ 或 $1$ 的最短路，用 `deque` 的 `push_front` / `push_back` 代替普通队列。
- **循环队列**：手写数组 + 取模指针实现，避免动态扩容，适合空间受限的场合。
- **优先队列与堆**：`priority_queue` 是对二叉堆的封装；需要随机删除或修改元素时，应改用手写堆或平衡树。
