#include<bits/stdc++.h>
using namespace std;
vector<int> deg,vis;
vector<pair<int,int>> edges;
vector<vector<int>> A;
int tim = 0;
void init(int n, int m)
{
	edges.assign(m + 1, {});
	deg.assign(n + 1, 0);
	vis.assign(n + 1, 0);
	tim = 0;
	A.assign(n + 1, vector<int>());
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
		cin >> edges[i].first >> edges[i].second;
		++deg[edges[i].first];
		++deg[edges[i].second];
	}

	//如果都成两个方向的边，然后再记录是否根据顺序重复等等等等常数会爆炸。
	//所以说能不要的就先不要了，就记录所有满足deg[u] > deg[v] || (deg[u] == deg[v] && u > v)的边。
	//然后就搜索u，那可以从u找到v，v找到w。这样子这个过程就没有额外的消耗了。然后再加上时间戳，就可以把常数加到一个可以接受的地步。
	for (int i = 1; i <= m; i++)
	{
		int u = edges[i].first;
		int v = edges[i].second;
		if (deg[u] > deg[v] || (deg[u] == deg[v] && u > v)) swap(u, v);
		A[u].emplace_back(v);
	}

	long long cnt = 0;
	for (int i = 1; i <= n; i++)
	{
		++tim;
		for (int j : A[i]) vis[j] = tim;

		for (int j : A[i])
		{
			for (int w : A[j])
			{
				if (vis[w] == tim) ++cnt;
			}
		}
	}
	cout << cnt;
	return 0;

}