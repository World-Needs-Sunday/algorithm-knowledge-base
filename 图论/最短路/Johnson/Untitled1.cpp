#include<bits/stdc++.h>
using namespace std;
const long long INF = 1e17;
int n, m;
vector<vector<pair<int, long long>>> edges;
vector<vector<long long>> ans;
vector<long long> h;
inline long long get_w(int u,int v,long long w)
{
	return w + h[u] - h[v];
}
bool init_h()
{
	h.resize(n + 1, INF);
	vector<int> stack1(n + 1), stack2(n + 1);
	vector<bool> vis(n + 1, false);
	int id1 = 0, id2 = 0;
	stack1[id1++] = 0;
	h[0] = 0;
	for (int i = 0; i <= n; ++i)
	{
		while (id1)
		{
			int u = stack1[--id1];
			for (int j = 0; j < edges[u].size(); ++j)
			{
				int v = edges[u][j].first;
				long long w = edges[u][j].second;
				if (h[v] > h[u] + w)
				{
					h[v] = h[u] + w;
					if (!vis[v])
					{
						vis[v] = true;
						stack2[id2++] = v;
					}
				}
			}
		}
		if (!id2) return false;
		while (id2)
		{
			vis[stack2[id2 - 1]] = false;
			stack1[id1++] = stack2[--id2];
		}
	}
	return id1;
}

void dij(int i)
{
	priority_queue<pair<long long, int>,vector<pair<long long,int>>,greater<pair<long long,int>>> que;
	vector<bool> vis(n + 1, false);
	ans[i][i] = 0;
	que.emplace(0, i);
	while (!que.empty())
	{
		long long wu = que.top().first;
		int u = que.top().second;
		que.pop();
		if (vis[u]) continue;
		vis[u] = true;
		for (int j = 0; j < edges[u].size(); ++j)
		{
			int v = edges[u][j].first;
			if (vis[v]) continue;
			long long w_uv = get_w(u,v,edges[u][j].second);
			if (ans[i][v] > wu + w_uv)
			{
				ans[i][v] = wu + w_uv;
				que.emplace(ans[i][v], v);
			}
		}
	}
}

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	cin >> n >> m;
	edges.resize(n + 1, vector<pair<int,long long>>());
	for (int i = 1; i <= n; i++) edges[0].emplace_back(i, 0);
	for (int i = 1; i <= m; i++)
	{
		int u, v;
		long long w;
		cin >> u >> v >> w;
		edges[u].emplace_back(v, w);
	}
	ans.resize(n + 1, vector<long long>(n + 1, INF));

	if (init_h()) cout << -1;
	else
	{
		for (int i = 1; i <= n; ++i)
		{
			dij(i);
			for (int j = 1; j <= n; ++j)
			{
				if (ans[i][j] >= INF) cout << "NO" << ' ';
				else cout << ans[i][j] - h[i] + h[j] << ' ';
			}
			cout << '\n';
		}
	}
	return 0;
}
