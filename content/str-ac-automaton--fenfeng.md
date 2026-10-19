---
id: str-ac-automaton
title: 'AC 自动机'
category: 字符串
subcategory: 字符串匹配
subSubcategory: ''
tags: ["多模式匹配", "字符串匹配", "Trie", "fail指针", "自动机", "KMP"]
timeComplexity: 'O(∑|s_i|·26 + |t|)'
spaceComplexity: 'O(∑|s_i|·26)'
level: A
verified: 2026-10-19
codePath: '字符串\AC自动机\Untitled1.cpp'
source: '焚风第三舰队指挥官 (QQ 3370864672)'
---

## 算法原理

AC 自动机（Aho–Corasick Automaton）用于解决**多模式串匹配**问题：给定若干模式串 $s_1,\dots,s_k$ 与一个文本串 $t$，希望在一次扫描中同时找出所有模式串的出现情况。它可以理解为「Trie 树上的 KMP」——Trie 树负责存储全部模式串，fail 指针则借用 KMP 的失配跳转思想，使文本串只需扫描一遍。

### 从暴力到 AC 自动机

若用 KMP 逐个匹配 $k$ 个模式串，每次都要从头扫描文本串，总复杂度为 $O(k(n+m))$；当 $k$ 很大（如 $10^5$）时不可接受。AC 自动机的思路是：先把所有模式串建成一棵 Trie 树，再在这棵树上一次性扫描文本串。这样每个字符只处理一次，总复杂度降到 $O(\sum |s_i| + |t|)$。

### 建 Trie 树

把所有模式串插入一棵普通的 Trie 树。Trie 的每个节点代表一个状态，即某个模式串的前缀；节点之间的边代表状态转移。模式串的结尾节点需要标记，本代码用 `val` 记录「当前节点是哪个模式串的结尾」，用 `rev` 做模式串编号到 `val` 的反向映射，以支持重复模式串。

### fail 指针

fail 指针是 AC 自动机的灵魂。定义：节点 $u$ 的 fail 指针指向节点 $v$，使得 $v$ 所代表的字符串是 $u$ 所代表字符串的最长后缀，且 $v$ 是某个模式串的前缀。简言之，fail 指针指向「当前状态的最长后缀状态」。fail 指针通过 BFS 按层构建，核心递推关系是：设 $v = c[u][i]$，则 $fail[v] = c[fail[u]][i]$。

### 路径压缩

构建 fail 时，若节点 $u$ 没有字符 $i$ 的子节点，就直接把 $c[u][i]$ 指向 $c[fail[u]][i]$。这样匹配时不需要 while 循环跳 fail，直接一步走到正确节点，本质上相当于并查集的路径压缩——把「不存在的边」直接连到「最终会到达的节点」上。经过路径压缩，Trie 变成一张 **Trie 图**，每个节点对每个字符都有明确转移，匹配时只需 $O(1)$ 跳转。

### 拓扑统计

匹配时只在当前节点打标记 `ans[p]++`，这样只知道哪些节点被经过。要统计每个模式串出现了多少次，还需沿 fail 链把标记「上传」给更长的后缀。fail 指针构成一棵 fail 树，直接在 fail 树上做一次拓扑排序，把子节点的 `ans` 累加到父节点，即可在线性时间内完成统计。

## 核心公式 / 状态定义与转移方程

**状态定义**：Trie 上的每个节点代表一个状态，即某个模式串的前缀；`c[u][i]` 表示节点 $u$ 沿字符 $i$（由 `idx` 映射到 $0\sim25$）的转移目标。

**状态转移**：

$$v = c[u][i]$$

**fail 指针递推**（BFS 按层构建，其中 $v = c[u][i]$）：

$$fail[v] = c[fail[u]][i]$$

**路径压缩**（当 $c[u][i]$ 为空时）：

$$c[u][i] = c[fail[u]][i]$$

## 逐行代码解析

### 模块一：头文件与结构体定义

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
const int N=1e6+10;
//int cnt=0;
//int a[N];
struct AC{
	int c[N][26],val[N],fail[N],ans[N],ind[N],rev[N],cxc[N];
	int rot=0,cnt=0;
	int idx(char a){
		return (int)(a-'a');
	}
```

- `c[N][26]` 是 Trie 的转移数组，每个节点存 26 个儿子；`val` 记录节点对应的模式串编号，`fail` 存 fail 指针，`ans` 记录节点被经过次数，`ind` 是 fail 树的入度，`rev` 做编号反查，`cxc` 回填答案。
- `#define int long long` 使所有 `int` 实际为 64 位整数，避免大数溢出，但同时使 `c[N][26]` 等数组内存翻倍，需留意空间。
- `rot = 0` 是根节点编号，`cnt` 从 0 起动态分配新节点；`idx` 把字符 `'a'~'z'` 映射到 $0\sim25$。

### 模块二：insert 插入模式串

```cpp
	void insert(string s,int x){
		int nrt=rot,len=s.size();
		for(int i=0;i<len;i++){
			int id=idx(s[i]);
			if(!c[nrt][id]) c[nrt][id]=++cnt;
			nrt=c[nrt][id];
		}
		if(!val[nrt]) val[nrt]=x;
		rev[x]=val[nrt];
	}
```

- 从根 `rot` 出发逐字符走 Trie，边不存在就新建节点 `++cnt`，是标准的 Trie 插入。
- `if(!val[nrt]) val[nrt]=x`：只在节点第一次被标记时记录编号，避免重复模式串互相覆盖。
- `rev[x]=val[nrt]`：把「第 $x$ 个模式串」映射到它最终落在的节点编号，重复模式串统一经此取答案。

### 模块三：build 构建 fail 指针

```cpp
	void build(){
		queue<int> q;
		for(int i=0;i<26;i++){
			if(c[rot][i]){
				fail[c[rot][i]]=rot;
				q.push(c[rot][i]);
				ind[rot]++;
			}
		}
		while(q.size()){
			int x=q.front();q.pop();
			for(int i=0;i<26;i++){
				if(c[x][i]){
					fail[c[x][i]]=c[fail[x]][i];
					q.push(c[x][i]);
					ind[fail[c[x][i]]]++;
				}else c[x][i]=c[fail[x]][i];
			}
		}
	}
```

- 先把根的所有儿子入队，它们的 fail 直接指向根；同时 `ind[rot]++` 统计根在 fail 树上的入度。
- BFS 按层处理：对存在的儿子按递推式 `fail[c[x][i]] = c[fail[x]][i]` 设置 fail，并让 `ind[fail[c[x][i]]]++` 记录入度。
- 对不存在的儿子走 `else` 分支做路径压缩 `c[x][i] = c[fail[x]][i]`，使后续匹配无需 while 跳 fail。

### 模块四：query 扫描文本串

```cpp
	void query(string s){
		int nrt=rot,len=s.size();
		for(int i=0;i<len;i++){
			int id=idx(s[i]);
			nrt=c[nrt][id];
			ans[nrt]++;
		}
	}
```

- 从根出发逐字符扫描文本串，`nrt = c[nrt][id]` 直接一步走转移边（路径压缩后无需跳 fail）。
- 每走到一个节点就 `ans[nrt]++`，只打标记，不做沿 fail 链的统计；真正的次数汇总交给 `topo`。

### 模块五：topo 在 fail 树上拓扑统计

```cpp
	void topo(){
		queue<int>q;
		for(int i=1;i<=cnt;i++){
			if(ind[i]==0)q.push(i);
		}
		while(!q.empty()){
			int tmp=q.front();
			q.pop();
			if(val[tmp]) cxc[val[tmp]]=ans[tmp];
			int x=fail[tmp];
			ind[x]--,ans[x]+=ans[tmp];
			if(ind[x]==0) q.push(x);
		}
		return;
	}
} zmy;
```

- 对所有入度为 0 的节点入队，在 fail 树上做拓扑排序（fail 指向父节点，故按入度从叶子向根处理）。
- `if(val[tmp]) cxc[val[tmp]]=ans[tmp]`：若当前节点是某模式串结尾，就把累计次数写入对应编号的答案数组。
- `ind[x]--, ans[x]+=ans[tmp]`：把子节点 `tmp` 的标记累加到父节点 `fail[tmp]`，即沿 fail 链上传出现次数。

### 模块六：main 读入与输出

```cpp
inline int read();
inline void write(int x);
signed main(){
//    freopen(".in","r",stdin);
//    freopen(".out","w",stdout);
	int n;cin>>n;
	for(int i=1;i<=n;i++){
		string s;cin>>s;
		zmy.insert(s,i);
	}
	zmy.build();
	string s;cin>>s;
	zmy.query(s);
	zmy.topo();
	for(int i=1;i<=n;i++){
		cout<<zmy.cxc[zmy.rev[i]]<<'\n';
	}
    return 0;
}
```

- `#define int long long` 会把 `main` 前的 `int` 一并替换成 `long long`，导致返回类型不合标准；`signed` 等价于 `int` 且不被宏替换，故写成 `signed main()`。
- 依次插入 $n$ 个模式串，`build` 构建 fail 指针，读入文本串后 `query` 打标记、`topo` 汇总。
- 最后按编号 $1\sim n$ 输出 `cxc[rev[i]]`：`rev[i]` 找到第 $i$ 个模式串对应的节点，`cxc` 取该节点累计的出现次数。

## 复杂度分析

| 阶段 | 时间复杂度 | 说明 |
|---|---|---|
| 建 Trie | $O(\sum \lvert s_i\rvert)$ | 每个字符访问一次 |
| 构建 fail | $O(\sum \lvert s_i\rvert \times 26)$ | 每个节点枚举 26 个字符 |
| 匹配 | $O(\lvert t\rvert)$ | 文本串每个字符走一步 |
| 拓扑统计 | $O(\sum \lvert s_i\rvert)$ | fail 树每个节点处理一次 |
| 总计 | $O(\sum \lvert s_i\rvert \times 26 + \lvert t\rvert)$ | 关于输入规模线性 |

空间复杂度由转移数组主导：节点数约等于 $\sum \lvert s_i\rvert$，故空间为 $O(\sum \lvert s_i\rvert \times 26)$。注意 `#define int long long` 使每个元素占 8 字节，$N=10^6$ 时 `c[N][26]` 约 208 MB，再加其余若干数组，总内存较大，需留意 MLE。

## 适用场景

1. **多模式串匹配**：一次扫描文本串，同时统计所有模式串的出现次数（本题即是典型用法）。
2. **敏感词过滤**：把敏感词建成 AC 自动机，对文本做一次扫描即可标记所有命中位置。
3. **AC 自动机 + DP**：把「已匹配的后缀」压缩成自动机节点，在状态机上演算数位 DP 等计数问题（如统计不含某子串的数）。
4. **fail 树上的树上问题**：fail 指针构成一棵树，可在其上做子树求和、树上差分等统计。

## 常见陷阱与注意事项

1. **路径压缩的写法**：`c[x][i] = c[fail[x]][i]` 必须写在 `else` 分支里，不能把 `v` 替换成 `c[fail[x]][i]` 之后还去更新 `fail[v]`。
2. **入度统计时机**：每执行一次 `fail[c[x][i]] = c[fail[x]][i]`，就要 `ind[fail[c[x][i]]]++`；根的儿子入队时也要 `ind[rot]++`。
3. **根节点处理**：根节点的 fail 指向自己（本代码根为 0，未显式赋值），根的子节点 fail 默认指向根；BFS 初始化时根的所有子节点入队。
4. **重复模式串**：用 `val` 记录编号、`rev` 反向映射，同一个 Trie 节点可能对应多个模式串，查询时经 `rev` 统一取答案。
5. **数组大小**：`c` 数组大小应为「节点数 × 26」，节点数约为 $\sum \lvert s_i\rvert$，不要开小。
6. **`#define int long long` 的代价**：所有 `int` 变成 64 位，`c[N][26]` 等数组内存翻倍，$N$ 较大时可能 MLE；`main` 需写成 `signed main`。

## 对比与扩展

| 算法 | 匹配对象 | 预处理 | 匹配复杂度 | 说明 |
|---|---|---|---|---|
| KMP | 单模式串 | $O(m)$ | $O(n)$ | 可视为 AC 自动机退化到单串时的特例 |
| Trie | 多模式串前缀查询 | $O(\sum \lvert s_i\rvert)$ | $O(\lvert t\rvert)$ | 无 fail 指针，只能做前缀匹配 |
| AC 自动机 | 多模式串 | $O(\sum \lvert s_i\rvert \times 26)$ | $O(\lvert t\rvert)$ | 本算法，一次扫描匹配所有模式串 |
| 后缀自动机 | 多模式串 / 子串 | $O(n)$ | $O(m)$ | 支持更丰富的子串查询，实现更复杂 |

**扩展方向**：

- **last 指针**：沿 fail 链跳到第一个单词结尾节点，匹配时避免空跳，属常数优化，对复杂度无理论改进。
- **AC 自动机 + 数位 DP**：把「不能出现的子串」建成自动机，DP 状态携带当前节点，统计满足条件的数。
- **动态增删模式串**：需要动态插入 / 删除模式串时，可用二进制分组或后缀自动机等替代。
