#include<bits/stdc++.h>
using namespace std;

int n;
vector<vector<int>> edges;
vector<int> ans;

void init()
{
	ans.clear();
	edges.assign(n + 1, vector<int>());
}

int dfs(int rt, int fa)
{
	int rt_sz = 1;
	int max_rt = 0, max_sz = 0;
	for (int i : edges[rt])
	{
		if (i == fa) continue;
		int k = dfs(i, rt);
		rt_sz += k;
		if (k > max_sz)
		{
			max_sz = k;
			max_rt = i;
		}
	}
	if (n - rt_sz <= n / 2 && max_sz <= n / 2)
		ans.emplace_back(rt);
	return rt_sz;
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	cin >> n;
	init();
	for (int i = 1; i < n; i++)
	{
		int u, v;
		cin >> u >> v;
		edges[u].emplace_back(v);
		edges[v].emplace_back(u);
	}
	dfs(1, -1);
	for (int i : ans) cout << i << ' ';
	return 0;
}
