#include<bits/stdc++.h>
using namespace std;
int main()
{
	//Hamilton路径的定义是从0到n - 1不重不漏地经过每个点一次。
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n, m;
	cin >> n >> m;
	//f[i][j]:当前状态为i,正在j点上
	vector<vector<long long>> f((1 << n), vector<long long>(n,INT_MAX));
	vector<vector<int>> edges(n, vector<int>(n,INT_MAX));
	for (int i = 1; i <= m; i++)
	{
		int u, v, k;
		cin >> u >> v >> k;
		edges[u][v] = min(edges[u][v], k);
	}

	f[1][0] = 0;
	for (int i = 1; i < (1 << n); i++)
	{
		for (int j = 0; j < n; j++)
		{
			//如果点j被经过了，则退出这一轮
			if (!((i >> j) & 1)) continue;
			for (int k = 0; k < n; k++)
			{
				//判断上一个状态的k是否被经过，如果是则松弛边
				if (((i ^ (1 << j)) >> k & 1))f[i][j] = min(f[i][j], f[i ^ (1 << j)][k] + edges[k][j]);
			}
		}
	}
	if (f[(1 << n) - 1][n - 1] != INT_MAX) cout << f[(1 << n) - 1][n - 1];
	else cout << -1;
}
