#include<bits/stdc++.h>
using namespace std;
long long INF = 1e17;
int main()
{
	int n, m;
	cin >> n >> m;
	vector<vector<long long>> A(n + 1,vector<long long>(n + 1,INF));
	for (int i = 1; i <= n; i++) A[i][i] = 0;
	for (int i = 1; i <= m; i++)
	{
		int u, v;
		long long w;
		cin >> u >> v >> w;
		if (A[u][v] > w)
		{
			A[u][v] = w;
			A[v][u] = w;
		}
	}
	for (int k = 1; k <= n; k++)
	{
		for (int i = 1; i <= n; i++)
		{
			for (int j = 1; j <= n; j++)
			{
				A[i][j] = min(A[i][j], A[i][k] + A[k][j]);
			}
		}
	}
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			cout << A[i][j] << ' ';
		}
		cout << '\n';
	}
	return 0;
}
