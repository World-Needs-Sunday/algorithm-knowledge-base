#include<bits/stdc++.h>
using namespace std;
int top,root;
vector<bool> vis;
vector<long long> p;
vector<int> sk, l, r, fa, ans;
vector<vector<pair<int, int>>> qs;
void init(int n,int m)
{
	top = 0;
	p.assign(n + 1, 0);
	sk.assign(n + 1, 0);
	l.assign(n + 1, 0);
	r.assign(n + 1, 0);
	fa.assign(n + 1, 0);
	qs.assign(n + 1,vector<pair<int,int>>());
	ans.assign(m + 1, 0);
	vis.assign(n + 1, false);
	for (int i = 1; i <= n; i++) fa[i] = i;
	root = 0;
}

int find(int idx)
{
	if (idx != fa[idx]) fa[idx] = find(fa[idx]);
	return fa[idx];
}

//想降低耦合，就写：      树的储存对象 init_tree(int n,const vector<long long>& p),然后创建临时数组并输出
void init_tree(int n)
{
	for (int i = 1; i <= n; i++)
	{
		//记录最后一个被弹出的节点（在维持中序遍历不变情况下，第一个比i大的元素）找到最右链的插入位置
		int last = 0;

		//找到在维持中序遍历不变情况下，第一个比i大的元素(弹完后栈顶是 i 的父节点（向左第一个 ≥ p[i] 的元素）)
		while (top > 0 && p[i] > p[sk[top - 1]]) last = sk[--top];
		
		//栈非空时，栈顶节点的右儿子设为 i；否则 i 成为根。
		if (top > 0) r[sk[top - 1]] = i;
		else root = i;

		//将右链插入位置后边的放到现节点的左边
		l[i] = last;
		//将元素加入栈中
		sk[top++] = i;
	}
}

void tarjan(int rt,int m)
{
	vis[rt] = true;
	if (l[rt] != 0)
	{
		tarjan(l[rt], m);
		fa[l[rt]] = rt;
	}
	if (r[rt] != 0)
	{
		tarjan(r[rt], m);
		fa[r[rt]] = rt;
	}
	for (pair<int, int>& i : qs[rt])
	{
		if (vis[i.first]) ans[i.second] = find(i.first);
	}
}

int main()
{

	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	//反正就是笛卡尔树满足两个性质，一个是父节点大于子节点。另一个是中序遍历保持原序列的顺序。
	//因为中序遍历的话，那就是要插入新的节点，那肯a定是得插入在最右链上的。
	//那如果你创建个单调栈的话，会发现最右链正好是单调站中的元素

	//然后最基础的用法呢就是，两个节点最近公共祖先是整个区间的最大值。

	int n, m;
	cin >> n >> m;
	init(n, m);
	for (int i = 1; i <= n; i++) cin >> p[i];
	init_tree(n);
	for (int i = 1; i <= m; i++)
	{
		int L, R;
		cin >> L >> R;
		qs[L].emplace_back(R, i);
		if (L != R) qs[R].emplace_back(L, i);
	}
	tarjan(root, m);
	for (int i = 1; i <= m; i++) cout << p[ans[i]] << '\n';

	return 0;
}