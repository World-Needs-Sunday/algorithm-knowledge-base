---
id: dp-tree-dance
title: '没有上司的舞会（树形DP）'
category: 动态规划
subcategory: 树形DP
tags: ["动态规划", "树形DP", "最大独立集", "DFS", "DP"]
timeComplexity: 'O(n)'
spaceComplexity: 'O(n)'
codePath: '动态规划\树形DP\没有上司的舞会\Untitled1.cpp'
---

## 算法原理

"没有上司的舞会"是树形DP的经典问题，本质是树上的最大权独立集。一棵树代表公司层级关系，每个节点有权值（快乐值），约束是：若选了某节点，则不能选其直接下属（即相邻节点不能同时选）。目标是使选中节点的总权值最大。

定义两种状态：$dp[\text{node}][0]$ 表示不选节点 `node` 时，以 `node` 为根的子树的最大权值；$dp[\text{node}][1]$ 表示选节点 `node` 时，以 `node` 为根的子树的最大权值。

转移逻辑清晰：若选了当前节点（$dp[\text{node}][1]$），则所有子节点都不能选（因为相邻不能同时选），所以子节点的贡献全部来自 $dp[\text{child}][0]$，即 $dp[\text{node}][1] = A[\text{node}] + \sum dp[\text{child}][0]$。若不选当前节点（$dp[\text{node}][0]$），则每个子节点可以自由选择选或不选（取较优值），即 $dp[\text{node}][0] = \sum \max(dp[\text{child}][0], dp[\text{child}][1])$。

通过 DFS 后序遍历自底向上转移：先递归处理所有子节点（得到子节点的 `dp` 值），再根据子节点的 `dp` 值计算当前节点的 `dp` 值。根节点是唯一没有父节点的点，通过 `fa` 数组标记找出。最终答案为 $\max(dp[\text{root}][0], dp[\text{root}][1])$。

## 状态定义与转移方程

状态定义：

$$dp[\text{node}][0] = \text{不选 node 时，以 node 为根的子树的最大权值}$$

$$dp[\text{node}][1] = \text{选 node 时，以 node 为根的子树的最大权值}$$

转移方程（选当前节点）：

$$dp[\text{node}][1] = A[\text{node}] + \sum_{\text{child}} dp[\text{child}][0]$$

转移方程（不选当前节点）：

$$dp[\text{node}][0] = \sum_{\text{child}} \max\big(dp[\text{child}][0],\; dp[\text{child}][1]\big)$$

最终答案：

$$\text{ans} = \max\big(dp[\text{root}][0],\; dp[\text{root}][1]\big)$$

## 逐行代码解析

### 全局变量

```cpp
#include<bits/stdc++.h>
using namespace std;
/*洛谷P1352*/
vector<int> A;
vector<vector<int>> dp;
vector<bool> fa;
vector<vector<int>> son;
```

- **`A`**：存储每个节点的权值（快乐值），下标从 $1$ 开始，`A[0]` 不使用。
- **`dp`**：$(n+1) \times 2$ 的二维表，`dp[idx][0]` / `dp[idx][1]` 分别表示不选 / 选节点 `idx` 时以 `idx` 为根的子树最大权值，初始化为 $0$。
- **`fa`**：`bool` 数组，`fa[i] == true` 表示节点 $i$ 有父节点，唯一为 `false` 的节点就是根。
- **`son`**：邻接表，`son[k]` 存节点 $k$ 的所有子节点列表。

### dfs 函数

```cpp
void dfs(int idx)
{
	dp[idx][1] += A[idx];
	for(int i : son[idx])
	{
		dfs(i);
		dp[idx][1] += dp[i][0];
		dp[idx][0] += max(dp[i][0] , dp[i][1]);
	}
}
```

- 先执行 `dp[idx][1] += A[idx]`：选当前节点时先累加自身权值，是"选"状态的基础项。
- `for(int i : son[idx])` 遍历子节点，先 `dfs(i)` 递归——必须自底向上，先算完子节点的 `dp` 再更新父节点。
- 选状态累加子节点"不选"的值 `dp[i][0]`（相邻节点不能同时选）；不选状态累加两者较优值 `max(dp[i][0], dp[i][1])`。

### main：读入与初始化

```cpp
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	int n,root = 1;
	cin >> n;
	A.resize(n + 1,0);
	fa.resize(n + 1,false);
	son.resize(n + 1,vector<int>());
	dp.resize(n + 1,vector<int>(2,0));
	for(int i = 1;i <= n;i++) cin >> A[i];
	for(int i = 1;i < n;i++)
	{
		int l,k;
		cin >> l >> k;
		son[k].emplace_back(l);
		fa[l] = true;
	}
```

- `ios::sync_with_stdio(false)` 与 `cin.tie(nullptr)` 关闭同步、解绑输出，加速大量输入。
- 四个容器都 `resize(n + 1, ...)`，下标 $1\sim n$ 用满；`dp` 初始化为 $0$，递归时再累加自身权值。
- 读权值循环 `for(int i = 1; i <= n; i++)` 是 $1$ 到 $n$（含端点），与下标从 $1$ 开始保持一致。
- 读边循环 `for(int i = 1; i < n; i++)`：$n$ 个点的树恰好 $n-1$ 条边，所以到 `i < n`；`l` 是子节点、`k` 是父节点，把 `l` 挂到 `son[k]` 并标记 `fa[l] = true`。

### main：找根与输出

```cpp
	while(fa[root]) { ++root; }
	dfs(root);
	cout << max(dp[root][1],dp[root][0]);
	return 0;
} 
```

- `while(fa[root]) { ++root; }`：从 $1$ 开始线性找到第一个无父节点的节点作为根（假设编号连续，否则需遍历所有节点）。
- `dfs(root)` 从根开始递归，完成整棵树的 DP。
- 答案 `max(dp[root][1], dp[root][0])`：根节点可选可不选，取两者较优值；`return 0` 正常结束。

## 复杂度分析

- **时间复杂度**：$O(n)$。DFS 遍历每个节点一次，每个节点的转移为 $O(1)$（累加子节点值），总计 $O(n)$。
- **空间复杂度**：$O(n)$。`dp` 表 $O(n)$，邻接表 $O(n)$，`fa` 数组 $O(n)$。

## 适用场景

适用于树形最大独立集问题（对应洛谷P1352），即任何"相邻节点不能同时选"的树形问题。常见于：公司聚会中上下级不能同时参加、树形结构中相邻节点互斥的选择问题等。当问题可建模为树上的最大权独立集时使用。

## 常见陷阱与注意事项

- **根节点的识别**：通过 `fa` 数组找出唯一无父节点的节点作为根。若输入保证节点 $1$ 为根，可省略此步骤。
- **DFS 的顺序**：必须先递归子节点（自底向上），再用子节点的 `dp` 值更新当前节点。若先更新当前节点再递归，会用到未计算的子节点值。
- **`dp` 初始化为 0**：`dp[idx][1]` 初始为 $0$，递归时先加上自身权值 `A[idx]`，再累加子节点。若权值可能为负，需注意初始化（负无穷 vs $0$）。
- **递归深度**：树可能为链状，递归深度达 $n$，大规模数据需注意栈溢出。
- **答案取两种状态的最大值**：根节点可以选也可以不选，最终答案为 $\max(dp[\text{root}][0], dp[\text{root}][1])$。
- **`while(fa[root]) { ++root; }`**：从节点 $1$ 开始线性查找根，假设节点编号连续。若编号不连续需遍历所有节点。
- **空输入与找根越界**：$n=0$ 时 `fa` 仅大小 $1$，`while(fa[root])` 会访问 `fa[1]` 越界；且该循环依赖"恰有一个无父节点的根、编号连续"，否则会自增越界。建议约定输入合法（$n\ge1$ 且保证为树），或先判空、显式遍历找根并处理无根情况。

## 对比与扩展

与树状01背包相比，本问题更简单——只有选/不选两种状态，转移只需累加子节点的值，无需枚举容量分配。树形最大独立集可推广为：带权最大独立集（本问题即带权版本）、树上的最小点覆盖、树上的最大匹配等。若树退化为链，则等价于一维数组上"相邻不选"的最大子段和问题。若图不是树而是一般图，最大独立集为NP-hard问题，树形DP的 $O(n)$ 解法依赖树的无环性。
