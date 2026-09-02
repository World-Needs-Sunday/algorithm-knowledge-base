---
id: ds-st-table-2
title: 'ST表（标准版2 - 内建函数求对数）'
category: 数据结构
subcategory: ST表
tags: ["数据结构", "ST表", "RMQ", "倍增", "区间最值", "__builtin_clz"]
timeComplexity: '预处理O(n log n)，查询O(1)'
spaceComplexity: 'O(n log n)'
codePath: '数据结构\ST表\标准\2\Untitled1.cpp'
---

## 算法原理

ST表（Sparse Table，稀疏表）是基于**倍增**思想（也叫 binary lifting，二进制 lifting）的静态区间查询数据结构，专门解决不可修改数组的区间最值问题（Range Minimum/Maximum Query，RMQ）。倍增的核心是：任意一个区间长度都能表示为若干个 2 的幂次之和（这其实就是二进制分解），因此只要预先算出所有"长度为 2 的幂"的区间的答案，就能在常数次拼合内回答任意长度的查询。具体地，设 $\text{st}[i][j]$ 表示从下标 `i` 开始、长度为 $2^j$ 的区间（即 $[i, i + 2^j - 1]$）的最值，则查询区间 $[l, r]$（长度 $\text{len} = r - l + 1$）时，令 $k = \lfloor \log_2 \text{len} \rfloor$，把区间拆成两段：从 `l` 起长度 $2^k$ 的左段 $[l, l + 2^k - 1]$，和到 `r` 结束长度 $2^k$ 的右段 $[r - 2^k + 1, r]$。这两段可能重叠，但重叠不影响结果——这正是 ST 表能在 $O(1)$ 完成查询的关键。

预处理用递推：$$\text{st}[i][j] = \max(\text{st}[i][j-1],\; \text{st}[i + 2^{j-1}][j-1])$$。直观理解：长度 $2^j$ 的区间可以拆成两个长度 $2^{j-1}$ 的子区间——前半 $[i, i + 2^{j-1} - 1]$ 与后半 $[i + 2^{j-1}, i + 2^j - 1]$，分别对应 $\text{st}[i][j-1]$ 和 $\text{st}[i + 2^{j-1}][j-1]$，取两者最值即得 $\text{st}[i][j]$。由于 `j` 从小到大递推，子区间的答案已经算好，整张表可在 $O(n \log n)$ 时间内建立。注意第二维 `j` 的上界是 $\lfloor \log_2 N \rfloor$，因为 $2^j$ 不能超过 N。

**为什么查询是 $O(1)$**：因为 max/min 是**幂等运算**（idempotent），即 $\max(x, x) = x$。查询时两段子区间可以重叠，重叠部分被算两次但取 max 后仍正确。这一点对和、积等非幂等运算不成立——若要求和，两段重叠区域的元素会被计算两次。所以 ST 表只适用于 max、min、gcd、按位或等幂等运算；对于求和则必须用前缀和或线段树。本版本与版本 1 的核心算法完全相同，唯一区别在于用 GCC 内建函数 `__builtin_clz` 直接在硬件层面计算 $\lfloor \log_2 x \rfloor$，省去了手动预处理幂次表 T 和 lower_bound 二分查找，代码更短、常数更小。

**`__builtin_clz` 与 $\lfloor \log_2 x \rfloor$ 的关系**：`__builtin_clz(x)` 返回 32 位无符号整数 `x` 的二进制表示中前导 0 的个数。设 `x` 的最高位的 1 位于第 $p$ 位（从 0 开始，最低位为第 0 位），则比 $p$ 高的位（第 $p+1$ 到 第 31 位）全为 0，共 $31 - p$ 个前导 0。因此 $\text{\_\_builtin\_clz}(x) = 31 - p$，于是 $31 - \text{\_\_builtin\_clz}(x) = 31 - (31 - p) = p = \lfloor \log_2 x \rfloor$。例如 $x = 6$（二进制 `110`，最高位的 1 在第 2 位），32 位下为 `0...0110`，前导 0 共 29 个，$\text{\_\_builtin\_clz}(6) = 29$，$31 - 29 = 2 = \lfloor \log_2 6 \rfloor$ ✓。这一映射在硬件层面通常被编译为单条 `BSR` 或 `LZCNT` 指令，比手动二分快得多。

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

`__builtin_clz` 的位级工作示意：
```
x = 6 的 32 位表示:
位:  31 30 29 28 ... 5  4  3  2  1  0
     0  0  0  0      0  0  0  1  1  0
     ^^^^^^^^^^^^^^^^^^^^^^^^^^^^      <- 29 个前导 0 (位 31..3)
                                  ^^  <- 最高位的 1 在第 2 位

__builtin_clz(6) = 29
31 - 29 = 2  =  floor(log2(6))  ✓

x = 8 的 32 位表示:
位:  31 30 29 28 ... 4  3  2  1  0
     0  0  0  0      0  0  1  0  0
     ^^^^^^^^^^^^^^^^^^^^^^^^         <- 28 个前导 0 (位 31..4)
                              ^      <- 最高位的 1 在第 3 位

__builtin_clz(8) = 28
31 - 28 = 3  =  floor(log2(8))  ✓
```

## 核心操作详解

### get_h(i) — 求 $\lfloor \log_2 i \rfloor$（内建函数版）
```cpp
inline int get_h(int i) {
    return 31 - __builtin_clz((unsigned)i);
}
```
核心只有一行。`__builtin_clz((unsigned)i)` 计算 `i` 的 32 位无符号表示中前导 0 的个数。设 `i` 最高位的 1 在第 $p$ 位（0-indexed），则前导 0 共 $31 - p$ 个，于是 $31 - \text{\_\_builtin\_clz}(i) = 31 - (31 - p) = p = \lfloor \log_2 i \rfloor$。强转 `(unsigned)` 是为了在 `i` 为负时也按无符号位模式计算（虽然本场景 `i` 都是正数，但这是安全惯例）。相比版本 1 的 lower_bound 二分，此版本是真正的 $O(1)$ 单条指令，常数极小。注意 `__builtin_clz(0)` 的行为未定义，所以调用前必须保证 `i >= 1`。

### 建表双重循环
```cpp
for (int j = 1; j <= h; j++) {
    int k = 1 << j;          // 区间长度 2^j
    int t = j - 1;           // 上一层的 j
    for (int i = 1; i + k - 1 <= N; i++)
        A[i][j] = max(A[i][j - 1], A[i + (1 << t)][j - 1]);
}
```
与版本 1 完全一致。外层枚举区间长度层级 `j`（从小到大保证子问题已解），内层枚举起点 `i`。`A[i][j]` 由两个长度 $2^{j-1}$ 的子区间拼成：左半 $A[i][j-1]$ 覆盖 $[i, i+2^{j-1}-1]$，右半 $A[i + 2^{j-1}][j-1]$ 覆盖 $[i+2^{j-1}, i+2^j-1]$。内层循环条件 `i + k - 1 <= N` 保证区间不越界。

### 查询
```cpp
int sz = r - l + 1;
int mh = get_h(sz);
write(max(A[l][mh], A[r - (1 << mh) + 1][mh]));
```
与版本 1 完全一致。先求区间长度 `sz`，再算 $\text{mh} = \lfloor \log_2 \text{sz} \rfloor$，使得 $2^{\text{mh}} \leq \text{sz}$ 但 $2^{\text{mh}+1} > \text{sz}$。然后取两段长度均为 $2^{\text{mh}}$ 的区间：左段从 `l` 起覆盖 $[l, l+2^{\text{mh}}-1]$，右段以 `r` 结尾覆盖 $[r-2^{\text{mh}}+1, r]$。两段并集必覆盖整个 $[l, r]$（因为 $2^{\text{mh}} > \text{sz}/2$，两段各自长度过半必相交），取 max 即得答案。整个查询只做两次数组访问 + 一次 max + 一次 `__builtin_clz`，是真正的 $O(1)$。

## 逐行代码解析

```cpp
#include<bits/stdc++.h>
using namespace std;
vector<vector<int>> A;          // ST 表，A[i][j] = max[a[i..i+2^j-1]]
vector<int> T;                  // 声明但本版本未使用（保留以兼容）
inline int get_h(int i)         // 一行求 floor(log2(i))
{
    return 31 - __builtin_clz((unsigned)i);
    // __builtin_clz 返回前导 0 个数；31-之 即最高位 1 的位置 = log2
}
inline int read()               // 快速读入
{
	int x = 0,f = 1;char ch = getchar();
	while(ch < '0' || ch > '9')   // 跳过非数字
	{
		if(ch == '-') f = -1;
		ch = getchar();
	}
	while(ch >= '0' && ch <= '9') // 累加数字
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
    while(x){buf[p++]=x%10+48;x/=10;}
    while(p--) putchar(buf[p]);
}
int main()
{
	int N = read();int M = read(); // N 个数，M 次查询
	int h = get_h(N);              // h = floor(log2(N))
	A.resize(N + 1,vector<int>(h + 1)); // 二维表 (N+1) x (h+1)
	for(int i = 1;i <= N;i++) A[i][0] = read(); // j=0 层即原值
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

**关于 `__builtin_clz` 在位级别的工作机制**：CPU 通常提供"找最高位 1"的硬件指令，如 x86 的 `BSR`（Bit Scan Reverse）和 `LZCNT`（Leading Zero Count）。GCC 的 `__builtin_clz` 会被编译为这些单周期指令，效率远高于软件实现的二分查找。对一个 32 位无符号整数 `x`，设其二进制中最高位的 1 位于第 $p$ 位（最低位为第 0 位），则第 $p+1$ 位到第 31 位全为 0，前导 0 个数就是 $31 - p$。所以 $31 - \text{\_\_builtin\_clz}(x) = 31 - (31 - p) = p = \lfloor \log_2 x \rfloor$。几个验证示例：
- $x = 1$（`0...0001`），最高位 1 在第 0 位，$\text{\_\_builtin\_clz}(1) = 31$，$31 - 31 = 0 = \lfloor \log_2 1 \rfloor$ ✓
- $x = 6$（`0...0110`），最高位 1 在第 2 位，$\text{\_\_builtin\_clz}(6) = 29$，$31 - 29 = 2 = \lfloor \log_2 6 \rfloor$ ✓
- $x = 8$（`0...1000`），最高位 1 在第 3 位，$\text{\_\_builtin\_clz}(8) = 28$，$31 - 28 = 3 = \lfloor \log_2 8 \rfloor$ ✓
- $x = 10^9$（约 $2^{30}$），最高位 1 在第 29 位附近，结果约 29。

**与版本 1 的对比**：版本 1 需要 `init()` 预处理 T 表（$O(\log N)$）+ 每次 `get_h` 做 `lower_bound` 二分（$O(\log \log N)$），且有一处容易出错的 off-by-one 修正 `if (1 << ans != i) ans -= 1`。版本 2 用一行内建函数替代，无预处理、无修正、单指令完成，代码更短、更不易错、常数更小。代价是依赖 GCC 内建函数，可移植性略差（MSVC 需用 `_BitScanReverse` 等替代）。

## 复杂度分析

- **时间预处理**：建表双重循环，外层 $h = \lfloor \log_2 N \rfloor$ 层，内层至多 N 个起点，总计 $O(N \log N)$。无 `init` 预处理步骤。
- **时间查询**：`get_h` 编译为单条 `BSR`/`LZCNT` 指令，严格 $O(1)$；两次数组访问 + 一次 max，整体 $O(1)$。M 次查询共 $O(M)$。相比版本 1 省去了 lower_bound 的 $O(\log \log N)$ 二分。
- **空间**：ST 表 A 是 `(N+1) × (h+1)` 的二维数组，共 $O(N \log N)$ 整数。虽然声明了 `vector<int> T` 但本版本未使用，不占实际空间（可删除）。
- **为何查询 O(1) 成立**：核心是 max 运算的幂等性——$\max(x, x) = x$，因此两个子区间重叠不会影响结果。若改用求和（非幂等），重叠区域会被算两次，必须保证不重叠，但 $2 \cdot 2^k > \text{sz}$（因为 $2^k > \text{sz}/2$），无法不重叠，所以 ST 表无法支持求和的 $O(1)$ 查询。

## 适用场景

- **静态 RMQ**：数组建立后不修改，需要大量区间最值查询，是 ST 表最经典的用途。
- **离线大批量查询**：M 很大、且不要求在线更新时，$O(1)$ 单次查询远胜线段树的 $O(\log n)$。
- **需要幂等运算的区间查询**：max、min、gcd、按位或、按位与等满足 $\text{op}(x,x)=x$ 的运算都可用 ST 表。
- **竞赛快速实现**：相比版本 1，本版本代码更短、常数更小，是竞赛中的首选写法（只要环境支持 GCC）。

## 常见陷阱与注意事项

- **`__builtin_clz(0)` 未定义行为**：传入 0 会导致未定义结果（部分 CPU 上 `LZCNT` 返回 32，但 `BSR` 不写结果）。本代码中 `get_h` 的调用者 `sz = r - l + 1 >= 1`、`N >= 1`，保证不会传 0；若其他场景需处理 0 必须特判。
- **强转 (unsigned) 的必要性**：若 `i` 是负数，作为 `int` 传给 `__builtin_clz` 行为依赖实现；强转 `unsigned` 后按位模式计算，结果明确。本场景 `i` 都是正数，但这是良好习惯。
- **A 数组维度必须 (N+1) × (h+1)**：下标从 1 用到 N，所以行数 N+1；`j` 从 0 用到 `h`，所以列数 h+1。少开一维会越界。
- **建表外层是 j 不是 i**：必须先枚举长度层级 `j`（小到大），再枚举起点 `i`，保证 $A[i][j-1]$ 与 $A[i+2^{j-1}][j-1]$ 已算好。若把循环顺序写反会读到未初始化的值。
- **内层循环终止条件 `i + k - 1 <= N`**：保证区间右端 $i + 2^j - 1$ 不超过 N。若写成 `i <= N` 会越界访问。
- **查询两段可重叠**：这是 ST 表正确性的核心，但只对幂等运算成立。不要把 ST 表用于求和、积等非幂等运算。
- **不支持修改**：若数组需要动态修改，必须改用线段树或树状数组。ST 表一旦建好就是只读的。
- **依赖 GCC 内建函数**：`__builtin_clz` 是 GCC/Clang 扩展，MSVC 需用 `_BitScanReverse` 替代；若需跨编译器可移植，回到版本 1 的手动表法。

## 对比与扩展

- **与版本 1 对比**：算法完全相同，仅 `get_h` 实现不同。版本 2 用 `__builtin_clz` 一行替代版本 1 的 `init` + `lower_bound` + off-by-one 修正，代码更短、更不易错、常数更小（单指令 vs 二分）。竞赛中优先选版本 2。
- **与线段树对比**：ST 表查询 $O(1)$ 但不支持修改；线段树查询 $O(\log n)$ 但支持单点/区间修改。静态场景用 ST 表，动态场景用线段树。
- **与树状数组对比**：BIT 维护可减信息（如和），$O(\log n)$ 查询；ST 表维护幂等信息（如 max），$O(1)$ 查询但只静态。两者适用问题类型不同。
- **支持任意幂等运算**：把 `max` 换成 `min`、`gcd`、`|`、`&` 即可，只要满足 $\text{op}(x, x) = x$。例如区间 gcd 查询、区间按位或查询。
- **相关内建函数**：`__builtin_ctz(x)` 返回末尾 0 个数（用于 lowbit 风格的算法）；`__builtin_popcount(x)` 返回 1 的个数；`__lg(x)` 直接返回 $\lfloor \log_2 x \rfloor$（GCC 提供，等价于 `31 - __builtin_clz`）。
- **二维 ST 表**：将 ST 表扩展到二维，$\text{st}[i][j][k_1][k_2]$ 存子矩阵的最值，预处理 $O(nm \log n \log m)$，查询 $O(1)$，但空间常数大。
- **笛卡尔树 + LCP**：把 RMQ 转化为 LCA 问题，配合笛卡尔树可在 $O(n)$ 预处理 + $O(1)$ 查询（带欧拉序 + ST 表），是更优的 RMQ 理论算法。
