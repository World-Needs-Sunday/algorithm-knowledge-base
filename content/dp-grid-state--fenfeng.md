---
id: dp-grid-state
title: '棋盘状压 DP'
category: 动态规划
subcategory: 状态压缩DP
subSubcategory: ''
tags: [状态压缩, 状压DP, 炮兵阵地, 位运算, 滚动数组]
timeComplexity: 'O(N·S³)'
spaceComplexity: 'O(S²)'
level: A
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
codePath: '动态规划\棋盘状压DP\Untitled1.cpp'
---

## 算法原理

棋盘状压 DP 是「行状压 DP」的一种：把棋盘每一行的摆放情况压成一个二进制数，一位代表一个格子（1 表示放、0 表示不放），再沿行做动态规划。它适用于「行内状态可以用二进制表示、行与行之间只通过相邻的若干行相互约束」的棋盘类问题。

以炮兵阵地（洛谷 P2704）为例：在 $N \times M$ 的网格上布置炮兵，每门炮兵的攻击范围是上下左右各两格，任意两门炮兵不能互相攻击，且只能放在平原（P）上，求最多能放多少门。

这个问题的关键在于「攻击距离更远」：像「互不侵犯」这类题只需相邻两行不冲突，而炮兵的射程跨两格，因此第 $i$ 行是否合法不仅取决于第 $i-1$ 行，还取决于第 $i-2$ 行。于是状态必须同时记录「当前行」与「上一行」两行，形成三维状态 $dp[i][cur][pre]$。

由于 $M \le 10$，每行至多用一个 10 位二进制数表示；再预处理出每行「内部合法（不存在相距小于 3 的两门炮）」且「不与山地冲突」的状态集合，转移时只枚举这些合法状态，就能把指数级的状态枚举压缩到很小的范围。

## 核心公式 / 状态定义与转移方程

记 $V(i)$ 为第 $i$ 行的合法状态集合：行内不存在相距小于 3 的两个 1，且不与山地冲突。

### 状态定义

$$dp[i][cur][pre]$$

表示前 $i$ 行中、第 $i$ 行状态为 $cur$、第 $i-1$ 行状态为 $pre$ 时能放的最多炮兵数。

### 行内合法状态

每一行内部不能存在相距小于 3 的两门炮（任意两个 1 之间至少隔两个 0），等价于：

```text
(state & (state << 1)) == 0   且   (state & (state << 2)) == 0
```

### 山地约束

状态必须落在平原上，不能压到山地（H 格）：

```text
state & mountain[i] == 0
```

### 转移方程

枚举第 $i-2$ 行的合法状态 $prepre$，若三行两两互不冲突，则用「上一行的值 + 当前行新增炮兵数」更新：

$$dp[i][cur][pre] = \max_{prepre \in V(i-2)} \Big( dp[i-1][pre][prepre] + \mathrm{popcount}(cur) \Big)$$

其中枚举的 $prepre$ 还需满足 `cur & pre == 0`、`cur & prepre == 0`、`pre & prepre == 0`，$\mathrm{popcount}(cur)$ 是状态 $cur$ 中 1 的个数，即本行新增的炮兵数。

### 最终答案

$$\mathrm{answer} = \max_{cur \in V(n),\, pre \in V(n-1)} dp[n][cur][pre]$$

## 逐行代码解析

### 头文件与全局数组

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
//const int N=1e5+10;
//int cnt=0;
//int a[N];
int mp[110];
int dp[3][1<<11][1<<11];
int ndp[110][1<<11];
vector<int> pas[110];
inline int read();
inline void write(int x);
int popcount(int x){
    return __builtin_popcount(x);
}
```

- 万能头加快读快写模板的宏；`#define int long long` 把整型统一成 64 位，配合 `signed main()` 使用，避免答案溢出。
- `mp[i]` 记录第 $i$ 行的山地掩码（H 格对应位为 1）；`pas[i]` 存第 $i$ 行的全部合法状态。
- `dp[3][1<<11][1<<11]` 是滚动数组，3 层轮流表示第 $i$、$i-1$、$i-2$ 三行；`popcount` 用 GCC 内置的 `__builtin_popcount` 统计 1 的个数。
- `ndp`、`read`、`write` 是模板残留，本算法并未使用它们。

### 读入地图并预处理合法状态

```cpp
signed main(){
//    freopen(".in","r",stdin);
//    freopen(".out","w",stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);cout.tie(nullptr);
    
    int n,m;cin>>n>>m;
    for(int i=1;i<=n;i++){
        for(int j=1;j<=m;j++){
            char c;cin>>c;
            if(c=='H') mp[i]|=(1<<(m-j));
        }
    }
    for(int i=1;i<=n;i++){
        for(int j=0;j<(1<<m);j++){
                //cout<<dp[i][j]<<' ';
                //if(i!=1) dp[i][j]=LLONG_MIN;
            if((((j<<2)&j)>0)||(((j<<1)&j)>0)||(((j>>2)&j)>0)||(((j>>1)&j)>0)) continue;
            if(j&mp[i]) continue;
            pas[i].push_back(j);
        }
           //cout<<endl;
    }
    pas[0].push_back(0);
```

- 读入 $n$、$m$ 后，把每行的 H 格压进 `mp[i]`：第 $j$ 列对应第 `m-j` 位，位序与后续状态定义保持一致。
- 枚举 $0 \sim 2^m-1$ 的所有状态，`(j<<1)&j`、`(j<<2)&j` 非 0 说明存在相距小于 3 的两个 1，直接跳过（`>>` 方向是等价的冗余判断）。
- 再用 `j & mp[i]` 排除压到山地的状态，剩下的就是本行的合法状态，存入 `pas[i]`。
- `pas[0].push_back(0)` 让「第 0 行为空」成为合法状态，作为第 1 行的上一行、以及 `n=1` 时枚举 `pas[n-1]` 的兜底。

### 初始化 dp 数组与第一行

```cpp
    for(int i=0;i<=2;i++){
        for(int j=0;j<(1<<m);j++){
            for(int u=0;u<(1<<m);u++){
                dp[i][j][u]=LLONG_MIN;
            }
        }
    }
    for(int j:pas[1]){
        dp[1][j][0]=popcount(j);
    }
```

- 先把 3 层滚动数组整体置为 `LLONG_MIN`，表示「不可达」。
- 第 1 行没有上一行，只需与「空上一行」（状态 0）相容，因此 `dp[1][j][0] = popcount(j)`。

### 三行约束的状态转移

```cpp
    for(int i=2;i<=n;i++){
        for(int j:pas[i]){
            for(int u:pas[i-1]){
                if(u&j) continue;
                for(int o:pas[max(i-2,0ll)]){
                    if(o&u) continue;
                    if(o&j) continue;
                    
                    dp[i%3][j][u]=max(dp[i%3][j][u],dp[(i-1)%3][u][o]+popcount(j));
                }
                
            }
        }
    }
```

- 第 $i$ 行从第 $i-1$、$i-2$ 行转移：`j` 是当前行、`u` 是上一行、`o` 是上上行，三层都从各自行的合法状态集里取。
- `u&j`、`o&u`、`o&j` 三处 `continue` 保证三行两两不冲突。
- `max(i-2,0ll)` 处理 $i=2$ 时上上行为第 0 行（只有空状态 0）。
- 用 `dp[i%3][j][u] = max(..., dp[(i-1)%3][u][o] + popcount(j))` 完成滚动更新，把上一行的最优值加上当前行的炮兵数。

### 统计答案并输出

```cpp
    
    int cnt=0;
    for(int j:pas[n]){
        for(int k:pas[n-1]){
            cnt=max(cnt,dp[n%3][j][k]);
        }
    }
       cout<<cnt<<endl;
    
    
    return 0;
}
```

- 最后枚举末两行的全部组合（`pas[n]` 与 `pas[n-1]`），取最大值。
- `dp[n%3][j][k]` 就是前 $n$ 行、末两行为 $(j,k)$ 时的最优值。
- 输出 `cnt` 即为最多能放的炮兵数。

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

- `read` 用 `getchar` 逐字符读入并累加，`write` 递归输出，是竞赛常用的快读快写模板。
- 本程序通过 `cin/cout`（已关闭同步）完成输入输出，这两个函数未参与本次算法流程。

## 复杂度分析

- **时间复杂度**：$O(N \cdot S^3)$
  - 预处理每行枚举 $2^M$ 个状态：$O(N \cdot 2^M)$
  - 转移共 $N$ 行，每行三重循环分别枚举 `pas[i]`、`pas[i-1]`、`pas[i-2]`，每层至多 $S$ 个合法状态：$O(N \cdot S^3)$
  - $S$ 是「行内不存在相距小于 3 的两个 1」的状态数，$M=10$ 时 $S=60$；$N \le 100$ 时约为 $100 \times 60^3 \approx 2.2 \times 10^7$，非常快

- **空间复杂度**：$O(S^2)$
  - 滚动数组只保留 3 层，每层存 `(cur, pre)` 两维：$O(3 \cdot S^2)=O(S^2)$
  - 实现里 `dp` 直接按 $2^M \times 2^M$ 开满、元素是 `long long`，$M=10$ 时约 $96$ MB；若把合法状态编号压缩，可降到真正的 $O(S^2)$ 内存

## 适用场景

1. **棋盘/网格上的「放置」类问题**：每格只有放/不放两种状态，单行可以用一个二进制数表示。
2. **行与行之间只依赖相邻的常数行**：炮兵的攻击跨两格，故状态要记当前行与上一行两维；依赖越多，状态维数越高。
3. **列数很小、行数可以很大**：$M \le 10$ 使 $2^M$ 可枚举，而 $N$ 只出现在外层循环里，不影响状态维度。
4. **典型题目**：炮兵阵地（P2704）、互不侵犯（P1896）、玉米田（Corn Fields）、骑士等。

## 常见陷阱与注意事项

1. **行内合法性要同时检查距离 1 和距离 2**：必须同时判 `(state & (state<<1))` 与 `(state & (state<<2))`，漏掉 `<<2` 会多算相邻隔一格的错误状态。
2. **位序全程保持一致**：读图用 `1 << (m-j)` 表示第 $j$ 列，则状态里「第几位对应第几列」必须沿用同一方向，否则山地约束会错位。
3. **山地过滤不能省**：`j & mp[i]` 这层判断少了会允许炮兵放到 H 格上。
4. **边界行**：$n=1$ 或 $n=2$ 时上一行/上上行可能越界，代码里 `pas[0].push_back(0)` 与 `max(i-2,0ll)` 正是为兜底准备的。
5. **滚动数组只开 3 层**：只需相邻三行，读 `(i-1)%3`、写 `i%3`，三层的读写方向不要搞反。
6. **内存**：`1<<11` 的两维配合 `long long` 约 $96$ MB，注意题目内存限制；空间紧张时可用「合法状态编号压缩」把两维从 $2^M \times 2^M$ 降到 $S \times S$。

## 对比与扩展

- **与「互不侵犯」对比**：互不侵犯（P1896）只约束相邻两行，状态是二维的 $dp[i][state][k]$；炮兵阵地攻击距离两格，需同时看两行，状态升到三维。
- **与「集合状压 DP」对比**：TSP / Hamilton 路径压缩的是「已访问的点集」，这里压缩的是「单行棋盘状态」，属于「行状压」这一支。
- **扩展**：可用滚动数组降空间；把合法状态编号压缩到 $S$ 维，减少无效枚举；更复杂的棋盘覆盖（连通性约束）可引入轮廓线 DP（插头 DP）逐格推进。
