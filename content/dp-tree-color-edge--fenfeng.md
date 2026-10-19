---
id: dp-tree-color-edge
title: '树上染色（边贡献分解）'
category: 动态规划
subcategory: 树形DP
subSubcategory: ''
tags: ["动态规划", "树形DP", "树上背包", "边贡献分解", "黑白染色"]
timeComplexity: 'O(nk)'
spaceComplexity: 'O(nk)'
codePath: '动态规划\树上染色\Untitled1.cpp'
level: A
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 算法原理

「树上染色」是树形背包与“边贡献分解”相结合的经典问题：给定一棵 $n$ 个节点、边带权的树，要求给恰好 $k$ 个点染成黑色、其余 $n-k$ 个点染成白色，最大化所有黑点两两距离之和与所有白点两两距离之和的总和。

直接枚举同色点对来统计距离，复杂度高且难以在树上高效维护。核心洞察在于：树上任意一条路径都会被它经过的每条边**恰好**切分一次，因此“所有同色点对的距离之和”可以按边拆成贡献——每条边对答案的贡献等于“经过这条边的同色点对数量 $\times$ 边权”。

把某条连接父节点 $u$ 与子节点 $v$ 的边 $(u,v)$ 砍掉后，整棵树被分成两个连通块：$v$ 所在的子树（大小 $sz_v$）与其余部分（大小 $n-sz_v$）。若在 $v$ 子树内选了 $t$ 个黑点，那么：

- 子树内的黑点与子树外的黑点组成的黑点对都要经过这条边，共 $t(k-t)$ 对；
- 子树内的白点与子树外的白点组成的白点对都要经过这条边，共 $(sz_v-t)\big((n-k)-(sz_v-t)\big)$ 对。

两者相加再乘边权 $w$，就是这条边对答案的贡献。于是“最大化点对距离”被转化为“在树上分配黑点数量、使所有边贡献之和最大”，这正是树形背包能处理的形式——每个节点要决定“自己子树内选几个黑点”，各子树的分配方案通过背包合并到父节点。

## 核心公式 / 状态定义与转移方程

状态定义：

$$dp[u][j] = \text{在 } u \text{ 的子树内选 } j \text{ 个黑点时，子树内部所有边的贡献和（不含 } u \text{ 到父节点的边）}$$

其中 $j$ 的取值范围为 $0 \le j \le \min(k, sz_u)$。

初始化（尚未合并任何儿子，$u$ 自身可黑可白，子树内还没有内部边）：

$$dp[u][0] = dp[u][1] = 0$$

边贡献（合并儿子 $v$，从 $v$ 子树取 $t$ 个黑点）：

$$\text{contrib} = \big[\, t(k-t) + (sz_v - t)\big((n-k)-(sz_v-t)\big) \big] \cdot w$$

其中 $w$ 是边 $(u,v)$ 的边权。

转移方程（把儿子 $v$ 背包合并进 $u$）：

$$dp[u][m] = \max_{0 \le t \le m}\Big( dp[u][m-t] + dp[v][t] + \text{contrib} \Big)$$

最终答案（以节点 $1$ 为根）：

$$\text{ans} = dp[1][k]$$

黑白对称优化：黑、白两色完全对称，把 $k$ 换成 $n-k$ 答案不变，因此代码里先做 $k = \min(k, n-k)$，以缩小 DP 的第二维。

## 逐行代码解析

### 头文件、宏与全局变量

```cpp
#include<bits/stdc++.h>
#define endl '\n'
#define fi first
#define se second
#define lowbit(x) (x&(-x))
#define int long long
using namespace std;
typedef pair<int,int> PII;
typedef long long ll;
const int N=2e3+10;
int cnt=0,n,k;
vector<PII> G1[N];
int siz[N],dp[N][N];
```

- `#define int long long` 把 `int` 全局替换为 `long long`，因此 `main` 必须写成 `signed main()`，否则 `int main()` 会被宏替换成 `long long main()` 而编译失败。
- `PII` 即 `pair<long long,long long>`，邻接表 `G1[x]` 的每个元素是 `{相邻节点, 边权}`，用 `i.fi` 取节点、`i.se` 取边权。
- `dp[N][N]` 是树形背包的核心表，`dp[u][j]` 表示子树 $u$ 内选 $j$ 个黑点的最大贡献；`N = 2\times 10^3 + 10`。

### dfs1：预处理子树大小

```cpp
void dfs1(int x,int fa){
	siz[x]=1;
	for(auto i:G1[x]){
		if(i.fi==fa) continue;
		dfs1(i.fi,x);
		siz[x]+=siz[i.fi];
	}
}
```

- 一次自底向上的 DFS，把每个节点的子树大小 `siz[x]` 全部求出来。
- `if(i.fi==fa) continue;` 跳过父节点，避免在无向树上往回走形成环。
- 子树大小会在 `dfs2` 中用于限制背包容量、计算边贡献，所以先单独跑一遍。

### dfs2：树形背包核心

```cpp
void dfs2(int x,int fa){
	dp[x][0]=dp[x][1]=0;
	for(auto j:G1[x]){
		if(j.fi==fa) continue;
        dfs2(j.fi,x);
		for(int m=min(k,siz[x]);m>=0;m--){
			if(dp[x][m]!=-1) dp[x][m]+=dp[j.fi][0]+siz[j.fi]*(n-k-siz[j.fi])*j.se;
			for(int v=min(m,siz[j.fi]);v>0;v--){
				if(dp[x][m-v]==-1) continue;
				int tot=v*(k-v)+(siz[j.fi]-v)*(n-k-siz[j.fi]+v);
				dp[x][m]=max(dp[x][m],dp[x][m-v]+dp[j.fi][v]+tot*j.se);
			}
		}		
	}
}
```

- `dp[x][0]=dp[x][1]=0` 初始化：$x$ 自身可白（$0$ 个黑点）或黑（$1$ 个黑点），此时还没有内部边，贡献为 $0$；其余状态被 `memset` 填成 $-1$ 表示“当前不可达”。
- `if(dp[x][m]!=-1) dp[x][m]+=...` 处理 $t=0$ 的分支：儿子 $v$ 全白，边贡献为 $sz_v(n-k-sz_v)\cdot w$，累加到旧值上。
- 内层 `v` 枚举分给儿子子树的黑点数 $t$，`tot = t(k-t) + (sz_v-t)(n-k-sz_v+t)` 正是这条边被同色点对经过的次数，乘 `j.se` 得边贡献。
- 容量 $m$ 从大到小枚举，保证 `dp[x][m-v]`（$m-v<m$）仍是“合并本儿子之前”的旧值，从而每个儿子只被合并一次、不会重复计数。

### 快读快写声明与 main

```cpp
inline int read();
inline void write(int x);
signed main(){
//    freopen(".in","r",stdin);
//    freopen(".out","w",stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);cout.tie(nullptr);
    memset(dp,-1,sizeof dp);
	cin>>n>>k;
    if(n-k<k) k=n-k;
	for(int i=1;i<n;i++){
		int u,v,w;cin>>u>>v>>w;
		G1[u].push_back({v,w});
		G1[v].push_back({u,w});
	}
	dfs1(1,0);
	dfs2(1,0);
	cout<<dp[1][k];
    return 0;
}
```

- `memset(dp,-1,sizeof dp)` 把整张 `dp` 表填成 $-1$ 作为“不可达”哨兵，再由 `dp[x][0]=dp[x][1]=0` 显式打开可达状态。
- `if(n-k<k) k=n-k;` 利用黑白对称把 $k$ 换成较小值，减小 DP 第二维的规模。
- 读边用 `G1[u].push_back({v,w})` 双向加边；根固定为节点 $1$，答案即 `dp[1][k]`。

### 快读快写实现

```cpp
inline int read(){
    int x=0,f=1;
    char ch=getchar();
    while(ch< '0'||ch>'9'){
        if(ch=='-') f=-1;
        ch=getchar();
    }
    while(ch>='0'&&ch<='9'){
        x=(x<<1)+(x<<3)+(ch^48);
        ch=getchar();
    }
    return x*f;
}
inline void write(int x){
    if(x<0){
        putchar('-');
        x=-x;
    }
    if(x>9) write(x/10);
    putchar(x%10+'0');
}
```

- `read()` 用 `getchar` 逐字符读整数：跳过前导非数字、处理负号，`x=(x<<1)+(x<<3)+(ch^48)` 等价于 `x = x*10 + (ch-'0')`。
- `write(x)` 递归从高位到低位输出，负数先打负号再取相反数。
- 这两个函数在本程序里其实未被 `main` 调用（`main` 用的是 `cin/cout`），保留它们是作者常用的快读快写模板。

## 复杂度分析

- **时间复杂度**：$O(nk)$。每个节点合并儿子时枚举容量 $m$ 与分配给儿子的黑点数 $t$，表面看是 $O(nk^2)$；但受子树大小 `siz` 限制，每对节点只会在其最近公共祖先（LCA）处被合并一次，均摊下来为 $O(nk)$。
- **空间复杂度**：$O(nk)$。`dp[u][j]` 共有 $n$ 个节点 $\times$ $(k+1)$ 个可选黑点数；代码为方便静态声明成 `dp[N][N]`（$N=2\times 10^3+10$），第二维实际只用到 $0 \dots k$。配合 $k=\min(k,n-k)$ 的对称优化，$k \le n/2$。

## 适用场景

适用于“树上点染色 + 点对距离/边贡献求和”类问题：给树上的点分组（如黑白染色），要求最大化或最小化某一类点对的总距离、总贡献。典型特征是——目标函数能按“边两侧各组点数”拆成每条边的独立贡献，从而把点对层面的统计转化为树形背包。凡是能拆成“边两侧某类点对数 $\times$ 边权”的问题，都可套用这一框架。

## 常见陷阱与注意事项

- **哨兵 $-1$ 与可达性**：`dp` 表用 $-1$ 表示“该黑点数当前不可达”，合并前必须先 `memset(dp,-1,sizeof dp)`，再用 `dp[x][0]=dp[x][1]=0` 打开初始状态。若用 $0$ 当初始值，会把“不可达”与“贡献为 $0$”混淆，导致错误转移。
- **黑白对称优化**：`if(n-k<k) k=n-k;` 依赖黑白两色答案对称；若题目对两种颜色赋了不同权值（不对称），则不能这样交换。
- **容量逆序遍历**：合并儿子时 $m$ 必须从大到小枚举，否则 `dp[x][m-v]` 读到的是“已合并当前儿子”的新值，会把同一个儿子重复计入。
- **`#define int long long`**：一旦开启，`main` 必须写成 `signed main()`；`memset` 填 $-1$ 依赖二进制补码，对 `long long` 数组同样成立。
- **递归深度**：树可能退化成链，`dfs1`/`dfs2` 递归深度可达 $n$，数据较大时需注意栈溢出。
- **边界与下标**：循环上限取 `min(k,siz[x])`、`min(m,siz[j.fi])`，避免访问超出子树实际大小的无效状态。

## 对比与扩展

- **与普通树形背包的关系**：本质是树上分组背包的变体——每个儿子是一个“组”，从该组选 $t$ 个黑点是一种“物品”，只是“物品价值”额外叠加了这条边的贡献项。
- **与「没有上司的舞会」（树形最大独立集）的区别**：后者只有选/不选两种状态、转移 $O(1)$；本问题要枚举黑点数做背包合并，状态是“选了几个”，复杂度更高。
- **扩展**：若边权或点权改为有向/非对称，只需改边贡献公式中的同色点对数计算；若限制的是“同色连通块”而非点对距离，则转化为另一类树上计数问题。
