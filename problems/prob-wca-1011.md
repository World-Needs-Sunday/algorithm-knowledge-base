---
id: prob-wca-1011
title: '2026WCA1011 完美旅行'
oj: 'WCA'
problemId: '2026WCA1011'
difficulty: '困难'
category: '动态规划'
tags: ["状态压缩DP", "树上差分", "容斥原理", "高维前缀和", "组合数学"]
timeComplexity: 'O(n·m + m·2^m)'
spaceComplexity: 'O(n + 2^m)'
codePath: '题目专辑\动态规划\完美旅行\代码.cpp'
level: A
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 题目描述

有一棵 $n$ 个节点的树，树上有 $m$ 条“指定道路”（编号 $1 \sim m$），以及 $k$ 条“路线”。每条路线是树上的一条简单路径，用它的两个端点表示。

一条路线会“覆盖”它经过的所有指定道路。现在要找最小的正整数 $i$（$1 \le i \le m$），使得能从所有路线中选出 $i$ 条，使它们覆盖的指定道路集合大小恰好为 $m$——也就是覆盖了全部指定道路。

输出最小的 $i$ 以及对应的方案数（对 $998244353$ 取模）。若不存在这样的 $i$，输出 `-1`。

### 输入格式

题面未提供完整输入格式。依据所附代码，实际读入顺序为：

- 第一行三个整数 $n, m, k$；
- 随后 $n-1$ 行，每行两个整数 $u, v$，表示一条树边（边按输入顺序编号为 $1 \sim n-1$）；
- 随后 $m$ 行，每行一个整数 $x$，表示第 $x$ 条输入的树边是一条指定道路；
- 最后 $k$ 行，每行两个整数 $u, v$，表示一条路线的两个端点。

### 输出格式

若存在解，输出一行两个整数：最小的 $i$ 和对应方案数（方案数对 $998244353$ 取模），中间用空格分隔；否则输出 `-1`。

### 数据范围

- $n \le 3 \times 10^5$
- $m \le 22$
- $k \le 3 \times 10^5$

## 样例

官方样例未取得。

## 解题思路

### 一、从数据范围读出“状压”

$m \le 22$ 而 $n, k$ 都高达 $3 \times 10^5$。$2^{22} \approx 4.2 \times 10^6$，是既能开数组、又能逐位枚举的大小，这几乎是在明示：对“指定道路”做**状态压缩**。

每条路线 $u \to v$ 覆盖它经过的所有指定道路，于是可以用一个 $m$ 位二进制掩码表示它：第 $i$ 位为 $1$ 当且仅当这条路线经过了第 $i$ 条指定道路。问题由此转化为一个**组合覆盖**问题：

> 给定 $k$ 个 $m$ 位掩码，求最小的 $i$，使得存在 $i$ 个掩码的按位或等于全集 $U = 2^m - 1$，并输出方案数。

整个问题随之拆成三个独立步骤：路径转掩码 → 子集和 → 容斥计数。

### 二、树上前缀异或：$O(1)$ 求路径掩码

朴素做法是对每条路线在树上走一遍路径、逐条检查经过哪些指定道路，单次 $O(\text{路径长度} \times m)$，$k$ 条路线下来不可接受。需要用树上前缀异或把单次降到 $O(1)$。

任取根 $1$，定义 $fh[x]$ 为“根到 $x$ 的路径上经过的指定道路集合”（$m$ 位掩码）。那么：

$$
\text{path\_mask}(u, v) = fh[u] \oplus fh[v]
$$

**为什么是异或？** 设 $l = \operatorname{LCA}(u, v)$。根到 $u$ 的路径 = 根到 $l$ 的路径 ∪ $l$ 到 $u$ 的路径，根到 $v$ 同理。两段“根到 $l$”完全重合，异或后互相抵消；剩下的 $l \to u$ 与 $l \to v$ 两段没有公共边，恰好拼成 $u \to v$ 的完整路径。因此异或正好“抵消掉公共前缀、留下真正路径”。

预处理 $fh[x]$ 只需一次 DFS：$fh[x] = fh[fa]$；若边 $(x, fa)$ 恰好是第 $i$ 条指定道路，就把第 $i$ 位置 $1$。

### 三、子集和：SOS DP（高维前缀和）

设 `cnt[mask]` 为“掩码恰好等于 `mask`”的路线数（初始时 `dp[mask]++`）。现在要回答：对每个掩码 $S$，有多少条路线的掩码是 $S$ 的**子集**。这正是**子集和**，用 SOS DP（Sum over Subsets，即高维前缀和）解决：

$$
\text{for } i \in [0, m):\quad \text{for } sta:\quad \text{若 } sta \text{ 含第 } i \text{ 位，则 } dp[sta] \mathrel{+}= dp[sta \oplus 2^i]
$$

对每一位 $i$，把所有“含第 $i$ 位”的状态加上“去掉第 $i$ 位”的状态。处理完 $m$ 位后，`dp[S]` 就表示“掩码是 $S$ 的子集的路线总数”。注意到更新时读的 `dp[sta ^ (1<<i)]` 在第 $i$ 位为 $0$、本轮不会被写，因此无论内层循环升序还是降序结果都正确。

### 四、容斥：恰好覆盖全集的方案数

固定 $i$，要求“选 $i$ 条路线、掩码按位或恰好等于全集 $U$”的方案数。直接枚举组合是 $\binom{k}{i}$ 级，不可行，需要反着来。

设 $f(S) = \binom{dp[S]}{i}$，表示“从掩码 $\subseteq S$ 的路线池里选 $i$ 条”的方案数——注意这些路线覆盖的并集一定 $\subseteq S$，但不一定等于 $S$。由布尔格上的 Möbius 反演：

$$
\text{恰好覆盖 } U \text{ 的方案数} = \sum_{S \subseteq U} (-1)^{m - \lvert S \rvert} \binom{dp[S]}{i}
$$

**正确性证明**：固定某一种“实际覆盖集合为 $T$”的选择，它在原式中被计入的次数为 $\sum_{S \supseteq T} (-1)^{m - \lvert S \rvert}$。令 $t = \lvert T \rvert$：

$$
\sum_{S \supseteq T} (-1)^{m - \lvert S \rvert}
= \sum_{j=0}^{m-t} \binom{m-t}{j} (-1)^{m-(t+j)}
= (-1)^{m-t}(1-1)^{m-t}
$$

当 $t < m$ 时该和为 $0$，当 $t = m$（即 $T = U$）时为 $1$。所以该式恰好只留下“覆盖全集”的方案，其余全部抵消。

代码用奇偶性判断符号：`(ct & 1) == (m & 1)` 表示 $m - ct$ 为偶数，对应系数 $(-1)^{m-ct} = +1$；否则为 $-1$。

### 五、从小到大枚举 $i$

外层 $i$ 从 $1$ 到 $m$，每次容斥算出“恰好覆盖全集”的方案数 $ans$。第一个 $ans \neq 0$ 的 $i$ 就是答案，输出 $i$ 与 $ans$；若 $i$ 从 $1$ 到 $m$ 全部为 $0$，输出 `-1`。

为什么只需枚举到 $m$：要覆盖 $m$ 条不同的指定道路，最优选择中每条路线至少覆盖一条“此前未覆盖”的新道路，否则它可被删去，所以最小覆盖的路线数不超过 $m$。答案若存在必满足 $i \le m$。

## 逐行代码解析

### 头文件、宏定义与全局数组

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
const int N=3e5+10,M=23,mod=998244353;
int n,m,k;
vector<int> G8[N];
PII a[N];
int fh[N],mac[N];
int dp[(1<<M)];
int jcr[4*N],nyj[4*N];
```

- `#define int long long` 把所有 `int` 替换为 `long long`，因此主函数必须写成 `signed main()`（`signed` 等价于 `int`，不会被宏再次展开）。
- `M=23` 给 $m \le 22$ 留了余量，`dp[1<<M]` 是子集和用的数组，大小 $2^{23}$。
- `fh[x]` 存根到 $x$ 的前缀掩码，`mac[i]` 存第 $i$ 条路线的掩码，`a[]` 前半段存路线、`a[k+i]` 存第 $i$ 条指定道路的两个端点。

### 组合数：快速幂、阶乘与逆元

```cpp
int ksm(int x,int y,int p){//快速幂
    int res=1;
    if(y==0) return 1;
    if(y&1) res=x*ksm(x,y-1,p)%p;
    else res=ksm(x,y/2,p),res=res*res%p;
    return res;
}
void jiecycl(int n){
    jcr[0]=1;
    for(int i=1;i<=n;i++){
        jcr[i]=jcr[i-1]*i%mod;
    }
    nyj[n]=ksm(jcr[n],mod-2,mod);
    for(int i=n-1;i>=0;i--){
        nyj[i]=nyj[i+1]*(i+1)%mod;
    }
}
int cabz(int n,int k){//组合数
    if(k<0||k>n) return 0;
    return jcr[n]*nyj[k]%mod*nyj[n-k]%mod;
}
```

- `ksm` 是快速幂，`jiecycl` 预处理阶乘 `jcr` 与阶乘逆元 `nyj`（用费马小定理求 $jcr[n]^{mod-2}$）。
- `cabz(n,k)` 用阶乘公式算 $\binom{n}{k}$，并对 $k<0$ 或 $k>n$ 的非法情况返回 $0$。
- 容斥里要算 $\binom{dp[sta]}{i}$，其中 $dp[sta] \le k$，因此阶乘只需预处理到不小于 $k$ 的上界即可（代码用 `2*(n+k)` 保证够用）。

### DFS 计算树上前缀异或掩码

```cpp
void dfs(int x,int fa){
    fh[x]=fh[fa];
    for(int i=1;i<=m;i++){
        if(a[k+i]==(PII){x,fa}||a[k+i]==(PII){fa,x}) fh[x]|=1<<(i-1);
    }
    for(auto i:G8[x]){
        if(i==fa) continue;
        dfs(i,x);
    }
}
```

- `fh[x] = fh[fa]` 继承父节点的前缀掩码；再枚举 $m$ 条指定道路，若边 $(x, fa)$（无向，两个方向都判）是第 $i$ 条指定道路，就把第 $i$ 位置 $1$。
- `a[k+i] == (PII){x, fa}` 会构造临时 `pair` 做比较，两边都要判断以处理无向边。
- 每个节点都完整枚举一遍 $m$，故本段复杂度是 $O(n \cdot m)$。

### 主函数（一）：读入、建图与路线存储

```cpp
inline int read();
inline void write(int x);
PII sb010[N];
signed main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);cout.tie(nullptr);
    cin>>n>>m>>k;
    jiecycl(2*(n+k));
    for(int i=1;i<n;i++){
        int u,v;cin>>u>>v;
        G8[u].push_back(v);
        G8[v].push_back(u);
        sb010[i]={u,v};
    }
    for(int i=1;i<=m;i++){
        int x;cin>>x;
        a[k+i]=sb010[x];
    }
    for(int i=1;i<=k;i++){
        int u,v;cin>>u>>v;
        a[i]={u,v};
    }
```

- 树边按输入顺序存入 `sb010[i]`；指定道路以“树边编号 $x$”给出，通过 `a[k+i] = sb010[x]` 还原成端点对。
- `a[1..k]` 存路线，`a[k+1..k+m]` 存指定道路，与 DFS 中的下标约定一致。
- 开 `ios::sync_with_stdio(false)` 加速，因此后面 `read()/write()` 实际并未使用（冗余但无害）。

### 主函数（二）：路线转掩码与 SOS 子集和

```cpp
    dfs(1,0);
    for(int i=1;i<=k;i++){
        mac[i]=fh[a[i].fi]^fh[a[i].se];
        dp[mac[i]]++;
    }
    for(int i=0;i<m;i++){
        for(int sta=(1<<m)-1;sta>=0;sta--){
            if(sta&(1<<i)) dp[sta]+=dp[sta^(1<<i)];
        }
    }
```

- `mac[i] = fh[u] ^ fh[v]`：用树上前缀异或一次求出第 $i$ 条路线的掩码，随后 `dp[mac[i]]++` 统计每种掩码的路线数。
- 两重循环做 SOS DP：对每一位 $i$，把含第 $i$ 位的状态的子集和累加进来，结束后 `dp[S]` 是“掩码为 $S$ 子集的路线数”。
- 内层 `sta` 从大到小或从小到大均可，关键是读源 `sta^(1<<i)` 的第 $i$ 位为 $0$，不会在本轮被改写。

### 主函数（三）：容斥统计答案与快读快写

```cpp
    for(int i=1;i<=m;i++){
        int ans=0;
        for(int sta=0;sta<=(1<<m)-1;sta++){
            int ct=__builtin_popcount(sta);
            if((ct&1)==(m&1)) ans=(ans%mod+cabz(dp[sta],i)%mod+mod)%mod;
            else ans=(ans%mod-cabz(dp[sta],i)%mod+mod)%mod;
        }
        ans%=mod;
        if(ans!=0){
            cout<<i<<" "<<ans;
            exit(0);
        }
    }
    cout<<-1;
    return 0;
}
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

- 外层枚举 $i$，内层枚举所有子集 `sta`，按容斥系数 $\pm \binom{dp[sta]}{i}$ 累加；奇偶判断 `(ct&1)==(m&1)` 决定加还是减。
- 减法统一写成 `(... - cabz(...) + mod) % mod` 避免出现负数取模。
- 第一个 `ans != 0` 的 $i$ 直接输出并 `exit(0)`；全为 $0$ 则输出 `-1`。
- 文件末尾的 `read()`/`write()` 是手写快读快写，但 `main` 已用 `cin/cout`，二者未被调用，属冗余代码。

## 复杂度分析

- **路径转掩码（DFS）**：每个节点都要枚举 $m$ 条指定道路，复杂度 $O(n \cdot m)$；另有 $k$ 条路线各做一次异或，$O(k)$。
- **SOS 子集和**：外层 $m$ 位、内层 $2^m$ 个状态，$O(m \cdot 2^m)$。
- **容斥枚举答案**：外层 $i$ 循环 $m$ 次、内层枚举 $2^m$ 个状态，$O(m \cdot 2^m)$。
- **总计**：$O(n \cdot m + m \cdot 2^m)$。代入上限，$n \cdot m \approx 6.6 \times 10^6$，$m \cdot 2^m \approx 22 \times 4.2 \times 10^6 \approx 9.2 \times 10^7$，总约 $10^8$ 量级，注意常数。

空间上，`dp[1<<M]`（$2^{23}$ 个元素）、`jcr/nyj`、邻接表 `G8` 及各辅助数组共 $O(n + 2^m)$。由于 `#define int long long` 会把 `dp` 也扩展成 `long long`，`dp` 实际约占 67 MB，叠加阶乘逆元数组后内存消耗较高，需留意题目内存限制。

## 常见陷阱与注意事项

1. **`#define int long long` 必须配 `signed main()`**：写 `int main()` 会被宏替换成 `long long main()`，是未定义行为。

2. **路径掩码用异或而非或**：`fh[u] ^ fh[v]` 正是要“抵消根到 LCA 的公共段”，换成按位或会重复统计公共段上的指定道路。

3. **容斥符号别写反**：`(ct & 1) == (m & 1)` 对应系数 $(-1)^{m-ct}=+1$；奇偶判断颠倒会使全部符号翻转、答案出错。

4. **SOS DP 的读源方向**：更新 `dp[sta] += dp[sta ^ (1<<i)]` 时，读源必须是不含第 $i$ 位的状态，才能保证它未被本轮修改；不要写成 `sta + (1<<i)` 之类把方向搞反。

5. **组合数取模减法的负数处理**：`ans` 累减时统一 `(... % mod + mod) % mod`，否则负数取模会产生错误结果。

6. **指定道路按“边编号”读入**：`a[k+i] = sb010[x]` 说明输入给的是树边编号 $x$ 而非端点；若题面给的是端点，需要先建立“端点对 → 边”的映射再转换。

7. **$i$ 只需枚举到 $m$**：最小覆盖至多需要 $m$ 条路线，若 $i=m$ 仍无解则不存在解，输出 `-1` 即可。

8. **内存常数较大**：`dp[1<<23]` 在 `long long` 下约 67 MB，加上阶乘逆元数组可能逼近常见内存上限，必要时可把 `dp` 单独声明为 `int`。

## 对比与扩展

- **对比朴素做法**：枚举“选 $i$ 条路线”是 $\binom{k}{i}$ 级，完全不可行；本题把覆盖关系压成 $m$ 位掩码，再用 SOS DP 与容斥把计数降到 $O(m \cdot 2^m)$。

- **SOS DP（高维前缀和）**：是求“子集和 / 超集和”的标准工具，复杂度 $O(m \cdot 2^m)$；把加法换成 $\max$ / $\min$ 即可扩展到“子集内最值”等变体。

- **布尔格上的容斥 / Möbius 反演**：$f(S) = \binom{dp[S]}{i}$ 是“覆盖 $\subseteq S$”的计数，反演后得到“恰好覆盖 $S$”，与“至少/恰好”类计数问题同源。

- **树上前缀异或**：不限于“指定道路”，任何可沿边累加/异或的边权（如边权异或和、边权积）都能用 `fh[u] ^ fh[v]` 在 $O(1)$ 内求路径值；结合 LCA 与离线查询可扩展到更一般的树路径统计。
