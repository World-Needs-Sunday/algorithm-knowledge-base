#include<bits/stdc++.h>
using namespace std;
vector<vector<int>> edges;//左边点的邻接表
vector<int> match;//右边节点配对的左边点
vector<int> vis;//右边节点是否有配对
int top;
void init(int n,int m)
{
	edges.assign(n + 1, vector<int>());
	vis.assign(m + 1, false);
	match.assign(m + 1, 0);
	top = 0;
}
bool dfs(int idx,int t)
{
	for (int i : edges[idx])
	{
		if (vis[i] == t) continue;
		vis[i] = t;
		if (!match[i] || dfs(match[i],t))
		{
			match[i] = idx;
			return true;
		}
	}
	return false;
}
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n, m, e;
	cin >> n >> m >> e;
	init(n, m);
	for (int i = 1; i <= e; i++)
	{
		//u:左边节点，v:右边节点
		int u, v;
		cin >> u >> v;
		edges[u].emplace_back(v);
	}
	int cnt = 0;
	for (int i = 1; i <= n; i++)
	{
		if (dfs(i,i)) ++cnt;
	}
	cout << cnt;
	return 0;
}
