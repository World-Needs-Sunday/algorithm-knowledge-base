#include<bits/stdc++.h>
using namespace std;
vector<vector<bitset<51>>> A;
vector<bitset<51>> edges;
void init(int n)
{
	A.assign(n + 1, vector<bitset<51>>(31));
	edges.assign(n + 1, bitset<51>());
}
void init_edges(int n)
{
	for (int k = 1; k < 31; k++)
	{
		for (int i = 1; i <= n; i++)
		{
			for (int j = 1; j <= n; j++)
			{
				if (A[i][k - 1][j]) A[i][k] |= A[j][k - 1];
			}
		}
	}
	for (int k = 0; k < 31; k++)
	{
		for (int i = 1; i <= n; i++)
		{
			edges[i] |= A[i][k];
		}
	}
}
int bfs(int n)
{
	queue<pair<int, int>> que;
	vector<bool> vis(n + 1, false);
	que.emplace(0, 1);
	vis[1] = true;
	while (!que.empty())
	{
		int idx = que.front().second;
		int w = que.front().first;
		que.pop();
		if (idx == n) return w;

		for (int j = 1; j <= n; j++)
		{
			if (edges[idx][j])
			{
				if (vis[j]) continue;
				vis[j] = true;
				que.emplace(w + 1, j);
			}
		}
	}

	return -1;
}


int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n, m;
	cin >> n >> m;
	init(n);
	for (int i = 1; i <= m; i++)
	{
		int u, v;
		cin >> u >> v;
		A[u][0][v] = 1;
	}
	init_edges(n);

	cout << bfs(n);
	return 0;
}