---
id: dp-digit-memo
title: '数位 DP（记忆化搜索框架）'
category: 动态规划
subcategory: 数位DP
subSubcategory: ''
tags: ["数位DP", "记忆化搜索", "按位枚举", "区间转前缀", "前导零"]
timeComplexity: 'O(L·S·10)'
spaceComplexity: 'O(L·S)'
codePath: '动态规划\数位DP记忆化搜索\Untitled1.cpp'
level: A
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 算法原理

数位 DP 是一类专门处理「与数字的每一位有关」的计数 / 最值问题的算法。它不枚举区间里的每一个数，而是从最高位开始一位一位地「填数字」，配合记忆化搜索，把「数位上的限制」翻译成状态与转移。套路非常固定：只要理解了「按位枚举 + 记忆化搜索」这个框架，再复杂的限制也不过是往状态里多加几个维度。

**区间转前缀**。这类问题通常问「$[L, R]$ 中有多少个数满足某个性质」。直接统计区间很麻烦，于是定义 `solve(x)` 表示 $[0, x]$（或 $[1, x]$，视题目而定）中满足条件的个数，答案可以写成一个前缀差：

$$
\text{ans} = \text{solve}(R) - \text{solve}(L - 1)
$$

于是任务被规约成：给定一个上界 `x`，统计从 $0$ 到 `x` 的所有整数中有多少个符合条件。这一步把「区间」转化为「前缀」，是数位 DP 的第一个关键动作。

**为什么用记忆化搜索而不是循环 DP**。数位 DP 的状态里通常有「是否贴着上界」这一维，它与具体上界强相关，循环 DP 需要精细处理从高位到低位的贴界与进位；而记忆化搜索直接用递归参数表达这些约束，写起来更直观，且天然按位推进。这也是本知识点以「记忆化搜索框架」命名的原因。

## 核心公式 / 状态定义与转移方程

### 状态定义

从最高位开始逐位填数，递归过程中维护几类通用状态：

- `pos`：当前处理到第几位（从高位向低位推进）。
- `tight`（本实现写为 `is_limited`）：是否已经「贴住上界」。若为真，当前位最大只能填上界的对应位；否则可填 $0 \sim 9$。
- `started`（本实现写为 `qdz`，含义是「是否仍处于前导零」）：是否已经开始填非零数字。前导零不参与某些限制，比如相邻数字差的判断。
- 附加状态视题目而定：上一个数字、连续相同长度、是否出现过某个数码、当前已填数字的模数余数等。

### 转移方程

设当前位上限为 $\text{upper}$：当 `tight` 为真时 $\text{upper} = \text{digits}[pos]$，否则 $\text{upper} = 9$。枚举数字 $d \in [0, \text{upper}]$，有：

$$
\text{dfs}(pos, \cdots) = \sum_{d = 0}^{\text{upper}} \text{dfs}\bigl(pos - 1, \cdots\bigr)
$$

其中状态的更新为：新贴界 = `is_limited && (d == digits[pos])`，新前导零 = `qdz && (d == 0)`，其余附加状态按题目约束更新。递归基为「所有位都填完时返回 $1$」，表示当前这条填数路径是一个合法方案。

### 通用框架（伪代码）

```cpp
int digits[...];     // 上界 N 的各位数字
long long dp[...];   // 记忆化数组，维度按状态而定

long long dfs(int pos, /* 附加状态… */, bool is_limited, bool is_leading_zero) {
    if (pos == 0) return 1;                            // 递归基：所有位填完
    if (!is_limited && !is_leading_zero && dp[pos][...] != -1)
        return dp[pos][...];                           // 记忆化查表
    long long ans = 0;
    int upper_bound = is_limited ? digits[pos] : 9;    // 当前位上限
    for (int d = 0; d <= upper_bound; ++d) {
        // …检查 d 是否合法（附加状态）…
        ans += dfs(pos - 1, /* 新附加状态… */, is_limited && (d == digits[pos]),
                   is_leading_zero && (d == 0));
    }
    if (!is_limited && !is_leading_zero)
        dp[pos][...] = ans;                            // 记录状态
    return ans;
}

long long solve(long long n) {
    if (n < 0) return 0;
    if (n == 0) return 1;                              // 0 是否算数按题意处理
    int len = 0;
    while (n > 0) { digits[++len] = n % 10; n /= 10; }
    memset(dp, -1, sizeof(dp));
    return dfs(len, /* 附加状态初值… */, true, true);
}

long long result = solve(R) - solve(L - 1);            // 区间转前缀
```

### 本题（windy 数）的状态与转移

下方代码以「windy 数」为例：不含前导零、且相邻两个数字之差至少为 $2$ 的正整数称为 windy 数，统计区间 $[a, b]$ 内 windy 数的个数（$a, b \le 10^{18}$）。状态设计：

- `w`：当前位（从 $1$ 到 `dqw`，`dqw` 为上界位数）。
- `is_limited`：是否贴住上界。
- `qianyiwei`：上一位填的数字（$0 \sim 9$），用于判断相邻差。
- `qdz`：是否仍处于前导零（$1$ 表示尚未开始）。

转移时枚举数字 $i \in [0, \text{upper}]$，其中 $\text{upper}$ 由贴界情况决定；若「已开始且 $\lvert \text{qianyiwei} - i \rvert < 2$」则剪枝，若 `is_limited` 且 $i > N[w]$ 则越界剪枝。

## 逐行代码解析

### 模块一：全局定义与 DP 数组

```cpp
#include <bits/stdc++.h>   // 万能头文件
using namespace std;
#define int long long   // 全局 long long，避免计数溢出
string l,r;   // 区间端点（字符串，便于逐位拆分）
int dp[30][2][30][2],N[30];   // 记忆化数组 / 上界各位数字
int dqw;   // 上界位数
```

- `dp[30][2][30][2]` 的四个下标依次是 `pos / is_limited / qianyiwei / qdz`；`30` 足够容纳 `long long` 范围内最多 $19$ 位的数。
- `N[i]` 存上界第 $i$ 位的数字（下标从 $1$ 开始），配合 `is_limited` 限制当前位能填的上限。
- `#define int long long` 把 `int` 全局替换为 `long long`，主函数因此写成 `signed main()`，避免答案溢出。

### 模块二：dfs 函数（记忆化搜索主体）

```cpp
int dfs(int w,int is_limited,int qianyiwei,int qdz){
    if(dp[w][is_limited][qianyiwei][qdz]!=-1) return dp[w][is_limited][qianyiwei][qdz];   // 已算过：查表直接返回
    if(w==dqw+1) return 1;   // 递归基：所有位都填完
    dp[w][is_limited][qianyiwei][qdz]=0;   // 清零后累加子状态
    for(int i=0;i<10;i++){   // 枚举当前位 0~9
        if(abs(qianyiwei-i)<2&&qdz!=1) continue;   // 相邻差不足 2 且已开始 → 非法
        if(is_limited&&i>N[w]) continue;   // 贴界却填大 → 非法
        dp[w][is_limited][qianyiwei][qdz]+=dfs(w+1,is_limited&&(i==N[w]),i,qdz&&(i==0));   // 转移：更新贴界与前导零
    }
    return dp[w][is_limited][qianyiwei][qdz];
}
```

- 递归基是 `w == dqw + 1`：此时所有位都已填完，返回 $1$ 表示这是一条合法路径。
- 记忆化查表放在最前面：`dp` 初始化为 $-1$，一旦 `!= -1` 说明该状态已算过，直接复用，避免重复计算。
- `is_limited && (i == N[w])` 更新贴界：只有原本贴界且本位置恰好等于上界对应位，下一层才继续贴界。
- `qdz && (i == 0)` 更新前导零：只有原本全是 $0$ 且本位置仍填 $0$，才继续处于前导零。

### 模块三：solve 函数（数位拆分）

```cpp
int solve(string x){
	memset(dp,-1,sizeof dp);   // 每次 solve 重置记忆化
	 for(int i=0;i<x.size();i++){   // 逐字符拆分上界
	           N[i+1]=x[i]-48;   // 字符转数字（ASCII 48 即 0）
	       }
	 dqw=x.size();   // 记录位数
	 return dfs(1,1,0,1);   // 从第 1 位开始，贴界且处在前导零
}
```

- 入口参数是字符串 `x`，用 `x[i] - 48` 把字符转成数字存进 `N`，避免了 `long long` 拆位时反复取模的麻烦。
- `dfs(1, 1, 0, 1)` 的四个初值：从第 $1$ 位开始、贴住上界、上一位为 $0$、仍处在前导零。
- 每次 `solve` 前必须 `memset(dp, -1, ...)`，因为不同的上界对应不同的 `dp` 表，不能跨上界复用。

### 模块四：主函数（区间转前缀）

```cpp
signed main(){
	int a,b;cin>>a>>b;a--;   // 读区间，a 减一
	l=to_string(a),r=to_string(b);   // 端点转字符串
	cout<<solve(r)-solve(l);   // 前缀差即答案
}
```

- `a--` 对应公式 $\text{solve}(b) - \text{solve}(a - 1)$，把区间 $[a, b]$ 转成两个前缀之差。
- `to_string` 把端点转成字符串，交给 `solve` 逐位拆分。
- 主函数写成 `signed main()`，因为 `#define int long long` 会改写 `int main` 中的 `int`。

## 复杂度分析

数位 DP 的复杂度只与「位数」和「状态数」有关，与数字本身的大小无关。

设上界有 $L$ 位（`long long` 范围内 $L \le 19$），除 `pos / tight / started` 之外的附加状态总数为 $S$。记忆化搜索保证每个状态最多被计算一次，而每个状态要枚举 $0 \sim 9$ 共 $10$ 个数做转移，因此单次 `solve`：

- **时间复杂度**：$O(L \cdot S \cdot 10)$。`tight`、`started` 各两种取值带来的常数因子并入大 O。
- **空间复杂度**：$O(L \cdot S)$，即记忆化数组 `dp` 的大小。

以本实现（windy 数）为例：$L \le 19$，附加状态只有「上一位数字」$S = 10$，状态总数约 $19 \times 2 \times 10 \times 2 = 760$，每个状态枚举 $10$ 个数，单次 `solve` 只有几千次操作。这也是数位 DP 能轻松应付 $10^{18}$ 量级区间的原因。

## 适用场景

1. **区间计数**：统计 $[L, R]$ 内满足「数位性质」的数的个数（不含某数码、相邻数字差、各位和等）。
2. **超大上界**：$L, R$ 可达 $10^{18}$ 甚至大整数（逐位存储），暴力枚举不可行而数位 DP 仍很快。
3. **带数位约束的最值 / 构造**：在满足数位限制的前提下求最大值、第 $k$ 小等。
4. **前导零敏感问题**：需要区分「真实数字」与「前导零」的题目（如 windy 数、统计数码出现次数）。

不适合的场景：与数位无关的计数，或状态必须与「完整数字」强绑定的问题——此时「区间转前缀」与按位状态拆分发挥不了作用。

## 常见陷阱与注意事项

1. **前导零**：前导零不算真实数字。若把它当成普通数字参与相邻差、出现次数等限制会多算；维护 `started` 状态，直到第一次填非零数字才真正「开始」。
2. **0 是否算数**：`solve(-1)` 应为 $0$，而 `solve(0)` 返回 $1$ 还是 $0$ 取决于题意是否承认 $0$ 满足条件，需单独确认。
3. **记忆化的时机**：严格说只在 `!tight && !started` 时缓存结果。把「贴界」状态也放进 `dp` 维度（如本例）正确性不受影响，但会把状态与具体上界绑定，复用率下降。
4. **状态维度爆炸**：不要随意加维度。若某个信息只在递归基里用到，可考虑用数学方法（数位和、余数）代替，避免状态数激增。
5. **贴界上限**：贴界时当前位上限是上界对应位 `N[w]`，否则是 $9$；把上限恒设成 $9$ 会漏掉贴界剪枝，导致结果偏大。

## 对比与扩展

| 方法 | 思路 | 适用 | 特点 |
|---|---|---|---|
| 按位统计法 | 用公式直接算每一位的贡献 | 数码计数、各位和等简单问题 | 代码短、$O(\log N)$，但约束一复杂就难写 |
| 数位 DP（记忆化搜索） | 逐位填数 + 状态转移 | 任意数位约束的计数 / 最值 | 通用性强、状态灵活 |

按位统计法可以看作数位 DP 的最简形态：两者都「按位处理」，而记忆化搜索框架把「紧贴上界、前导零、附加约束」统一成状态，能推广到任意数位限制。

**变体与扩展方向**：

- **固定位数**：如「11 位手机号码」问题，首位不能是 $0$，可直接限定第一位枚举 $1 \sim 9$，省去 `started` 状态。
- **连续相同段**：增加 `same`（当前连续长度）、`has4 / has8`（是否出现过某数码）等状态，用于「至少三个相邻相同且不能同时出现 4 和 8」这类约束。
- **子串禁止 / 自动机**：若限制「不能出现某子串」，可把已填后缀压成 AC 自动机节点作为状态。
- **记忆化搜索 ↔ 迭代 DP**：两者可互转，记忆化搜索更直观，迭代 DP 常数更小。
