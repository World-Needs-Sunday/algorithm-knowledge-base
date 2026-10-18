---
id: prob-luogu-p17248
title: 'P17248 审判与救赎'
oj: '洛谷'
problemId: 'P17248'
difficulty: '困难'
category: 数据结构
tags: ["主席树", "贪心", "异或", "区间查询"]
timeComplexity: 'O((n+q) log V)'
spaceComplexity: 'O(n log V)'
codePath: '题目专辑\数据结构\审判与救赎\代码.cpp'
level: A
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 题目描述

给定长度为 $n$ 的非负整数序列 $a_1,a_2,\dots,a_n$。有 $q$ 次询问，每次给定 $l,r$。

对区间 $[l,r]$ 中的所有数，将它们划分成若干非空组（组内元素不要求连续）。每组权值定义为组内所有数的按位异或和。若各组权值为 $b_1,b_2,\dots,b_k$，则本次划分的价值为 $b_1 \times b_2 \times \cdots \times b_k$。

求每次询问可能的最大价值，对 $998244353$ 取模后输出。

### 输入格式

第一行两个整数 $n,q$；第二行 $n$ 个整数 $a_1,a_2,\dots,a_n$；接下来 $q$ 行，每行两个整数 $l,r$，表示一次询问。

### 输出格式

输出 $q$ 行，第 $i$ 行一个整数，表示第 $i$ 次询问的答案。

### 数据范围

- 对于所有数据：$1 \le n,q \le 5\times10^5$，$0 \le a_i < 2^{30}$，$1 \le l \le r \le n$。
- Subtask 1/2：$n,q\le5$；Subtask 3/4：$n,q\le2000$；Subtask 5/6：$n,q\le10^5$；Subtask 7/8：无特殊限制（部分 Subtask 另有 $a_i<2^{29}$ 的限制）。

## 样例

> 官方样例见洛谷 P17248（https://www.luogu.com.cn/problem/P17248）。样例 #1 解释（官方）：三次询问的区间分别为 $[1,2,3]$、$[2,3,0]$、$[3,0,4]$，最大价值分别为 $9$、$6$、$12$。

## 解题思路

看到「异或 + 分组 + 最大化乘积」，第一反应可能是按位贪心求异或最大值，但本题真正起决定作用的其实是两条关于异或与乘法的基本不等式，和「求异或最值」完全是两码事。

**关键性质：权值 $\ge 2$ 的组不应合并。** 对任意非负整数 $x,y$，异或不超过加法：

$$
x \oplus y \le x + y
$$

而当 $x,y \ge 2$ 时，加法不超过乘法：

$$
x + y \le x \times y
$$

把两条连起来：若两个组的权值分别为 $x,y$（都 $\ge 2$），把它们**合并**成一个组后权值变为 $x\oplus y$，乘积贡献从 $x\times y$ 变成 $x\oplus y$；由于 $x\oplus y \le x+y \le x\times y$，合并后的贡献不超过合并前。所以最优划分里，**任意两个权值 $\ge 2$ 的组都不应合并**——除了 $0$ 和 $1$ 之外，所有数独立成组（必要时把 $0$ 或 $1$ 并进这些组）。

于是问题只剩两件事：$0$ 和 $1$ 怎么处理。

**处理 $0$。** 任意数异或 $0$ 等于它本身，$0$ 可以随意并入任何组而不改变该组权值。因此：若区间内**全是 $0$**，答案就是 $0$；否则把所有 $0$ 忽略（并入其他组即可，对乘积无贡献）。

**处理 $1$。** 异或 $1$ 只翻转最低位：偶数 $c$ 变 $c+1$，奇数 $c$ 变 $c-1$。所以「把 $1$ 并进权值为偶数的组」能让该组贡献从 $c$ 变成 $c+1$，收益倍率为 $\frac{c+1}{c}=1+\frac1c$；并进权值为奇数的组只会让贡献变小。因此：

- 每个 $1$ 至多用来给一个偶数「加 $1$」；
- 每个偶数至多被加一次（加完变奇数，不能再加）；
- 收益倍率 $\frac{c+1}{c}$ 关于 $c$ 单调递减，$c$ 越小收益越大。

于是问题转化为：区间内有偶数 $c_1 \le c_2 \le \dots \le c_k$ 和 $h$ 个 $1$，选择 $\min(h,k)$ 个**最小**的偶数各加 $1$，使乘积最大。最优答案为：

$$
\prod_{i=1}^{\min(h,k)}(c_i+1)\times\prod_{i=\min(h,k)+1}^{k} c_i
$$

**为什么取最小偶数（贪心正确性）。** 每次操作的收益倍率 $\frac{c+1}{c}>1$，所以能用就尽量用；而倍率随 $c$ 增大递减，把有限的 $h$ 次操作全部分配给最小的 $h$ 个偶数，等价于「逐次选择当前收益最大者」。更严格地用交换论证：若某方案选了较大的 $c_j$ 却漏掉较小的 $c_i$（$c_i<c_j$），把这次操作从 $c_j$ 换到 $c_i$，总乘积会乘上 $\frac{c_i+1}{c_i}\cdot\frac{c_j}{c_j+1}>1$，严格更优，故最优解必取最小的一批偶数。

**区间查询怎么高效做。** 上面的每一步都要区间信息：$0$ 的个数（判全零）、$1$ 的个数（操作次数 $h$）、所有非零数的乘积（基础乘积）、以及「前 $h$ 小偶数加 $1$ 后的比值乘积」。前三个用前缀和 $O(1)$ 即可；最后一个需要「按值序取前 $h$ 小的偶数」，是典型的**主席树（可持久化权值线段树）**场景：以值域为下标，每个前缀版本维护子树内偶数的个数、偶数乘积与逆元、偶数「加 $1$ 与自身之比」的乘积与逆元，在版本差 $[l-1,r]$ 上做值域二分，$O(\log V)$ 取出前 $h$ 小偶数的比值乘积，再与基础乘积相乘即得答案。

### 关键洞察

把「异或分组最优化」用两条不等式降维成「只处理 $0$ 和 $1$」；再注意到「给偶数加 $1$」的收益随偶数变小而变大，把 $1$ 的分配变成「取最小的一批偶数」，最终落到主席树求「前 $h$ 小偶数的某类乘积」。

---

## 逐行代码解析

### 模块一：头文件、宏定义与全局数组

```cpp
#include<bits/stdc++.h>
using namespace std;
#define endl '\n'
#define fi first
#define se second
#define lowbit(x) (x&(-x))
#define mid ((l+r)>>1)
#define lc goc[u].ls
#define rc goc[u].rs

typedef pair<int,int> PII;
typedef long long ll;
const int N=5e5+10;
const int INF=1e9+10;
const int mod=998244353;

int a[N], h1[N], hf0[N], hmod[N];
```

- `N=5e5+10` 是数组上界；`INF=1e9+10` 作为值域上界（下界取 `-INF`），`mod=998244353` 是模数。
- `mid` 是线段树中点宏；`lc`/`rc` 是左右儿子宏（本代码直接用 `goc[u].ls/.rs`，这两个宏实际未使用，属模板残留）。
- 全局数组 `a` 存原序列；`h1`、`hf0`、`hmod` 分别是「$1$ 的个数」「非零元素个数」「等于模数的元素个数」的前缀和。

### 模块二：快速幂与逆元缓存表

```cpp
inline int ksm(int x,int y,int p){
    int res=1%p;
    x%=p;
    while(y){
        if(y&1) res=(ll)res*x%p;
        x=(ll)x*x%p;
        y>>=1;
    }
    return res;
}

unordered_map<int,int> ic, fc, ifc;
```

- `ksm(x,y,p)` 是快速幂，算 $x^y \bmod p$，用于通过费马小定理求逆元（$x^{mod-2} \bmod mod$）。
- `ic` 缓存逆元 $x^{-1}$，`fc` 缓存 $\frac{x+1}{x}$，`ifc` 缓存 $\frac{x}{x+1}$；把「每次查询都快速幂」降为均摊 $O(1)$。

### 模块三：主席树节点与带缓存的逆元/比值查询

```cpp
struct siryessir{
    struct note{
        int ls=0, rs=0;
        int wh=1, iwh=1;
        int cnt=0;
        int osp=1, iosp=1;
    };
    vector<note> goc;
    int tot=0;
    int rt[N];

    inline int getinv(int x){
        x %= mod;
        if(x==0) return 0;
        auto it = ic.find(x);
        if(it != ic.end()){
            int res = it->second;
            return res;
        }
        int res = ksm(x, mod-2, mod);
        ic[x] = res;
        return res;
    }

    inline int getf(int x){
        x %= mod;
        auto it = fc.find(x);
        if(it != fc.end()){
            int res = it->second;
            return res;
        }
        int invx = getinv(x);
        int res = (ll)(x+1) * invx % mod;
        fc[x] = res;
        return res;
    }

    inline int getif(int x){
        x %= mod;
        auto it = ifc.find(x);
        if(it != ifc.end()){
            int res = it->second;
            return res;
        }
        int invx1 = getinv((x+1) % mod);
        int res = (ll)x * invx1 % mod;
        ifc[x] = res;
        return res;
    }

    siryessir(){
        goc.clear();
        tot=0;
        goc.push_back({0,0,1,1,0,1,1});
    }
```

- `note` 是主席树节点：`ls/rs` 左右儿子；`wh/iwh` 子树内「乘积」及其逆元；`cnt` 偶数个数；`osp/iosp` 子树内「$\frac{c+1}{c}$」连乘及其逆元。
- `goc` 是动态节点池（`vector`），`rt[i]` 是前缀 $[1,i]$ 对应版本的根节点编号。
- `getinv/getf/getif` 分别是带缓存查 $x^{-1}$、$\frac{x+1}{x}$、$\frac{x}{x+1}$；$x=0$ 时逆元按 $0$ 返回。
- 构造函数压入一个**全空节点**（编号 0）作空树：`{0,0,1,1,0,1,1}` 即 `ls=rs=0`、`wh=iwh=1`、`cnt=0`、`osp=iosp=1`。

### 模块四：主席树单点插入 `zxpro`

```cpp
    int zxpro(int u,int l,int r,int k,int v){
        if(k==0) return u;
        goc.push_back(goc[u]);
        u=++tot;

        int kmod = k % mod;
        goc[u].wh = (ll)goc[u].wh * kmod % mod;
        goc[u].iwh = (ll)goc[u].iwh * getinv(kmod) % mod;

        if((k & 1) == 0){
            goc[u].cnt += v;
            int inv_k = getinv(kmod);
            int inv_k1 = getinv((kmod + 1) % mod);
            int f = (ll)(kmod + 1) * inv_k % mod;
            int vf = (ll)kmod * inv_k1 % mod;
            goc[u].osp = (ll)goc[u].osp * f % mod;
            goc[u].iosp = (ll)goc[u].iosp * vf % mod;
        }

        if(l==r) return u;
        if(k <= mid) goc[u].ls = zxpro(goc[u].ls, l, mid, k, v);
        else goc[u].rs = zxpro(goc[u].rs, mid+1, r, k, v);

        int lu = goc[u].ls, ru = goc[u].rs;
        goc[u].wh = (ll)goc[lu].wh * goc[ru].wh % mod;
        goc[u].iwh = (ll)goc[lu].iwh * goc[ru].iwh % mod;
        goc[u].cnt = goc[lu].cnt + goc[ru].cnt;
        goc[u].osp = (ll)goc[lu].osp * goc[ru].osp % mod;
        goc[u].iosp = (ll)goc[lu].iosp * goc[ru].iosp % mod;
        return u;
    }
```

- `zxpro` 是主席树的「单点插入」：先复制节点 `u` 新建节点，再沿值域 $[l,r]$ 走到叶子。`k==0` 直接返回旧节点，即 $0$ **不插入**（$0$ 对乘积无贡献，也避免对 $0$ 算 $\frac{0+1}{0}$）。
- 路径上每个节点 `wh` 乘 $k$（累积所有非零数的乘积）、`iwh` 乘其逆元；仅当 $k$ 为偶数时 `cnt` 加 $v$，并让 `osp` 乘 $\frac{k+1}{k}$、`iosp` 乘 $\frac{k}{k+1}$。
- 回溯时用左右儿子合并 `wh/iwh/cnt/osp/iosp`，实现可持久化：每次插入只新建 $O(\log V)$ 个节点。

### 模块五：主席树查询前 $k$ 小偶数的比值乘积 `aliez`

```cpp
    int aliez(int p1,int p2,int l,int r,int k){
        if(k <= 0) return 1;
        int cntz = goc[p2].cnt - goc[p1].cnt;
        if(k > cntz) k = cntz;
        if(k == 0) return 1;
        if(l == r){
            return ksm(getf(l), k, mod);
        }
        int lu1 = goc[p1].ls, ru1 = goc[p1].rs;
        int lu2 = goc[p2].ls, ru2 = goc[p2].rs;
        int now = goc[lu2].cnt - goc[lu1].cnt;
        if(k <= now){
            return aliez(lu1, lu2, l, mid, k);
        } else {
            int cntl = (ll)goc[lu2].osp * goc[lu1].iosp % mod;
            int cntr = aliez(ru1, ru2, mid+1, r, k - now);
            return (ll)cntl * cntr % mod;
        }
    }
} hjt;
```

- `aliez(p1,p2,l,r,k)` 在版本差 $[p1+1,p2]$ 上求「前 $k$ 小偶数的 $\frac{c+1}{c}$ 连乘」：`p1` 是 $l-1$ 版本、`p2` 是 $r$ 版本。
- 先用 `cnt` 把 $k$ 压到不超过区间内偶数总数；到叶子（值为 $l$）时该位置有 $k$ 个相同偶数，返回 $(\frac{l+1}{l})^k$。
- 值域二分：左子树偶数个数 `now` 够 $k$ 个就只进左子树；否则左子树**全部**加 $1$，取左子树整体比值 `goc[lu2].osp * goc[lu1].iosp`，再进右子树找剩余 $k-now$ 个。
- 结尾 `} hjt;` 声明全局主席树对象 `hjt`。

### 模块六：快读快写与主流程

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

int main(){
    int n, m;
    n = read(); m = read();
    for(int i=1;i<=n;i++){
        a[i] = read();
        h1[i] = h1[i-1] + (a[i] == 1);
        hf0[i] = hf0[i-1] + (a[i] != 0);
        hmod[i] = hmod[i-1] + (a[i] == mod);
        if(a[i] == mod){
            hjt.rt[i] = hjt.rt[i-1];
        } else {
            hjt.rt[i] = hjt.zxpro(hjt.rt[i-1], -INF, INF, a[i], 1);
        }
    }
    while(m--){
        int l, r;
        l = read(); r = read();
        if(hf0[r] - hf0[l-1] == 0){
            write(0); putchar('\n');
            continue;
        }
        if(hmod[r] - hmod[l-1] > 0){
            write(0); putchar('\n');
            continue;
        }
        int res = (ll)hjt.goc[hjt.rt[r]].wh * hjt.goc[hjt.rt[l-1]].iwh % mod;
        int cnt1 = h1[r] - h1[l-1];
        int cntj = hjt.goc[hjt.rt[r]].cnt - hjt.goc[hjt.rt[l-1]].cnt;
        int k = min(cnt1, cntj);
        if(k == 0){
            write(res); putchar('\n');
            continue;
        }
        int ans = hjt.aliez(hjt.rt[l-1], hjt.rt[r], -INF, INF, k);
        write((ll)res * ans % mod); putchar('\n');
    }
    return 0;
}
```

- `read/write` 是手写快读快写（`getchar/putchar`），应对较大的输入输出量。
- 建版本：顺次读入，同步维护 `h1/hf0/hmod` 前缀；若 `a[i]==mod`，本版本直接复用上一版本（该值模意义为 $0$，需特判），否则在上一版本上插入 `a[i]`。
- 查询四个分支：区间非零数为 $0$（全零）→ 输出 $0$；区间含 `mod` → 输出 $0$；`k=min(1的个数,偶数个数)` 为 $0$ → 输出基础乘积 `res`。
- `res` 是区间非零数乘积（`wh[r] * iwh[l-1]`），再乘 `aliez` 求出的「前 $k$ 小偶数加 $1$ 比值」即得最终答案。

---

## 复杂度分析

- **时间复杂度**：建树时每个元素插入一次，每次 $O(\log V)$，共 $O(n\log V)$；单次询问在主席树上值域二分，$O(\log V)$；总计 $O((n+q)\log V)$，其中 $V$ 为值域大小（代码中值域约为 $2\times 10^9$）。
- **空间复杂度**：主席树每插入一个值新建 $O(\log V)$ 个节点，共 $O(n\log V)$ 个节点；再加 $O(n)$ 的前缀数组与版本根数组，总 $O(n\log V)$。代码用 `vector<note> goc` 动态分配，避免按固定大小开满导致 MLE。

---

## 常见陷阱与注意事项

1. **$0$ 不能被当偶数「加 $1$」**：若把 $0$ 当作偶数套用 $\frac{c+1}{c}$ 会除零。代码在 `zxpro` 里对 `k==0` 直接返回，$0$ 不进树；且 $0$ 本就可免费并入任意组，无需单独成组。
2. **含 $998244353$ 必须特判为 $0$**：该值在模意义下为 $0$，且它是 $\ge 2$ 的奇数，最优划分下独立成组，乘积含因子 $998244353 \equiv 0$，无法被「加 $1$」抵消（奇数异或 $1$ 只会变小）。代码用 `hmod` 前缀和检测并直接输出 $0$。
3. **逆元要缓存**：每次查询都快速幂求逆元会超时；`ic/fc/ifc` 三个哈希表把重复的逆元与比值查询降为均摊 $O(1)$。
4. **`wh` 与 `osp` 别混淆**：`wh` 是「所有非零数」的乘积（含奇数），`osp` 是「偶数 $\frac{c+1}{c}$」的连乘（只含偶数）；答案 = 基础乘积 × 前 $k$ 小偶数的比值乘积，二者相乘才正确。
5. **偶数加 $1$ 的约束**：奇数异或 $1$ 会减 $1$，所以只有偶数能被 $1$「提升」；每个偶数被提升一次后变奇数，不能再提升，故最多操作 $\min(h,k)$ 次。
6. **分支顺序**：先判全零、再判含模数、再判 `k==0`，否则在「全零但无模数」等边界上会错误地进入 `aliez` 或算出非零结果。

---

## 对比与扩展

- **异或的不等式刻画**：$x\oplus y \le x+y \le x\times y$（$x,y\ge 2$）是本题的核心，它把「如何分组」直接化约为「只需处理 $0$ 和 $1$」；这与「异或最大值/最小异或对」等按位 Trie 贪心题是两条不同的路子。
- **主席树求「前 $k$ 小的聚合量」**：经典主席树是求第 $k$ 小/区间 $k$ 小个数，本题把聚合量从「个数」扩展到「乘积、比值乘积」，靠每个节点维护 `cnt/wh/osp` 三组信息、查询时值域二分，是同类的可持久化权值线段树模板变形。
- **交换论证贪心**：「收益 $\frac{c+1}{c}$ 单调递减 → 取最小的一批」是收益独立、可交换的典型贪心，证明手法与「排序后按收益降序取」的贪心题相通。
- **诈骗题性质**：题面用「异或」作包装，真正考的是两条不等式与一个数据结构；对「异或」这类运算的性质不熟时容易误入按位贪心的歧途。
