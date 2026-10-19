#include<bits/stdc++.h>
using namespace std;
const int nmax = 1005;
bitset<nmax> vis[nmax];
int main()
{
	ios::sync_with_stdio(false);
	cin.tie(nullptr); cout.tie(nullptr);
	int n, m;
	cin >> n >> m;
	for (int i = 1; i <= m; i++)
	{
		int x, y;
		cin >> x >> y;
		vis[x][y] = true;
	}

	for (int i = 1; i <= n; i++) vis[i][i] = true;

	for (int k = 1; k <= n; k++)
	{
		for (int i = 1; i <= n; i++)
		{
			if (vis[i][k]) vis[i] |= vis[k];
		}
	}

	long long sum = 0;
	for (int i = 1; i <= n; i++) sum += vis[i].count();
	cout << (long long)n * (n - 1) / 2 - sum + n;
	return 0;
}