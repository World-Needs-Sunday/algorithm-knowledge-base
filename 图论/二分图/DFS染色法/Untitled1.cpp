#include<bits/stdc++.h>
using namespace std;
vector<vector<int>> edges;
vector<int> vis, node[2];
bool ans;
void init(int n)
{
	edges.assign(n + 1, vector<int>());
	vis.assign(n + 1, 0);
	node[0].clear(), node[1].clear();
	ans = true;
}
void add(int u, int v)
{
	edges[u].emplace_back(v);
	edges[v].emplace_back(u);
}
void dfs(int idx,int k)
{
	if (!ans) return;
	if (vis[idx] == 0)
	{
		vis[idx] = k;
		node[k - 1].emplace_back(idx);
		for (int i : edges[idx]) dfs(i, 3 - k);
	}
	else if (vis[idx] == k) return;
	else if (vis[idx] + k == 3)
	{
		ans = false;
		return;
	}
}
int main()
{
	//注意，所有的点都是联通的
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n, m;
	cin >> n >> m;
	init(n);
	for (int i = 1; i <= m; i++)
	{
		int u, v;
		cin >> u >> v;
		add(u, v);
	}
	dfs(1, 1);
	if (ans)
	{
		cout << "Yes\n";
		for (int i : node[0]) cout << i << ' ';
		cout << '\n';
		for (int i : node[1]) cout << i << ' ';
		cout << '\n';
	}
	else cout << "No\n";
	return 0;
}
