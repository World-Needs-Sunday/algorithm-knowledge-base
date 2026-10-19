---
id: ds-segtree-dynamic
title: '动态开点线段树'
category: 数据结构
subcategory: 线段树
subSubcategory: ''
tags: ["数据结构", "线段树", "动态开点", "大值域", "区间求和"]
timeComplexity: '单次修改/查询O(log V)，V为值域'
spaceComplexity: 'O(m log V)'
codePath: '数据结构\动态开点线段树\Untitled1.cpp'
level: A
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 算法原理

普通线段树通过 `rt<<1`、`rt<<1|1` 给左右儿子固定编号，因此必须在一开始就开好 $4n$ 大小的数组。当数组长度 $n$ 高达 $10^9$ 时，$4n$ 个节点本身就会把内存撑爆（MLE）。

然而这类问题的另一个关键约束是：虽然 $n$ 很大，但操作次数 $m$ 往往只有 $10^5$ 量级。也就是说，整棵线段树中绝大多数区间根本不会被访问到，真正"有意义"的区间只有 $O(m \log n)$ 个。动态开点的核心思想正是**按需建点**——不在建树时一次性开出全部节点，而是"访问到哪个区间，才创建哪个区间对应的节点"。

实现上，动态开点不再用 `u<<1` 推导儿子编号，而是：

1. 用一个计数器 `tot` 统一分配新节点编号；
2. 每个节点记录左右儿子的编号 `lc`、`rc`（初始为 `-1` 表示尚未创建）；
3. 递归进入某个儿子之前，先判断该儿子是否已创建，若没有就用 `build` 现场 `++tot` 建点并挂到父节点的 `lc`/`rc` 上。

由于每次操作沿树高下降，至多访问 $O(\log V)$ 个节点、新增 $O(\log V)$ 个节点，总节点数 $O(m \log V)$，远小于 $4n$。

本题的初始序列是 $a_i = i$（值域 $[1, V]$，其中 $V = \text{INF} = 10^9 + 10$），因此任意区间 $[l, r]$ 的初始和可直接由等差数列求和得到，动态开点时无需另存原数组——新建节点的初值在 `build` 时当场算出来即可。

## 核心公式 / 状态定义与转移方程

为叙述方便，下文用 $S_u$ 表示代码里的 `wh`（区间和），$A_u$ 表示 `addz`（加法懒标记），$L_u, R_u$ 表示 `l, r`（管辖区间端点），$lc_u, rc_u$ 表示 `lc, rc`（左右儿子编号）。

每个节点 $u$ 维护的状态：

- $L_u, R_u$：节点管辖的区间 $[L_u, R_u]$。
- $lc_u, rc_u$：左右儿子的节点编号，$-1$ 表示尚未创建。
- $S_u$：区间和。
- $A_u$：加法懒标记，表示该区间每个元素还需再加 $A_u$。

**pushup（fixtree）**：用儿子更新父亲。

$$S_u = S_{lc_u} + S_{rc_u}$$

**pushdown（banzai）**：把懒标记下传给儿子。令 $v$ 为 $lc_u$ 或 $rc_u$：

$$S_v \leftarrow S_v + A_u \times (R_v - L_v + 1)$$

$$A_v \leftarrow A_v + A_u$$

下传完成后 $A_u \leftarrow 0$。

**区间加（完全覆盖时）**：

$$S_u \leftarrow S_u + k \times (R_u - L_u + 1), \quad A_u \leftarrow A_u + k$$

**动态开点的初始值**：初始序列 $a_i = i$，新建节点 $[l, r]$ 的初始和为

$$\sum_{i=l}^{r} i = \frac{r(r+1)}{2} - \frac{(l-1)\,l}{2}$$

## 逐行代码解析

### 头文件与快读快写

```cpp
#include<bits/stdc++.h>
#define int unsigned long long
using namespace std;
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

- `#define int unsigned long long` 把 `int` 全部替换为 `unsigned long long`，应对值域 $10^9$ 下区间和可达 $10^{18}$ 量级的溢出风险；相应地 `main` 必须写成 `signed main`，否则会与 `int` 宏定义冲突。
- `read`/`write` 是手写快读快写，`(ch^48)` 把字符数字转成数值，`x=(x<<1)+(x<<3)` 等价于 `x*10`。
- 用 `getchar`/`putchar` 而非 `cin/cout`，避免大输入输出下超时。

### 常量与节点结构

```cpp
const int N=5e6+10,INF=1e9+10;
struct aqtw{
	int tot=0;
	struct redstone{
		int l=-1,r=-1;
		int lc=-1,rc=-1;
		int wh;
		int addz=0;
	} wcj[N];
```

- `INF = 1e9+10` 是值域上界；根区间为 $[1, \text{INF}]$，动态开点只需在值域内按需建点。
- `wcj[N]` 预开 $5 \times 10^6 + 10$ 个节点，对应 $m = 10^5$ 次操作、每次新增 $O(\log V)$ 个节点的上界。
- `l/r` 存节点管辖区间端点，`lc/rc` 存左右儿子编号（初始 `-1` 表示未创建），`wh` 存区间和，`addz` 存加法懒标记。

### fixtree 与 banzai（pushup / pushdown）

```cpp
	void fixtree(int u){
		if(wcj[u].l==-1||wcj[u].r==-1) return;
        int lc=wcj[u].lc, rc=wcj[u].rc;//记录左右儿子的指针。由于动态开点，原来 u<<1,u<<1|1 的用法不再生效
        wcj[u].wh = wcj[lc].wh + wcj[rc].wh;
    }

	void banzai(int p){
		if(wcj[p].l==-1||wcj[p].r==-1) return;
		int lc=wcj[p].lc, rc=wcj[p].rc;
		int lenL = wcj[lc].r - wcj[lc].l + 1;
		int lenR = wcj[rc].r - wcj[rc].l + 1;
		if(wcj[p].addz != 0){
			wcj[lc].wh += wcj[p].addz * lenL;
			wcj[rc].wh += wcj[p].addz * lenR;
			wcj[lc].addz += wcj[p].addz;
			wcj[rc].addz += wcj[p].addz;
			wcj[p].addz = 0;
		}
	}
```

- `fixtree` 是 pushup：父亲的区间和 = 左右儿子区间和之和；`l==-1` 是空节点保护，避免递归到未创建节点时越界。
- `banzai` 是 pushdown：先算出左右儿子的区间长度 `lenL/lenR`，再按"懒标记 × 长度"把 `addz[p]` 累加到儿子，最后清零 `addz[p]`。
- 懒标记的增量必须是 $A \times \text{len}$ 而不是 $A$ 本身——懒标记表示"每个元素都要加"，区间和的增量要乘上元素个数。
- 下传前先判 `addz != 0`，避免无标记时做无谓的累加。

### init 与 build（初始化与动态建点）

```cpp
	void init(){//初始化第一个节点
	    tot = 0;
	    wcj[1].l = wcj[1].r = 0;
	    wcj[1].wh = wcj[1].addz = 0;
	    wcj[1].lc = wcj[1].rc = -1;
	}

	int build(int l,int r,int x){//加入节点
		++tot;
		wcj[tot].l = l;
		wcj[tot].r = r;
		wcj[tot].wh=x;
		wcj[tot].addz = 0;
		wcj[tot].lc = wcj[tot].rc = -1;
		return tot;
	}
```

- `init` 把根节点（编号 1）置为"空"，随后在 `main` 里立刻用 `build(1, INF, ...)` 把它改写成真正的根区间 $[1, \text{INF}]$。
- `build` 是动态开点的唯一入口：`++tot` 分配新编号，写入区间端点和初值，儿子指针置 `-1`，返回新节点编号供父节点挂接。
- 新建节点的初值 `x` 由"初始序列"决定，本题在调用处直接传入等差数列求和结果，无需读取原数组。

### plustree（区间加）

```cpp
	void plustree(int u,int l,int r,int obl,int obr,int k){
		if(wcj[u].l==-1||wcj[u].r==-1) return;
		if(obl<=l && r<=obr){
            wcj[u].wh += k * (r-l+1);
            wcj[u].addz += k;
            return;
        }
        int mid=(l+r)>>1;
        int cns=0;
        if(wcj[u].lc==-1){
        	cns=(mid*(mid+1)/2)-(l*(l-1)/2);
			build(l,mid,cns);
			wcj[u].lc=tot;
		}
		if(wcj[u].rc==-1){
			cns=(r*(r+1)/2)-(mid*(mid+1)/2);
			build(mid+1,r,cns);
			wcj[u].rc=tot;
		}
        banzai(u);
        if(obl<=mid) plustree(wcj[u].lc,l,mid,obl,obr,k);
        if(obr>mid) plustree(wcj[u].rc,mid+1,r,obl,obr,k);
        fixtree(u);	
	}
```

- 完全覆盖 `obl<=l && r<=obr` 时只更新 `wh += k×长度`、`addz += k` 后返回，不下传——这正是懒标记的核心，避免递归到叶子退化到 $O(n)$。
- 进入子节点前判 `lc/rc == -1`，若未创建则现场 `build`：左区间初值 `mid(mid+1)/2 - (l-1)l/2`、右区间初值 `r(r+1)/2 - mid(mid+1)/2`，即 $\sum_{i=l}^{mid} i$ 与 $\sum_{i=mid+1}^{r} i$。
- 递归前先 `banzai(u)` 把父亲的懒标记下传，保证子节点区间和正确；递归后 `fixtree(u)` 用儿子回传父亲。
- 新节点编号取自 `tot`（`build` 返回后 `tot` 已自增），挂到 `wcj[u].lc/rc` 上，取代固定编号 `u<<1`。

### findxigema 与 main（区间求和与主函数）

```cpp
	int findxigema(int u,int l,int r,int obl,int obr){
		if(wcj[u].l==-1||wcj[u].r==-1) return 0;
		if(obl<=l && r<=obr) return wcj[u].wh;
		int mid=(l+r)>>1;
		int cns=0;
		if(wcj[u].lc==-1){
		cns=(mid*(mid+1)/2)-(l*(l-1)/2);
			build(l,mid,cns);
			wcj[u].lc=tot;
		}
		if(wcj[u].rc==-1){
			cns=(r*(r+1)/2)-(mid*(mid+1)/2);
			build(mid+1,r,cns);
			wcj[u].rc=tot;
		}
		banzai(u);
		int res=0;
		if(obl<=mid) res += findxigema(wcj[u].lc,l,mid,obl,obr);
		if(obr>mid) res += findxigema(wcj[u].rc,mid+1,r,obl,obr);
		return res;
	}
} lzx;

signed main(){
	int n,m;
	n=read(); m=read(); 
	lzx.init();
	lzx.build(1,INF,(INF*(INF+1)/2));
	while(m--){
		int c,x,y,k;
		c=read();
		if(c==1){
			x=read(); y=read(); k=read();
			lzx.plustree(1,1,INF,x,y,k);
		}else{
			x=read(); y=read();
			int ans = lzx.findxigema(1,1,INF,x,y);
			write(ans);
			putchar('\n');
		}
	}
    return 0; 
}
```

- `findxigema` 是区间求和：空节点返回 0，完全覆盖直接返回 `wh`，否则同样按需建点、下传懒标记后递归左右儿子并累加。
- 查询也要按需建点：因为懒标记可能停留在某个"半创建"节点上，只有保证结构完整才能正确下传。
- 根节点初值 `INF*(INF+1)/2` 即 $\sum_{i=1}^{\text{INF}} i$，对应初始序列 $a_i=i$。
- `c==1` 是区间加 `[x,y]+=k`，否则是区间求和 `[x,y]`；`n` 读入后未使用，值域由 `INF` 决定。

## 复杂度分析

- **时间**：每次区间加 / 区间求和都沿树高下降，深度为 $O(\log V)$（$V = \text{INF}$ 为值域大小）。每层常数时间，单次操作 $O(\log V)$；$m$ 次操作共 $O(m \log V)$。
- **空间**：每个操作至多新增 $O(\log V)$ 个节点（每层最多左右各建一个），总节点数 $O(m \log V)$；每个节点 $O(1)$，故空间 $O(m \log V)$。代码预开 `N = 5e6+10` 个节点的数组。
- **与普通线段树对比**：普通线段树需要 $O(V)$（约 $4V$）空间，在 $V = 10^9$ 时不可行；动态开点把空间压到与操作次数相关的 $O(m \log V)$。

## 适用场景

- 值域 / 下标范围极大（如 $10^9$）但有效操作点稀疏（如 $10^5$）的区间问题。
- 无法或不想预先离散化，或需要在原始值域上直接维护区间结构时。
- 作为可持久化线段树（主席树）、线段树合并、权值线段树等技巧的共同基础——它们都依赖"按需建点"。

## 常见陷阱与注意事项

1. 儿子编号从 `u<<1`/`u<<1|1` 换成 `lc/rc` 指针后，递归与 pushup/pushdown 都要用 `wcj[u].lc`、`wcj[u].rc`，不能再用移位公式（源码注释也专门强调）。
2. 访问子节点前必须判 `lc/rc == -1` 并按需 `build`，否则会读未初始化的节点，导致越界或错误答案。
3. 懒标记下传的增量要乘区间长度 $R_v - L_v + 1$，漏乘会让区间和少算。
4. 动态开点的初始值必须与"初始序列"一致：本题初始 $a_i=i$，初值用等差数列求和公式；换一道题初值不同，公式也要跟着换。
5. 数组要按 $O(m \log V)$ 预开足够大（本题 `5e6+10`），开小会越界。
6. `#define int unsigned long long` 之后 `main` 必须写 `signed main`，否则编译报错。

## 对比与扩展

- **与普通线段树**：普通线段树预开 $4n$、儿子编号固定、常数小，适合 $n$ 已知且不大；动态开点按需建点、常数略大，适合大值域稀疏数据。
- **与离散化线段树**：离散化把坐标压到 $O(m)$，空间更省，但需要先读入全部坐标离线处理；动态开点可在线处理、无需预处理，代价是常数略大（源码也提到这一点）。
- **扩展方向**：可持久化线段树（主席树）在动态开点基础上，每次修改只新建一条 $O(\log V)$ 的路径并保留历史版本；线段树合并利用动态开点的节点指针实现两棵树的合并；权值线段树把下标从"位置"换成"值"，同样依赖动态开点。
