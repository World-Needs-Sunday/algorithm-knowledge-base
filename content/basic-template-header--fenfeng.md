---
id: basic-template-header
title: '自用头文件'
category: 基础算法
subcategory: 实用工具
subSubcategory: ''
tags: ["比赛模板", "快读快写", "宏定义", "输入输出", "实用工具"]
timeComplexity: '视具体实现'
spaceComplexity: '视具体实现'
level: A
noCode: true
verified: 2026-10-19
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 算法原理

自用头文件是竞赛选手长期积累、固定复用的代码模板：把高频出现的宏定义、类型别名和快读快写函数集中放在一起，每道题开头直接套用，省去重复编写、统一代码风格。它的核心价值是「约定优于配置」——用一套固定的宏与函数名，把注意力留给算法本身，而不是输入输出细节。

本模板主要包含四块内容：

- **万能头文件与命名空间**：`#include<bits/stdc++.h>` 加 `using namespace std`，一次包含绝大多数标准库并省略 `std::` 前缀。
- **宏定义**：`endl`、`fi`、`se`、`lowbit`、`int`、`pb`、`vi`，用于缩短高频写法。
- **类型别名**：`PII`（二元组）、`ll`（`long long`）。
- **快读快写**：`read()` / `write()`，以及 `main` 里的同步关闭与文件重定向占位。

其中最核心的两点约定是：用 `#define int long long` 统一把 `int` 提升为 `long long`、配合 `signed main()` 规避溢出；用「字符逐位转数字 + 位运算加速」的快读思路把输入输出常数压到最低。

## 核心公式 / 状态定义与转移方程

模板本身没有递推状态，这里给出几个宏与函数背后的等价变换，便于理解「为什么这样写」。

- **取最低位的 1**（`lowbit`）：

$$
\mathrm{lowbit}(x) = x \mathbin{\&} (-x)
$$

其中 $-x$ 在补码下等于按位取反再加 1，即 $\sim x + 1$，二者按位与后只保留 $x$ 最低位的 1。

- **字符转数字**：设字符 $c$ 为数字字符 `'0'`～`'9'`，其 ASCII 值为 $48$～$57$，则

$$
d = c - 48 = c \mathbin{\oplus} 48
$$

数字字符的高位固定，低 4 位恰好为 $0$～$9$，与 $48$（二进制 `110000`）按位异或即等价于减 $48$。

- **快读累加**：读到数字位 $d$ 时，把已读入的值 $x$ 左移合并：

$$
x \leftarrow 10x + d = (x \ll 1) + (x \ll 3) + d
$$

其中 $(x \ll 1) + (x \ll 3) = 2x + 8x = 10x$，用位运算代替乘法。

- **快写递归**：设 $x$ 为非负整数，先递归输出 $\lfloor x/10 \rfloor$，再输出个位字符 $(x \bmod 10) + 48$；当 $x < 10$ 时不再递归，直接输出最高位。

## 逐行代码解析

模板按「宏定义 → 类型别名与 main → 快读 → 快写」四块组织，下面逐块讲解。

### 头文件与宏定义

```cpp
#include<bits/stdc++.h>
#define endl '\n'//交互题去掉
#define fi first
#define se second
#define lowbit(x) (x&(-x))
#define int long long
#define pb push_back
#define vi vector<int>
//#define v2 vector<vector<int>>
using namespace std;
```

- `#include<bits/stdc++.h>` 是万能头文件，一次包含几乎全部标准库；它是 GCC 扩展，正式竞赛环境（GCC）可用，但 MSVC 没有此头文件。
- `#define endl '\n'`：把 `endl` 替换为换行符 `'\n'`，避免 `std::endl` 每次 flush 带来的性能损失；交互题需要及时刷新输出，必须去掉这行。
- `#define fi first` / `#define se second`：把 `pair` 的 `first` / `second` 缩写为 `fi` / `se`。
- `#define lowbit(x) (x&(-x))`：取最低位的 1，常用于树状数组，见「核心公式」。
- `#define int long long`：把 `int` 全部替换为 `long long`，规避中间结果溢出；代价是 `main` 必须写成 `signed main()`。
- `#define pb push_back`：容器 `push_back` 缩写。
- `#define vi vector<int>`：`vector<int>` 缩写；注意宏会二次展开，`int` 会被替换，见「常见陷阱」。
- `//#define v2 vector<vector<int>>` 是注释掉的二维 vector 缩写，按需放开。

### 类型别名与 main 框架

```cpp
typedef pair<int,int> PII;
typedef long long ll;
//const int N=1e5+10;
//int cnt=0;
//int a[N];

signed main(){
//    freopen(".in","r",stdin);
//    freopen(".out","w",stdout);
    ios::sync_with_stdio(false);
    cin.tie(nullptr);cout.tie(nullptr);
	//int n;cin>>n;
    return 0;
}
```

- `typedef pair<int,int> PII;`：常用二元组别名；由于 `#define int long long`，它实际等价于 `pair<long long, long long>`。
- `typedef long long ll;`：这行不含 `int` 记号，不受宏影响，`ll` 仍为 `long long`。
- 注释掉的 `const int N`、`cnt`、`a[N]` 是预留的数组空间，按题目需求放开并填大小。
- `signed main()`：因为 `#define int long long`，写 `int main()` 会被展开成 `long long main()`，违反 main 必须返回 `int` 的规定而编译失败；`signed` 等价于 `signed int`，不受宏影响。
- `freopen` 两行是注释占位：本地或评测需要文件输入输出时放开并填入文件名。
- `ios::sync_with_stdio(false);` 关闭 C 与 C++ 流同步，`cin.tie(nullptr); cout.tie(nullptr);` 解绑输入输出流，二者配合加速流式读写。

### 快读 read()

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
```

- `x=0` 累计结果，`f=1` 记录符号。
- 第一个 `while` 跳过非数字字符，遇到 `-` 置 `f=-1`，用于读负数。
- 第二个 `while` 逐位读数字：`ch^48` 等价 `ch-'0'`，`(x<<1)+(x<<3)` 等价 `x*10`，三者相加即 `x = x*10 + digit`。
- 返回 `x*f` 带上符号。由于 `#define int long long`，`read` 的返回类型实际是 `long long`，可读入超出 `int` 范围的数。

### 快写 write()

```cpp
inline void write(int x){
    if(x<0){
        putchar('-');
        x=-x;
    }
    if(x>9) write(x/10);
    putchar(x%10+'0');
}
```

- 负数先输出 `-` 并取相反数。
- `if(x>9) write(x/10)` 递归先输出高位，实现「先高位后低位」。
- `putchar(x%10+'0')` 输出当前个位：`x%10` 得到数字，`+'0'` 转回字符。

## 复杂度分析

- **快读快写**：每次调用按十进制位数逐字符处理，时间复杂度为 $O(\log V)$，其中 $V$ 是数值大小，位数 $=\Theta(\log_{10} V)$。
- **关闭同步的流式读写**：单次读写接近常数，整体取决于数据规模。
- **模板整体**：视具体实现。本模板只提供框架，不决定所写算法的复杂度。
- **空间复杂度**：视具体实现。宏与类型别名不占用运行空间；快读快写只用常数个局部变量，为 $O(1)$。

## 适用场景

- 竞赛日常刷题、比赛的统一答题模板，开题即套用，省去重复的输入输出样板。
- 数据量大的题目：用 `read()` / `write()` 快读快写替代 `cin` / `cout` 或 `scanf` / `printf`，避免输入输出成为瓶颈。
- 需要 `long long` 的题目：`#define int long long` 规避累加、乘法等中间结果溢出。
- 树状数组等需要 `lowbit`、或大量使用 `pair`、`vector` 的题目：宏与别名显著减少代码量。

## 常见陷阱与注意事项

1. `#define int long long` 必须配 `signed main()`：否则 `int main()` 被展开成 `long long main()`，导致编译失败。
2. `#define endl '\n'` 在交互题必须去掉：`std::endl` 会 flush，`'\n'` 不会；交互题需要每次输出立即刷新，用 `'\n'` 可能因缓冲导致评测挂起。
3. 宏缺少整体括号：`#define lowbit(x) (x&(-x))` 只给参数加了括号，未给整体加括号。传 `lowbit(a+1)` 会展开成 `a+1&(-a+1)`，优先级出错；稳妥写法是 `((x)&(-(x)))`。
4. 别名被二次展开：`#define int long long` 之后，`vi` 展开为 `vector<int>` 再被二次展开为 `vector<long long>`；`typedef pair<int,int> PII` 同样变为 `pair<long long,long long>`。需要严格的 32 位 `int` 时应改用 `signed` 或 `int32_t`。
5. 快读不处理文件末尾：`while(ch<'0'||ch>'9')` 在 `getchar()` 读到 EOF（返回 $-1$）时，$-1 < 48$ 恒成立，会死循环。题目保证输入合法时无碍，稳健写法应在循环内判断 `ch == EOF`。
6. 万能头文件是 GCC 扩展：`<bits/stdc++.h>` 在 MSVC 下不存在，提交 OJ 多为 GCC 环境无碍，本地若用 VS 需注意。
7. 快读快写与流混用：`read` / `write` 走 `getchar` / `putchar`，与关闭同步后的 `cin` / `cout` 是两套缓冲，混用可能出现顺序错乱；同一程序要么全用快读快写，要么全用流。

## 对比与扩展

| 输入输出方式 | 写法 | 特点 | 适用 |
|---|---|---|---|
| 标准 cin/cout | `cin >> x; cout << x;` | 默认与 C 同步，较慢 | 数据量小、调试 |
| 关闭同步 cin/cout | `ios::sync_with_stdio(false)` | 关闭同步，较快 | 中等数据量 |
| scanf/printf | `scanf("%d",&x)` | C 风格，格式控制强 | 大数据量、格式控制 |
| 快读快写 | `read()` / `write()` | 无流同步，常数小 | 大数据量、追求常数 |

**扩展方向**：

- 万能头文件可换 `<bits/extc++.h>` 并引入 `__gnu_pbds` 命名空间，获得平衡树、堆等扩展结构，但需注意与 `std::priority_queue` 的冲突。
- 快读可扩展为读取 `long double`、`__int128`（需手动解析）、`string` 等版本；快写同理可扩展为写 `long long`（本模板中 `int` 已宏展开为 `long long`，`x/10`、`x%10` 已适用于 `long long`）。
- `#define int long long` 可换 `using ll = long long` 加显式使用，更可控但写起来略啰嗦。
- 可按个人习惯补充 `#define v2 vector<vector<int>>`、`for` 循环缩写等宏。
