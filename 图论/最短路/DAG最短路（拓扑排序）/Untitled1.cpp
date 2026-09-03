#include<bits/stdc++.h>
using namespace std;
//输入为有向无环图(DAG),且入口有且仅有一个
vector<vector<pair<int, long long>>> edges;
vector<int> in;
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n, m;
	cin >> n >> m;
	edges.resize(n + 1);
	in.resize(n + 1,0);
	for (int i = 1; i <= m; i++)
	{
		int u, v;
		long long w;
		cin >> u >> v >> w;
		edges[u].emplace_back(v, w);
		++in[v];
	}
	int idx = 1;
	while (in[idx]) idx++;
	cout << "DAG入口为: " << idx << '\n';

	vector<int> que(n + 5);
	vector<long long> dis(n + 1, 1e18);
	dis[idx] = 0;
	int l = 0, r = 0;
	que[r++] = idx;
	while (l != r)
	{
		int top = que[l++];
		for (pair<int, long long> i : edges[top])
		{
			dis[i.first] = min(dis[top] + i.second, dis[i.first]);
			if (!(--in[i.first])) que[r++] = i.first;
		}
	}
	for (int i = 1; i <= n; i++)
	{
		if (dis[i] != 1e18) cout << dis[i] << ' ';
		else cout << -1 << ' ';
	}
	return 0;
}
