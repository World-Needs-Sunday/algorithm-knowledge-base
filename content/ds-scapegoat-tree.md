---
id: ds-scapegoat-tree
title: '替罪羊树'
category: 数据结构
subcategory: 平衡树
subSubcategory: ''
tags: ["平衡树", "替罪羊树", "重量平衡树", "重构", "均摊分析"]
timeComplexity: '插入/删除均摊 O(log n)，查找 O(log n)'
spaceComplexity: 'O(n)'
codePath: '数据结构\替罪羊树\Untitled1.cpp'
level: A
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 算法原理

替罪羊树（Scapegoat Tree）是一种**重量平衡树**（weight-balanced tree），其核心思路非常直接：**平时不调整结构，等树变得"太不平衡"时，直接抓出罪魁祸首的整棵子树推倒重建**。

大多数平衡树（AVL、红黑树、Splay、Treap 等）会在每次插入或删除后小心翼翼地通过旋转来维持平衡，代码复杂且容易写错。替罪羊树则选择了一条截然不同的路：插入、删除时先按普通二叉搜索树（BST）的方式操作，**不做任何平衡调整**；等某棵子树的大小失衡到一定程度，就把导致失衡的那棵子树——也就是"**替罪羊**"——整个中序遍历后重新建成一棵完全平衡的树，这就是它名字的由来。

替罪羊树用一个参数 $\alpha$（平衡因子）来刻画"失衡"。对树上每个节点 $b$，设 $\text{sz}(b)$ 为以 $b$ 为根的子树大小，$\text{sz}(\text{ls}(b))$、$\text{sz}(\text{rs}(b))$ 分别为左右子树的大小。若存在节点 $b$ 满足

$$\max\bigl(\text{sz}(\text{ls}(b)),\ \text{sz}(\text{rs}(b))\bigr) > \alpha \cdot \text{sz}(b)$$

即某棵子树的大小超过了整棵子树大小的 $\alpha$ 倍，就认为以 $b$ 为根的子树"失衡"，这棵子树就是替罪羊，需要被重构。

重构分两步：**中序遍历**把替罪羊子树中的所有节点按升序收集进数组（BST 的中序序列天然有序），**二分建树**取数组中点作为新根、左右递归建树，得到一棵完全平衡的子树。单次重构的代价为 $O(\lvert T \rvert)$，其中 $\lvert T \rvert$ 是替罪羊子树的大小。

插入时先按普通 BST 插入新节点，再从插入点**自底向上回溯**，检查路径上是否存在失衡节点；找到（最深的）失衡节点后，就对它的整棵子树重构。删除有两种常见实现：**惰性删除**（不真正删节点，只把节点标记为已删除，等有效节点比例过低时触发重构）与**真正删除**（按 BST 方式删节点，再像插入一样回溯检查并重构），本代码采用后者。

## 核心公式 / 状态定义与转移方程

**状态定义**：每个节点 $b$ 存储键值 $\text{cnt}$（本代码沿用这一命名，`cnt` 即键值而非重复计数）、左右儿子 $\text{ls}$、$\text{rs}$、子树大小 $\text{sz}$，以及节点自存下标 $\text{u}$（恒等于其数组下标）。整棵树以根下标 $\text{rot}$ 标识，空节点用下标 0 表示（$\text{u}=0,\ \text{sz}=0$）。

**子树大小递推**（每个节点代表一个键值，本代码不做重复计数，重复键值以多个独立节点存在）：

$$\text{sz}(b) = \text{sz}(\text{ls}(b)) + \text{sz}(\text{rs}(b)) + 1$$

**平衡判定**：节点 $b$ 平衡当且仅当

$$\max\bigl(\text{sz}(\text{ls}(b)),\ \text{sz}(\text{rs}(b))\bigr) \le \alpha \cdot \text{sz}(b)$$

整棵树平衡当且仅当每个节点都满足上式。插入/删除后自底向上回溯，第一个不满足该式（即最深失衡）的节点就是替罪羊。

**重构转移**：设替罪羊子树的中序节点序列为 $A[0 \ldots m-1]$，重建时取 $mid=\lfloor (l+r)/2 \rfloor$ 作为根，递归处理区间 $[l, mid-1]$ 与 $[mid+1, r]$；当 $l>r$ 时返回空节点 0。重建后的子树完全平衡，高度为 $O(\log m)$。

## 逐行代码解析

下面是完整实现（洛谷 P3369「普通平衡树」的六种操作）。字段 `cnt` 存键值、`u` 存节点自存下标、`sz` 为子树大小、`ls/rs` 为左右儿子。

### 模块一 · 头文件、全局数组与节点结构

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
const double ALPHA=0.75;
int tot=0,rot=0;
struct bst{
    int u,cnt;
    int ls,rs;
    int sz;
    bst():u(0),cnt(0),ls(0),rs(0),sz(0){}
    bst(int b,int x):u(b),cnt(x),ls(0),rs(0),sz(1){}
} wcj[N];
```

- `#define int long long` 把 `int` 全局替换为 `long long`，因此主函数必须写成 `signed main`，否则返回值类型冲突。
- 结构体字段 `cnt` 存键值、`u` 存节点自存下标（恒等于数组下标）、`ls/rs` 为左右儿子、`sz` 为子树大小；本代码不设重复计数，重复键值以多个独立节点存在。
- `wcj` 是定长数组（$N=4\times10^5+10$），下标即节点编号；`wcj[0]` 由默认构造函数初始化成全 0，作为空节点，`wcj[b].u==0` 即表示节点 $b$ 为空。
- 全局 `tot` 记录已分配节点数、`rot` 记录根下标；`ALPHA=0.75` 是平衡因子，落在经验最优区间 $0.7 \sim 0.8$ 内。

### 模块二 · 查找与平衡判定

```cpp
int cxsearch(int b,int v){
    if(wcj[b].u==0||wcj[b].cnt==v) return wcj[b].u;
    if(v<wcj[b].cnt) return cxsearch(wcj[b].ls,v);
    else return cxsearch(wcj[b].rs,v);
}
bool isping(int b){           //判断子树是否平衡
    if(b==0) return true;
    int lsz=wcj[wcj[b].ls].sz;
    int rsz=wcj[wcj[b].rs].sz;
    return max(lsz,rsz)<=ALPHA*wcj[b].sz;
}
```

- `cxsearch` 沿 BST 性质递归下探：命中键值 $v$ 或走到空节点即返回节点下标；本代码中它未被主流程调用，仅作为查找原语保留。
- `isping` 判定子树是否平衡：空节点恒平衡；否则取左右子树大小，返回 $\max(\text{lsz},\text{rsz}) \le \alpha \cdot \text{sz}$，与"核心公式"中的失衡判据一一对应。
- 平衡因子 $\alpha$ 越大越接近普通 BST，越小重构越频繁；`ALPHA=0.75` 是常见的折中取值。

### 模块三 · 中序遍历与重建

```cpp
void findnode(int b,vector<int>& son){//中序遍历收集节点编号
    if(b==0) return;
    findnode(wcj[b].ls,son);
    son.push_back(b);
    findnode(wcj[b].rs,son);
}
int build(vector<int>& son,int l,int r){//重建平衡树
    if(l>r) return 0;
    int mid=(l+r)>>1;
    int nwr=son[mid];
    wcj[nwr].ls=build(son,l,mid-1);
    wcj[nwr].rs=build(son,mid+1,r);
    wcj[nwr].sz=wcj[wcj[nwr].ls].sz+wcj[wcj[nwr].rs].sz+1;
    return nwr;
}
void rebuild(int &b){              //重构子树
    vector<int>son;
    findnode(b,son);
    for(int id:son){
        wcj[id].ls=wcj[id].rs=0;
    }
    b=build(son,0,son.size()-1);
}
```

- `findnode` 中序遍历把子树内所有节点编号按键值升序收集进 `son` 向量，BST 的中序序列天然有序。
- `build` 取区间中点作根、左右递归建树，并沿途重算 `sz`，建出完全平衡的子树；`l>r` 时返回 0（空节点）。
- `rebuild` 先收集节点、清空各节点的左右儿子指针，再整体二分重建，并把新根写回引用参数 `b`（`b` 即被重构子树的根下标）。
- 这三个函数合起来实现"推倒重建"：单次重构代价为 $O(m)$，$m$ 为替罪羊子树大小。

### 模块四 · 插入与删除

```cpp
int crinsert(int b,int v){
    if(wcj[b].u==0){
        tot++;
        wcj[tot]=bst(tot,v);
        return wcj[tot].u;
    }
    if(v<wcj[b].cnt) wcj[b].ls=crinsert(wcj[b].ls,v);
    else wcj[b].rs=crinsert(wcj[b].rs,v);
    wcj[b].sz=wcj[wcj[b].ls].sz+wcj[wcj[b].rs].sz+1;
    if(!isping(b)) rebuild(b);
    return wcj[b].u;
}
int scdelete(int b,int v){
    if(wcj[b].u==0) return 0;
    if(v<wcj[b].cnt){
        wcj[b].ls=scdelete(wcj[b].ls,v);
        wcj[b].sz=wcj[wcj[b].ls].sz+wcj[wcj[b].rs].sz+1;
        if(!isping(b)) rebuild(b);
    }
    else if(v>wcj[b].cnt){
        wcj[b].rs=scdelete(wcj[b].rs,v);
        wcj[b].sz=wcj[wcj[b].ls].sz+wcj[wcj[b].rs].sz+1;
        if(!isping(b)) rebuild(b);
    }
    else{
        if(wcj[b].ls==0){
            return wcj[b].rs;
        }
        else if(wcj[b].rs==0){
            return wcj[b].ls;
        }
        else{
            int nw=wcj[b].rs;
            while(wcj[nw].ls!=0) nw=wcj[nw].ls;
            wcj[b].cnt=wcj[nw].cnt;
            wcj[b].rs=scdelete(wcj[b].rs,wcj[nw].cnt);
            wcj[b].sz=wcj[wcj[b].ls].sz+wcj[wcj[b].rs].sz+1;
            if(!isping(b)) rebuild(b);
        }
    }
    return wcj[b].u;
}
```

- `crinsert` 走到空位（`wcj[b].u==0`）就 `tot++` 新建节点；否则按 BST 方向递归插入，返回后更新 `sz` 并检查 `isping`，失衡即 `rebuild`。
- `scdelete` 按"未找到 / 去左 / 去右 / 命中"四分：命中时若只有一个儿子则直接接上另一个儿子；双儿子则取右子树最小键值覆盖当前键值，再递归删除该后继。
- 插入/删除都沿递归**自底向上回溯**，返回路径上每个节点都重算 `sz` 并判失衡，最深失衡节点会最先被重构。
- `return wcj[b].u` 返回节点下标，调用处 `rot=crinsert(rot,x)` 把新根写回全局 `rot`，保持根指针正确。

### 模块五 · 查排名与查第 k 小

```cpp
int queryless(int b,int x){
    if(b==0) return 0;
    if(x<=wcj[b].cnt) return queryless(wcj[b].ls,x);
    else return wcj[wcj[b].ls].sz+1+queryless(wcj[b].rs,x);
}
int kth(int b,int k){
    if(k<=wcj[wcj[b].ls].sz) return kth(wcj[b].ls,k);
    else if(k==wcj[wcj[b].ls].sz+1) return wcj[b].cnt;
    else return kth(wcj[b].rs,k-wcj[wcj[b].ls].sz-1);
}
```

- `queryless(b,x)` 返回「键值 $<x$ 的节点个数」：沿键值下探，向左走不累加，向右走累加左子树大小 +1，故排名 $=\text{queryless}(\text{rot},x)+1$。
- `kth(b,k)` 按子树大小二分：$k \le \text{sz}(\text{ls})$ 走左子树，$k = \text{sz}(\text{ls})+1$ 命中当前键值，否则走右子树并把 $k$ 减掉左子树大小 +1。
- 两个函数都只沿一条到叶子的路径递归，树高 $O(\log n)$，故均摊/期望 $O(\log n)$。

### 模块六 · 前驱、后继与主函数

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
    ios::sync_with_stdio(false);
    cin.tie(nullptr);cout.tie(nullptr);
    int n;cin>>n;
    while(n--){
        int opt,x;cin>>opt>>x;
        if(opt==1) rot=crinsert(rot,x);
        else if(opt==2) rot=scdelete(rot,x);
        else if(opt==3) cout<<queryless(rot,x)+1<<'\n';
        else if(opt==4) cout<<kth(rot,x)<<'\n';
        else if(opt==5) cout<<qianququ(rot,x)<<'\n';
        else if(opt==6) cout<<houjiji(rot,x)<<'\n';
    }
    return 0;
}
```

- `qianququ` 迭代下探，维护「$<x$ 的键值中的最大者」作为前驱；`houjiji` 对称地维护「$>x$ 的键值中的最小者」作为后继；无解时分别返回 `LLONG_MIN` / `LLONG_MAX`。
- `main` 实现 P3369 六种操作：1 插入、2 删除、3 查排名、4 查第 $k$ 小、5 前驱、6 后继。
- 每次插入/删除后 `rot` 都被赋值为新的根下标，保证下一次操作作用在最新树上。
- `#define int long long` 下主函数用 `signed main`，避免 `long long main` 非法。

## 复杂度分析

- **单次重构**：中序遍历 + 二分重建均为 $O(m)$，$m$ 为替罪羊子树大小；最坏一次重构可达 $O(n)$。
- **均摊复杂度**：替罪羊树的复杂度是**均摊**的，很大程度上取决于 $\alpha$ 的取值。插入、删除均摊 $O(\log n)$，查找 $O(\log n)$。虽然单次操作可能因触发重构而变慢，但把重构代价摊到足够多次的插入/删除上，所有操作的均摊代价都是 $O(\log n)$。
- **空间复杂度**：$O(n)$。`wcj` 是定长数组（$N=4\times10^5+10$），每个节点是定长结构体，惰性删除之外不额外分配；`rebuild` 中临时用到的 `vector<int> son` 最多 $O(n)$ 且随调用结束释放。
- 参数 $\alpha$ 的经验最优区间为 $0.7 \sim 0.8$；$\alpha$ 越大越接近普通 BST（退化为链则查找变 $O(n)$），$\alpha$ 越小重构越频繁、常数越大。

## 适用场景

- **入门与理解平衡树**：替罪羊树不写旋转、不维护平衡因子，用"重构"这一直观操作代替平衡调整，实现简单、常数小，适合作为理解平衡树思想的切入点。
- **动态有序集合**：需要同时支持插入、删除、查排名、查第 $k$ 小、前驱后继的题目，典型如洛谷 P3369「普通平衡树」。
- **不适用**：需要**区间操作**、**完全可持久化**的场景（替罪羊树不支持）；对均摊复杂度敏感、可能被构造数据针对性卡掉的场合也应换严格平衡树（AVL/红黑树）或 `std::set`。

## 常见陷阱与注意事项

- **$\alpha$ 的取值**：$\alpha$ 过大（接近 1）树近乎普通 BST，易退化；$\alpha$ 过小（接近 0.5）重构过于频繁、常数大。经验取 $0.7 \sim 0.8$，本代码用 $0.75$。
- **均摊而非最坏**：单次插入/删除可能因触发重构而达到 $O(n)$，单次操作的峰值时间不保证 $O(\log n)$，只有均摊下才是 $O(\log n)$。
- **空节点必须全 0**：`crinsert` 依赖 `wcj[b].u==0` 判空、`isping` 依赖 `wcj[0].sz==0`，全局数组的默认构造恰好满足这一点；若改用动态分配需显式初始化 0 号位。
- **`#define int long long` 的连锁**：主函数必须写 `signed main`，且 `LLONG_MIN` / `LLONG_MAX` 用作前驱/后继的"无穷"哨兵，值域应与实际键值保持距离。
- **被卡风险**：重量平衡树的均摊分析依赖 $\alpha$ 与操作序列，某些构造数据可能使重构频繁触发、常数骤增，竞赛中作为正解时应评估数据强度。
- **删除的两种实现**：本代码是"真正删除"；若改用"惰性删除"，需额外维护有效节点计数，并在有效比例过低时触发重构，两者复杂度结论相同但代码结构不同。

## 对比与扩展

| 结构 | 是否旋转 | 复杂度 | 主要优势 | 主要代价 |
|---|---|---|---|---|
| 替罪羊树 | 否（重构） | 均摊 $O(\log n)$ | 实现简单、常数小、直观 | 均摊非最坏，不支持区间/可持久化 |
| 旋转 Treap | 是 | 期望 $O(\log n)$ | 思路直观 | 需写旋转 |
| FHQ-Treap | 否（分裂合并） | 期望 $O(\log n)$ | 代码短、可持久化 | 随机化，最坏退化 |
| Splay | 是（伸展） | 均摊 $O(\log n)$ | 区间操作强 | 常数较大 |
| AVL / 红黑树 | 是 | 严格 $O(\log n)$ | 最坏有保证 | 平衡调整复杂 |

- **与旋转平衡树对比**：替罪羊树用"整棵子树重建"代替旋转，免去旋转的边界处理；缺点是单次操作可能被重构拖慢，只有均摊保证。
- **与 FHQ-Treap 对比**：FHQ-Treap 靠 Split/Merge 组合完成所有操作、天然可持久化；替罪羊树不支持区间操作、也无法高效可持久化。
- **扩展方向**：**惰性删除 + 定期重构**可把删除也摊还到 $O(\log n)$；重量平衡的思想还被用于 KD 树等"按子树重量判平衡并重构"的数据结构。
- **前置/相关**：需先掌握 BST 与中序遍历；平衡树入门可先看「平衡树」总述，进阶可对比 Splay 与 FHQ-Treap。
