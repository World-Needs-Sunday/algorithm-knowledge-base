---
id: ds-segtree-mul
title: '乘法线段树（区间乘法+区间加法双标记 P3373）'
category: 数据结构
subcategory: 线段树
tags: ["数据结构", "线段树", "区间乘法", "区间加法", "双标记", "P3373"]
timeComplexity: 'O(log n)'
spaceComplexity: 'O(n)'
codePath: '数据结构\普通线段树\乘法线段树\Untitled1.cpp'
---

## 算法原理

乘法线段树在普通线段树（区间加+区间求和）的基础上，额外支持"区间乘法"操作，对应洛谷 P3373。它仍采用二叉树结构与 lazy 延迟标记思想：根节点 rt=1 维护 [1,n]，节点 rt 的左子 `rt<<1` 维护 [l,mid]，右子 `(rt<<1)+1` 维护 [mid+1,r]。但由于同时存在乘法和加法两种修改，需要维护两个 lazy 标记：`mul_lazy`（乘法标记，初值 1）和 `sum_lazy`（加法标记，初值 0）。任意节点 rt 维护的值可形式化地看作 $\text{tree}[rt] = (\text{原始值} \times \text{mul\_lazy}[rt] + \text{sum\_lazy}[rt] \times \text{len}) \bmod m$，即"先乘后加"的复合变换。

两种标记必须按"先乘后加"的顺序复合，这是整个算法的核心。考虑一个已有标记 $(\text{mul},\; \text{sum})$（表示"先乘 mul 再加 $\text{sum} \times \text{len}$"）的节点，又收到一个新的乘法 k：新变换为 $(\text{原值} \times \text{mul} + \text{sum} \times \text{len}) \times k = \text{原值} \times (\text{mul} \times k) + (\text{sum} \times k) \times \text{len}$，因此新标记变为 $(\text{mul} \times k,\; \text{sum} \times k)$——即乘法标记和加法标记都要乘以 k。若收到的是新的加法 k：新变换为 $\text{原值} \times \text{mul} + \text{sum} \times \text{len} + k \times \text{len} = \text{原值} \times \text{mul} + (\text{sum}+k) \times \text{len}$，标记变为 $(\text{mul},\; \text{sum}+k)$——只有加法标记变化。这正是代码中 `add_mul` 要写 `sum_lazy[rt] = sum_lazy[rt]*k` 而 `add_sum` 不动 mul_lazy 的原因。

pushdown（my_down）的下传顺序同样必须"先乘后加"。设父节点标记为 $(M,\; S)$，下传给子节点（子节点原标记 $(m,\; s)$，原值 v，长度 len）。若先传乘法 M：子标记变为 $(m \times M,\; s \times M)$，值变为 $v \times M$。再传加法 S：子标记变为 $(m \times M,\; s \times M + S)$，值变为 $v \times M + S \times \text{len}$。最终子节点表示 $\text{原值} \times (m \times M) + (s \times M + S) \times \text{len}$，与"先应用父的乘 M 再应用父的加 S"语义一致。若顺序反过来先传加再传乘，加法标记会被错误地多乘一次 M，结果出错。

四个操作配合如下：建树读入数组并对 m 取模；my_down 在访问子节点前按"先乘后加"下传双标记；add_sum（区间加）和 add_mul（区间乘）在完全覆盖时分别只更新 sum_lazy 或同时更新 mul_lazy 与 sum_lazy；myget 查询前同样 my_down。所有运算都对 m 取模，防止溢出。注意 my_down 在 add_sum/add_mul/myget 中都在判断"完全包含/不相交"之前调用，保证子节点 tree 值是最新的。

## 数据结构图示

```
                 [1,8] rt=1   (mul=1, sum=0)
                /              \
        [1,4] rt=2            [5,8] rt=3
        /        \             /        \
   [1,2] rt=4  [3,4] rt=5  [5,6] rt=6  [7,8] rt=7
    /    \      /    \       /    \       /    \
 [1]rt=8 [2]rt=9 [3]rt=10 [4]rt=11 [5]rt=12 [6]rt=13 [7]rt=14 [8]rt=15
```

双标记下传示例：对 [1,8] 先区间乘 2，再区间加 3。根节点 rt=1 标记变为 `mul=2, sum=3`。下次访问子节点时 `my_down(1)`：

```
my_down(1):  先传 mul_lazy=2
  子 rt=2: mul_lazy[2] *= 2; tree[2] *= 2; sum_lazy[2] *= 2;
  子 rt=3: mul_lazy[3] *= 2; tree[3] *= 2; sum_lazy[3] *= 2;
  mul_lazy[1] = 1;
再传 sum_lazy=3
  子 rt=2: sum_lazy[2] += 3; tree[2] += 3*(mid-l+1);
  子 rt=3: sum_lazy[3] += 3; tree[3] += 3*(r-mid);
  sum_lazy[1] = 0;
```

关键：传乘法时 `sum_lazy` 也要乘 2，否则后续加法会少乘。数学上 $(v \times 2 + 0) \times 1 + 3 \times \text{len} = v \times 2 + 3 \times \text{len}$，等价于先乘 2 再加 3；若 sum_lazy 不乘，则变成 $v \times 2 + 0 + 3 \times \text{len}$ 对加法部分正确，但当子节点自身已有 sum_lazy 时就会出错。

## 核心操作详解

### init(rt, l, r, m, A) / init(n, m)

- **作用**：建树。叶子读 A[l]，内部节点为左右子和对 m 取模。
- **参数**：rt,l,r 节点与区间，m 模数，A 输入数组。
- **算法步骤**：`init(n,m)` 分配 tree(4n+1)、mul_lazy(4n+1,初值1)、sum_lazy(4n+1,初值0)；读入 A 并对 m 取模；调用 `init(1,1,n,m,A)` 递归建树，回溯时 $\text{tree}[rt] = (\text{tree}[\text{左}] + \text{tree}[\text{右}]) \bmod m$。
- **关键细节**：mul_lazy 必须初始化为 1（乘法单位元），sum_lazy 初始化为 0（加法单位元）。若 mul_lazy 初值为 0，所有值都会被乘成 0。

### my_down(rt, l, r, mid, m)

- **作用**：下传双标记，先乘后加。
- **参数**：rt 节点，l,r 区间，mid 中点，m 模数。
- **算法步骤**：若 `l==r` 返回。先下传乘法标记（若 `mul_lazy[rt]!=1`），对左右子 child 分别执行：
  - $\text{mul\_lazy}[\text{child}] \leftarrow \text{mul\_lazy}[rt] \times \text{mul\_lazy}[\text{child}] \bmod m$
  - $\text{tree}[\text{child}] \leftarrow \text{tree}[\text{child}] \times \text{mul\_lazy}[rt] \bmod m$
  - $\text{sum\_lazy}[\text{child}] \leftarrow \text{sum\_lazy}[\text{child}] \times \text{mul\_lazy}[rt] \bmod m$

  然后置 `mul_lazy[rt]=1`。再下传加法标记（若 `sum_lazy[rt]!=0`），对左右子分别执行：
  - $\text{sum\_lazy}[\text{child}] \leftarrow (\text{sum\_lazy}[rt] + \text{sum\_lazy}[\text{child}]) \bmod m$
  - $\text{tree}[\text{child}] \leftarrow (\text{tree}[\text{child}] + \text{sum\_lazy}[rt] \times \text{len}) \bmod m$

  然后置 `sum_lazy[rt]=0`。
- **关键细节**：len 对左子是 $mid-l+1$，右子是 $r-mid$。乘法标记同时作用于三个量（mul_lazy、tree、sum_lazy），缺一不可。

### add_sum(rt, l, r, ml, mr, m, k)

- **作用**：区间 [ml,mr] 每个元素加 k。
- **算法步骤**：不相交返回；先 `my_down`；若完全包含，执行以下更新后返回：

  $$\text{tree}[rt] \leftarrow (\text{tree}[rt] + k \times (r-l+1)) \bmod m$$

  $$\text{sum\_lazy}[rt] \leftarrow (\text{sum\_lazy}[rt] + k) \bmod m$$

  否则递归左右子并 pushup。
- **关键细节**：只更新 sum_lazy，不动 mul_lazy（加法不影响已有的乘法标记）。

### add_mul(rt, l, r, ml, mr, m, k)

- **作用**：区间 [ml,mr] 每个元素乘 k。
- **算法步骤**：不相交返回；先 `my_down`；若完全包含，执行以下三步更新后返回：

  $$\text{tree}[rt] \leftarrow \text{tree}[rt] \times k \bmod m$$

  $$\text{mul\_lazy}[rt] \leftarrow \text{mul\_lazy}[rt] \times k \bmod m$$

  $$\text{sum\_lazy}[rt] \leftarrow \text{sum\_lazy}[rt] \times k \bmod m$$

  否则递归左右子并 pushup。
- **关键细节**：sum_lazy 也要乘 k，这是最易错点。因为 $(\text{原值} \times \text{mul} + \text{sum} \times \text{len}) \times k = \text{原值} \times (\text{mul} \times k) + (\text{sum} \times k) \times \text{len}$。

### myget(rt, l, r, ml, mr, m)

- **作用**：查询区间 [ml,mr] 的和对 m 取模。
- **算法步骤**：不相交返回 0；`my_down`；完全包含返回 `tree[rt]`；否则递归左右子相加取模。
- **关键细节**：先 my_down 再判断完全包含，保证返回的 $\text{tree}[rt]$ 已应用所有历史标记。

## 逐行代码解析

```cpp
#include<bits/stdc++.h> 
using namespace std;
vector<long long> tree;
vector<long long> mul_lazy;   // 乘法标记，初值 1
vector<long long> sum_lazy;   // 加法标记，初值 0
```

三个全局数组分别存区间和、乘法标记、加法标记。mul_lazy 与 sum_lazy 的初值由 init 函数设定。

```cpp
void init(int rt,int l,int r,int m,const vector<long long>& A)
{
	if(l == r)
	{
		tree[rt] = A[l];        // 叶子读入初值
		return;
	}
	int mid = (l + r) >> 1;
	init(rt << 1,l,mid,m,A);
	init((rt << 1) + 1,mid + 1,r,m,A);
	tree[rt] = (tree[rt << 1] + tree[(rt << 1) + 1]) % m;  // pushup 取模
}
void init(int n,int m)
{
	vector<long long> A(n + 1);
	tree.resize(4 * n + 1);
	mul_lazy.resize(4 * n + 1 , 1);   // 乘法单位元 1
	sum_lazy.resize(4 * n + 1 , 0);   // 加法单位元 0
	for(int i = 1;i <= n;i++)
	{
		cin >> A[i];
		A[i] %= m;                    // 输入即取模
	}
	init(1,1,n,m,A);
}
```

`mul_lazy.resize(4*n+1,1)` 是关键：乘法单位元为 1，写成 0 会导致全树归零。输入 A[i] 读入后立即 `%m`，避免后续运算溢出。

```cpp
void my_down(int rt,int l,int r,int mid,int m)
{
	if(l == r) return;
	if(mul_lazy[rt] != 1)              // 有乘法标记：先下传
	{
		// 左子：mul、tree、sum 三者都乘以 mul_lazy[rt]
		mul_lazy[rt << 1] = (mul_lazy[rt] * mul_lazy[rt << 1]) % m;
		tree[rt << 1] = (tree[rt << 1] * mul_lazy[rt]) % m;
		sum_lazy[rt << 1] = (sum_lazy[rt << 1] * mul_lazy[rt]) % m;
		// 右子同理（(rt<<1)|1 等价于 (rt<<1)+1）
		mul_lazy[(rt << 1) | 1] = (mul_lazy[rt] * mul_lazy[(rt << 1) | 1]) % m;
		tree[(rt << 1) | 1] = (tree[(rt << 1) | 1] * mul_lazy[rt]) % m;
		sum_lazy[(rt << 1) | 1] = (sum_lazy[(rt << 1) | 1] * mul_lazy[rt]) % m;
		mul_lazy[rt] = 1;              // 清空为单位元
	}
	if(sum_lazy[rt] != 0)              // 有加法标记：后下传
	{
		// 左子：sum 累加，tree 加 sum*左子长度
		sum_lazy[rt << 1] = (sum_lazy[rt] + sum_lazy[rt << 1]) % m;
		tree[rt << 1] = (tree[rt << 1] + sum_lazy[rt] * (mid - l + 1)) % m;
		// 右子：长度为 r-mid
		sum_lazy[(rt << 1) | 1] = (sum_lazy[rt] + sum_lazy[(rt << 1) | 1]) % m;
		tree[(rt << 1) | 1] = (tree[(rt << 1) | 1] + sum_lazy[rt] * (r - mid)) % m;
		sum_lazy[rt] = 0;
	}
}
```

这是全模板最核心的函数。两个 if 块的顺序不可交换：必须先处理乘法标记，再处理加法标记。乘法块中 `sum_lazy[child] *= mul_lazy[rt]` 是最易漏的一行——它把子节点原有的加法标记也按比例放大，因为后续的乘法应当作用在"加法之前"。

```cpp
void add_sum(int rt,int l,int r,int ml,int mr,int m,long long k)
{
	if(r < ml || l > mr) return;
	int mid = (l + r) >> 1;
	my_down(rt,l,r,mid,m);                 // 先下传
	if(ml <= l && mr >= r)                 // 完全包含
	{
		tree[rt] = (tree[rt] + k * (r - l + 1)) % m;
		sum_lazy[rt] = (sum_lazy[rt] + k) % m;   // 只动加法标记
		return;
	}
	add_sum(rt << 1,l,mid,ml,mr,m,k);
	add_sum((rt << 1) | 1,mid + 1,r,ml,mr,m,k);
	tree[rt] = (tree[rt << 1] + tree[(rt << 1) | 1]) % m;  // pushup
}
void add_mul(int rt,int l,int r,int ml,int mr,int m,long long k)
{
	if(r < ml || l > mr) return;
	int mid = (l + r) >> 1;
	my_down(rt,l,r,mid,m);
	if(ml <= l && mr >= r)
	{
		tree[rt] = (tree[rt] * k) % m;
		mul_lazy[rt] = (mul_lazy[rt] * k) % m;
		sum_lazy[rt] = (sum_lazy[rt] * k) % m;   // 关键：加法标记也要乘
		return;
	}
	add_mul(rt << 1,l,mid,ml,mr,m,k);
	add_mul((rt << 1) | 1,mid + 1,r,ml,mr,m,k);
	tree[rt] = (tree[rt << 1] + tree[(rt << 1) | 1]) % m;
}
```

对比 add_sum 与 add_mul：add_sum 只改 `sum_lazy`，add_mul 同时改 `mul_lazy` 和 `sum_lazy`。这是因为乘法会影响"已经登记但未下传的加法"，而加法不会影响乘法。

```cpp
long long myget(int rt,int l,int r,int ml,int mr,int m)
{
	if(r < ml || l > mr) return 0;
	int mid = (l + r) >> 1;
	my_down(rt,l,r,mid,m);
	if(ml <= l && mr >= r) return tree[rt];
	return (myget(rt << 1,l,mid,ml,mr,m) + myget((rt << 1) | 1,mid + 1,r,ml,mr,m)) % m;
}
```

查询函数结构与普通线段树一致，区别仅在 my_down 多了一个 mid 和 m 参数。

```cpp
int main()
{
	int n,q;
	long long m;
	cin >> n >> q >> m;
	init(n,m);
	for(int i = 1;i <= q;i++)
	{
		int op;
		cin >> op;
		if(op == 1) { /* 区间乘 */ int x,y; long long k; cin >> x >> y >> k; add_mul(1,1,n,x,y,m,k); }
		else if(op == 2) { /* 区间加 */ int x,y; long long k; cin >> x >> y >> k; add_sum(1,1,n,x,y,m,k); }
		else { /* 区间查询 */ int x,y; cin >> x >> y; cout << myget(1,1,n,x,y,m) << '\n'; }
	}
	return 0;
}
```

主程序根据 op 分发：op=1 区间乘、op=2 区间加、op=3 区间查询。

## 复杂度分析

- **建树** $O(n)$：每个节点访问一次。
- **add_sum / add_mul / myget** 单次 $O(\log n)$：树高 $O(\log n)$，每层至多 4 个节点。由于双标记，my_down 常数约为普通线段树的 2~3 倍，但渐进复杂度不变。
- **空间** $O(4n)$：tree、mul_lazy、sum_lazy 各 $4n+1$，共约 $12n$。
- **总复杂度**：q 次操作 $O((n + q) \log n)$。

## 适用场景

适用于同时需要区间乘法和区间加法的序列维护问题（洛谷 P3373）。它是理解"多标记 lazy 线段树"的典型例子，掌握"先乘后加"的复合规则后，可推广到更多标记组合（如区间赋值+区间加、历史最值等）。若题目只有单种区间操作，使用普通线段树或 BIT 即可，无需引入双标记的复杂性。

## 常见陷阱与注意事项

1. **mul_lazy 必须初始化为 1**（乘法单位元），写成 0 会导致所有值被乘成 0；sum_lazy 初始化为 0。
2. **my_down 必须先下传乘法标记再下传加法标记**，顺序反了会导致加法标记被多乘一次。
3. **乘法标记下传时必须同时作用于子节点的 mul_lazy、tree、sum_lazy 三个量**，漏掉 sum_lazy 是最常见错误。
4. **add_mul 中 `sum_lazy[rt] = sum_lazy[rt]*k` 不可省略**；add_sum 中不要误改 mul_lazy。
5. **所有运算都要对 m 取模**，且 k 读取后最好先取模，防止中间结果溢出 long long（当 m 接近 $10^9$ 时两数相乘可达 $10^{18}$，long long 仍可承载，但三数相乘需小心）。
6. **my_down 中判断 `mul_lazy[rt] != 1` 和 `sum_lazy[rt] != 0`** 是优化，避免无意义运算；但即使省略判断逻辑仍正确。

## 对比与扩展

- **与普通线段树对比**：双标记的难点在于标记的复合顺序，本质上是在维护一个仿射变换（affine transformation）$x \to a \cdot x + b$ 的复合。普通线段树只有加法，相当于 $a$ 恒为 $1$ 的退化情形。
- **与矩阵乘法线段树对比**：可将每个元素视为向量 $(值,\; 1)$，每次操作是 $2 \times 2$ 矩阵 $\begin{bmatrix} a & b \\ 0 & 1 \end{bmatrix}$，标记复合即矩阵乘法，能统一处理加、乘、赋值，但常数更大。乘法线段树可看作矩阵法的特例优化。
- **扩展方向**：(1) 区间赋值标记（需额外 bool 区分是否赋值，赋值优先级最高）；(2) 历史最值线段树（维护历史最大/最小，需额外历史标记）；(3) 多项式标记推广到更高次；(4) 结合离散化处理大值域（见 ds-segtree-discretize）。
