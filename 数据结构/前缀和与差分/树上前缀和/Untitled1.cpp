#include<bits/stdc++.h>
using namespace std;
int N, M ,S,sz;
vector<long long> A, sum;
vector<vector<int>> edges,ST;
vector<int> fa,h;
int dfs(int rt,int rh)
{
	int ans = rh;
	h[rt] = rh;
	for (int i : edges[rt])
	{
		if (fa[i]) continue;
		fa[i] = rt;
		sum[i] = sum[rt] + A[i];
		ans = max(dfs(i, rh + 1) , ans);
	}
	return ans;
}

void init()
{
	fa.resize(N + 1, 0);
	h.resize(N + 1, 0);
	sum.resize(N + 1, 0);
	fa[S] = S,sum[S] = A[S];
	int q = dfs(S, 1);
	sz = 1;
	while (q > ((long long)1 << sz)) ++sz;
	ST.resize(N + 1,vector<int>(sz + 1,S));
	for (int i = 1; i <= N; i++) ST[i][0] = fa[i];
	for (int k = 1; k <= sz; k++)
	{
		for (int i = 1; i <= N; i++)
		{
			ST[i][k] = ST[ST[i][k - 1]][k - 1];
		}
	}
}

int find_fa(int a, int b)
{
	if (h[a] < h[b]) swap(a, b);
	int tmp = h[a] - h[b];
	for (int i = 0; i <= sz; i++)
	{
		if (tmp & 1) a = ST[a][i];
		tmp >>= 1;
	}
	if (a == b) return a;

	for (int i = sz; i >= 0; i--)
	{
		if (ST[a][i] != ST[b][i])
		{
			a = ST[a][i];
			b = ST[b][i];
		}
	}
	return fa[a];
}

long long find_sum(int a,int b)
{
	int g_fa = find_fa(a, b);
	if(g_fa != S) return sum[a] + sum[b] - sum[g_fa] - sum[fa[g_fa]];
	return sum[a] + sum[b] - sum[g_fa];
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	//N:点的数量,M:测试案例数量,S:根节点
	cin >> N >> M >> S;
	A.resize(N + 1);
	edges.resize(N + 1);
	for (int i = 1; i <= N; i++) cin >> A[i];
	for (int i = 1; i < N; i++)
	{
		int a, b;
		cin >> a >> b;
		edges[a].emplace_back(b);
		edges[b].emplace_back(a);
	}
	init();

	while (M--)
	{
		int a, b;
		cin >> a >> b;
		cout << find_sum(a, b) << '\n';
	}
	return 0;
}
