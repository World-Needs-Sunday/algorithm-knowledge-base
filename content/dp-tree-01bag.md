---
id: dp-tree-01bag
title: '树状01背包'
category: 动态规划
subcategory: 树形DP
tags: ["动态规划", "树形DP", "背包问题", "依赖背包", "DFS", "DP"]
timeComplexity: 'O(N×V²)最坏'
spaceComplexity: 'O(N×V)'
codePath: '动态规划\树形DP\树状01背包\Untitled1.cpp'
---

## 算法原理

树状01背包是01背包与树形DP的结合。树上有 $N$ 个节点，每个节点有重量和价值，选中一个节点必须同时选中其父节点（保证连通性——即选了子节点就必须选父节点，形成"依赖"关系）。目标是在容量 $V$ 的限制下使选中节点的总价值最大。

算法通过 DFS 后序遍历处理。对每个节点 `idx`，先初始化其 `dp` 值：若容量 $j \geq$ 当前节点的重量，则 $dp[\text{idx}][j] = v_{\text{idx}}$（即只选当前节点本身），否则为 $0$。这一步体现了依赖关系——选了子节点的前提是当前节点已被选中。

然后将每个子树"合并"到当前节点。对子节点 `child`，先递归处理子节点（得到 $dp[\text{child}][\cdot]$），然后将子节点的 `dp` 值合并到当前节点。合并过程是一个树上分组背包：将总容量 $j$ 分配给当前节点自身和各子树，枚举分配给子树 `child` 的容量 $k$，转移为 $dp[\text{idx}][j] = \max(dp[\text{idx}][j-k] + dp[\text{child}][k], dp[\text{idx}][j])$。

容量 $j$ 逆序遍历（保证不重复计算），$k$ 从 $1$ 到 $j - w_{\text{idx}}$（预留当前节点的重量）。DFS 从根节点开始，最终答案为 $dp[\text{root}][V]$。

## 状态定义与转移方程

状态定义：

$$dp[\text{idx}][j] = \text{在以 idx 为根的子树中选取、总重量不超过 } j \text{ 时的最大价值}$$

初始化（体现依赖关系）：

$$dp[\text{idx}][j] = v_{\text{idx}}, \quad j \geq w_{\text{idx}}$$

合并子树（对子节点 `child`）：

$$dp[\text{idx}][j] = \max\big(dp[\text{idx}][j-k] + dp[\text{child}][k],\; dp[\text{idx}][j]\big)$$

最终答案：

$$\text{ans} = dp[\text{root}][V]$$

## 逐行代码解析

```cpp
#include<bits/stdc++.h>
using namespace std;
/*U436983*/
int N , V , root;
vector<pair<int,int>> A;
vector<vector<int>> son;
vector<vector<int>> dp;
void dfs(int idx)
{
	for(int i = A[idx].first;i <= V;i++) dp[idx][i] = A[idx].second;
	for(int i : son[idx])
	{
		dfs(i);
		for(int j = V ; j > A[idx].first ; j--)
		{
			for(int k = 1;j - k >= A[idx].first;k++)
			{
				dp[idx][j] = max(dp[idx][j - k] + dp[i][k] , dp[idx][j]);
			}
		}
	}
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr);cout.tie(nullptr);
	
	cin >> N >> V;
	dp.resize(N + 1,vector<int>(V + 1,0));
	A.resize(N + 1);
	son.resize(N + 1,vector<int>());
	for(int i = 1;i <= N;i++)
	{
		int p;
		cin >> A[i].first >> A[i].second >> p;
		if(p == -1) root = i;
		else son[p].emplace_back(i);
	}
	dfs(root);
	cout << dp[root][V];
	return 0;	
} 
```

- **`A` 数组**：`A[i].first` 存节点 $i$ 的重量，`A[i].second` 存价值。
- **`son` 数组**：邻接表存储子节点列表，`son[p]` 存节点 $p$ 的所有子节点。
- **`dp` 表**：二维数组 $(N+1) \times (V+1)$，`dp[idx][j]` 表示以 `idx` 为根的子树中重量不超过 $j$ 时的最大价值。
- **`dfs` 函数**：
  - **初始化**：`for(int i = A[idx].first; i <= V; i++) dp[idx][i] = A[idx].second;` —— 只要容量够选当前节点，`dp` 值就是该节点价值，体现依赖关系。
  - **遍历子节点**：对每个子节点 `i`，先递归 `dfs(i)` 处理子树，然后合并。
  - **合并转移**：外层 `j` 从 $V$ 逆序到 $A[\text{idx}].\text{first} + 1$（预留当前节点重量），内层 `k` 从 $1$ 到 $j - A[\text{idx}].\text{first}$（分配给子树的容量），转移 `dp[idx][j-k] + dp[i][k]`。
- **根节点**：`p == -1` 表示该节点无父节点，即为根。
- **输出**：`dp[root][V]` 即为以根为起点的最大价值。

## 复杂度分析

- **时间复杂度**：最坏 $O(N \times V^2)$。每个节点合并子树时，三重循环（子节点 $\times$ 容量 $j$ $\times$ 分配 $k$）的最坏复杂度为 $O(V^2)$。优化后（利用子树大小的前缀和技巧）可降到 $O(N \times V)$，但本实现未优化。
- **空间复杂度**：$O(N \times V)$。每个节点维护一个长度 $V+1$ 的 `dp` 数组，共 $N$ 个节点。

## 适用场景

适用于树形依赖背包问题——选子节点必须先选父节点（对应U436983），也适用于有依赖关系的资源分配问题。当物品之间存在树形依赖关系（选A必须先选B）时使用。常见于游戏中的技能树解锁、课程先修关系等场景。

## 常见陷阱与注意事项

- **初始化体现依赖关系**：`dp[idx][j] = A[idx].second` 仅在 $j \geq w_{\text{idx}}$ 时设置，这意味着子节点的价值只能在父节点已选的基础上累加——这是依赖背包的核心。
- **容量逆序遍历**：合并子树时 `j` 逆序遍历，防止同一子树被重复合并。
- **`k` 的范围**：从 $1$ 到 $j - w_{\text{idx}}$，因为必须预留当前节点的重量。若 `k` 从 $0$ 开始会出错（$k=0$ 表示不给子树容量，但子节点仍需父节点已选）。
- **根节点的识别**：`p == -1` 标记根节点，题目保证只有一个根。
- **DFS的递归深度**：树可能为链状，递归深度达 $N$，需注意栈溢出（可改用迭代或手动栈）。
- **复杂度优化**：本实现为 $O(N \times V^2)$，可通过"左子树大小 + 右子树大小"的前缀和技巧优化到 $O(N \times V)$，但实现更复杂。

## 对比与扩展

与标准01背包相比，树状01背包增加了依赖关系约束，通过DFS后序遍历和树上分组背包合并实现。与分组背包的联系在于：每个节点的子节点集合可以视为一个"组"，合并过程即为分组背包。可推广为泛化依赖背包（依赖关系为DAG而非树）、树上多重背包等变种。复杂度优化可利用DFS序将树形DP转化为序列上的背包问题，达到 $O(N \times V)$。
