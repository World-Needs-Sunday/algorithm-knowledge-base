#include<bits/stdc++.h>
using namespace std;
int N, M;
vector<int> fa,vis,h,z_son,top;
vector<vector<int>> A;
int dfs1(int rt,int t_h)
{
	int sz = 1, nmax = 0, tz_son = 0;
	h[rt] = t_h;
	for (int i : A[rt])
	{
		fa[i] = rt;
		int t_s = dfs1(i, t_h + 1);
		sz += t_s;
		if (t_s > nmax) tz_son = i,nmax = t_s;
	}
	z_son[rt] = tz_son;
	return sz;
}

void dfs2(int rt,int z_rt)
{
	for (int i : A[rt])
	{
		if (i == z_son[rt])
		{
			top[i] = z_rt;
			dfs2(i, z_rt);
		}
		else
		{
			top[i] = i;
			dfs2(i, i);
		}
	}
}

int find(int a, int b)
{
	while (top[a] != top[b])
	{
		if (h[top[a]] >= h[top[b]]) a = fa[top[a]];
		else b = fa[top[b]];
	}
	return h[a] < h[b] ? a : b;
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	//接收输入
	cin >> N >> M;
	A.resize(N + 1);
	vis.resize(N + 1, false);
	for (int i = 1; i <= N; i++)
	{
		int n;
		cin >> n;
		A[i].resize(n);
		for (int j = 0; j < n; j++)
		{
			cin >> A[i][j];
			vis[A[i][j]] = true;
		}
	}
	int rt = 1;
	while (vis[rt]) rt++;

	//第一次dfs，高度(h),重孩子(z_son),父节点(fa)
	fa.resize(N + 1);
	fa[rt] = rt;
	h.resize(N + 1);
	z_son.resize(N + 1,-1);
	dfs1(rt,1);

	//第二次dfs,重链顶点(top)
	top.resize(N + 1);
	top[rt] = rt;
	dfs2(rt, rt);

	for (int i = 1; i <= M; i++)
	{
		int a, b;
		cin >> a >> b;
		cout << find(a, b) << '\n';
	}
	return 0;
}
