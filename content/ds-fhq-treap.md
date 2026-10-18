---
id: ds-fhq-treap
title: 'FHQ-Treap（无旋平衡树）'
category: 数据结构
subcategory: 平衡树
subSubcategory: ''
tags: ["平衡树", "FHQ-Treap", "分裂合并", "随机优先级", "可持久化"]
timeComplexity: '所有操作期望 O(log n)'
spaceComplexity: 'O(n)'
codePath: '数据结构\FHQ-Treap\Untitled1.cpp'
level: A
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 算法原理

FHQ-Treap（全称「非旋转式 Treap 及可持久化」）由中国 OIer 范浩强发明。它保留 Treap「用随机优先级维持平衡」的核心思想，却完全抛弃旋转操作，把整棵平衡树的所有增删改查归结为两个原子操作：**Split（分裂）** 与 **Merge（合并）**。

**Treap = Tree + Heap**，每个节点同时满足两条性质：

1. **二叉搜索树（BST）性质**：左子树所有键值小于根，右子树所有键值大于根，中序遍历得到有序序列。
2. **堆性质**：每个节点携带一个与键值完全无关的**随机优先级**，父节点的优先级大于子节点，形成堆结构。

随机优先级是平衡的秘诀：因为优先级随机、与键值无关，每个节点等概率成为根，整棵树的期望高度为 $O(\log n)$，从而所有操作期望 $O(\log n)$。相比带旋转的 Treap / AVL / 红黑树，FHQ-Treap 不维护平衡因子、不写旋转函数，代码短、逻辑清晰，还天然支持可持久化，因此成为竞赛中最常用的平衡树实现之一。

## 核心公式 / 状态定义与转移方程

**状态定义**：节点 $u$ 存键值 `cnt`、随机优先级 `prw`、左右儿子 `ls,rs`、子树大小 `sz`、重复计数 `cho`；整棵树以根下标 `rot` 标识。子树大小不变量：

$$sz_u = sz_{ls_u} + sz_{rs_u} + cho_u$$

**Split 转移**：$\mathrm{split}(u,v)$ 返回两棵树 $(x,y)$，其中 $x$ 含所有键值 $\le v$ 的节点，$y$ 含所有键值 $>v$ 的节点。按「当前根与 $v$ 的大小关系」分三种情况：

- $u=0$：$(x,y)=(0,0)$；
- $val_u \le v$：根连同整个左子树归入 $x$（令 $x=u$），再对右子树递归 $\mathrm{split}(rs_u,v)\Rightarrow(rs_u,y)$；
- $val_u > v$：根连同整个右子树归入 $y$（令 $y=u$），再对左子树递归 $\mathrm{split}(ls_u,v)\Rightarrow(x,ls_u)$。

**Merge 转移**：$\mathrm{merge}(x,y)$ 把两棵树合并为一棵，前提是 $x$ 中所有键值都不大于 $y$ 中所有键值。比较两棵根的随机优先级（本代码为小根堆，`prw` 小者作根）：

- $x=0$ 或 $y=0$：返回另一棵；
- $pri_x < pri_y$：$x$ 作新根，其右子树改为 $\mathrm{merge}(rs_x,y)$；
- $pri_x \ge pri_y$：$y$ 作新根，其左子树改为 $\mathrm{merge}(x,ls_y)$。

**操作组合**（$a,b,c$ 为切出的三段）：

$$\mathrm{insert}(v):\quad \mathrm{split}(rot,v)\Rightarrow(a,b),\quad rot=\mathrm{merge}(\mathrm{merge}(a,\mathrm{new}(v)),b)$$

$$\mathrm{erase}(v):\quad \mathrm{split}(rot,v)\Rightarrow(a,c),\ \mathrm{split}(a,v-1)\Rightarrow(a,b),\ b=\mathrm{merge}(ls_b,rs_b),\ rot=\mathrm{merge}(\mathrm{merge}(a,b),c)$$

查询排名用 $\mathrm{split}(rot,x-1)$ 后取 $|a|+1$；查询第 $k$ 小沿 `sz` 在树上二分；前驱/后继分别取切分后左半最大值、右半最小值。

## 逐行代码解析

下面是完整实现（洛谷 P3369「普通平衡树」的六种操作）。字段命名与上文对应：`cnt` 为键值、`cho` 为重复计数、`prw` 为随机优先级、`u` 为节点自存的下标。

### 模块一：头文件、全局数组、节点结构与 `nwn`/`pulp`

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
const int N=4e5+10;
int tot=0,rot=0;
auto seed=chrono::steady_clock::now().time_since_epoch().count();
mt19937_64 rng(seed);
int rdm[N];
struct bst{
	int u,cnt;
	int ls,rs;
	int sz;
	int cho,prw;
	bst():u(0),cnt(0),ls(0),rs(0),sz(0),cho(0),prw(0){}
	bst(int b,int x):u(b),cnt(x),ls(0),rs(0),sz(1),cho(1),prw(rdm[tot]){}
} wcj[N];
int nwn(int v) {
    tot++;
    wcj[tot]=bst(tot,v);
    return tot;
}
void pulp(int u){
	wcj[u].sz=wcj[wcj[u].ls].sz+wcj[wcj[u].rs].sz+wcj[u].cho;
}
```

- `#define int long long` 把 `int` 全局替换为 `long long`，因此主函数必须写成 `signed main`（`signed` 即 `int`），避免返回值类型冲突。
- `rdm[N]` 预生成 $n$ 个随机优先级，节点构造时直接取 `rdm[tot]`，保证每个节点的优先级稳定不变。
- `wcj` 是节点数组，下标即节点编号；`bst(int b,int x)` 用键值 $x$ 初始化节点，`cho=1` 表示当前有 1 个该键值。
- `pulp`（pushup）维护子树大小：`sz = 左子树大小 + 右子树大小 + 重复计数`。

### 模块二：`split` 按键值分裂

```cpp
void split(int u,int v,int &x,int &y){
	if(!u){
		x=y=0;return ;
	}
	if(wcj[u].cnt<=v){
		x=u;split(wcj[u].rs,v,wcj[u].rs,y);pulp(x);
	}else{
		y=u;split(wcj[u].ls,v,x,wcj[u].ls);pulp(y);
	}
}
```

- 空节点直接置 `x=y=0` 返回。
- `wcj[u].cnt <= v`：根及整棵左子树都 $\le v$，归入 $x$；右子树可能混合，于是 `split(rs, v, rs, y)` 把切出的左半**接回** `u` 的右儿子。
- 否则根及右子树都 $>v$，归入 $y$，递归分裂左子树并接回 `u` 的左儿子。
- `wcj[u].rs` / `wcj[u].ls` 作为**引用**传入，分裂结果原地写回对应儿子，不改变父子关系，堆性质自然保持。

### 模块三：`merge` 按优先级合并

```cpp
int merge(int x,int y){
	if(!x||!y) return x|y;
	if(wcj[x].prw<wcj[y].prw){
		wcj[x].rs=merge(wcj[x].rs,y);pulp(x);return x;
	}else{
		wcj[y].ls=merge(x,wcj[y].ls);pulp(y);return y;
	}
}
```

- 一棵为空时直接返回另一棵（`x|y` 等价于 `x ? x : y`，因为节点下标从 1 开始、0 表示空）。
- 小根堆：`wcj[x].prw < wcj[y].prw` 时 $x$ 作根，递归合并 $x$ 的右子树与 $y$；否则 $y$ 作根，递归合并 $x$ 与 $y$ 的左子树。
- 每次合并后调用 `pulp` 更新子树大小。
- 正确性前提：$x$ 中所有键值 $\le$ $y$ 中所有键值，Merge 才保证合并后仍是 BST。

### 模块四：查找、插入与删除

```cpp
int cxsearch(int b,int v){
	if(wcj[b].u==0||wcj[b].cnt==v) return wcj[b].u;//返回下标
	if(v<wcj[b].cnt) return cxsearch(wcj[b].ls,v);
	else return cxsearch(wcj[b].rs,v);
}
int crinsert(int v){
	int a,b,c;
	split(rot,v,b,c);
	split(b,v-1,a,b);
	if(b==0) return merge(merge(a,nwn(v)),c);
	else wcj[b].cho++;
	pulp(b);
	return merge(merge(a,b),c);;
}
int scdelete(int v){
	int a,b,c;
	split(rot,v,b,c);
	split(b,v-1,a,b);
	if(b==0) return rot;
	else if(wcj[b].cho==1){
		return merge(merge(a,merge(wcj[b].ls,wcj[b].rs)),c);
	} 
	else wcj[b].cho--;
	pulp(b);
	return merge(merge(a,b),c);
}
```

- `cxsearch` 沿键值比较递归下探，找到键值为 $v$ 的节点返回其下标。
- `crinsert`：两次 `split` 把 $\le v$、$=v$、$>v$ 三段切出；若 $=v$ 段为空则新建节点插入，否则 `cho++` 累加重复计数。
- `scdelete`：同理切出三段；若重复计数 `cho==1` 则丢弃根（合并其左右儿子），否则 `cho--`。
- 插入/删除后都要把三段 `merge` 回去，并把结果写回 `rot`（由调用处 `rot=crinsert(x)` 完成）。

### 模块五：查排名与查第 $k$ 小

```cpp
// 返回树中值 < x 的节点个数
int queryless(int b,int x){
    if(b==0) return 0;
    if(x<=wcj[b].cnt){
        return queryless(wcj[b].ls,x);
    }else{
        return wcj[wcj[b].ls].sz+wcj[b].cho+queryless(wcj[b].rs,x);
    }
}
// 返回第 k 小的值
int kth(int b,int k){
    if(k<=wcj[wcj[b].ls].sz){
        return kth(wcj[b].ls,k);
    }else if (k<=wcj[wcj[b].ls].sz+wcj[b].cho){
        return wcj[b].cnt;
    }else{
        return kth(wcj[b].rs,k-wcj[wcj[b].ls].sz-wcj[b].cho);
    }
}
```

- `queryless(b,x)` 返回「键值 $<x$ 的节点个数」：沿键值下探，向左走不累加，向右走累加左子树大小 + 当前 `cho`。
- 排名 $=queryless(rot,x)+1$，见主函数 opt 3。
- `kth(b,k)` 按子树大小二分：$k\le sz_{ls}$ 走左，$k\le sz_{ls}+cho$ 命中当前键值，否则走右并减掉左子树与当前节点的大小。
- 两个函数都只沿一条到叶子的路径递归，期望 $O(\log n)$。

### 模块六：前驱、后继与主函数

```cpp
int qianququ(int b,int x){
    int ans=LLONG_MIN;
    while(b!=0){
        if(wcj[b].cnt<x){
            ans=wcj[b].cnt;
            b=wcj[b].rs;
        }else{
            b=wcj[b].ls;
        }
    }
    return ans;
}
int houjiji(int b,int x){
    int ans=LLONG_MAX;
    while(b!=0){
        if(wcj[b].cnt>x){
            ans=wcj[b].cnt;
            b=wcj[b].ls;
        }else{
            b=wcj[b].rs;
        }
    }
    return ans;
}
signed main(){
	//主函数实现的是P3369
    ios::sync_with_stdio(false);
    cin.tie(nullptr);cout.tie(nullptr);
    int n;cin>>n;
    for(int i=1;i<=n;i++) rdm[i]=rng(); 
    while(n--){
		int opt,x;cin>>opt>>x;
		if(opt==1) rot=crinsert(x);
		else if(opt==2) rot=scdelete(x);
		else if(opt==3) cout<<queryless(rot,x)+1<<'\n';
		else if(opt==4) cout<<kth(rot,x)<<'\n';
		else if(opt==5) cout<<qianququ(rot,x)<<"\n";
		else if(opt==6) cout<<houjiji(rot,x)<<'\n';
	}
    return 0;
}
```

- `qianququ` 沿键值下探，维护「$<$ x 的键值中的最大者」，返回前驱（无则 `LLONG_MIN`）。
- `houjiji` 对称地维护「$>$ x 的键值中的最小者」，返回后继（无则 `LLONG_MAX`）。
- `main` 实现 P3369 六种操作：1 插入、2 删除、3 查排名、4 查第 $k$ 小、5 前驱、6 后继。
- 每次操作后 `rot` 被赋值为新的根下标，保证下一次操作作用在最新树上。

## 复杂度分析

- **时间复杂度**：`split` 与 `merge` 都只沿一条从根到叶的路径递归，期望树高为 $O(\log n)$，故两者期望 $O(\log n)$；插入、删除、排名、第 $k$ 小、前驱后继都由常数次 `split`/`merge` 或一次树上二分组成，期望均为 $O(\log n)$。
- **空间复杂度**：每个键值对应一个节点，节点为定长结构体，总空间 $O(n)$；`rdm` 数组同样 $O(n)$。

## 适用场景

- **动态有序集合**：需要同时支持插入、删除、查排名、查第 $k$ 小、前驱后继的题目，典型如洛谷 P3369、P6136（普通平衡树）。
- **竞赛快速实现**：不写旋转、不维护平衡因子，代码量小、逻辑清晰，适合需要平衡树但不想手写 AVL/红黑树的场景。
- **可持久化基础**：FHQ-Treap 无旋转、天然适合路径复制，是可持久化平衡树的标准底座。
- **序列/区间维护**：把「按键值分裂」换成「按子树大小分裂」，即可实现区间翻转等操作（文艺平衡树）。
- 不适用：需要严格最坏 $O(\log n)$ 且常数敏感的场合，可考虑 AVL/红黑树或 `std::set`/`std::multiset`；极少数刻意针对随机优先级的构造数据也需换严格平衡树。

## 常见陷阱与注意事项

1. **`split` 的引用回写**：`wcj[u].rs` 作为引用参数传给下一层 `split`，切出的左半必须接回 $u$ 的右儿子；若改成传值，分裂结果丢失、树被拆散。
2. **`merge` 的前提**：$x$ 中所有键值必须 $\le$ $y$ 中所有键值。随意 `merge` 两棵不满足该前提的树会破坏 BST 性质。
3. **删除要两次 `split` 隔离**：先按 $v$、再按 $v-1$ 切出「恰好等于 $v$」的一段；重复计数 `cho==1` 时才真正删节点（合并左右儿子），否则只 `cho--`。
4. **`#define int long long` 的连锁**：主函数必须用 `signed main()`，否则 `long long main` 非法；`int` 与 `long long` 混用也要注意，否则可能编译报错或溢出。
5. **随机数质量**：用 `mt19937_64` 而非 `rand()`，并用 `rdm` 数组预生成，保证节点优先级稳定；若用固定的 `rand()` 种子，可能被针对性卡掉。
6. **小根堆方向**：本代码 `prw` 小的作根；若改写成大根堆，`merge` 中的比较号要同步反转，否则堆性质失效。
7. **排名要 +1**：`queryless` 统计的是「严格小于 $x$」的个数，输出排名需 `queryless(rot,x)+1`。
8. **重复值的子树大小**：`kth` 的边界比较要含 `cho`（$k\le sz_{ls}+cho$），忽略重复计数会在有重复键值时找错第 $k$ 小。

## 对比与扩展

| 结构 | 是否旋转 | 复杂度 | 主要优势 | 主要代价 |
|---|---|---|---|---|
| FHQ-Treap | 否 | 期望 $O(\log n)$ | 代码短、可持久化 | 随机化，最坏退化 |
| 旋转 Treap | 是 | 期望 $O(\log n)$ | 思路直观 | 需写旋转 |
| Splay | 是（伸展） | 均摊 $O(\log n)$ | 区间操作强 | 常数较大 |
| AVL / 红黑树 | 是 | 严格 $O(\log n)$ | 最坏有保证 | 平衡调整复杂 |
| `std::set` / `std::multiset` | 无 | $O(\log n)$ | 现成可用 | 功能受限、无法扩展 |

- **与旋转 Treap 对比**：两者都用随机优先级维持平衡，但 FHQ-Treap 用 Split/Merge 替代旋转，省去旋转函数的边界处理，且天然支持可持久化。
- **与 Splay 对比**：Splay 靠均摊保证复杂度、区间操作能力更强；FHQ-Treap 是期望复杂度、实现更简单。
- **扩展方向**：可持久化 FHQ-Treap（路径复制）、文艺平衡树（按子树大小分裂 + 懒标记区间翻转）、区间第 $k$ 大等。
- **前置/相关**：随机优先级依赖随机数生成，见「mt19937 随机数」；可持久化思想见「可持久化数据结构」；本题是普通平衡树模板 P3369。
