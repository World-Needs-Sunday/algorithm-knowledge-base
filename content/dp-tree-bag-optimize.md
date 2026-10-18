---
id: dp-tree-bag-optimize
title: '树形背包 O(nk) 优化'
category: 动态规划
subcategory: 树形DP
subSubcategory: ''
tags: ["树形背包", "树上染色", "树形DP", "背包DP", "边贡献转化"]
timeComplexity: 'O(nk)'
spaceComplexity: 'O(n²)'
codePath: '动态规划\树形背包优化\Untitled1.cpp'
level: A
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 算法原理

树形背包把「树上的最优决策」和「背包问题」结合起来：以节点为单位，在 DFS 自底向上的过程中，把每个儿子的子树当作一个「物品组」，用分组背包的方式合并进父亲的状态里。朴素做法对「父亲容量」和「儿子贡献」各枚举一层，看似 $O(nk^2)$；但只要把每一维的枚举上界都限制在子树大小（或 $k$）以内，均摊后就是 $O(nk)$，这正是「树形背包 $O(nk)$ 优化」名字的由来。

以例题「树上染色」为例：给定一棵 $n$ 个点的带权树，要把其中恰好 $k$ 个点染成黑色、其余 $n-k$ 个点染成白色，最大化「所有黑点两两距离之和 + 所有白点两两距离之和」。直接枚举点对再求距离既不方便转移、也难与树的结构结合，于是把「点对距离」拆成「每条边的贡献」：

> 一条边 $(u,v)$ 对答案的贡献，等于「经过这条边的同色点对数量 × 这条边的边权」。而「经过这条边的同色点对数量」只由边两侧各自的颜色分布决定，也就是只依赖「子树里取了多少个黑点」。

这样，总答案就被写成所有边贡献之和，每条边的贡献只依赖子树内的黑点数，恰好可以用树形背包逐层合并求出。

## 核心公式 / 状态定义与转移方程

设树以 $1$ 为根，$sz_v$ 表示 $v$ 的子树大小，$k$ 表示要染的黑点数。

**状态定义**：$dp[u][j]$ 表示在 $u$ 的子树内选 $j$ 个黑点时，子树内部所有边的贡献之和（不含 $u$ 到父节点的那条边）。

**边贡献**：对于边 $(u,v)$（$v$ 是 $u$ 的儿子，边权为 $w$），若 $v$ 的子树内取了 $t$ 个黑点，则经过这条边的同色点对数为：

$$
t(k-t) + (sz_v - t)\big((n-k) - (sz_v - t)\big)
$$

第一项是「跨过这条边的黑点对」：$v$ 侧 $t$ 个黑点、另一侧 $k-t$ 个黑点，两两配对共 $t(k-t)$ 对；第二项是「跨过这条边的白点对」：$v$ 侧有 $sz_v-t$ 个白点、另一侧有 $(n-k)-(sz_v-t)$ 个白点。于是这条边的贡献为：

$$
w \cdot \left[\, t(k-t) + (sz_v - t)\big((n-k) - (sz_v - t)\big) \,\right]
$$

**转移方程**（把儿子 $v$ 合并进 $u$，枚举从 $v$ 子树取 $t$ 个黑点）：

$$
dp[u][m] = \max_{0 \le t \le \min(m,\,sz_v)} \Big( dp[u][m-t] + dp[v][t] + w \cdot \left[ t(k-t) + (sz_v-t)\big((n-k)-(sz_v-t)\big) \right] \Big)
$$

**初始化**：$dp[u][0] = dp[u][1] = 0$（$u$ 自身可白或黑，此时子树内还没有边，贡献为 $0$），其余状态初始化为 $-1$ 表示「不可达」（代码里用 `memset` 统一完成）。

**答案**：$dp[1][k]$。另外 $k$ 与 $n-k$ 黑白对称，可以先用 `if(n-k<k) k=n-k;` 把第二维压缩到不超过 $\lceil n/2 \rceil$。

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

- `#define int long long` 把 `int` 全局替换成 `long long`，配合后文的 `signed main()`；边权与点对数的乘积可达 $10^{12}$ 量级，必须用 64 位整数避免溢出。
- `G1[x]` 是邻接表，元素 `PII` 的 `fi` 存邻居编号、`se` 存边权，`fi`/`se` 分别是 `first`/`second` 的宏缩写。
- `dp[N][N]` 是二维背包表，`dp[u][j]` 表示 $u$ 子树选 $j$ 个黑点时的贡献；`siz[N]` 存子树大小。
- `N=2e3+10` 对应数据范围 $n \le 2000$；`cnt` 是模板残留变量、未使用。

### dfs1 预处理子树大小

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

- 第一遍 DFS 只做一件事：自底向上统计每个点的子树大小 `siz[x]`。
- `siz[x]=1` 先把 $x$ 自己算进去，再跳过父节点、把每个儿子的 `siz` 累加进来。
- `siz` 在第二遍 DFS 中用于给背包维度设上界（`min(k, siz[x])`、`min(m, siz[v])`），是复杂度降到 $O(nk)$ 的关键。

### dfs2 树形背包主转移

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

- `dp[x][0]=dp[x][1]=0`：$x$ 自身可白（$0$ 个黑点）或黑（$1$ 个黑点），此时子树内还没有边，贡献为 $0$。
- 先递归 `dfs2(j.fi,x)` 算出儿子的完整状态，再用背包把它合并进 $x$；外层 `m` 逆序枚举，保证同一个儿子只被合并一次（0/1 背包性质）。
- `tot = v*(k-v)+(siz[j.fi]-v)*(n-k-siz[j.fi]+v)` 是「儿子子树取 $v$ 个黑点」时边 $(x,j)$ 的贡献：前一项是跨边黑点对、后一项是跨边白点对。
- 内层 `v` 从 $1$ 开始，$v=0$（儿子全白）的贡献由前一行 `dp[x][m]+=dp[j.fi][0]+siz[j.fi]*(n-k-siz[j.fi])*j.se` 单独补齐。

### 主函数

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

- `memset(dp,-1,sizeof dp)` 把整张 `dp` 表初始化为 $-1$（不可达），之后只有从 `dp[x][0]=dp[x][1]=0` 递推得到的状态才会被更新。
- `if(n-k<k) k=n-k;` 利用黑白对称性，取黑、白两色中数量较少的一方作为 `k`，显著缩小 DP 第二维。
- 读入 $n-1$ 条无向边并建双向邻接表，以 $1$ 为根先 `dfs1` 求子树大小、再 `dfs2` 做背包，答案即 `dp[1][k]`。
- 前两行 `read`/`write` 是模板自带的快读快写声明；本程序实际用 `cin/cout`，未调用它们。

### 快读快写辅助函数

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

- `read()` 用 `getchar()` 逐字符读入整数，跳过前导空白并处理负号；`x=(x<<1)+(x<<3)+(ch^48)` 等价于 `x=x*10+(ch-'0')`。
- `write()` 递归按位输出：负数先输出 `-`，`x>9` 时先输出高位，最后 `putchar(x%10+'0')` 输出个位。
- 这两个函数是模板自带的快速 I/O 工具，本程序未调用，保留不影响正确性。

## 复杂度分析

- **时间复杂度 $O(nk)$**：合并儿子时，外层 `m` 的上界是 $\min(k, siz[x])$、内层 `v` 的上界是 $\min(m, siz[v])$，两层看似 $O(nk^2)$；但每一维都被子树大小截断，均摊后总复杂度为 $O(nk)$，可平稳通过 $n,k \le 2000$ 的数据（对应代码中的 $N=2\times10^3+10$）。
- **空间复杂度 $O(n^2)$**：代码用固定的二维表 `dp[N][N]` 存状态，即 $O(n^2)$。实际上有意义的状态只有 $\sum_u \min(k, siz[u]) = O(nk)$ 个；若按每个节点动态开 $\min(k,siz[u])+1$ 长度的数组，空间可进一步降到 $O(nk)$。

## 适用场景

- 树上「按数量分配 / 选点」的最优决策问题，状态为「子树内选了多少个」，如树上染色、树上资源分配。
- 带容量约束的树上背包：把每个儿子当作一个物品组，用分组背包合并进父亲。
- 需要把「全局距离 / 全局贡献」拆成「每条边的贡献」再逐层 DP 的树形计数与最优化题。
- 数据规模在 $n,k \le 10^3 \sim 10^4$ 量级、背包容量与 $n$ 同阶的树形背包题。

## 常见陷阱与注意事项

- **边贡献要在合并儿子时算**：边 $(u,v)$ 的贡献依赖 $v$ 子树内的黑点数 $t$，必须在合并 $v$ 的当下连同 `dp[v][t]` 一起加进转移，不能等整棵树合并完再统一处理。
- **dp 初始化为 -1 表示不可达**：`dp[x][0]=dp[x][1]=0` 是唯一合法起点，其余状态用 `memset(dp,-1,...)` 标记，转移前用 `if(dp[...]!=-1)` 判断，避免从不可达状态出发得到错误答案。
- **外层 m 必须逆序**：这是 0/1 背包合并，同一个儿子只能取一次；若正序枚举，`dp[x][m-v]` 会被本次已更新的值污染，导致同一儿子被重复使用。
- **黑白对称压缩维度**：$k$ 与 $n-k$ 对称，先 `if(n-k<k) k=n-k;` 把第二维缩到不超过 $n/2$，是本题通过大数据的关键。
- **用 64 位整数**：边贡献是「点对数量 × 边权」，量级可达 $O(n^2 \cdot w)$，`#define int long long` 必不可少。
- **$O(nk)$ 依赖维度截断**：若合并时把 `m`、`v` 都无脑枚举到 $k$ 而不做 `min(k, siz)` 截断，复杂度会退化成 $O(nk^2)$。

## 对比与扩展

- **对比「树状01背包」**（选子必选父的依赖背包）：本算法的状态是「子树内取了多少个黑点」，只关心数量、没有父子依赖，转移是「分组背包 + 边贡献」；依赖背包则把「选父才可选子」的约束写进状态与转移。
- **对比朴素树上背包 $O(nk^2)$**：朴素版把每维都枚举到 $k$，总复杂度 $O(nk^2)$；本算法用 $\min(k, siz)$ 截断每维，均摊降到 $O(nk)$。
- **扩展**：把「全局点对距离之和」拆成「每条边贡献」的技巧，可推广到树上两两距离求和、带权路径计数等需要按边统计的问题；分组背包的合并方式还能推广到虚树上的多组询问背包、树上依赖背包等变种。
