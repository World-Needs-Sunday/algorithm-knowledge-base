#include<bits/stdc++.h>
using namespace std;
vector<long long> tree, lazy, arr;
vector<vector<int>> edges;
vector<int> id,sz,h,w_son,top,fa;
int cnt,n;
void dfs1(int rt)
{
	int son_id = 0, nmax = 0;
	for (int i : edges[rt])
	{
		if (i == fa[rt]) continue;
		fa[i] = rt;
		h[i] = h[rt] + 1;
		dfs1(i);
		sz[rt] += sz[i];
		if (sz[i] > nmax)
		{
			son_id = i;
			nmax = sz[i];
		}
	}
	w_son[rt] = son_id;
}

void dfs2(int rt, int tp)
{
	id[rt] = ++cnt;
	top[rt] = tp;
	if (w_son[rt]) dfs2(w_son[rt], tp);
	for (int i : edges[rt])
	{
		if (i == fa[rt] || i == w_son[rt]) continue;
		dfs2(i, i);
	}
}

void init(int l, int r, int rt,const vector<long long>& A)
{
	if (l == r)
	{
		tree[rt] = A[l];
		return;
	}
	int mid = l + (r - l) / 2;
	int ls = rt << 1, rs = (rt << 1) | 1;
	init(l, mid, ls, A);
	init(mid + 1, r, rs, A);
	tree[rt] = tree[ls] + tree[rs];
}
void init(int rt)
{
	id.resize(n + 1);
	sz.resize(n + 1, 1);
	h.resize(n + 1, 1);
	w_son.resize(n + 1,0);
	top.resize(n + 1);
	fa.resize(n + 1);
	fa[rt] = rt, cnt = 0;
	dfs1(rt);
	dfs2(rt, rt);
	vector<long long> A(n + 1);
	for (int i = 1; i <= n; i++) A[id[i]] = arr[i];
	tree.resize(4 * n + 1);
	lazy.resize(4 * n + 1);
	init(1, n, 1, A);
}
void down_lazy(int l, int r, int rt)
{
	if (!lazy[rt]) return;
	int mid = l + (r - l) / 2;
	int ls = rt << 1, rs = (rt << 1) | 1;
	lazy[ls] += lazy[rt];
	tree[ls] += lazy[rt] * (mid - l + 1);
	lazy[rs] += lazy[rt];
	tree[rs] += lazy[rt] * (r - mid);
	lazy[rt] = 0;
}
long long tree_get(int l,int r,int rt,int ml,int mr)
{
	if (ml <= l && r <= mr) return tree[rt];
	if (ml > r || mr < l) return 0;
	down_lazy(l, r, rt);
	int mid = l + (r - l) / 2;
	int ls = rt << 1, rs = (rt << 1) | 1;
	return tree_get(l, mid, ls, ml, mr) + tree_get(mid + 1, r, rs, ml, mr);
}
void tree_set(int l, int r, int rt, int ml, int mr, long long k)
{
	if (ml <= l && r <= mr)
	{
		lazy[rt] += k;
		tree[rt] += k * (r - l + 1);
		return;
	}
	if (ml > r || mr < l) return;
	down_lazy(l, r, rt);
	int mid = l + (r - l) / 2;
	int ls = rt << 1, rs = (rt << 1) | 1;
	tree_set(l, mid, ls, ml, mr, k);
	tree_set(mid + 1, r, rs, ml, mr, k);
	tree[rt] = tree[ls] + tree[rs];
}

vector<pair<int,int>> split(int u,int v)
{
	vector<pair<int, int>> sp;
	while (top[u] != top[v])
	{
		if (h[top[u]] < h[top[v]]) swap(u, v);
		sp.emplace_back(id[top[u]], id[u]);
		u = fa[top[u]];
	}
	if (h[u] > h[v]) swap(u, v);
	sp.emplace_back(id[u], id[v]);
	return sp;
}

long long mget(int u, int v)
{
	vector<pair<int, int>> sp = split(u, v);
	long long ans = 0;
	for (pair<int, int> nd : sp) ans += tree_get(1, n, 1, nd.first, nd.second);
	return ans;
}
void mset(int u, int v, long long k)
{
	vector<pair<int, int>> sp = split(u, v);
	for (pair<int, int> nd : sp) tree_set(1, n, 1, nd.first, nd.second, k);
}

long long subtree_get(int x)
{
	return tree_get(1, n, 1, id[x], id[x] + sz[x] - 1);
}
void subtree_set(int x,long long k)
{
	tree_set(1, n, 1, id[x], id[x] + sz[x] - 1, k);
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int m, r;
	cin >> n >> m >> r;
	arr.resize(n + 1);
	edges.resize(n + 1);
	for (int i = 1; i <= n; i++) cin >> arr[i];
	for (int i = 1; i < n; i++)
	{
		int x, y;
		cin >> x >> y;
		edges[x].emplace_back(y);
		edges[y].emplace_back(x);
	}
	init(r);
	while (m--)
	{
		int op;
		cin >> op;
		if (op == 1)
		{
			int x, y;
			long long z;
			cin >> x >> y >> z;
			mset(x, y, z);
		}
		else if (op == 2)
		{
			int x, y;
			cin >> x >> y;
			cout << mget(x, y) << '\n';
		}
		else if (op == 3)
		{
			int x;
			long long z;
			cin >> x >> z;
			subtree_set(x, z);
		}
		else if (op == 4)
		{
			int x;
			cin >> x;
			cout << subtree_get(x) << '\n';
		}
	}
	return 0;
}
