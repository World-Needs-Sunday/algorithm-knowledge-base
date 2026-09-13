#include<bits/stdc++.h>
using namespace std;
vector<vector<int>> edges;
vector<int> dfn, low;
vector<pair<int, int>> is_cut;
int cnt;
void init(int n,int m)
{
	dfn.assign(n + 1, 0);
	low.assign(n + 1, 0);
	edges.assign(n + 1, vector<int>());
	is_cut.clear();
	cnt = 0;
}
void add(int u,int v)
{
	edges[u].emplace_back(v);
	edges[v].emplace_back(u);
}
void tarjan(int rt,int fa)
{
	dfn[rt] = low[rt] = ++cnt;
	for (int i : edges[rt])
	{
		if (fa == i) continue;
		if (!dfn[i])
		{
			tarjan(i, rt);
			low[rt] = min(low[i], low[rt]);
			if (low[i] > dfn[rt]) is_cut.emplace_back(rt, i);
		}
		else low[rt] = min(dfn[rt], low[i]);
	}
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n, m;
	cin >> n >> m;
	init(n, m);
	for (int i = 1; i <= m; i++)
	{
		int u, v;
		cin >> u >> v;
		add(u, v);
	}
	for (int i = 1; i <= n; i++)
	{
		if (!dfn[i]) tarjan(i, -1);
	}
	cout << is_cut.size() << '\n';
	for (pair<int, int> i : is_cut) cout << i.first << ' ' << i.second << '\n';
	return 0;
}
