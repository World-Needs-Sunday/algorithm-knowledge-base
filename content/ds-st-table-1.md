---
id: ds-st-table-1
title: 'ST表（标准版1 - 手动预处理对数表）'
category: 数据结构
subcategory: ST表
tags: ["数据结构", "ST表", "RMQ", "倍增", "区间最值"]
timeComplexity: '预处理O(n log n)，查询O(1)'
spaceComplexity: 'O(n log n)'
codePath: '数据结构\ST表\标准\1\Untitled1.cpp'
---

## 算法原理

ST表（Sparse Table，稀疏表）是基于**倍增**思想（也叫 binary lifting，二进制 lifting）的静态区间查询数据结构，专门解决不可修改数组的区间最值问题（Range Minimum/Maximum Query，RMQ）。倍增的核心是：任意一个区间长度都能表示为若干个 2 的幂次之和（这其实就是二进制分解），因此只要预先算出所有"长度为 2 的幂"的区间的答案，就能在常数次拼合内回答任意长度的查询。具体地，设 $\text{st}[i][j]$ 表示从下标 `i` 开始、长度为 $2^j$ 的区间（即 $[i, i + 2^j - 1]$）的最值，则查询区间 `[l, r]`（长度 $\text{len} = r - l + 1$）时，令 $k = \lfloor \log_2 \text{len} \rfloor$，把区间拆成两段：从 `l` 起长度 $2^k$ 的左段 $[l, l + 2^k - 1]$，和到 `r` 结束长度 $2^k$ 的右段 $[r - 2^k + 1, r]$。这两段可能重叠，但重叠不影响结果——这正是 ST 表能在 $O(1)$ 完成查询的关键。

预处理用递推：$$\text{st}[i][j] = \max(\text{st}[i][j-1],\; \text{st}[i + 2^{j-1}][j-1])$$。直观理解：长度 $2^j$ 的区间可以拆成两个长度 $2^{j-1}$ 的子区间——前半 $[i, i + 2^{j-1} - 1]$ 与后半 $[i + 2^{j-1}, i + 2^j - 1]$，分别对应 $\text{st}[i][j-1]$ 和 $\text{st}[i + 2^{j-1}][j-1]$，取两者最值即得 $\text{st}[i][j]$。由于 `j` 从小到大递推，子区间的答案已经算好，整张表可在 $O(n \log n)$ 时间内建立。注意第二维 `j` 的上界是 $\lfloor \log_2 N \rfloor$，因为 $2^j$ 不能超过 N。

**为什么查询是 O(1)**：因为 max/min 是**幂等运算**（idempotent），即 $\max(x, x) = x$。查询时两段子区间可以重叠，重叠部分被算两次但取 max 后仍正确。这一点对和、积等非幂等运算不成立——若要求和，两段重叠区域的元素会被计算两次。所以 ST 表只适用于 max、min、gcd、按位或等幂等运算；对于求和则必须用前缀和或线段树。本版本的特别之处在于用**手动预处理的幂次表 T 配合 lower_bound 二分查找**来计算 $\lfloor \log_2 x \rfloor$，不依赖编译器内建函数，可移植性更好但代码稍长。

## 数据结构图示

设原数组 `a = [3, 1, 4, 1, 5, 9, 2, 6]`（下标 1~8），ST 表 $\text{st}[i][j]$ 存储从 `i` 起长度 $2^j$ 区间的最大值：

```
j=0 (长度1):  st[1][0]=3  st[2][0]=1  st[3][0]=4  st[4][0]=1
              st[5][0]=5  st[6][0]=9  st[7][0]=2  st[8][0]=6

j=1 (长度2):  st[1][1]=max(3,1)=4   st[2][1]=max(1,4)=4
              st[3][1]=max(4,1)=4   st[4][1]=max(1,5)=5
              st[5][1]=max(5,9)=9   st[6][1]=max(9,2)=9
              st[7][1]=max(2,6)=6

j=2 (长度4):  st[1][2]=max(st[1][1], st[3][1])=max(4,4)=4
              st[2][2]=max(st[2][1], st[4][1])=max(4,5)=5
              st[3][2]=max(st[3][1], st[5][1])=max(4,9)=9
              st[4][2]=max(st[4][1], st[6][1])=max(5,9)=9
              st[5][2]=max(st[5][1], st[7][1])=max(9,6)=9

j=3 (长度8):  st[1][3]=max(st[1][2], st[5][2])=max(4,9)=9

表的形式（行=起点i，列=长度2^j）：
       j=0  j=1  j=2  j=3
i=1 :   3    4    4    9
i=2 :   1    4    5    -
i=3 :   4    4    9    -
i=4 :   1    5    9    -
i=5 :   5    9    9    -
i=6 :   9    9    -    -
i=7 :   2    6    -    -
i=8 :   6    -    -    -

查询 [2,7] 的最大值 (len=6, 2^k<=6 最大 k=2, 2^2=4):
  左段: st[2][2] = max[a[2..5]] = 5
  右段: st[7-4+1][2] = st[4][2] = max[a[4..7]] = 9
  ans = max(5, 9) = 9
  (两段 [2,5] 与 [4,7] 重叠 [4,5]，但 max 幂等无影响)
```

辅助表 T 存 2 的幂，用于 get_h 求 log2：
```
T = [1, 2, 4, 8, 16, 32, 64, ...]  (直到 10^9)
索引: 0   1  2  3   4   5   6  ...
get_h(6): lower_bound(T, 6) 找到第一个 >=6 的元素 T[3]=8
          ans=3, 但 1<<3 = 8 != 6，所以 ans -= 1 => 2
          即 floor(log2(6)) = 2 ✓
```

## 核心操作详解

### init() — 预处理幂次表 T
```cpp
void init() {
    for (int i = 1, k = 1; k <= 1000000000; ++i, k <<= 1)
        T.emplace_back(k);
}
```
预先把 $2^0, 2^1, 2^2, \ldots, 2^{30}$（约 $10^9$）依次存入 `T`，形成一个有序的"2 的幂"表。这样后续 `get_h` 可以用二分在 T 上快速查找。$10^9$ 足够覆盖竞赛常见的 N 上限。预处理只做一次，开销可忽略。

### get_h(i) — 求 $\lfloor \log_2 i \rfloor$
```cpp
int get_h(int i) {
    int ans = lower_bound(T.begin(), T.end(), i) - T.begin();
    if (1 << ans != i) ans -= 1;
    return ans;
}
```
在有序表 T 上做 `lower_bound`，找到第一个 `>= i` 的位置 `ans`。此时 $T[\text{ans}] = 2^{\text{ans}}$。若 $2^{\text{ans}} = i$（i 本身就是 2 的幂），则 $\lfloor \log_2 i \rfloor = \text{ans}$；否则 $2^{\text{ans}} > i$，真正答案应是 $\text{ans} - 1$（因为 $2^{\text{ans}-1} < i < 2^{\text{ans}}$，故 $\lfloor \log_2 i \rfloor = \text{ans}-1$）。这一 off-by-one 的修正非常关键。复杂度 $O(\log |T|) = O(\log \log N)$，实际可视为 $O(1)$。

### 建表双重循环
```cpp
for (int j = 1; j <= h; j++) {
    int k = 1 << j;          // 区间长度 2^j
    int t = j - 1;           // 上一层的 j
    for (int i = 1; i + k - 1 <= N; i++)
        A[i][j] = max(A[i][j - 1], A[i + (1 << t)][j - 1]);
}
```
外层枚举区间长度层级 `j`（从小到大保证子问题已解），内层枚举起点 `i`。`A[i][j]` 由两个长度 $2^{j-1}$ 的子区间拼成：左半 $A[i][j-1]$ 覆盖 $[i, i+2^{j-1}-1]$，右半 $A[i + 2^{j-1}][j-1]$ 覆盖 $[i+2^{j-1}, i+2^j-1]$。内层循环条件 `i + k - 1 <= N` 保证区间不越界。

### 查询
```cpp
int sz = r - l + 1;
int mh = get_h(sz);
write(max(A[l][mh], A[r - (1 << mh) + 1][mh]));
```
先求区间长度 `sz`，再算 $\text{mh} = \lfloor \log_2 \text{sz} \rfloor$，使得 $2^{\text{mh}} \leq \text{sz}$ 但 $2^{\text{mh}+1} > \text{sz}$。然后取两段长度均为 $2^{\text{mh}}$ 的区间：左段从 `l` 起覆盖 $[l, l+2^{\text{mh}}-1]$，右段以 `r` 结尾覆盖 $[r-2^{\text{mh}}+1, r]$。两段并集必覆盖整个 $[l, r]$（因为 $2^{\text{mh}} > \text{sz}/2$，两段各自长度过半必相交），取 max 即得答案。整个查询只做两次数组访问 + 一次 max，是真正的 $O(1)$。

## 逐行代码解析

```cpp
#include<bits/stdc++.h>
using namespace std;
vector<vector<int>> A;          // ST 表，A[i][j] = max[a[i..i+2^j-1]]
vector<int> T;                  // 预处理的 2 的幂次表
void init()                     // 预处理 T 表
{
	for(int i = 1,k = 1;k <= 1000000000;++i,k <<= 1) T.emplace_back(k);
    // 依次放入 2^0,2^1,...,2^30，共 31 个元素
}
int get_h(int i)                // 求 floor(log2(i))
{
	int ans = lower_bound(T.begin(),T.end(),i) - T.begin();
    // 找 T 中第一个 >= i 的元素下标
	if(1 << ans != i) ans -= 1;  // 若 i 不是 2 的幂，要回退一层
	return ans;                  // 此时 2^ans <= i < 2^(ans+1)
}
inline int read()               // 快速读入，避免 cin 拖慢
{
	int x = 0,f = 1;char ch = getchar();
	while(ch < '0' || ch > '9')   // 跳过非数字字符
	{
		if(ch == '-') f = -1;      // 处理负号
		ch = getchar();
	}
	while(ch >= '0' && ch <= '9') // 读入数字
	{
		x = x * 10 + ch - 48;
		ch = getchar();
	}
	return x * f;
}
inline void write(int x)         // 快速输出
{
    static char buf[12];
    int p=0;
    if(x<0){putchar('-');x=-x;}
    if(x==0) buf[p++]='0';
    while(x){buf[p++]=x%10+48;x/=10;} // 逆序存数字字符
    while(p--) putchar(buf[p]);        // 正序输出
}
int main()
{
	init();                       // 建 T 表
	int N = read();int M = read(); // N 个数，M 次查询
	int h = get_h(N);              // h = floor(log2(N))
	A.resize(N + 1,vector<int>(h + 1)); // 二维表 (N+1) x (h+1)
	for(int i = 1;i <= N;i++) A[i][0] = read(); // j=0 层：长度1即原值
	for(int j = 1;j <= h;j++)      // 逐层递推
	{
		int k = 1 << j;            // 当前层区间长度 2^j
		int t = j - 1;             // 子区间层数
		for(int i = 1;i + k - 1 <= N;i++) // 起点遍历，保证不越界
            A[i][j] = max(A[i][j - 1] , A[i + (1 << t)][j - 1]);
        // 拼接左右两半长度 2^(j-1) 的子区间
	}
	for(int i = 1;i <= M;i++)      // 处理每次查询
	{
		int l = read();
		int r = read();
		int sz = r - l + 1;         // 区间长度
		int mh = get_h(sz);         // 最大的 k 使 2^k <= sz
		write(max(A[l][mh] , A[r - (1 << mh) + 1][mh]));
        // 左段从 l 起，右段到 r 结束，取两者 max
		putchar('\n');
	}
	return 0;
} 
```

**关于 T表与 get_h 的工作机制**：T 是一个递增的 2 的幂序列。`lower_bound(T.begin(), T.end(), i)` 返回指向第一个 `>= i` 元素的迭代器，减去 `T.begin()` 得到下标 `ans`。此时 $T[\text{ans}] = 2^{\text{ans}}$。如果 `i` 恰好等于 $2^{\text{ans}}$（例如 `i=8`，`T[3]=8`），则 `1 << ans == i` 成立，$\lfloor \log_2 i \rfloor = \text{ans}$；否则 `i` 严格介于 $2^{\text{ans}-1}$ 与 $2^{\text{ans}}$ 之间（例如 `i=6`，$T[2]=4 < 6 < 8 = T[3]$），需要 `ans -= 1` 修正为 $\text{ans}-1$，即 $\lfloor \log_2 6 \rfloor = 2$。这种"先找上界再回退"的做法避免了在 T 上做两次二分（找 `<= i` 的最大元素），只需一次 lower_bound 加一次比较。

## 复杂度分析

- **时间预处理**：建表双重循环，外层 $h = \lfloor \log_2 N \rfloor$ 层，内层至多 N 个起点，总计 $O(N \log N)$。`init` 建 T 表 $O(\log N)$ 可忽略。
- **时间查询**：`get_h` 是 $O(\log |T|) = O(\log \log N)$ 的二分查找（实际极快，可视为 $O(1)$），两次数组访问 + 一次 max，整体 $O(1)$。M 次查询共 $O(M)$。
- **空间**：ST 表 A 是 `(N+1) × (h+1)` 的二维数组，共 $O(N \log N)$ 整数。T 表 $O(\log N)$ 可忽略。这是 ST 表相对线段树的主要"空间代价"。
- **为何查询 O(1) 成立**：核心是 max 运算的幂等性——$\max(x, x) = x$，因此两个子区间重叠不会影响结果。若改用求和（非幂等），重叠区域会被算两次，必须保证不重叠，则两段长度之和不能超过 sz，但 $2 \cdot 2^k > \text{sz}$（因为 $2^k > \text{sz}/2$），无法不重叠，所以 ST 表无法支持求和的 $O(1)$ 查询。

## 适用场景

- **静态 RMQ**：数组建立后不修改，需要大量区间最值查询，是 ST 表最经典的用途。
- **离线大批量查询**：M 很大、且不要求在线更新时，$O(1)$ 单次查询远胜线段树的 $O(\log n)$。
- **需要幂等运算的区间查询**：max、min、gcd、按位或、按位与等满足 $\text{op}(x,x)=x$ 的运算都可用 ST 表。
- **对常数敏感**：ST 表查询只做两次数组访问 + 一次运算，无递归、无 lazy，常数极小。

## 常见陷阱与注意事项

- **get_h 的 off-by-one 修正**：`if (1 << ans != i) ans -= 1;` 这一步不可省略。若省略，对于非 2 的幂的 `i`，会返回 $\lfloor \log_2 i \rfloor + 1$，导致查询时 $2^{\text{mh}} > \text{sz}$，下标 $r - 2^{\text{mh}} + 1$ 可能小于 `l`，逻辑错误甚至越界。
- **A 数组维度必须 (N+1) × (h+1)**：下标从 1 用到 N，所以行数 N+1；`j` 从 0 用到 `h`，所以列数 h+1。少开一维会越界。
- **建表外层是 j 不是 i**：必须先枚举长度层级 `j`（小到大），再枚举起点 `i`，保证 $A[i][j-1]$ 与 $A[i+2^{j-1}][j-1]$ 已算好。若把循环顺序写反会读到未初始化的值。
- **内层循环终止条件 `i + k - 1 <= N`**：保证区间右端 $i + 2^j - 1$ 不超过 N。若写成 `i <= N` 会越界访问。
- **查询两段可重叠**：这是 ST 表正确性的核心，但只对幂等运算成立。不要把 ST 表用于求和、积等非幂等运算。
- **不支持修改**：若数组需要动态修改，必须改用线段树或树状数组。ST 表一旦建好就是只读的。
- **T 表大小**：`init` 把 2 的幂一直填到 $10^9$，足够覆盖 $N \leq 10^9$。若 N 更大需扩大上界。

## 对比与扩展

- **与线段树对比**：ST 表查询 $O(1)$ 但不支持修改；线段树查询 $O(\log n)$ 但支持单点/区间修改。静态场景用 ST 表，动态场景用线段树。
- **与树状数组对比**：BIT 维护可减信息（如和），$O(\log n)$ 查询；ST 表维护幂等信息（如 max），$O(1)$ 查询但只静态。两者适用问题类型不同。
- **版本 2 的改进**：本版本手动建 T 表 + 二分求 log，可移植但代码长；版本 2 用 `__builtin_clz` 内建函数，一行求出 $\lfloor \log_2 x \rfloor$，代码更短、常数更小，但依赖 GCC。
- **支持任意幂等运算**：把 `max` 换成 `min`、`gcd`、`|`、`&` 即可，只要满足 $\text{op}(x, x) = x$。例如区间 gcd 查询、区间按位或查询。
- **二维 ST 表**：将 ST 表扩展到二维，$\text{st}[i][j][k_1][k_2]$ 存子矩阵的最值，预处理 $O(nm \log n \log m)$，查询 $O(1)$，但空间常数大。
- **笛卡尔树 + LCP**：把 RMQ 转化为 LCA 问题，配合笛卡尔树可在 $O(n)$ 预处理 + $O(1)$ 查询（带欧拉序 + ST 表），是更优的 RMQ 理论算法。
