#include<bits/stdc++.h>
using namespace std;
int n, m, tim, cnt;
vector<vector<int>> edges;
vector<int> dfn, low;
vector<bool> is_cut;
void init()
{
	tim = 0;
	cnt = 0;
	dfn.assign(n + 1, 0);
	low.assign(n + 1, 0);
	is_cut.assign(n + 1, false);
}
void tarjan(int nd,int fa)
{
	int child_cnt = 0;
	dfn[nd] = low[nd] = ++tim;
	for (int to : edges[nd])
	{
		if (to == fa) continue;
		if (!dfn[to])
		{
			++child_cnt;
			tarjan(to, nd);
			low[nd] = min(low[to], low[nd]);
			if (fa != -1 && low[to] >= dfn[nd] && !is_cut[nd]) is_cut[nd] = true, cnt++;
		}
		else low[nd] = min(low[nd], dfn[to]);
	}

	if (fa == -1 && child_cnt >= 2) is_cut[nd] = true, cnt++;
}
void fn()
{
	init();
	for (int i = 1; i <= n; i++)
	{
		if(!dfn[i]) tarjan(i, -1);
	}
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	cin >> n >> m;
	edges.assign(n + 1, {});
	for (int i = 1; i <= m; i++)
	{
		int u, v;
		cin >> u >> v;
		edges[u].emplace_back(v);
		edges[v].emplace_back(u);
	}
	fn();
	cout << cnt << '\n';
	for (int i = 1; i <= n; i++)
	{
		if (is_cut[i]) cout << i << ' ';
	}
	return 0;
}