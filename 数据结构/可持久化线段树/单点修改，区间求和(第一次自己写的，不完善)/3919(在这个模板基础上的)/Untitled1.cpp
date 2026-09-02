#include<bits/stdc++.h>
using namespace std;
/*洛谷3919*/
struct node
{
	long long val;
	int son_l, son_r;
	node() :val(0), son_l(-1), son_r(-1) {}
};
int rt_idx = 0;
int nd_idx = 0;
vector<node> root;
vector<node> nd;
void init(int rt, int l, int r, const vector<long long>& A)
{
	if (l == r) nd[rt].val = A[l];
	else
	{
		int mid = (l + r) >> 1;
		nd[rt].son_l = ++nd_idx;
		init(nd[rt].son_l, l, mid, A);
		nd[rt].son_r = ++nd_idx;
		init(nd[rt].son_r, mid + 1, r, A);
		nd[rt].val = nd[nd[rt].son_l].val + nd[nd[rt].son_r].val;
	}
}
void init(int n, int m)
{
	vector<long long> A(n + 1);
	root.resize(m + 1);
	nd.resize(n * 4 + m * ceil(log2(n)));
	for (int i = 1; i <= n; i++) cin >> A[i];
	int l = 1, r = n;
	int mid = (l + r) >> 1;
	root[rt_idx].son_l = ++nd_idx;
	init(root[rt_idx].son_l, l, mid, A);
	root[rt_idx].son_r = ++nd_idx;
	init(root[rt_idx].son_r, mid + 1, r, A);
	root[rt_idx].val = nd[root[rt_idx].son_l].val + nd[root[rt_idx].son_r].val;
	++rt_idx;
}
void insert(int rt, int old, int l, int r, int i, long long k)
{
	nd[rt] = nd[old];
	if (l == r)
	{
		nd[rt].val = k;
		return;
	}
	int mid = (l + r) >> 1;
	if (i <= mid)
	{
		nd[rt].son_l = ++nd_idx;
		insert(nd[rt].son_l, nd[old].son_l, l, mid, i, k);
	}
	else
	{
		nd[rt].son_r = ++nd_idx;
		insert(nd[rt].son_r, nd[old].son_r, mid + 1, r, i, k);
	}
	nd[rt].val = nd[nd[rt].son_l].val + nd[nd[rt].son_r].val;
}
void insert(int old, int n, int i, long long k)
{
	int l = 1; int r = n;
	int mid = (l + r) >> 1;
	root[rt_idx] = root[old];
	if (i <= mid)
	{
		root[rt_idx].son_l = ++nd_idx;
		insert(root[rt_idx].son_l, root[old].son_l, l, mid, i, k);
	}
	else
	{
		root[rt_idx].son_r = ++nd_idx;
		insert(root[rt_idx].son_r, root[old].son_r, mid + 1, r, i, k);
	}
	root[rt_idx].val = nd[root[rt_idx].son_l].val + nd[root[rt_idx].son_r].val;
	++rt_idx;
}
long long myget(int rt, int l, int r, int i)
{
	if (i < l || i > r) return 0;
	if (l == r) return nd[rt].val;
	int mid = (l + r) >> 1;
	if (i <= mid) return myget(nd[rt].son_l, l, mid, i);
	else return myget(nd[rt].son_r, mid + 1, r, i);
}
long long myget(int ver, int n, int i)
{
	int l = 1, r = n;
	int mid = (l + r) >> 1;
	if (i <= mid) return myget(root[ver].son_l, l, mid, i);
	else return myget(root[ver].son_r, mid + 1, r, i);
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int N, M;
	cin >> N >> M;
	init(N, M);
	for (int i = 1; i <= M; i++)
	{
		int v, op;
		cin >> v >> op;
		if (op == 1)
		{
			int idx;
			long long k;
			cin >> idx >> k;
			insert(v, N, idx, k);
		}
		else
		{
			int x;
			cin >> x;
			cout << myget(v, N, x) << '\n';
			root[rt_idx++] = root[v];
		}
	}
	return 0;
}
